/*******************************************************************************
 * @file        cli_commands.h
 * @brief       Header file for custom CLI command registrations.
 * @author      Binata
 * @date        Jun 22, 2025
 ******************************************************************************/

#ifndef CLI_COMMANDS_H
#define CLI_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Registers all custom CLI commands with the FreeRTOS-CLI engine.
 * @note This function should be called once during system initialization,
 *       before the FreeRTOS scheduler is started. It uses pvPortMalloc
 *       to allocate memory for the command definitions.
 */
void vRegisterCLICommands(void);

#ifdef __cplusplus
}
#endif

#endif /* CLI_COMMANDS_H */
