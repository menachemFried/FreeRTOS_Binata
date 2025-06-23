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
#ifndef USART_RX_DRIVER_H
#define USART_RX_DRIVER_H

/*******************************************************************************
 *  Include
 ******************************************************************************/

//  include C headers
#include <stdbool.h>
#include <stdint.h>

//  include ST headers
#include "cmsis_os2.h"
#include "stm32f4xx_hal.h"

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



/*
 * @brief Size of the circular buffer used by the DMA for UART reception.
 * @note  A larger buffer provides more tolerance to processing latency,
 *        preventing data loss if the parser task is delayed by higher-priority tasks.
 *        A size of 256 or 512 is a reasonable starting point.
 */
#define DMA_RX_BUFFER_SIZE         256

/*
 * @brief Size of the linear buffer used for assembling commands before parsing.
 * @note  This defines the maximum length of a single command the system can handle,
 *        including the delimiter.
 */
#define CMD_ASSEMBLY_BUFFER_SIZE   128



// The flag to be used for signaling the parser task from the ISR.
#define PARSER_TASK_SIGNAL_FLAG    (0x0001U)



/*------------------------------------------------------------------------------
  Configurations
------------------------------------------------------------------------------*/



/*******************************************************************************
 *  Constants and Macros
 ******************************************************************************/


/*******************************************************************************
 *  Data types
 ******************************************************************************/


/*******************************************************************************
 *  Interface functions
 ******************************************************************************/

/**
 * @brief Initializes the UART DMA reception and prepares the parser task.
 * @note  This function should be called once before the RTOS scheduler starts,
 *        or at the beginning of the parser task itself.
 */
void usart_parser_init(void);

/**
 * @brief The entry function for the command parser RTOS task.
 * @param argument Unused RTOS task argument.
 */
void StartCmdParserTask(void *argument);


/*******************************************************************************
 *  Driver End
 ******************************************************************************/



#endif /* USART_RX_DRIVER_H */
