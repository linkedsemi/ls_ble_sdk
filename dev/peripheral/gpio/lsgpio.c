#include <string.h>
#include "HAL_def.h"
#include "lsgpio.h"
#include "reg_lsgpio.h"
#include "field_manipulate.h"
#include "ls_dbg.h"
#include "compile_flag.h"
#include "reg_lsgpio_type.h"

/**
  * @brief  Initializes the GPIOx peripheral according to the specified parameters in the GPIO_Init.
  * @param  GPIOx: where x can be (A..C depending on device used) to select the GPIO peripheral
  * @param  GPIO_Init: pointer to a GPIO_InitTypeDef structure that contains
  *         the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void HAL_GPIO_Init(reg_lsgpio_t *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    uint32_t position = 0x00u;
    uint32_t ioposition;
    uint32_t iocurrent;
    volatile uint32_t *configregister; /* Store the address of GPIO register based on pin number */
    uint32_t registeroffset;           /* offset used during computation of CNF and MODE bits placement inside CRL or CRH register */

    /* Configure the port pins */
    while (((GPIO_Init->Pin) >> position) != 0x00u)
    {
        /* Get the IO position */
        ioposition = (0x01uL << position);

        /* Get the current IO position */
        iocurrent = (uint32_t)(GPIO_Init->Pin) & ioposition;

        if (iocurrent == ioposition)
        {
            /* Based on the required mode, filling config variable with MODEy[1:0] and CNFy[3:2] corresponding bits */
            switch (GPIO_Init->Mode)
            {
                /* If we are configuring the pin in OUTPUT  mode */
            case GPIO_MODE_OUTPUT:
                if (GPIO_Init->OT == GPIO_OUTPUT_PUSHPLL)
                {
                    MODIFY_REG(GPIOx->OT, (GPIO_OT0_MASK << position), (GPIO_OUTPUT_PUSHPLL << position));
                }
                else
                {
                    MODIFY_REG(GPIOx->OT, (GPIO_OT0_MASK << position), (GPIO_OUTPUT_OPENDRAIN << position));
                }
                MODIFY_REG(GPIOx->MODE, (GPIO_MODE0_MASK << (position << 1u)), (SET_GPIO_MODE_GPIO << (position << 1u)));
                MODIFY_REG(GPIOx->OD, (GPIO_OD0_MASK << (position << 1u)), (GPIO_Init->Driver_Pwr << (position << 1u)));
                MODIFY_REG(GPIOx->OE, (GPIO_OE0_MASK << position), (GPIO_OUTPUT_EN << position));
								MODIFY_REG(GPIOx->IE, (GPIO_IE0_MASK << position), (0U << position));
                break;
            case GPIO_MODE_AF:
                configregister = (iocurrent < GPIO_PIN_8) ? ((iocurrent < GPIO_PIN_4) ? (&(GPIOx->AF0)) : (&(GPIOx->AF1))) : ((iocurrent < GPIO_PIN_12) ? (&(GPIOx->AF2)) : (&(GPIOx->AF3)));
                registeroffset = (iocurrent < GPIO_PIN_8) ? ((iocurrent < GPIO_PIN_4) ? (position) : (position - 4u)) : ((iocurrent < GPIO_PIN_12) ? (position - 8u) : (position - 12u));
                registeroffset <<= 3u;
                MODIFY_REG(GPIOx->MODE, (GPIO_MODE0_MASK << (position << 1u)), (SET_GPIO_MODE_AF << (position << 1u)));
                MODIFY_REG((*configregister), (GPIO_IO0_AF_MASK << (registeroffset)), (GPIO_Init->AF_Type << (registeroffset)));
                break;
                /* If we are configuring the pin in INPUT  mode  (also applicable to EVENT and IT mode) */
            case GPIO_MODE_INPUT:
            case GPIO_MODE_IT_RISING:
            case GPIO_MODE_IT_RISING_FLT:
            case GPIO_MODE_IT_FALLING:
            case GPIO_MODE_IT_FALLING_FLT:
            case GPIO_MODE_IT_RISING_FALLING:
            case GPIO_MODE_IT_RISING_FALLING_FLT:
            case GPIO_MODE_IT_EAD:
            case GPIO_MODE_IT_EAD_FLT:
                if (GPIO_Init->Pull == GPIO_INPUT_NOPULL)
                {
                    MODIFY_REG(GPIOx->PUPD, (GPIO_PUPD0_MASK << (position << 1u)), (GPIO_INPUT_NOPULL << (position << 1u)));
                }
                else if (GPIO_Init->Pull == GPIO_INPUT_PULLUP)
                {
                    MODIFY_REG(GPIOx->PUPD, (GPIO_PUPD0_MASK << (position << 1u)), (GPIO_INPUT_PULLUP << (position << 1u)));
                    /* Set the corresponding ODR bit */
                    SET_BIT(GPIOx->BSBR, ioposition);
                }
                else /* GPIO_PULLDOWN */
                {
                    MODIFY_REG(GPIOx->PUPD, (GPIO_PUPD0_MASK << (position << 1u)), (GPIO_INPUT_PULLDOWN << (position << 1u)));
                    /* Reset the corresponding ODR bit */
                    SET_BIT(GPIOx->BSBR, (ioposition << 16u));
                }
                MODIFY_REG(GPIOx->MODE, (GPIO_MODE0_MASK << (position << 1u)), (SET_GPIO_MODE_GPIO << (position << 1u)));
                MODIFY_REG(GPIOx->FLT, (GPIO_FLT0_MASK << position), ((GPIO_Init->Filter) << position));
                MODIFY_REG(GPIOx->IE, (GPIO_IE0_MASK << position), (GPIO_INPUT_EN << position));
								MODIFY_REG(GPIOx->OE, (GPIO_OE0_MASK << position), (0U << position));
                break;

            /* If we are configuring the pin in INPUT analog mode */
            case GPIO_MODE_ANALOG:
                MODIFY_REG(GPIOx->MODE, (GPIO_MODE0_MASK << (position << 1u)), (SET_GPIO_MODE_ANALOG << (position << 1u)));
                MODIFY_REG(GPIOx->AE, (GPIO_AE0_MASK << position), (GPIO_ANA_EN << position));
                break;

                /* If we are configuring the pin in Test mode */
            case GPIO_MODE_TEST:
                MODIFY_REG(GPIOx->MODE, (GPIO_MODE0_MASK << (position << 1u)), (SET_GPIO_MODE_TEST << (position << 1u)));
                break;

            /* Parameters are checked with assert_param */
            default:
                break;
            }

            /*--------------------- EXTI Mode Configuration ------------------------*/
            /* Configure the External Interrupt for the current IO */
            if ((GPIO_Init->Mode & EXTI_MODE) == EXTI_MODE)
            {
                configregister = (iocurrent < GPIO_PIN_8) ? (&(EXTI->EICFG0)) : (&(EXTI->EICFG1));
                registeroffset = (iocurrent < GPIO_PIN_8) ? position : (position - 8u);
                registeroffset <<= 2u;
                MODIFY_REG((*configregister), (EXTI_EICFGS0_MASK << (registeroffset)), (GPIO_GET_INDEX(GPIOx) << (registeroffset)));

                /* Configure the interrupt mask */
                if ((GPIO_Init->Mode & GPIO_MODE_IT) == GPIO_MODE_IT)
                {
                    SET_BIT(EXTI->EIER, iocurrent);
                }
                else
                {
                    CLEAR_BIT(EXTI->EIER, iocurrent);
                }

                /* Enable or disable ad trigger */
                if ((GPIO_Init->Mode & EAD_MODE_IT) == EAD_MODE_IT)
                {
                    SET_BIT(EXTI->EDTE, iocurrent);
                }
                else
                {
                    CLEAR_BIT(EXTI->EDTE, iocurrent);
                }

                /* Enable or disable the rising trigger */
                if ((GPIO_Init->Mode & RISING_EDGE) == RISING_EDGE)
                {
                    SET_BIT(EXTI->ERTS, iocurrent);
                }
                else
                {
                    CLEAR_BIT(EXTI->ERTS, iocurrent);
                }

                /* Enable or disable the falling trigger */
                if ((GPIO_Init->Mode & FALLING_EDGE) == FALLING_EDGE)
                {
                    SET_BIT(EXTI->EFTS, iocurrent);
                }
                else
                {
                    CLEAR_BIT(EXTI->EFTS, iocurrent);
                }

                /* Enable or disable the Rising/falling trigger */
                if ((GPIO_Init->Mode & RISING_FALLING_EDGE) == RISING_FALLING_EDGE)
                {
                    SET_BIT(EXTI->EFTS, iocurrent);
                    SET_BIT(EXTI->ERTS, iocurrent);
                }
                else
                {
                    CLEAR_BIT(EXTI->EFTS, iocurrent);
                    CLEAR_BIT(EXTI->ERTS, iocurrent);
                }

                /* Enable or disable the filter function */
                if ((GPIO_Init->Mode & EXTI_FLT_MODE) == EXTI_FLT_MODE)
                {
                    SET_BIT(EXTI->EDBC, iocurrent);
                    REG_FIELD_WR(EXTI->EDBC, EXTI_CNT, GPIO_Init->Edbc_filter.Cnt);
                    REG_FIELD_WR(EXTI->EDBC, EXTI_PRE, GPIO_Init->Edbc_filter.Pre);
                }
                else
                {
                    CLEAR_BIT(EXTI->EDB, iocurrent);
                }
            }
        }

        position++;
    }
}

