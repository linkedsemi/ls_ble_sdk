#define LOG_TAG "MAIN"
#include <string.h>
#include <stdio.h>
#include "ls_ble.h"
#include "platform.h"
#include "ls_mesh.h"
#include "log.h"
#include "ls_dbg.h"
#include "lsgpio.h"
#include "spi_flash.h"
#include "tinyfs.h"
#include "sha256.h"
#include "constants.h"
#include "lscrypt.h"
#include "field_manipulate.h"
#include "reg_lscrypt.h"

#define ALI_COMPANY_ID 0x01a8
#define COMPA_DATA_PAGES 1
#define TRIPLE_PID_LEN 4
#define TRIPLE_MAC_LEN 6
#define TRIPLE_SECRET_LEN 16
#define ALI_AUTH_VALUE_LEN 16
#define ALI_TRIPLE_SUM_LEN (TRIPLE_PID_LEN + TRIPLE_MAC_LEN + TRIPLE_SECRET_LEN)
#define PROV_AUTH_ACCEPT 1
#define PROV_AUTH_NOACCEPT 0
#define TMALL_TRITUPLE_FLASH_OFFSET (0x0200)

uint8_t vendor_tid = 0;
static uint16_t CurrentLevel = 65535;
uint8_t CurrentOnoffState = 0;
static bool timer_state_flag = false;
#define LED_0 GPIO_PIN_8
#define LED_1 GPIO_PIN_9

struct mesh_model_info model_env;
#define RECORD_KEY1 1
tinyfs_dir_t dir0;
SIGMESH_ModelHandle_TypeDef ModelHandle_OnOff;
SIGMESH_ModelHandle_TypeDef ModelHandle_Lightness;
SIGMESH_NodeInfo_TypeDef Node_Get_Proved_State = 0;
SIGMESH_NodeInfo_TypeDef Node_Proved_State = 0;

static uint8_t ali_pid[TRIPLE_PID_LEN] = {0};
static uint32_t ali_pid_u32 = 10638;
static uint8_t ali_mac[TRIPLE_MAC_LEN] = {0xf8, 0xa7, 0x63, 0x87, 0xa7, 0x54};
static uint8_t ali_secret[TRIPLE_SECRET_LEN] = {0xf1, 0x7b, 0xc5, 0xe3, 0x11, 0xc0, 0xde, 0xd2, 0x3b, 0xa0, 0x34, 0x4e, 0x8a, 0xe3, 0xb7, 0x4a};
static uint8_t ali_authvalue[ALI_AUTH_VALUE_LEN] = {0x85, 0x29, 0xec, 0xc7, 0xef, 0x7b, 0x8c, 0x90, 0xda, 0x61, 0xea, 0xba, 0xaa, 0xd6, 0xde, 0xfe};

//static uint8_t ali_pid[TRIPLE_PID_LEN] = {0x8e,0x29,0x00,0x00};
//static uint32_t ali_pid_u32 = 10638;
//static uint8_t ali_mac[TRIPLE_MAC_LEN] = {0x70,0xa7,0x87,0x63,0xa7,0xf8};
//static uint8_t ali_secret[TRIPLE_SECRET_LEN] = {0xf1, 0x7b, 0xc5, 0xe3, 0x11, 0xc0, 0xde, 0xd2, 0x3b, 0xa0, 0x34, 0x4e, 0x8a, 0xe3, 0xb7, 0x4a};
//static uint8_t ali_authvalue[ALI_AUTH_VALUE_LEN] = {0x11,0x65,0xa3,0x06,0xb3,0x9a,0x3d,0xba,0xbd,0x6c,0x00,0x05,0x5c,0x12,0x8c,0x72};

uint8_t rsp_data_info[40] = {0};

uint8_t tmall_app_key_lid = 0;
uint8_t tmall_ModelHandle = 0;
uint16_t tmall_source_addr = 0;

void hextostring(const uint8_t *source, char *dest, int len)
{
    int i;
    char tmp[3];

    for (i = 0; i < len; i++)
    {
        sprintf(tmp, "%02x", (unsigned char)source[i]);
        memcpy(&dest[i * 2], tmp, 2);
    }
}

