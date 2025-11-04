/**
 * @brief  gbtk-sio Link cable bridge adapter 
 * 
 * @author Sascha Klick
 * @date   10/04/2025 
 **/

#include <Arduino.h>

#include "main.h"

#include "gbtk_config.h"
#include "inject.h"

#define DBG_ECHO_DETECT  0
#define DBG_SEND_MREAD   0
#define DBG_SERIAL_WRITE 0

volatile uint32_t tick_ms = millis();

const char UART_SEQUENCE[] = "GBTKUART";
volatile GBTK_Mode dev_mode;
volatile uint32_t  uart_seq_cnt = 0;

void set_mode(GBTK_Mode mode){
  switch(mode){
    case GBTK_SIO:
      #ifdef UART_BAUDRATE
        SerialUART.end();
      #endif
      pinMode(PIN_CLK, OUTPUT);
      digitalWrite(PIN_CLK, LOW);
      pinMode(PIN_SO, OUTPUT);
      digitalWrite(PIN_SO, LOW);
      pinMode(PIN_SI, INPUT);
      break;
    case GBTK_DUMP:
        pinMode(PIN_CLK, INPUT_PULLDOWN);
        pinMode(PIN_SO, INPUT_PULLDOWN);
        pinMode(PIN_SI, INPUT_PULLDOWN); 
        #ifdef UART_INIT
          UART_INIT();        
          while(SerialUART.available()){
            SerialUART.read();
          }          
        #endif
      break;
  }
  #ifdef PIN_STAT
    digitalWrite(PIN_STAT, mode == GBTK_DUMP ? HIGH : LOW);
  #endif
  dev_mode = mode;
}

void setup() {
  Serial.begin(SERIAL_BAUDRATE);

  #ifdef PIN_STAT
    pinMode(PIN_STAT, OUTPUT);    
  #endif  

  set_mode(GBTK_SIO);
}

void loop() {   
   #ifdef UART_INIT
    if(dev_mode == GBTK_DUMP){      
      while(SerialUART.available()){
        int in = SerialUART.read();      
        Serial.write(in);
      }
      return;
    }
  #endif
  
  while(Serial.available()) {
    uint8_t in;
    Serial.readBytes(&in, 1);

    if(in == UART_SEQUENCE[uart_seq_cnt]){
      uart_seq_cnt++;        
      if(uart_seq_cnt == sizeof(UART_SEQUENCE) - 1){
        set_mode(GBTK_DUMP);
        return;
      }
    }else{
      uart_seq_cnt = 0;
    }    

    send.buf[send.write] = in;
    send.write = (send.write + 1) % sizeof(send.buf);    
  }

  if(send.write == send.read){
    #if DBG_SEND_MREAD
      inject_read_title();
      delay(500);   
    #else
      uint32_t ms = millis();
      if(tick_ms + 1000 < ms){
        #if DBG_ECHO_DETECT
          inject_random_echo();
        #else
          detect.status = 0;
        #endif
        tick_ms = ms;
      }
    #endif
  }

  if(send.write != send.read){ 
    #ifdef PIN_STAT    
      digitalWrite(PIN_STAT, detect.status ? LOW : HIGH);
    #endif

    uint8_t out = send.buf[send.read];
    uint8_t in  = 0;
    
    for(uint8_t i = 0; i < 8; i++){      
      uint8_t mask = 1 << (7 - i);
      digitalWrite(PIN_SO, ((out & mask) != 0) ? HIGH : LOW);      

      digitalWrite(PIN_CLK, LOW);      
      delayMicroseconds(WAIT_BIT_US);   
      
      digitalWrite(PIN_CLK, HIGH);       
      delayMicroseconds(WAIT_BIT_US);

      in <<= 1;      
      if(digitalRead(PIN_SI)) in |= 0b00000001;      
    }

    send.read = (send.read + 1) % sizeof(send.buf);

    #ifdef PIN_STAT    
      digitalWrite(PIN_STAT, detect.status ? HIGH : LOW);
    #endif

    delayMicroseconds(WAIT_BYTE_US);    

    #if DBG_SERIAL_WRITE
      Serial.printf("[%5d] send: %02x '%c' recv: %02x '%c'\r\n", send.cnt, out, out >= ' ' && out < 0x7f ? out : '.', in, in >= ' ' && in < 0x7f ? in : '.');
    #endif

    if(detect.read != detect.write){
      if(detect.resp[detect.read] != in){
          detect.status = 0;
      }
      detect.read = (detect.read + 1) % sizeof(detect.resp);
    }else{
      #if !DBG_SERIAL_WRITE
        Serial.write(in);              
      #endif
    }
    
    send.cnt++;     
  }    
}