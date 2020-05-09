#define LOG_TAG "MAIN"
#include "string.h"
#include "ls_ble.h"
#include "platform.h"
#include "ls_mesh.h"
#include "log.h"
#include "ls_dbg.h"
#include "lsgpio.h"

#define ALI_COMPANY_ID  0x01a8
#define COMPA_DATA_PAGES  1
#define NUM_TRIPLE_PID 4
#define NUM_TRIPLE_MAC 6
#define NUM_TRIPLE_KEY 16
#define PROV_AUTH_ACCEPT 1
#define PROV_AUTH_NOACCEPT 0
SIGMESH_ModelHandle_TypeDef ModelHandle_OnOff;
SIGMESH_ModelHandle_TypeDef ModelHandle_Lightness;
SIGMESH_NodeInfo_TypeDef Node_Proved_State;
static const uint8_t ali_pid[NUM_TRIPLE_PID] = {0x1a, 0x0d, 0x00, 0x00};
static const uint8_t ali_mac[NUM_TRIPLE_MAC] = {0x87, 0x79, 0x1a, 0xca, 0xd2, 0x38};
static const uint8_t ali_key[NUM_TRIPLE_KEY] = {0x8f, 0x59, 0xcf, 0x72, 0x4e, 0xae, 0xb5, 0x50, 0x45, 0xd2, 0xad, 0x46, 0xce, 0x3c, 0x8f, 0x54};
static uint16_t CurrentLevel = 65535;
uint8_t CurrentOnoffState = 0;
static bool timer_state_flag = false;
#define LED_0  GPIO_PIN_8
#define LED_1  GPIO_PIN_9

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


static void LightControl(uint16_t led_sel,uint8_t OnOff)
{
    if (OnOff)
    {
        HAL_GPIO_WritePin(LSGPIOB,((led_sel == LED_0)?LED_0:LED_1), GPIO_PIN_SET);
    }
    else
    {
         HAL_GPIO_WritePin(LSGPIOB,((led_sel == LED_0)?LED_0:LED_1), GPIO_PIN_RESET);
    }
}

