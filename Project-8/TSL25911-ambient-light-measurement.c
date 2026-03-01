/**
 * 
 * TSL25911 ambient light sensor test code as of February 28, 2026
 * 
 * raw ambient light data output on an OLED display (High Byte and Low Byte of Channel 1 and Channel 0)
 * see data sheet for more information
 *   
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/time.h"

#include "connections.h"

// OLED SSD1306 w/ SSD1306 controller
#include "oled.h"
#include "lookupTable.h"

void initUART()
{
  uart_init(UART_ID, 115200);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
  uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
  // shell command: Pi 3 "minicom -b 115200 -D /dev/serial0", Pi 5 "(...) /dev/ttyAMA0"
}  

void initI2C()
{  
  i2c_init(I2C_ID, 400 * 1000);  
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  
  // using external 3.9 or 4.7 kOhm resistors, internal actually not needed
  //gpio_pull_up(SDA_PIN);
  //gpio_pull_up(SCL_PIN);   
}


void show_digits(uint digit, uint a, uint b, uint c , uint d)
{
    struct render_area frame_area = {a, b, c, d};
    calc_render_area_buflen(&frame_area);
    
    if (digit == 0)
        render(null, &frame_area); 
    else if (digit == 1)
        render(one, &frame_area);
    else if (digit == 2)
        render(two, &frame_area);
    else if (digit == 3)
        render(three, &frame_area);
    else if (digit == 4)
        render(four, &frame_area);
    else if (digit == 5)
        render(five, &frame_area);
    else if (digit == 6)
        render(six, &frame_area);
    else if (digit == 7)
        render(seven, &frame_area);
    else if (digit == 8)
        render(eight, &frame_area);
    else if (digit == 9)
        render(nine, &frame_area);        
    
}

void renderNumbers(uint AmbientLight, uint c, uint d, uint e){      
            
    // this is just a weird hack, but it works sufficiently for the moment
    // exploits properties of the int datatype
    uint digit[3];
    digit[0] = AmbientLight/100;
    digit[1] = (AmbientLight - (100*digit[0]))/10;  
    uint interimValue = AmbientLight/10;
    digit[2] = AmbientLight - interimValue*10;
    
    for (int i=0; i < 3; i++)
    {          
      show_digits(digit[i], (48+(i*8)+e), (55+(i*8)+e), c, d);
    }
            
}

int main() 
{  
    stdio_init_all();
    initUART();
    initI2C();  
    
    // graphic scaffold, start screen
    oled_init();
    struct render_area title_area = {0, 127, 0, 4};
    calc_render_area_buflen(&title_area);
    render(startScreen, &title_area);    
        
    sleep_ms(1000);                       
           
    /*
    Initialize the TSL25911 sensor
    */
    
    // see data sheet, it's a bit tricky with the registers  
     /*
    uint8_t TSL25911_init_0[] = {0xA0};  // COMMAND register, normal operation, adressing ENABLE register   
    i2c_write_blocking(I2C_ID, 0x29, TSL25911_init_0, 1, true);  
    while (i2c_read_blocking(I2C_ID, 0x29, TSL25911_init_0, 1, false) < 0)  // repeats until the slave device sends ACK
    {
      sleep_ms(1);
    };  
  
    uint8_t TSL25911_init_1[] = {0x03}; // ENABLE register, PON and AEN 
    i2c_write_blocking(I2C_ID, 0x29, TSL25911_init_1, 1, true);
    while (i2c_read_blocking(I2C_ID, 0x29, TSL25911_init_1, 1, false) < 0)  // repeats until the slave device sends ACK
    {
      sleep_ms(1);
    };  
    */
    uint8_t TSL25911_init_2[] = {0xA1};  // COMMAND register, normal operation, adressing CONFIG/CONTROL register 
    i2c_write_blocking(I2C_ID, 0x29, TSL25911_init_2, 1, true); 
    while (i2c_read_blocking(I2C_ID, 0x29, TSL25911_init_2, 1, false) < 0)  // repeats until the slave device sends ACK
    {
      sleep_ms(1);
    };   
    
    uint8_t TSL25911_init_3[] = {0x14};  //  CONFIG/CONTROL register, modifying gain mode and integration time
    i2c_write_blocking(I2C_ID, 0x29, TSL25911_init_3, 1, true);  
    while (i2c_read_blocking(I2C_ID, 0x29, TSL25911_init_3, 1, false) < 0)  // repeats until the slave device sends ACK
    {
      sleep_ms(1);
    };  
        
    char readValue[32];
    
    while(true){ 
    
      // Acquiring the TSL25911 ambient light data
     
      uint8_t TSL25911_LB_CH0[] = {0xB4};    
    
      i2c_write_blocking(I2C_ID, 0x29, TSL25911_LB_CH0, 1, true); // "true" means final stop bit is not sent, data transfer can continue as part of the same transaction   
      i2c_read_blocking(I2C_ID, 0x29, TSL25911_LB_CH0, 1, false);
      
      uint8_t TSL25911_HB_CH0[] = {0xB5};
    
      i2c_write_blocking(I2C_ID, 0x29, TSL25911_HB_CH0, 1, true);    
      i2c_read_blocking(I2C_ID, 0x29, TSL25911_HB_CH0, 1, false);   
            
      uint8_t TSL25911_LB_CH1[] = {0xB6};    
    
      i2c_write_blocking(I2C_ID, 0x29, TSL25911_LB_CH1, 1, true);    
      i2c_read_blocking(I2C_ID, 0x29, TSL25911_LB_CH1, 1, false);
      
      uint8_t TSL25911_HB_CH1[] = {0xB7};
    
      i2c_write_blocking(I2C_ID, 0x29, TSL25911_HB_CH1, 1, true);    
      i2c_read_blocking(I2C_ID, 0x29, TSL25911_HB_CH1, 1, false);   
    
      sprintf(readValue, "TSL25911 CH0: %d %d \r\n", TSL25911_HB_CH0[0], TSL25911_LB_CH0[0]);  // values from 0 to 255
      uart_write_blocking(UART_ID, readValue, strlen(readValue));   
      sprintf(readValue, "TSL25911 CH1: %d %d \r\n", TSL25911_HB_CH1[0], TSL25911_LB_CH1[0]);  // values from 0 to 255
      uart_write_blocking(UART_ID, readValue, strlen(readValue));       
     
      renderNumbers(TSL25911_HB_CH0[0], 2, 2, 0);
      renderNumbers(TSL25911_LB_CH0[0], 2, 2, 32);
      
      renderNumbers(TSL25911_HB_CH1[0], 3, 3, 0);       
      renderNumbers(TSL25911_LB_CH1[0], 3, 3, 32);  
      
	  sleep_ms(1000); 
	  
	} 
          
    return 0;       
}
