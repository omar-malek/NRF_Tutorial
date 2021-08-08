
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"
#include "nrf_gpio.h"



#define led 17



/**
 * @brief Function for application main entry.
 */
int main(void)
{
  nrf_gpio_cfg_output(led); // Configure the pin as output 


  while(1)
  {
    nrf_gpio_pin_set(led);// Setiing logic 1 on pin17
  nrf_delay_ms(500);
  nrf_gpio_pin_clear(led);// Setting  logic 0 on pin 17
  nrf_delay_ms(500);


  }


}

/**
 *@}
 **/
