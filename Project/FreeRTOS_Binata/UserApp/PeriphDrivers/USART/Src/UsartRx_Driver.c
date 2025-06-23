/*******************************************************************************
*
*   Binata project
*
*******************************************************************************
* All software (C)
******************************************************************************/

/*******************************************************************************
*  Includes
******************************************************************************/
//  include C headers
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

//  include ST headers
#include "stm32f4xx_hal.h"


//  include Main definitions
#include "main.h"
#include "usart.h"


//  include file Header
#include "UsartRx_Driver.h"
#include "UsartTx_Driver.h" // Needed for usart_send_async

//  include from - Root -

//  include from - PeriphDrivers -

//  include from - Modules -

//  include from - Tests




/* FreeRTOS-CLI include */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS_CLI.h"


/*================================================================================================*/
/*                                    Private Defines and Types                                   */
/*================================================================================================*/

// Define a welcome message to be sent when the CLI task starts.
#define CLI_WELCOME_MESSAGE "Welcome to Binata CLI. Type 'help' to list commands.\r\n> "


/*================================================================================================*/
/*                                  External Handle Declarations                                  */
/*================================================================================================*/

// These handles are defined in main.c by STM32CubeMX.
// Replace 'huart2' and 'hdma_usart2_rx' with the actual handles for your project.
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef  hdma_usart2_rx;
extern osThreadId_t cmdParserTaskHandle; // Defined in freertos.c

/*================================================================================================*/
/*                                    File-Scope Static Variables                                 */
/*================================================================================================*/



// The circular buffer that the DMA will write incoming UART data into.
// Declared as a correctly sized array as per the project requirements.
uint8_t dma_rx_buffer[DMA_RX_BUFFER_SIZE] = {0};

// The linear buffer used to assemble complete commands from potentially fragmented data chunks.
// This buffer holds the command before it's passed to the CLI processor.
uint8_t cmd_assembly_buffer[CMD_ASSEMBLY_BUFFER_SIZE] = {0};

// Tracks the current length of the data stored in the command assembly buffer.
// Volatile as it's modified in the task and accessed by the data processing logic.
volatile size_t cmd_assembly_len = 0;

// Tracks the last position from which we read data in the DMA circular buffer.
// This is essential for calculating the position and length of new data.
static size_t last_dma_read_pos = 0;

// Buffer for receiving output from the CLI command interpreter.
// Its size is defined in FreeRTOSConfig.h.
static char cOutputBuffer[configCOMMAND_INT_MAX_OUTPUT_SIZE] = {0};



/*================================================================================================*/
/*                                    Private Function Prototypes                                 */
/*================================================================================================*/


static void process_new_data(const uint8_t* data, size_t len);
static void process_cli_commands(void);

/*================================================================================================*/
/*                                     Public Function Definitions                                */
/*================================================================================================*/


/**
 * @brief Initializes the DMA for continuous UART reception.
 * @note This function starts the HAL_UARTEx_ReceiveToIdle_DMA process.
 * This is a "set-and-forget" operation; the DMA will run continuously
 * in circular mode without further intervention.
 */
void usart_parser_init(void)
{
    // Start the DMA reception. The DMA will now continuously fill dma_rx_buffer.
    // The HAL will trigger HAL_UARTEx_RxEventCallback on IDLE, HT, and TC events.
    HAL_StatusTypeDef status = HAL_UARTEx_ReceiveToIdle_DMA(&huart2, dma_rx_buffer, DMA_RX_BUFFER_SIZE);
    if (status!= HAL_OK)
    {
        // Handle error: DMA reception could not be started.
        Error_Handler();
    }
}

/**
 * @brief The entry function and main loop for the command parser RTOS task.
 *        This task now acts as the host for the FreeRTOS+CLI command interpreter.
 */
void StartCmdParserTask(void *argument)
{
    // Initialize the USART DMA reception engine.
    usart_parser_init();

    // Send a welcome message to the user.
    usart_send_async((uint8_t*)CLI_WELCOME_MESSAGE, strlen(CLI_WELCOME_MESSAGE));

    // The main, infinite loop of the task.
    for(;;)
    {
        // Wait indefinitely for a signal from the UART ISR.
        // The task will block here, consuming zero CPU, until a flag is set.
        osThreadFlagsWait(PARSER_TASK_SIGNAL_FLAG, osFlagsWaitAny, osWaitForever);

        // Calculate the current position of the DMA write pointer in the circular buffer.
        // The NDTR register counts down from the total buffer size.
        // We read this volatile register only ONCE per wakeup to ensure atomicity.
        size_t current_pos = DMA_RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);

        // If the position hasn't changed, it might be a spurious wakeup. Ignore.
        if (current_pos == last_dma_read_pos)
        {
            continue;
        }

        // Check if the DMA has wrapped around the buffer.
        if (current_pos > last_dma_read_pos)
        {
            // Linear read: new data is in a single contiguous block.
            size_t new_data_len = current_pos - last_dma_read_pos;
            process_new_data(&dma_rx_buffer[last_dma_read_pos], new_data_len);
        }
        else
        {
            // Wrap-around read: new data is in two parts.
            // Part 1: From the last read position to the end of the buffer.
            size_t new_data_len_part1 = DMA_RX_BUFFER_SIZE - last_dma_read_pos;
            process_new_data(&dma_rx_buffer[last_dma_read_pos], new_data_len_part1);

            // Part 2: From the beginning of the buffer to the current position.
            if (current_pos > 0)
            {
                process_new_data(dma_rx_buffer, current_pos);
            }
        }

        // Update the last read position for the next iteration.
        last_dma_read_pos = current_pos;

        // Attempt to parse and execute any complete commands now in the assembly buffer.
        process_cli_commands();
    }
}