/**
  * @brief  De-initializes the GPIOx peripheral registers to their default reset values.
  * @param  GPIOx: where x can be (A..C depending on device used) to select the GPIO peripheral
  * @param  GPIO_Pin: specifies the port bit to be written.
  *         This parameter can be one of GPIO_PIN_x where x can be (0..15).
  * @retval None
  */
void HAL_GPIO_DeInit(reg_lsgpio_t *GPIOx, uint32_t GPIO_Pin)
{
    uint32_t position = 0x00u;
    uint32_t iocurrent;
    uint32_t tmp;
    volatile uint32_t *configregister;
    uint32_t registeroffset;

    /* Configure the port pins */
    while ((GPIO_Pin >> position) != 0u)
    {
        /* Get current io position */
        iocurrent = (GPIO_Pin) & (1uL << position);

        if (iocurrent)
        {
            /*------------------------- EXTI Mode Configuration --------------------*/
            /* Clear the External Interrupt or Event for the current IO */
            configregister = (iocurrent < GPIO_PIN_8) ? (&EXTI->EICFG0) : (&EXTI->EICFG1);
            registeroffset = (iocurrent < GPIO_PIN_8) ? (position << 2u) : ((position - 8u) >> 2u);
            tmp = *configregister;
            tmp >>= registeroffset;
            if (tmp == GPIO_GET_INDEX(GPIOx))
            {
                tmp = 0x0FuL << registeroffset;
                CLEAR_BIT(*configregister, tmp);

                /* Clear EXTI line configuration */
                CLEAR_BIT(EXTI->EEIFM, (uint32_t)iocurrent);

                /* Clear Rising Falling edge configuration */
                CLEAR_BIT(EXTI->ERTS, (uint32_t)iocurrent);
                CLEAR_BIT(EXTI->EFTS, (uint32_t)iocurrent);
                /* Clear sw trigger configuration */
                CLEAR_BIT(EXTI->ESWI, (uint32_t)iocurrent);
                /* Clear ad trigger configuration */
                CLEAR_BIT(EXTI->EDTE, (uint32_t)iocurrent);

                /* Clear filter configuration */
                CLEAR_BIT(EXTI->EDB, (uint32_t)iocurrent);
            }
            /*------------------------- GPIO Mode Configuration --------------------*/
            /* default value is floating input shifted to correct position */
            
            MODIFY_REG(GPIOx->MODE, (GPIO_MODE0_MASK << (position << 1u)), (SET_GPIO_MODE_GPIO << (position << 1u)));
            MODIFY_REG(GPIOx->PUPD, (GPIO_PUPD0_MASK << (position << 1u)), (GPIO_INPUT_NOPULL << (position << 1u)));
            /* DOUT default value is 0 */
            CLEAR_BIT(GPIOx->DOUT, iocurrent);
        }

        position++;
    }
}

