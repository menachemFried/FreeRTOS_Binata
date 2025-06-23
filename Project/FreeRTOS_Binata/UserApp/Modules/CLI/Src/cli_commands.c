/*******************************************************************************
 * @file        cli_commands.c
 * @brief       Implementation of custom CLI commands for the application.
 * @author      Binata
 * @date        Jun 22, 2025
 ******************************************************************************/

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"

/* FreeRTOS-CLI includes */
#include "FreeRTOS_CLI.h"

/* Local includes */
#include "cli_commands.h"

/* --- Private Function Prototypes for Command Implementations --- */
static BaseType_t prvTaskStatsCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
static BaseType_t prvEchoCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
static BaseType_t prvLedCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);

/* --- Command Definitions --- */

/**
 * @brief Command definition for the "task-stats" command.
 */
static const CLI_Command_Definition_t xTaskStats =
{
    "task-stats",
    "task-stats:\r\n Displays a table showing the state of each FreeRTOS task\r\n",
    prvTaskStatsCommand,
    0 /* No parameters are expected */
};

/**
 * @brief Command definition for the "echo" command.
 */
static const CLI_Command_Definition_t xEchoCommand =
{
    "echo",
    "echo <param1> <param2>...:\r\n Echos back each parameter on a new line\r\n",
    prvEchoCommand,
    -1 /* Variable number of parameters */
};

/**
 * @brief Command definition for the "led" command.
 */
static const CLI_Command_Definition_t xLedCommand =
{
    "led",
    "led set-rate <ms>:\r\n Sets the blink rate of the onboard LED in milliseconds\r\n",
    prvLedCommand,
    2 /* Expects exactly two parameters: "set-rate" and a number */
};


/* --- Public Function to Register All Commands --- */

/**
 * @brief Registers all defined CLI commands.
 */
void vRegisterCLICommands(void)
{
    FreeRTOS_CLIRegisterCommand(&xTaskStats);
    FreeRTOS_CLIRegisterCommand(&xEchoCommand);
    FreeRTOS_CLIRegisterCommand(&xLedCommand);
}


/* --- Command Implementations --- */

/**
 * @brief Implements the "task-stats" command.
 */
static BaseType_t prvTaskStatsCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    // Unused parameters
    (void) pcCommandString;
    (void) xWriteBufferLen;

    // Ensure the buffer is clear before writing to it.
    *pcWriteBuffer = '\0';

    // The vTaskList() function writes a formatted table of task stats into the buffer.
    // It's a single-shot output.
    vTaskList(pcWriteBuffer);

    // Return pdFALSE to indicate that the command has finished generating output.
    return pdFALSE;
}

/**
 * @brief Implements the "echo" command with variable parameters.
 */
static BaseType_t prvEchoCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    static UBaseType_t uxParameterNumber = 0;
    const char *pcParameter;
    BaseType_t xParameterStringLength;

    // This static variable tracks which parameter to process on each call.
    // It's initialized to 0 before the first call.
    if (uxParameterNumber == 0)
    {
        // This is the first call for this command execution. Start with the first parameter.
        uxParameterNumber = 1;
    }

    // Get the next parameter.
    pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, uxParameterNumber, &xParameterStringLength);

    if (pcParameter!= NULL)
    {
        // A parameter was found. Copy it to the output buffer.
        memset(pcWriteBuffer, 0x00, xWriteBufferLen);
        snprintf(pcWriteBuffer, xWriteBufferLen, "Param %lu: %.*s\r\n",
                 (unsigned long)uxParameterNumber, (int)xParameterStringLength, pcParameter);

        // Increment for the next call.
        uxParameterNumber++;

        // Return pdTRUE to indicate that there might be more output to generate.
        return pdTRUE;
    }
    else
    {
        // No more parameters were found. Reset for the next command execution.
        uxParameterNumber = 0;
        *pcWriteBuffer = '\0'; // Ensure buffer is empty for the final call.

        // Return pdFALSE to indicate the command is complete.
        return pdFALSE;
    }
}

/**
 * @brief Implements the "led" command for hardware control.
 */
static BaseType_t prvLedCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    const char *pcParam1, *pcParam2;
    BaseType_t xParam1Len, xParam2Len;
    long new_rate;

    // Get the first and second parameters.
    pcParam1 = FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParam1Len);
    pcParam2 = FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParam2Len);

    // Ensure the output buffer is clear.
    *pcWriteBuffer = '\0';

    if (pcParam1!= NULL && pcParam2!= NULL)
    {
        // Check if the first parameter is "set-rate"
        if (strncmp(pcParam1, "set-rate", xParam1Len) == 0)
        {
            // Convert the second parameter to a number
            new_rate = atol(pcParam2);
            if (new_rate > 0 && new_rate < 5000)
            {
                // Here you would call a function to update the LED blink rate.
                // For example: led_controller_set_rate(new_rate);
                snprintf(pcWriteBuffer, xWriteBufferLen, "OK: LED blink rate set to %ld ms.\r\n", new_rate);
            }
            else
            {
                snprintf(pcWriteBuffer, xWriteBufferLen, "Error: Invalid rate value. Must be between 1 and 4999.\r\n");
            }
        }
        else
        {
            snprintf(pcWriteBuffer, xWriteBufferLen, "Error: Unknown 'led' subcommand. Use 'set-rate'.\r\n");
        }
    }
    else
    {
        // This case should not be reached if cExpectedNumberOfParameters is set correctly,
        // but it's good practice to handle it.
        snprintf(pcWriteBuffer, xWriteBufferLen, "Error: Incorrect parameters. Usage: led set-rate <ms>\r\n");
    }

    // Command processing is complete in a single call, so return pdFALSE.
    return pdFALSE;
}
