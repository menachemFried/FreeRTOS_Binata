/*******************************************************************************
 *
 *     BBBBBBB      IIII   NNN     N   AA       TTTTTTTT   AA
 *     B      B      II    N N     N   AAA        TTTT     AAA
 *     B      BB     II    N  N    N   AAAA        TT      AAAA
 *     BBBBBBBBB     II    N   N   N   AAAAA       TT      AAAAA
 *     B        BB   II    N   N   N   AA   A      TT      AA   A
 *     B        BB   II    N    N  N   AA    A     TT      AA    A
 *     BBBBBBBBBB   IIII   N    NNNN   AA     A   TTTT     AA     A
 *
 ********************************************************************************
 * All software (C)
 ******************************************************************************/



/*******************************************************************************
*
* Description: USART_Driver header file
*
* Author: Menachem fried
* Date:   Jun 21, 2025
*
******************************************************************************/


/*******************************************************************************
  Multiple include protection
 ******************************************************************************/
#ifndef USART_TX_DRIVER_H
#define USART_TX_DRIVER_H

/*******************************************************************************
 *  Include
 ******************************************************************************/

//  include C headers
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

//  include ST headers
#include "cmsis_os2.h"
#include "stm32f4xx_hal.h" // Change to your specific STM32 family header

//  include Main definitions
#include "main.h"
#include "app_main.h"

//  include from - Root -

//  include from - PeriphDrivers -

//  include from - Modules -

//  include from - Middleware -


//  include from - Tests


/*******************************************************************************
 *  Defines
 ******************************************************************************/


/* Defines for the memory pool */
#define USART_MSG_BLOCK_COUNT   8
#define USART_MSG_PAYLOAD_SIZE  128 // Max payload size for a single message




/*------------------------------------------------------------------------------
  Configurations
------------------------------------------------------------------------------*/



/*******************************************************************************
 *  Constants and Macros
 ******************************************************************************/


/*******************************************************************************
 *  Data types
 ******************************************************************************/

// Define a structure for messages. This is what the memory pool will contain.
// It's good practice to wrap the data and length together.
typedef struct {
    uint8_t buf[USART_MSG_PAYLOAD_SIZE];
    size_t  len;
} usart_msg_t;


/*******************************************************************************
 *  Interface functions
 ******************************************************************************/



/**
 * @brief Initializes the USART driver subsystem.
 * @param huart Pointer to the UART_HandleTypeDef structure for the USART peripheral.
 * @return osStatus_t status code.
 * @note Must be called after RTOS objects are created but before the scheduler starts.
 */
osStatus_t usart_driver_init(void);

/**
 * @brief Asynchronously sends data over USART. "Fire-and-Forget" API.
 * @param data Pointer to the data buffer to send.
 * @param len Length of the data in bytes.
 * @return osStatus_t status code. osOK on success, osErrorResource if no memory/queue space is available.
 * @note This function is thread-safe and non-blocking.
 */
osStatus_t usart_send_async(const uint8_t* data, size_t len);





/*******************************************************************************
 *  Driver End
 ******************************************************************************/


#endif /* USART_TX_DRIVER_H */
