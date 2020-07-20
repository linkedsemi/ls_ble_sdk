#include "spi_flash.h"
#include "le501x.h"
#include "lscache.h"
#include "compile_flag.h"
#include "reg_rcc.h"
#include "field_manipulate.h"
#include "sdk_config.h"
#include "platform.h"
#include "reg_lsgpio.h"
#include "reg_syscfg.h"
#include "reg_rf.h"
#include "prf_fotas.h"
#include "tinycrypt/ecc_dsa.h"
#include "ls_ble.h"
#include "ls_dbg.h"
#include "log.h"
#include "common.h"
#define APP_IMAGE_BASE (0x1800D000)
#define OTA_COPY_STATUS_OFFSET (OTA_INFO_OFFSET + FLASH_PAGE_SIZE)
#define FW_ECC_VERIFY (0)

static uint8_t adv_obj_hdl;
static uint8_t advertising_data[28] = {5,8,'F','O','T','A'};
static uint8_t scan_response_data[31];

static void swd_pull_down()
{
    MODIFY_REG(LSGPIOB->PUPD,GPIO_PUPD5_MASK|GPIO_PUPD6_MASK,2<<GPIO_PUPD5_POS | 2<<GPIO_PUPD6_POS);
}

uint16_t trim_head_load()
{
    uint16_t head[8];
    uint16_t version;
    spi_flash_read_security_area(1, 0,(void *)head, sizeof(head));
    if(head[0]==~head[1] && head[2]==~head[3])
    {
        version = head[0];
        return version;
    }else
    {
        return 0;
    }
    
}

void trim_version_4100_load()
{
    uint16_t buf[10];
    spi_flash_read_security_area(1, 0x10,(void *)buf, sizeof(buf));
    uint8_t i;
    for(i=0;i<10;i+=2)
    {
        if(buf[i]!=(uint16_t)~buf[i+1])
        {
            while(1);
        }
    }
    struct {
        uint16_t bg_vref_fine:2,
                bg_reg_trim:6,
                dcdc_vbg_vctl:4,
                lvd_ref:3;
    } *cal_0 = (void *)&buf[0];
    struct {
        uint16_t hpldo_trim:2,
                res:6,
                ldo_tx_trim:3,
                res2:1,
                ldo_rx_trim:3;
    } *cal_1 = (void *)&buf[2];
    struct {
        uint16_t xo16m_cap_trim:6,
                xo16m_adj:2;
    } *cal_2 = (void *)&buf[4];
    struct {
        uint16_t osc_rc24m_cal:15;
    } *cal_3 = (void *)&buf[6];
    struct {
        uint16_t lpldo_trim:3;
    } *cal_4 = (void *)&buf[8];
    cal_2->xo16m_cap_trim = 0x20;
    cal_4->lpldo_trim = 5;
    REG_FIELD_WR(SYSCFG->DCDC, SYSCFG_VBG_VCTL, cal_0->dcdc_vbg_vctl);
    MODIFY_REG(SYSCFG->ANACFG0, SYSCFG_BG_RES_TRIM_MASK | SYSCFG_BG_VREF_FINE_MASK | SYSCFG_LDO_DG_TRIM_MASK | SYSCFG_LVD_REF_MASK
        , cal_0->bg_reg_trim<<SYSCFG_BG_RES_TRIM_POS | cal_0->bg_vref_fine<<SYSCFG_BG_VREF_FINE_POS 
        | cal_1->hpldo_trim << SYSCFG_LDO_DG_TRIM_POS | cal_0->lvd_ref << SYSCFG_LVD_REF_POS);
    MODIFY_REG(SYSCFG->ANACFG1, SYSCFG_XO16M_ADJ_MASK | SYSCFG_XO16M_CAP_TRIM_MASK, 
        cal_2->xo16m_adj << SYSCFG_XO16M_ADJ_POS | cal_2->xo16m_cap_trim << SYSCFG_XO16M_CAP_TRIM_POS);
    MODIFY_REG(SYSCFG->CFG, SYSCFG_HAI_SEL_MASK | SYSCFG_HAI_IBIAS_SEL_MASK | SYSCFG_HAI_CAL_MASK | SYSCFG_HAI_CAP_MASK,
        cal_3->osc_rc24m_cal);
    REG_FIELD_WR(SYSCFG->PMU_TRIM,SYSCFG_LDO_LP_TRIM,cal_4->lpldo_trim);

    MODIFY_REG(RF->REG08,RF_LDO_TX_TRIM_MASK | RF_LDO_RX_TRIM_MASK,
        cal_1->ldo_tx_trim<<RF_LDO_TX_TRIM_POS | cal_1->ldo_rx_trim<<RF_LDO_RX_TRIM_POS);
}