static uint8_t gen_ali_authValue(void)
{
    uint8_t status = TC_CRYPTO_FAIL;
    char cal_ble_key_input[55] = ""; // pid + ',' + mac + ',' + secret = 8+1+12+1+32+'\0'
    char mac_str[(TRIPLE_MAC_LEN << 1) + 1] = "";
    char secret_str[(TRIPLE_SECRET_LEN << 1) + 1] = "";
    uint8_t tmp_arry[ALI_AUTH_VALUE_LEN] = {0};

    uint8_t ali_trituple[ALI_TRIPLE_SUM_LEN] = {0};
    spi_flash_fast_read(TMALL_TRITUPLE_FLASH_OFFSET, &ali_trituple[0], ALI_TRIPLE_SUM_LEN);
    if ((ali_trituple[0] != 0xff) && (ali_trituple[1] != 0xff) && (ali_trituple[2] != 0xff))
    {
        ali_pid_u32 = (((uint32_t)ali_trituple[0])<<24) | (((uint32_t)ali_trituple[1])<< 16) | (((uint32_t)ali_trituple[2])<< 8) | (((uint32_t)ali_trituple[3]));
        memcpy(&ali_mac[0], &ali_trituple[TRIPLE_PID_LEN], TRIPLE_MAC_LEN);
        memcpy(&ali_secret[0], &ali_trituple[TRIPLE_PID_LEN+TRIPLE_MAC_LEN], TRIPLE_SECRET_LEN);
    }

    hextostring(ali_mac, mac_str, TRIPLE_MAC_LEN);
    hextostring(ali_secret, secret_str, TRIPLE_SECRET_LEN);

    sprintf(&cal_ble_key_input[0], "%08x,%s,%s", ali_pid_u32, mac_str, secret_str);

    status = sha256_handler(&cal_ble_key_input[0], &ali_authvalue[0]);

    memcpy(&ali_pid[0], ((uint8_t *)&ali_pid_u32), TRIPLE_PID_LEN);

    memcpy(tmp_arry, ali_mac, TRIPLE_MAC_LEN);
    for (uint8_t j = 0; j < TRIPLE_MAC_LEN; j++)
    {
        ali_mac[j] = tmp_arry[TRIPLE_MAC_LEN - 1 - j];
    }

    memcpy(tmp_arry, ali_authvalue, ALI_AUTH_VALUE_LEN);
    for (uint8_t j = 0; j < ALI_AUTH_VALUE_LEN; j++)
    {
        ali_authvalue[j] = tmp_arry[ALI_AUTH_VALUE_LEN - 1 - j];
    }

    if (status == TC_CRYPTO_FAIL)
    {
        memset(&ali_authvalue[0], 0, ALI_AUTH_VALUE_LEN);
        return (0);
    }

    return (1);
}

void auto_check_unbind(void)
{
    uint16_t length = 1;
    uint8_t coutinu_power_up_num = 0;
    tinyfs_mkdir(&dir0, ROOT_DIR, 5);
    tinyfs_read(dir0, RECORD_KEY1, &coutinu_power_up_num, &length);
    LOG_I("coutinu_power_up_num:%d", coutinu_power_up_num);

    if (coutinu_power_up_num > 4)
    {
        coutinu_power_up_num = 0;
        tinyfs_write(dir0, RECORD_KEY1, &coutinu_power_up_num, sizeof(coutinu_power_up_num));
        SIGMESH_UnbindAll();
    }
    else
    {
        coutinu_power_up_num++;
        tinyfs_write(dir0, RECORD_KEY1, &coutinu_power_up_num, sizeof(coutinu_power_up_num));
        tinyfs_write_through();
    }
}

void enable_tx_unprov_beacon(void)
{
    struct bcn_start_unprov_param info;
    info.DevUuid[0] = 0xA8;
    info.DevUuid[1] = 0x01;
    info.DevUuid[2] = 0x71;
    memcpy(&info.DevUuid[3], &ali_pid[0], TRIPLE_PID_LEN);
    memcpy(&info.DevUuid[3 + TRIPLE_PID_LEN], &ali_mac[0], TRIPLE_MAC_LEN);
    info.DevUuid[13] = 0x02;
    info.DevUuid[14] = 0x00;
    info.DevUuid[15] = 0x00;
    info.UriHash = 0xd97478b3;
    info.OobInfo = 0x0000;
    info.UriHash_Present = true;
    start_tx_unprov_beacon(&info);
}

void disable_tx_unprov_beacon(void)
{
    stop_tx_unprov_beacon();
}

