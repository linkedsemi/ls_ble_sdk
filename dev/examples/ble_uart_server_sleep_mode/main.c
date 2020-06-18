#define LOG_TAG "MAIN"
#include "ls_ble.h"
#include "platform.h"
#include "prf_diss.h"
#include "log.h"
#include "ls_dbg.h"
#include "cpu.h"
#include "uart.h"
#include "builtin_timer.h"
#include <string.h>
#include "co_math.h"
#include "lsgpio.h"
#include "le501x.h"
#include "core_cm0.h"
#include "reg_syscfg.h"
#include "compile_flag.h"
#include "reg_lsqspi.h"
#include "spi_flash.h"
#include "sleep.h"
#include "field_manipulate.h"
#define UART_SVC_ADV_NAME "LS UART Server"
#define UART_SERVER_MAX_MTU  247
#define UART_SERVER_MTU_DFT  23
#define UART_SERVER_MAX_DATA_LEN (uart_server_mtu - 3)
#define UART_SVC_RX_MAX_LEN (UART_SERVER_MAX_MTU - 3)
#define UART_SVC_TX_MAX_LEN (UART_SERVER_MAX_MTU - 3)
#define UART_SVC_BUFFER_SIZE (1024)

#define UART_SERVER_TIMEOUT 50 // timer units: ms
#define DEEP_SLEEP_TIMEOUT  3000 // // timer units: ms
uint8_t cnt_uart_server_idle=0;
static const uint8_t ls_uart_svc_uuid_128[] = {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x01,0x00,0x40,0x6e};
static const uint8_t ls_uart_rx_char_uuid_128[] = {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x02,0x00,0x40,0x6e};
static const uint8_t ls_uart_tx_char_uuid_128[] = {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x03,0x00,0x40,0x6e};
static const uint8_t att_decl_char_array[] = {0x03,0x28};
static const uint8_t att_desc_client_char_cfg_array[] = {0x02,0x29};

enum uart_svc_att_db_handles
{
    UART_SVC_IDX_RX_CHAR,
    UART_SVC_IDX_RX_VAL,
    UART_SVC_IDX_TX_CHAR,
    UART_SVC_IDX_TX_VAL,
    UART_SVC_IDX_TX_NTF_CFG,
    UART_SVC_ATT_NUM
};

static struct att_decl ls_uart_server_att_decl[UART_SVC_ATT_NUM] =
{
    [UART_SVC_IDX_RX_CHAR] = {
        .uuid = att_decl_char_array,
        .max_len = 0, 
        .char_prop.rd_en = 1,
        .uuid_len = UUID_LEN_16BIT,
    },
    [UART_SVC_IDX_RX_VAL] = {
        .uuid = ls_uart_rx_char_uuid_128,
        .max_len = UART_SVC_RX_MAX_LEN,
        .char_prop.wr_cmd = 1,
        .char_prop.wr_req = 1,
        .uuid_len = UUID_LEN_128BIT,
    },
    [UART_SVC_IDX_TX_CHAR] = {
        .uuid = att_decl_char_array,
        .max_len = 0,
        .char_prop.rd_en = 1,
        .uuid_len = UUID_LEN_16BIT,
    },
    [UART_SVC_IDX_TX_VAL] = {
        .uuid = ls_uart_tx_char_uuid_128,
        .max_len = UART_SVC_TX_MAX_LEN,
        .char_prop.ntf_en = 1,
        .uuid_len = UUID_LEN_128BIT,
    },
    [UART_SVC_IDX_TX_NTF_CFG] = {
        .uuid = att_desc_client_char_cfg_array,
        .max_len = 0,
        .char_prop.rd_en = 1,
        .char_prop.wr_req = 1,
        .uuid_len = UUID_LEN_16BIT,
    },
};
static struct svc_decl ls_uart_server_svc =
{
    .uuid = ls_uart_svc_uuid_128,
    .att = ls_uart_server_att_decl,
    .nb_att = UART_SVC_ATT_NUM,
    .uuid_len = UUID_LEN_128BIT,
};
static struct gatt_svc_env ls_uart_server_svc_env;
static uint8_t connect_id = 0xff; 
static uint8_t uart_server_rx_byte;
static uint8_t uart_server_buf[UART_SVC_BUFFER_SIZE];
static uint16_t uart_server_rx_index = 0;
static UART_HandleTypeDef UART_Server_Config; 
static bool uart_server_tx_busy;
static bool uart_server_ntf_done = true;
static uint16_t uart_server_mtu = UART_SERVER_MTU_DFT;
static struct builtin_timer *uart_server_timer_inst = NULL;
static struct builtin_timer *deep_sleep_timer_inst = NULL;