static void gap_manager_callback(enum gap_evt_type type,union gap_evt_u *evt,uint8_t con_idx)
{
    switch(type)
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

static void gatt_manager_callback(enum gatt_evt_type type,union gatt_evt_u *evt,uint8_t con_idx)
{
    
}


static void mesh_manager_callback(enum mesh_evt_type type,union mesh_evt_u *evt)
{
    switch (type)
    {
       case MESH_ACTIVE_ENABLE:
       {}
       break;
       case MESH_ACTIVE_DISABLE:
       {
 //        SIGMESH_UnbindAll();
        ls_mesh_platform_reset();  
       }
       break;
       case MESH_ACTIVE_REGISTER_MODEL:
       {
          ModelHandle_Lightness = evt->loc_id_param.model_lid[evt->loc_id_param.nb_model-1];
          ModelHandle_OnOff = evt->loc_id_param.model_lid[evt->loc_id_param.nb_model-2];
       }
       break;
       case MESH_ACTIVE_MODEL_PUBLISH:
       {}
       break;
       case MESH_ACTIVE_MODEL_SUBSCRIBE:
       {
        model_subscribe(ModelHandle_OnOff, 0xC000);
//        model_subscribe(ModelHandle_OnOff, 0xCFFF);
//        model_subscribe(ModelHandle_Lightness, 0xC000);
        model_subscribe(ModelHandle_Lightness, 0xCFFF);
       }
       break;
       case MESH_ACTIVE_MODEL_RSP_SEND:
       {}
       break;
       case MESH_ACTIVE_LPN_START:
       {}
       break;
       case MESH_ACTIVE_LPN_STOP:
       {}
       break;
       case MESH_ACTIVE_LPN_SELECT_FRIEND:
       {}
       break;
       case MESH_ACTIVE_PROXY_CTL:
       {}
       break;
       case MESH_ACTIVE_STORAGE_LOAD:
       {
         Node_Proved_State = evt->st_proved.proved_state;
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
           memcpy(&param.DevUuid[3], &ali_pid[0],NUM_TRIPLE_PID);
           memcpy(&param.DevUuid[3+NUM_TRIPLE_PID], &ali_mac[0],NUM_TRIPLE_MAC);
           param.DevUuid[13] = 0x02;
	       param.DevUuid[14] = 0x00;
	       param.DevUuid[15] = 0x00;
           param.UriHash = 0xd97478b3;
           param.OobInfo = 0x0000;
           param.PubKeyOob=0x00;
           param.StaticOob=0x01;
           param.OutOobSize=0x00;
           param.InOobSize=0x00;
           param.OutOobAction=0x0000;
           param.InOobAction=0x0000;
           param.Info=0x00;
           set_prov_param(&param);
          }
          break;
        case MESH_GET_PROV_AUTH_INFO:
        {
          struct mesh_prov_auth_info param;
          param.Adopt = PROV_AUTH_ACCEPT;
          memcpy(&param.AuthBuffer[0],&ali_key[0],NUM_TRIPLE_KEY);
          param.AuthSize = NUM_TRIPLE_KEY;
          set_prov_auth_info(&param);
        }
        break;
        case MESH_RESET_NODE:
        {}
        break;
        case MESH_REPORT_ATTENTION_STATE:
        {}
        break;
        case MESH_REPOPT_PROV_RESULT:
        {}
        break;
        case MESH_ACCEPT_MODEL_INFO:
        {
        switch (evt->rx_msg.opcode)
        {
        case 0x0282: // On Off
        {
            uint8_t *pOnOff = &(evt->rx_msg.info[0]);
            CurrentOnoffState = *pOnOff;

            if (evt->rx_msg.ModelHandle == ModelHandle_OnOff)
            {
                LightControl(LED_0, *pOnOff);
                LightControl(LED_1, *pOnOff);
                 struct rsp_model_info rsp;
                 rsp.ModelHandle = ModelHandle_OnOff;
                 rsp.opcode      = 0x0482;
                 rsp.len         =1;
                 memcpy(&rsp.info[0] ,&CurrentOnoffState,rsp.len);
                
                rsp_model_info_ind(&rsp);

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
        case 0x4c82: // Lightness
        {
            uint16_t *pLevel = (uint16_t *)&(evt->rx_msg.info[0]);
           uint8_t Status[2];

            if (evt->rx_msg.ModelHandle == ModelHandle_Lightness)
            {
                CurrentLevel = *pLevel;

                // printf("Set Lightness Level to %x\n", CurrentLevel);

 //               LightControl(1);

                Status[0] = CurrentLevel & 0xFF;
                Status[1] = CurrentLevel >> 8;
                
                struct rsp_model_info rsp;
                 rsp.ModelHandle = ModelHandle_Lightness;
                 rsp.opcode      = 0x4e82;
                 rsp.len         =2;
                 memcpy(&(rsp.info[0]) ,&Status, rsp.len);
                
                rsp_model_info_ind(&rsp);
            }
        }
        break;
        default:
            break;
        }  
        }
        break;
        case MESH_REPORT_TIMER_STATE:
        {}
        break;
       default:
       break;
    }
}

static void dev_manager_callback(enum dev_evt_type type,union dev_evt_u *evt)
{
    switch(type)
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
        struct ls_mesh_cfg feature= {
             .MeshFeatures = EN_RELAY_NODE|EN_MSG_API,
             .MeshCompanyID = ALI_COMPANY_ID,
             .MeshProID = 0,
             .MeshProVerID =0,
             .MeshLocDesc=0,
             .NbCompDataPage =COMPA_DATA_PAGES,
        };
        bool type;
        dev_manager_get_identity_bdaddr(addr,&type);
 //       LOG_I("type:%d,addr:",type);
 //       LOG_HEX(addr,sizeof(addr));

        dev_manager_prf_mesh_add(NO_SEC, &feature);
    }break;

    case SERVICE_ADDED:
    {}
    break;
    case PROFILE_ADDED:
    {
    prf_mesh_callback_init(mesh_manager_callback);    
    
    struct model_id_info req;
    req.nb_model =2;
    req.model_id[0] = 0x1000;
    req.model_id[1] = 0x1300;
    ls_mesh_init(&req);
    }
    break;
    case ADV_OBJ_CREATED:
    {}
    break;
    case ADV_STOPPED:
    {}
    break;
    case SCAN_STOPPED:
    {}
    break;
    default:

    break;
    }
    
}

       
int main()
{
    led_gpio_func();
    sys_init_app();
    ble_init();
    dev_manager_init(dev_manager_callback);
    gap_manager_init(gap_manager_callback);
    gatt_manager_init(gatt_manager_callback);
    while(1)
    {
        ble_loop();
    }
    return 0;
}