void trim_val_load()
{
    RCC->APB1EN |= 1<<RCC_RF_POS | 1<<RCC_MDM2_POS;
    uint16_t version = trim_head_load();
    switch(version)
    {
    case 0x4100:
        trim_version_4100_load();
    break;
    default:
        REG_FIELD_WR(SYSCFG->ANACFG1, SYSCFG_XO16M_CAP_TRIM,0x20);
        REG_FIELD_WR(SYSCFG->PMU_TRIM,SYSCFG_LDO_LP_TRIM,5);
        MODIFY_REG(RF->REG08,RF_LDO_TX_TRIM_MASK | RF_LDO_RX_TRIM_MASK,
            4<<RF_LDO_TX_TRIM_POS | 4<<RF_LDO_RX_TRIM_POS);
    break;
    }
}

#if FW_ECC_VERIFY
extern const uint8_t fotas_pub_key[64];
bool fw_signature_check(struct fw_digest *digest,struct fota_signature *signature)
{
    return uECC_verify(fotas_pub_key, digest->data, sizeof(digest->data), signature->data, uECC_secp256r1());
}
#else
bool fw_signature_check(struct fw_digest *digest,struct fota_signature *signature)
{
    return true;
}
#endif


static void ota_copy_info_set(struct fota_image_info *ptr)
{
    spi_flash_quad_page_program(OTA_COPY_STATUS_OFFSET, (uint8_t *)ptr, sizeof(struct fota_image_info));
}

static bool ota_copy_info_get(struct fota_image_info *ptr)
{
    spi_flash_quad_io_read(OTA_COPY_STATUS_OFFSET,(uint8_t *)ptr, sizeof(struct fota_image_info));
    if(ptr->base==0xffffffff && ptr->size ==0xffffffff)
    {
        return false;
    }else
    {
        return true;
    }
}

static void fw_copy(struct fota_image_info *ptr)
{
    static uint8_t fw_buf[FLASH_PAGE_SIZE];
    uint16_t i;
    for(i=0;i<CEILING(ptr->size, FLASH_PAGE_SIZE);++i)
    {
        spi_flash_quad_io_read(ptr->base - FLASH_BASE_ADDR + i*FLASH_PAGE_SIZE, fw_buf, FLASH_PAGE_SIZE);
        spi_flash_quad_page_program(APP_IMAGE_BASE - FLASH_BASE_ADDR + i*FLASH_PAGE_SIZE,fw_buf, FLASH_PAGE_SIZE);
    }
}

static void ota_settings_erase()
{
    spi_flash_sector_erase(OTA_INFO_OFFSET);
}

static void prf_fota_server_callback(enum fotas_evt_type type,union fotas_evt_u *evt,uint8_t con_idx)
{
    switch(type)
    {
    case FOTAS_START_REQ_EVT:
    {
        enum fota_start_cfm_status status;
        if(fw_signature_check(evt->fotas_start_req.digest, evt->fotas_start_req.signature))
        {
            status = FOTA_REQ_ACCEPTED;
        }else
        {
            status = FOTA_REQ_REJECTED;
        }
        prf_fotas_start_confirm(con_idx, status);
    }break;
    case FOTAS_FINISH_EVT:
        if(evt->fotas_finish.integrity_checking_result)
        {
            if(evt->fotas_finish.new_image->base != APP_IMAGE_BASE)
            {
                ota_copy_info_set(evt->fotas_finish.new_image);
                fw_copy(evt->fotas_finish.new_image);
            }
            ota_settings_erase();
            platform_reset(RESET_OTA_SUCCEED);
        }else
        {
            //TODO ota_settings_erase or not
            platform_reset(RESET_OTA_FAILED);
        }
    break;
    default:
        LS_ASSERT(0);
    break;
    }
}