static void led_gpio_func(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

    /*Configure GPIO pin : PB8 */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OT = GPIO_OUTPUT_PUSHPLL;
    GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_1_4_MAX_DRIVER;
    HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : PB9 */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OT = GPIO_OUTPUT_PUSHPLL;
    GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_1_4_MAX_DRIVER;
    HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);
}

static void LightControl(uint16_t led_sel, uint8_t OnOff)
{
    if (OnOff)
    {
        HAL_GPIO_WritePin(LSGPIOB, ((led_sel == LED_0) ? LED_0 : LED_1), GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(LSGPIOB, ((led_sel == LED_0) ? LED_0 : LED_1), GPIO_PIN_RESET);
    }
}

static void gap_manager_callback(enum gap_evt_type type, union gap_evt_u *evt, uint8_t con_idx)
{
    switch (type)
    {
    case CONNECTED:

        break;
    case DISCONNECTED:

        break;
    case CONN_PARAM_REQ:
        //LOG_I
        break;
    case CONN_PARAM_UPDATED:

        break;
    default:

        break;
    }
}

static void gatt_manager_callback(enum gatt_evt_type type, union gatt_evt_u *evt, uint8_t con_idx)
{
}

static void vendor_indication_handler(uint8_t const *info, uint16_t const info_len)
{
    struct rsp_model_info rsp;
    rsp.opcode = APP_MESH_VENDOR_INDICATION;
    rsp.app_key_lid = tmall_app_key_lid;
    rsp.ModelHandle = tmall_ModelHandle;
    rsp.dest_addr = tmall_source_addr;
    rsp.len = info_len;
    memcpy(&(rsp.info[1]), info, info_len);
    rsp.info[0] = vendor_tid;
    rsp.len += 1; //1 :tid_len
    rsp_model_info_ind(&rsp);
    vendor_tid++;
}

static void mesh_manager_callback(enum mesh_evt_type type, union mesh_evt_u *evt)
{
    switch (type)
    {
    case MESH_ACTIVE_ENABLE:
    {
        TIMER_Set(2, 3000); //clear power up num
    }
    break;
    case MESH_ACTIVE_DISABLE:
    {
        SIGMESH_UnbindAll();
        ls_mesh_platform_reset();
    }
    break;
    case MESH_ACTIVE_REGISTER_MODEL:
    {
        for (uint8_t i = 0; i < model_env.nb_model; i++)
        {
            model_env.info[i].model_lid = evt->loc_id_param.model_lid[i];
        }
    }
    break;
    case MESH_ACTIVE_MODEL_PUBLISH:
    {
    }
    break;
    case MESH_ACTIVE_MODEL_GROUP_MEMBERS:
    {
        model_subscribe(model_env.info[0].model_lid, 0xC000); //group address 0xc000
        model_subscribe(model_env.info[1].model_lid, 0xC000); //group address 0xc000                                                      //        model_subscribe(model_env.info[1].model_lid, 0xC000);   //group address 0xc000
                                                              //        model_subscribe(model_env.info[2].model_lid, 0xC000);   //group address 0xc007
    }
    break;
    case MESH_ACTIVE_MODEL_RSP_SEND:
    {
    }
    break;
    case MESH_ACTIVE_LPN_START:
    {
    }
    break;
    case MESH_ACTIVE_LPN_STOP:
    {
    }
    break;
    case MESH_ACTIVE_LPN_SELECT_FRIEND:
    {
    }
    break;
    case MESH_ACTIVE_PROXY_CTL:
    {
    }
    break;
    case MESH_ACTIVE_STORAGE_LOAD:
    {
        Node_Get_Proved_State = evt->st_proved.proved_state;
        if (Node_Get_Proved_State == PROVISIONED_OK)
        {
            vendor_indication_handler(&rsp_data_info[0], 40);
            HAL_GPIO_WritePin(LSGPIOB, LED_0 | LED_1, GPIO_PIN_SET);
        }
    }
    break;
    case MESH_ACTIVE_STORAGE_SAVE:
        break;
    case MESH_ACTIVE_STORAGE_CONFIG:
        break;
    case MESH_GET_PROV_INFO:
    {
        struct mesh_prov_info param;
        param.DevUuid[0] = 0xA8;
        param.DevUuid[1] = 0x01;
        param.DevUuid[2] = 0x71;
        memcpy(&param.DevUuid[3], &ali_pid[0], TRIPLE_PID_LEN);
        memcpy(&param.DevUuid[3 + TRIPLE_PID_LEN], &ali_mac[0], TRIPLE_MAC_LEN);
        param.DevUuid[13] = 0x02;
        param.DevUuid[14] = 0x00;
        param.DevUuid[15] = 0x00;
        param.UriHash = 0xd97478b3;
        param.OobInfo = 0x0000;
        param.PubKeyOob = 0x00;
        param.StaticOob = 0x01;
        param.OutOobSize = 0x00;
        param.InOobSize = 0x00;
        param.OutOobAction = 0x0000;
        param.InOobAction = 0x0000;
        param.Info = 0x00;
        set_prov_param(&param);
    }
    break;
    case MESH_GET_PROV_AUTH_INFO:
    {
        struct mesh_prov_auth_info param;
        param.Adopt = PROV_AUTH_ACCEPT;
        memcpy(&param.AuthBuffer[0], &ali_authvalue[0], ALI_AUTH_VALUE_LEN);
        param.AuthSize = ALI_AUTH_VALUE_LEN;
        set_prov_auth_info(&param);
    }
    break;
    case MESH_REPORT_ATTENTION_STATE:
    {
    }
    break;
    case MESH_REPOPT_PROV_RESULT:
    {
        vendor_indication_handler(&rsp_data_info[0], 40);
    }
    break;
    case MESH_ACCEPT_MODEL_INFO:
    {
        switch (evt->rx_msg.opcode)
        {
        case APP_MESH_VENDOR_TRANSPARENT_MSG:
        case APP_MESH_VENDOR_SET:
        case APP_MESH_VENDOR_INDICATION:
        case APP_MESH_VENDOR_CONFIRMATION:
        {
            //                   struct rsp_model_info rsp;
            //                  vendor_set = (struct app_mesh_led_vendor_model_set_new_t *)ind->msg;
            //vendor_set->attr_parameter = (uint8_t *)&ind->msg[3];

            if (evt->rx_msg.opcode == APP_MESH_VENDOR_CONFIRMATION)
            {
                //	Shift_report_item();
                //	ReportTmall_info.report_flag=0;		//ÊÕµ½Ó¦´ð£¬Í£Ö¹ÖØ¸´ÉÏ±¨
            }

            //vendor_set_msg_handler(evt);

            if (evt->rx_msg.opcode == APP_MESH_VENDOR_SET)
            {
                //rsp.ModelHandle = model_env.info[0].model_lid;
                //rsp.opcode = APP_MESH_VENDOR_STATUES;
                //rsp.len = evt->rx_msg.rx_info_len;
                //memcpy(&(rsp.info[0]),&(evt->rx_msg.info[0]),rsp.len);
                //rsp_model_info_ind(&rsp);
            }
            tmall_app_key_lid = evt->rx_msg.app_key_lid;
            tmall_ModelHandle = evt->rx_msg.ModelHandle;
            tmall_source_addr = evt->rx_msg.source_addr;

            if (evt->rx_msg.opcode == APP_MESH_VENDOR_SET)
            {
                struct rsp_model_info vendor_rsp;
                vendor_rsp.ModelHandle = model_env.info[0].model_lid;
                vendor_rsp.app_key_lid = tmall_app_key_lid;
                vendor_rsp.opcode = APP_MESH_VENDOR_STATUES;
                vendor_rsp.len = evt->rx_msg.rx_info_len;
                vendor_rsp.dest_addr = tmall_source_addr;
                memcpy(&(vendor_rsp.info[0]), &(evt->rx_msg.info[0]), vendor_rsp.len);
                rsp_model_info_ind(&vendor_rsp);
            }
            //      vendor_indication_handler(&rsp_data_info[0], 40);
        }
        break;
        case GENERIC_ONOFF_SET: // On Off
        {
            uint8_t *pOnOff = &(evt->rx_msg.info[0]);
            CurrentOnoffState = *pOnOff;

            if (evt->rx_msg.ModelHandle == model_env.info[0].model_lid)
            {
                struct rsp_model_info onoff_rsp;
                onoff_rsp.ModelHandle = model_env.info[0].model_lid;
                onoff_rsp.app_key_lid = evt->rx_msg.app_key_lid;
                onoff_rsp.opcode = GENERIC_ONOFF_STATUS;
                //                rsp.len = evt->rx_msg.rx_info_len;
                onoff_rsp.dest_addr = evt->rx_msg.source_addr;
                onoff_rsp.len = 1; //evt->rx_msg.rx_info_len;
                memcpy(&onoff_rsp.info[0], &evt->rx_msg.info[0], onoff_rsp.len);
                rsp_model_info_ind(&onoff_rsp);

                if (!timer_state_flag)
                {
                    timer_state_flag = true;
                }
                else
                {
                    TIMER_Cancel(2);
                }
                TIMER_Set(2, 180000);
            }
        }
        break;
        case LIGHT_LIGHTNESS_SET: // Lightness
        {
            uint16_t *pLevel = (uint16_t *)&(evt->rx_msg.info[0]);
            uint8_t Status[2];

            if (evt->rx_msg.ModelHandle == ModelHandle_Lightness)
            {
                CurrentLevel = *pLevel;

                // printf("Set Lightness Level to %x\n", CurrentLevel);

                //                    LightControl(1);

                Status[0] = CurrentLevel & 0xFF;
                Status[1] = CurrentLevel >> 8;

                struct rsp_model_info lightness_rsp;
                lightness_rsp.ModelHandle = ModelHandle_Lightness;
                lightness_rsp.opcode = LIGHT_LIGHTNESS_STATUS;
                lightness_rsp.len = 2;
                memcpy(&(lightness_rsp.info[0]), &Status, lightness_rsp.len);

                rsp_model_info_ind(&lightness_rsp);
            }
        }
        break;
        default:
            break;
        }
    }
    break;
    case MESH_REPORT_TIMER_STATE:
    {
        if (1 == evt->mesh_timer_state.timer_id)
        {
            TIMER_Cancel(1);
            ls_mesh_platform_reset();
        }
        else if (2 == evt->mesh_timer_state.timer_id)
        {
            uint8_t clear_power_on_num = 0;
            TIMER_Cancel(2);
            tinyfs_write(dir0, RECORD_KEY1, &clear_power_on_num, sizeof(clear_power_on_num));
            tinyfs_write_through();
        }
    }
    break;
    default:
        break;
    }
}

static void dev_manager_callback(enum dev_evt_type type, union dev_evt_u *evt)
{
    switch (type)
    {
    case STACK_INIT:
    {
        struct ble_stack_cfg cfg = {
            .private_addr = true,
            .controller_privacy = false,
        };
        dev_manager_stack_init(&cfg);
    }
    break;

    case STACK_READY:
    {
        uint8_t addr[6];
        struct ls_mesh_cfg feature = {
            .MeshFeatures = EN_RELAY_NODE | EN_MSG_API,
            .MeshCompanyID = ALI_COMPANY_ID,
            .MeshProID = 0,
            .MeshProVerID = 0,
            .MeshLocDesc = 0,
            .NbCompDataPage = COMPA_DATA_PAGES,
        };

        bool type;
        dev_manager_get_identity_bdaddr(addr, &type);
        dev_manager_prf_mesh_add(NO_SEC, &feature);
    }
    break;

    case SERVICE_ADDED:
    {
    }
    break;
    case PROFILE_ADDED:
    {
        prf_mesh_callback_init(mesh_manager_callback);

        //       model_env.nb_model = 3;
        model_env.nb_model = 2;
        model_env.info[0].model_id = GENERIC_ONOFF_SERVER;
        model_env.info[0].element_id = 0;
        //       model_env.info[1].model_id = LIGHTNESS_SERVER;
        //       model_env.info[1].element_id = 0;
        model_env.info[1].model_id = VENDOR_TMALL_SERVER;
        model_env.info[1].element_id = 0;
        ls_mesh_init(&model_env);
    }
    break;
    case ADV_OBJ_CREATED:
    {
    }
    break;
    case ADV_STOPPED:
    {
    }
    break;
    case SCAN_STOPPED:
    {
    }
    break;
    default:

        break;
    }
}

int main()
{
    led_gpio_func();
    sys_init_app();
    lscrypt_init();
    gen_ali_authValue();
    ble_init();
    auto_check_unbind();
    dev_manager_init(dev_manager_callback);
    gap_manager_init(gap_manager_callback);
    gatt_manager_init(gatt_manager_callback);
    while (1)
    {
        ble_loop();
    }
    return 0;
}
