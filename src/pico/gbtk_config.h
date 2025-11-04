/** 
 * @author Sascha Klick
 * @date   10/04/2025 
 **/

#ifndef CONFIG_PICO_H
#define CONFIG_PICO_H

#define PIN_CLK  2
#define PIN_SO   3 
#define PIN_SI   4
#define PIN_STAT LED_BUILTIN

#define SERIAL_BAUDRATE 524288
#define UART_BAUDRATE   524288

#define WAIT_BIT_US  1
#define WAIT_BYTE_US 200

#define UART_NO 1
#if UART_NO == 0
    UART SerialUART(0, 1, -1, -1);
#elif UART_NO == 1
    UART SerialUART(8, 5, -1, -1);
#endif

#define UART_INIT() { SerialUART.begin(UART_BAUDRATE, SERIAL_8N1); }

#endif