static void ls_deep_sleep_timer_cb(void *param);
extern struct sleep_wakeup_type env_sleep_wkup;

static uint8_t adv_obj_hdl;
static uint8_t advertising_data[28];  //LinkedSemi
static uint8_t scan_response_data[31];

static void ls_uart_server_init(void);
static void ls_uart_server_timer_cb(void *param);
static void ls_uart_init(void);
static void ls_uart_server_read_req_ind(uint8_t att_idx, uint8_t con_idx);
static void ls_uart_server_write_req_ind(uint8_t att_idx, uint8_t con_idx, uint16_t length, uint8_t const *value);
static void ls_uart_server_send_notification(void);

static void ls_uart_init(void)
{
   UART_Server_Config.UARTX = UART1;
    UART_Server_Config.Init.BaudRate = UART_BAUDRATE_115200;
    UART_Server_Config.Init.MSBEN = 0;
    UART_Server_Config.Init.Parity = UART_NOPARITY;
    UART_Server_Config.Init.StopBits = UART_STOPBITS1;
    UART_Server_Config.Init.WordLength = UART_BYTESIZE8;
    HAL_UART_Init(&UART_Server_Config);
}

XIP_BANNED void ls_uart_deinit(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE(); 
//  PB00 UART1_RX
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode= GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_INPUT_PULLUP;
    HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);

//  PB01  UART1_TX
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Driver_Pwr = GPIO_INPUT_PULLUP;
    HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);

    __HAL_RCC_GPIOA_CLK_DISABLE(); 
}

static void ls_uart_server_init(void)
{
    uart_server_timer_inst = builtin_timer_create(ls_uart_server_timer_cb);
    builtin_timer_start(uart_server_timer_inst, UART_SERVER_TIMEOUT, NULL);
}

static void ls_deep_sleep_timer_init(void)
{
    deep_sleep_timer_inst = builtin_timer_create(ls_deep_sleep_timer_cb);
}


static void  ls_deep_sleep_timer_cb (void *param)
{
    enter_deep_sleep_lvl3_mode(&env_sleep_wkup);
}

