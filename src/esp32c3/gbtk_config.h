/** 
 * @author Sascha Klick
 * @date   10/04/2025 
 **/

#ifndef CONFIG_ESP32C3_H
#define CONFIG_ESP32C3_H

#define PIN_CLK  4
#define PIN_SO   5 
#define PIN_SI   6
#define PIN_UART 7

#define PIN_STAT LED_BUILTIN

#define SERIAL_BAUDRATE 524288
#define UART_BAUDRATE   524288

#define WAIT_BIT_US  1
#define WAIT_BYTE_US 100


HardwareSerial SerialUART(1);
#define UART_INIT() { SerialUART.begin(UART_BAUDRATE, SERIAL_8N1, PIN_UART, -1, false);  }

#endif
