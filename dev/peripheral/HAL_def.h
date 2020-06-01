/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __HAL_DEF
#define __HAL_DEF

/**
  * @brief  HAL Status structures definition
  */
typedef enum
{
    HAL_OK             = 0x00U,
    HAL_INVALIAD_PARAM    = 0x01U,
    HAL_BUSY           = 0x02U,
    HAL_STATE_ERROR    = 0x03U,
    HAL_INVALID_OPERATION,
    HAL_DMAC_NO_AVAILABLE_CHANNEL
} HAL_StatusTypeDef;

typedef enum
{
    DISABLE = 0,
    ENABLE = !DISABLE
} FunctionalState;

/**
  * @brief  HAL Lock structures definition
  */
typedef enum
{
    HAL_UNLOCKED = 0x00U,
    HAL_LOCKED = 0x01U
} HAL_LockTypeDef;

typedef enum
{
    RESET = 0,
    SET = !RESET
} FlagStatus,
    ITStatus;

typedef enum
{
    ERROR = 0,
    SUCCESS = !ERROR
} ErrorStatus;    

#endif    // /* ___LE501X_HAL_DEF */

/****END OF FILE****/