static void ls_uart_server_timer_cb(void *param)
{
     if(connect_id != 0xff)
    {
        enter_critical();
        // LOG_I("uart timer out, length=%d", uart_server_rx_index);
        ls_uart_server_send_notification();
        exit_critical();
    }

    if(uart_server_timer_inst)
    {
        builtin_timer_start(uart_server_timer_inst, UART_SERVER_TIMEOUT, NULL); 
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart,void *tx_arg)
{
    uart_server_tx_busy = false;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart,void *rx_arg)
{
    if(uart_server_rx_index < UART_SVC_BUFFER_SIZE)
    {
        uart_server_buf[uart_server_rx_index++] = uart_server_rx_byte;
    }
    else
    {   
        LOG_I("uart server rx buffer overflow!");
    }
    HAL_UART_Receive_IT(&UART_Server_Config, &uart_server_rx_byte, 1, NULL);
}

static void ls_uart_server_read_req_ind(uint8_t att_idx, uint8_t con_idx)
{
    uint16_t handle = 0;
    if(att_idx == UART_SVC_IDX_TX_NTF_CFG)
    {
        handle = gatt_manager_get_svc_att_handle(&ls_uart_server_svc_env, att_idx);
        gatt_manager_server_read_req_reply(con_idx, handle, 0, NULL, 0);
    }
}

static void ls_uart_server_write_req_ind(uint8_t att_idx, uint8_t con_idx, uint16_t length, uint8_t const *value)
{
    uint16_t handle = 0;
    handle = gatt_manager_get_svc_att_handle(&ls_uart_server_svc_env, att_idx);
    gatt_manager_server_write_confirm(con_idx, handle, 0);
    if(att_idx == UART_SVC_IDX_RX_VAL)
    { 
        if(uart_server_tx_busy)
        {
            LOG_I("Uart tx busy, data discard!");
        }
        else
        {
            uart_server_tx_busy = true;
            HAL_UART_Transmit_IT(&UART_Server_Config, (uint8_t*)value, length, NULL);
        } 
    } 
}

static void ls_uart_server_send_notification(void)
{
    if(uart_server_rx_index > 0 && uart_server_ntf_done)
    {
        uart_server_ntf_done = false;
        uint16_t handle = gatt_manager_get_svc_att_handle(&ls_uart_server_svc_env, UART_SVC_IDX_TX_VAL);
        uint16_t tx_len = uart_server_rx_index > co_min(UART_SERVER_MAX_DATA_LEN, UART_SVC_TX_MAX_LEN) ? co_min(UART_SERVER_MAX_DATA_LEN, UART_SVC_TX_MAX_LEN) : uart_server_rx_index;
        uart_server_rx_index -= tx_len;
        gatt_manager_server_send_notification(connect_id, handle, uart_server_buf, tx_len, NULL);
        memcpy((void*)&uart_server_buf[0], (void*)&uart_server_buf[tx_len], uart_server_rx_index);
    }    
}

static void gap_manager_callback(enum gap_evt_type type,union gap_evt_u *evt,uint8_t con_idx)
{
    switch(type)
    {
    case CONNECTED:
        connect_id = con_idx;
        LOG_I("connected!");
        builtin_timer_stop(deep_sleep_timer_inst);
    break;
    case DISCONNECTED:
        connect_id = 0xff;
        uart_server_mtu = UART_SERVER_MTU_DFT;
        dev_manager_start_adv(adv_obj_hdl,advertising_data,sizeof(advertising_data),scan_response_data,sizeof(scan_response_data));
        LOG_I("disconnected!");
        HAL_UART_Receive_IT(&UART_Server_Config, &uart_server_rx_byte, 1, NULL); 
        builtin_timer_start(deep_sleep_timer_inst, DEEP_SLEEP_TIMEOUT, NULL);
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

static void gatt_manager_callback(enum gatt_evt_type type,union gatt_evt_u *evt,uint8_t con_idx)
{
    switch (type)
    {
    case SERVER_READ_REQ:
        LOG_I("read req");
        ls_uart_server_read_req_ind(evt->server_read_req.att_idx, con_idx);
    break;
    case SERVER_WRITE_REQ:
        LOG_I("write req");
        ls_uart_server_write_req_ind(evt->server_write_req.att_idx, con_idx, evt->server_write_req.length, evt->server_write_req.value);
    break;
    case SERVER_NOTIFICATION_DONE:
        uart_server_ntf_done = true;
        LOG_I("ntf done");
    break;
    case SERVER_MTU_CHANGED_INDICATION:
        uart_server_mtu = evt->mtu_changed_ind.mtu;
        LOG_I("mtu: %d", uart_server_mtu);
    break;
    default:
        LOG_I("Event not handled!");
        break;
    }
}

static void create_adv_obj()
{
    struct legacy_adv_obj_param adv_param = {
        .adv_intv_min = 0x20,
        .adv_intv_max = 0x20,
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

static void dev_manager_callback(enum dev_evt_type type,union dev_evt_u *evt)
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
    }
    break;
    case STACK_READY:
    {
        uint8_t addr[6];
        bool type;
        dev_manager_get_identity_bdaddr(addr,&type);
        LOG_I("type:%d,addr:",type);
        LOG_HEX(addr,sizeof(addr));
        dev_manager_add_service(&ls_uart_server_svc);
        ls_uart_init(); 
        HAL_UART_Receive_IT(&UART_Server_Config, &uart_server_rx_byte, 1, NULL); 
        ls_uart_server_init();
        ls_deep_sleep_timer_init();      
    }
    break;
    case SERVICE_ADDED:
        gatt_manager_svc_register(evt->service_added.start_hdl, UART_SVC_ATT_NUM, &ls_uart_server_svc_env);
        create_adv_obj();
    break;
    case ADV_OBJ_CREATED:
        LS_ASSERT(evt->obj_created.status == 0);
        adv_obj_hdl = evt->obj_created.handle;
        dev_manager_start_adv(adv_obj_hdl,advertising_data,sizeof(advertising_data),scan_response_data,sizeof(scan_response_data));
        builtin_timer_start(deep_sleep_timer_inst, DEEP_SLEEP_TIMEOUT, NULL);
    break;
    case ADV_STOPPED:
    
    break;
    case SCAN_STOPPED:
    
    break;
    default:

    break;
    }
}
	 
void gpio_init_wkup_cfg(uint32_t trig_src  ,uint32_t trig_edge)
{
     env_sleep_wkup.trig_src = trig_src;
     env_sleep_wkup.trig_edge = trig_edge;
     MODIFY_REG(SYSCFG->PMU_WKUP,(env_sleep_wkup.trig_src<<WKUP_EN_POS),((env_sleep_wkup.trig_src)<<WKUP_EN_POS));
     MODIFY_REG(SYSCFG->PMU_WKUP,(env_sleep_wkup.trig_src<<WKUP_EDGE_POS),((env_sleep_wkup.trig_edge)<<WKUP_EDGE_POS));
}	


int main()
{
    sys_init_app();
    gpio_init_wkup_cfg(PA00_IO_WKUP, PA00_IO_WKUP_EDGE_RISING);


    ble_init();
    dev_manager_init(dev_manager_callback);
    gap_manager_init(gap_manager_callback);
    gatt_manager_init(gatt_manager_callback);
    while(1)
    {
        ble_loop();
    }
}