/*================================================================================================*/
/* Private Function Definitions */
/*================================================================================================*/

/**
 * @brief Appends new data to the command assembly buffer and echoes it to the console.
 * @param data Pointer to the new data chunk.
 * @param len Length of the new data chunk.
 */
static void process_new_data(const uint8_t* data, size_t len)
{
    // Echo received characters back to the terminal for user feedback.
    usart_send_async(data, len);

    // Ensure we don't overflow the assembly buffer.
    size_t space_available = CMD_ASSEMBLY_BUFFER_SIZE - cmd_assembly_len;
    if (len > space_available)
    {
        // Handle error: Command too long or buffer overflow.
        // For simplicity, we'll just truncate, but a real system might log an error
        // and clear the buffer.
        len = space_available;

        char Errorbuffer[] = "\r\nWarning: Command assembly buffer overflow. Truncating data.\r\n> ";
        // Send it using the fire-and-forget API
        usart_send_async((uint8_t*)Errorbuffer, strlen(Errorbuffer));
    }

    // Copy the new data chunk to the end of the assembly buffer.
    memcpy(&cmd_assembly_buffer[cmd_assembly_len], data, len);
    cmd_assembly_len += len;
}

/**
 * @brief Scans the assembly buffer for newline-terminated commands and passes them
 *        to the FreeRTOS+CLI processor.
 */
static void process_cli_commands(void)
{
    uint8_t* p_cmd_start = cmd_assembly_buffer;
    uint8_t* p_cmd_end;

    // Loop in case multiple commands were received in one data chunk.
    // We search for '\r' as the command delimiter.
    while ((p_cmd_end = memchr(p_cmd_start, '\r', cmd_assembly_len))!= NULL)
    {
        // Temporarily null-terminate the command string for safe processing by the CLI.
        *p_cmd_end = '\0';

        // Send a newline to the terminal for clean formatting before the response.
        usart_send_async((uint8_t*)"\r\n", 2);

        BaseType_t xMoreDataToFollow;

        // The command interpreter is called repeatedly until it returns pdFALSE.
        // This is necessary to support commands that produce multi-line output.
        do
        {
            // Process the command. Output will be placed in cOutputBuffer.
            xMoreDataToFollow = FreeRTOS_CLIProcessCommand(
                (char*)p_cmd_start,      // The null-terminated command string.
                cOutputBuffer,           // The buffer to receive the output.
                configCOMMAND_INT_MAX_OUTPUT_SIZE // The size of the output buffer.
            );

            // Send the output generated by the command, if any.
            // The check for strlen > 0 prevents sending empty strings.
            if (strlen(cOutputBuffer) > 0)
            {
                usart_send_async((uint8_t*)cOutputBuffer, strlen(cOutputBuffer));
            }

        } while (xMoreDataToFollow!= pdFALSE);

        // Calculate the length of the processed command string.
        size_t processed_cmd_len = strlen((char*)p_cmd_start);

        // Calculate the total number of bytes to remove from the buffer.
        // This includes the command itself and the '\r' delimiter.
        size_t consumed_len = processed_cmd_len + 1;

        // Check for a following '\n' (common with terminals) and consume it as well.
        if ((p_cmd_end + 1 < cmd_assembly_buffer + cmd_assembly_len) && (*(p_cmd_end + 1) == '\n'))
        {
            consumed_len++;
        }

        // Calculate the length of the remaining data in the buffer.
        size_t remaining_len = cmd_assembly_len - consumed_len;

        // Remove the processed command from the assembly buffer by shifting the remaining data.
        memmove(cmd_assembly_buffer, p_cmd_start + consumed_len, remaining_len);
        cmd_assembly_len = remaining_len;

        // Reset the start pointer to the beginning of the buffer for the next scan.
        p_cmd_start = cmd_assembly_buffer;

        // Send a new prompt to the user.
        usart_send_async((uint8_t*)"> ", 2);
    }
}
