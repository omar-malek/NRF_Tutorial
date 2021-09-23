
#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_gpiote.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_gpiote.h"
#include "app_error.h"




#define LED_Pin1 17 // Pin connected with LED
#define Btn_Pin1 13 // Pin connected with Button




static nrf_ppi_channel_t ppi_channel; // A struct to hol ppi channel values


// Interrupt handler for GPIO button pin, we will not use it because we are directly routing
// the event to the pin, but you can use it in conjuction with the PPI
void intrrupt_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
 // Empty handler
}




// A function to initialize all the pins with all the configurations
static void gpiote_pins_init(void)
{
  // Variable to hold error values
  uint32_t error_code = NRF_SUCCESS;

  error_code = nrf_drv_gpiote_init(); // Initialize gpiote module
  APP_ERROR_CHECK(error_code);

  // create the configurations for input pin interrupt
  nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true); 
  // set the PULL UP resistor because the button gives logic zero so when
  // no button is being pressed the pullup resistors will pull the pin to logic high state
  in_config.pull = NRF_GPIO_PIN_PULLUP; 

 // initialize the interrupt pin for the button
    error_code = nrf_drv_gpiote_in_init(Btn_Pin1, &in_config, intrrupt_pin_handler);
    APP_ERROR_CHECK(error_code);


// enable and configure the task of LED pin with its function
// it can be:
// set high
// set low
// or toggle pin

    nrf_drv_gpiote_out_config_t out_config = GPIOTE_CONFIG_OUT_TASK_TOGGLE(true);
    error_code = nrf_drv_gpiote_out_init(LED_Pin1, &out_config);
    APP_ERROR_CHECK(error_code);



// once everything is done

// enable LED Pin task so that it can be executed when ever called by any event via PPI
    nrf_drv_gpiote_out_task_enable(LED_Pin1);

// enable the interrupt event on button pin
    nrf_drv_gpiote_in_event_enable(Btn_Pin1, true);


  
}




// A function to initialize the PPI Module with all the configurations
static void ppi_init(void)
{
  // a variable to hold the error value
    uint32_t err_code = NRF_SUCCESS;

  // Variables to hold the event address and task address
  // they are needed to connect with the PPI TEP(Task End Point) & PPI EEP (Event End Point)
    uint32_t btn_evt_addr;
    uint32_t led_task_addr;



// Initialize the PPI module, make sure its only enabled once in your code
   err_code =  nrf_drv_ppi_init();
   APP_ERROR_CHECK(err_code);


// Allocate the channel from the available PPI channels
    err_code = nrf_drv_ppi_channel_alloc(&ppi_channel);
    APP_ERROR_CHECK(err_code);


// Get the address of the respective event and tasks from the pins
    btn_evt_addr =  nrf_drv_gpiote_in_event_addr_get(Btn_Pin1);
    led_task_addr = nrf_drv_gpiote_out_task_addr_get(LED_Pin1);


// connect the EEP & TEP with Peripheral Events & Tasks using their addresses and assign them to an allocated channel
    err_code = nrf_drv_ppi_channel_assign(ppi_channel, btn_evt_addr, led_task_addr);
    APP_ERROR_CHECK(err_code);

// Enable the channel so that it can start receiving events and then route them to tasks
    err_code = nrf_drv_ppi_channel_enable(ppi_channel);
    APP_ERROR_CHECK(err_code);

    
}





int main(void)
{

// Call the GPIOTE intialization function here
    gpiote_pins_init();

// Call the PPI initialization function here
    ppi_init();

  
   
    while (true)
    {
       // Do nothing
    }
}


/** @} */
