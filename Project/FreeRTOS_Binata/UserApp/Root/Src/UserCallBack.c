/*******************************************************************************
*
* Stroke ReWalk project
*
*******************************************************************************
* All software (C)
******************************************************************************/

/*******************************************************************************
*
* Description: Call Back functions for IRQs
*
* Note:
*
* Author:	  Yehuda Bitton
* Date: 09/2016
*
******************************************************************************/


/*******************************************************************************
*  Includes
******************************************************************************/

//  include C headers


//  include ST headers
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"

//  include Main definitions
#include "main.h"

//  include file Header
#include "UserCallBack.h"

//  include from - Root -
#include "usart.h"


//  include from - PeriphDrivers -
#include "UsartTx_Driver.h"
#include "UsartRx_Driver.h"

//  include from - Modules -


//  include from - Tests






/*******************************************************************************
*  Data types
******************************************************************************/
/*------------------------------------------------------------------------------
Variables structures
------------------------------------------------------------------------------*/


/* RTOS objects handles - defined in freertos.c by CubeMX */
extern osSemaphoreId_t usartTxDoneSemHandle;
extern osThreadId_t cmdParserTaskHandle;
extern UART_HandleTypeDef huart2;
extern uint8_t dma_rx_buffer[DMA_RX_BUFFER_SIZE];

/*******************************************************************************
*  Constants and Macros
******************************************************************************/

/*******************************************************************************
*  Static Data Declarations
******************************************************************************/

/*******************************************************************************
*  Private Functions Prototypes
******************************************************************************/


/*******************************************************************************
*  Public Function Bodies
******************************************************************************/

/*================================================================================================*/
/*                                     HAL Callback Implementations                               */
/*================================================================================================*/




/**
  * @brief  Tx Transfer completed callback.
  * @param  huart: UART handle.
  * @retval None
  * @note   This function is called from an ISR context when the DMA transfer is complete
  *         AND the last byte has been shifted out of the UART peripheral.
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    // Check if the interrupt is from the correct UART instance
    if (huart->Instance == huart2.Instance) {
        // Release the semaphore to unblock the usartSendTask.
        // This is an ISR-safe RTOS call.
        osSemaphoreRelease(usartTxDoneSemHandle);
    }
}




/**
 * @brief  HAL UART Reception Event Callback.
 * @note   This function is called by the HAL driver when a UART reception event occurs.
 *         Events include IDLE line, DMA Half-Transfer (HT), and DMA Transfer-Complete (TC).
 *         This function executes in ISR context. IT MUST BE FAST.
 * @param  huart UART handle.
 * @param  Size  Number of data bytes received since the last call to HAL_UARTEx_ReceiveToIdle_DMA.
 *               We ignore this and calculate the position from NDTR for robustness.
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    // Ensure the event is from the correct UART peripheral.
    if (huart->Instance == USART2)
    {
        // Check if the task handle is valid.
        if (cmdParserTaskHandle!= NULL)
        {
            // Set the flag to signal the parser task that new data is available.
            // This is the ONLY operation performed in the ISR to keep it short and fast.
            osThreadFlagsSet(cmdParserTaskHandle, PARSER_TASK_SIGNAL_FLAG);
        }
    }
}

/**
 * @brief  HAL UART Error Callback.
 * @note   This function is called by the HAL driver when a UART error is detected
 *         (e.g., Overrun, Parity, Framing, Noise). A robust driver MUST implement this.
 *         The default HAL behavior on error is often to stop the peripheral, so we
 *         must restart the reception here to make the driver self-healing.
 * @param  huart UART handle.
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        uint32_t error_code = HAL_UART_GetError(huart);
        printf("UART Error Detected. Code: 0x%lX. Restarting reception.\r\n", error_code);

        // Re-start the DMA reception to recover from the error.
        // This is critical for making the driver robust against real-world noise
        // and communication glitches.
        HAL_UARTEx_ReceiveToIdle_DMA(&huart2, dma_rx_buffer, DMA_RX_BUFFER_SIZE);
    }
}