static void create_adv_obj()
{
    struct legacy_adv_obj_param adv_param = {
        .adv_intv_min = 32,
        .adv_intv_max = 32,
        .own_addr_type = PUBLIC_OR_RANDOM_STATIC_ADDR,
        .filter_policy = 0,
        .ch_map = 0x7,
        .disc_mode = ADV_MODE_GEN_DISC,
        .prop = {
            .connectable = 1,
            .scannable = 1,
            .directed = 0,
            .high_duty_cycle = 0,
        },
    };
    dev_manager_create_legacy_adv_object(&adv_param);
}

static void prf_added_handler(struct profile_added_evt *evt)
{
    LOG_I("profile:%d, start handle:0x%x\n",evt->id,evt->start_hdl);
    switch(evt->id)
    {
    case PRF_FOTA_SERVER:
        prf_fota_server_callback_init(prf_fota_server_callback);
        create_adv_obj();
    break;
    default:

    break;
    }
}

static void dev_manager_callback(enum dev_evt_type type, union dev_evt_u * evt)
{
    switch(type)
    {
    case STACK_INIT:
    {
        struct ble_stack_cfg cfg = {
            .private_addr = false,
            .controller_privacy = false,
        };
        dev_manager_stack_init(&cfg);
    }break;
    case STACK_READY:
        dev_manager_prf_fota_server_add(NO_SEC);
    break;
    case PROFILE_ADDED:
        prf_added_handler(&evt->profile_added);
    break;
    case ADV_OBJ_CREATED:
        LS_ASSERT(evt->obj_created.status == 0);
        adv_obj_hdl = evt->obj_created.handle;
        dev_manager_start_adv(adv_obj_hdl,advertising_data,sizeof(advertising_data),scan_response_data,sizeof(scan_response_data));
    break;
    case ADV_STOPPED:

    break;
    default:

    break;
    }
}

static void gap_manager_callback(enum gap_evt_type type, union gap_evt_u * evt, uint8_t con_idx)
{
    switch(type)
    {
    case CONNECTED:

    break;
    case DISCONNECTED:
        dev_manager_start_adv(adv_obj_hdl,advertising_data,sizeof(advertising_data),scan_response_data,sizeof(scan_response_data));
    break;
    case CONN_PARAM_REQ:
        //LOG_I
    break;
    case CONN_PARAM_UPDATED:

    break;
    case ADV_STOPPED:
        LOG_I("adv stopped");
    break;
    default:

    break;
    }


}

static void gatt_manager_callback(enum gatt_evt_type type, union gatt_evt_u * evt, uint8_t con_idx)
{


}

static bool need_ota()
{
    uint32_t ota_status;
    spi_flash_quad_io_read(OTA_INFO_OFFSET,(uint8_t *)&ota_status, sizeof(ota_status));
    return ota_status!=0xffffffff;
}


static void fota_service_start()
{
    sys_init_app();
    ble_init();
    dev_manager_init(dev_manager_callback);
    gap_manager_init(gap_manager_callback);
    gatt_manager_init(gatt_manager_callback);
    while(1)
    {
        ble_loop();
    }
}

static void boot_app()
{
    uint32_t *msp = (void *)APP_IMAGE_BASE;
    void (**reset_handler)(void) = (void *)(APP_IMAGE_BASE + 4);
    __set_MSP(*msp);
    (*reset_handler)();
}

void sbl_start()
{
    swd_pull_down();
    trim_val_load();
    if(need_ota())
    {
        fota_service_start();
    }else
    {
        struct fota_image_info image;
        if(ota_copy_info_get(&image))
        {
            fw_copy(&image);
            ota_settings_erase();
        }
        boot_app();
    }
}

XIP_BANNED int main()
{
    switch_to_rc32k();
    clk_switch();
    power_up_hardware_modules();
    remove_hw_isolation();
    __disable_irq();
    spi_flash_drv_var_init(false,false);
    spi_flash_init();
    spi_flash_qe_status_read_and_set();
    spi_flash_xip_start();
    lscache_cache_ctrl(0,1);
    sbl_start();
    return 0;
}