/*
 ===============================================================================
                       ##### IO operation functions #####
 ===============================================================================
*/
/**
  * @brief  Reads the specified input port pin.
  * @param  GPIOx: where x can be (A..C depending on device used) to select the GPIO peripheral
  * @param  GPIO_Pin: specifies the port bit to read.
  *         This parameter can be GPIO_PIN_x where x can be (0..15).
  * @retval The input port pin value.
  */
GPIO_PinState HAL_GPIO_ReadPin(reg_lsgpio_t *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_PinState bitstatus;

    if ((GPIOx->DIN & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)
    {
        bitstatus = GPIO_PIN_SET;
    }
    else
    {
        bitstatus = GPIO_PIN_RESET;
    }
    return bitstatus;
}

/**
  * @brief  Sets or clears the selected data port bit.
  *
  * @note   This function uses GPIOx_BSBR register to allow atomic read/modify
  *         accesses. In this way, there is no risk of an IRQ occurring between
  *         the read and the modify access.
  *
  * @param  GPIOx: where x can be (A..C depending on device used) to select the GPIO peripheral
  * @param  GPIO_Pin: specifies the port bit to be written.
  *          This parameter can be one of GPIO_PIN_x where x can be (0..15).
  * @param  PinState: specifies the value to be written to the selected bit.
  *          This parameter can be one of the GPIO_PinState enum values:
  *            @arg GPIO_PIN_RESET: to clear the port pin
  *            @arg GPIO_PIN_SET: to set the port pin
  * @retval None
  */
void HAL_GPIO_WritePin(reg_lsgpio_t *GPIOx, uint16_t GPIO_Pin, GPIO_PinState const PinState)
{
    if (PinState != GPIO_PIN_RESET)
   {
      WRITE_REG(GPIOx->BSBR, (uint32_t)GPIO_Pin);
   }
   else
   {
     WRITE_REG(GPIOx->BSBR, ((uint32_t)GPIO_Pin << 16u));
   }
}

/**
  * @brief  Toggles the specified GPIO pin
  * @param  GPIOx: where x can be (A..C depending on device used) to select the GPIO peripheral
  * @param  GPIO_Pin: Specifies the pins to be toggled.
  * @retval None
  */
void HAL_GPIO_TogglePin(reg_lsgpio_t *GPIOx, uint16_t GPIO_Pin)
{
    if ((GPIOx->DOUT & GPIO_Pin) != 0x00u)
    {
        SET_BIT(GPIOx->BSBR, ((uint32_t)GPIO_Pin << 16u));
    }
    else
    {
        SET_BIT(GPIOx->BSBR, (uint32_t)GPIO_Pin);
    }
}

/**
* @brief  Locks GPIO Pins configuration registers.
* @note   The locking mechanism allows the IO configuration to be frozen. When the LOCK sequence
*         has been applied on a port bit, it is no longer possible to modify the value of the port bit until
*         the next reset.
* @param  GPIOx: where x can be (A..C depending on device used) to select the GPIO peripheral
* @param  GPIO_Pin: specifies the port bit to be locked.
*         This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
* @retval None
*/
HAL_StatusTypeDef HAL_GPIO_LockPin(reg_lsgpio_t *GPIOx, uint16_t GPIO_Pin)
{
    volatile uint32_t tmp = FIELD_BUILD(GPIO_KEY, 0x1UL);

    /* Apply lock key write sequence */
    tmp |= GPIO_Pin;
    /* Set LCKx bit(s): LCKK='1' + LCK[15-0] */
    MODIFY_REG(GPIOx->LOCK, tmp, tmp);
    /* Reset LCKx bit(s): LCKK='0' + LCK[15-0] */
    tmp = GPIO_Pin;
    MODIFY_REG(GPIOx->LOCK, tmp, tmp);
    /* Set LCKx bit(s): LCKK='1' + LCK[15-0] */
    tmp |= GPIO_Pin;
    MODIFY_REG(GPIOx->LOCK, tmp, tmp);
    /* Read LCKK register. This read is mandatory to complete key lock sequence */
    tmp = GPIOx->LOCK;

    /* read again in order to confirm lock is active */
    if (REG_FIELD_RD(GPIOx->LOCK, GPIO_KEY))
    {
        return HAL_OK;
    }
    else
    {
        return HAL_STATE_ERROR;
    }
}

/**
  * @brief  This function handles EXTI interrupt request.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin)
{
    /* EXTI line interrupt detected */
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != 0x00u)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
        HAL_GPIO_EXTI_Callback(GPIO_Pin);
    }
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /* Prevent unused argument(s) compilation warning */
//    UNUSED(GPIO_Pin);
    /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
}

void HAL_GPIO_SET_INPUT(reg_lsgpio_t *GPIOx, uint16_t GPIO_Pin)
{
  CLEAR_BIT(GPIOx->OE, GPIO_Pin);
  SET_BIT(GPIOx->IE, GPIO_Pin);
}
void HAL_GPIO_SET_INPUT_FILT_EN(reg_lsgpio_t *GPIOx, uint16_t GPIO_Pin)
{
  SET_BIT(GPIOx->FLT, GPIO_Pin);
}

void HAL_GPIO_SET_OUTPUT(reg_lsgpio_t *GPIOx, uint16_t GPIO_Pin)
{
  CLEAR_BIT(GPIOx->IE, GPIO_Pin);
  SET_BIT(GPIOx->OE, GPIO_Pin);
}

