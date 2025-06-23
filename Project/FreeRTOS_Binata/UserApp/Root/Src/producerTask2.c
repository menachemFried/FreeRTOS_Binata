
#include "main.h"
#include "cmsis_os.h"
#include "UsartTx_Driver.h" // Include our driver header
#include <stdio.h>
#include <string.h>



/**
  * @brief  Function implementing the producerTask2 thread.
  * @param  argument: Not used
  * @retval None
  */
void StartProducerTask2(void *argument)
{
  char buffer[50] = {0};
  uint32_t count = 0;

  for(;;)
  {
    // Create a different message
    snprintf(buffer, sizeof(buffer), "Message from Producer 2. Iteration: %lu\r\n", count++);

    // Send it
    usart_send_async((uint8_t*)buffer, strlen(buffer));

    // Wait for a different amount of time to show concurrency
    osDelay(750);
  }
}


/*-----------------------------------------------------------*/
