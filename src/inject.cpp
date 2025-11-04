/** 
 * @author Sascha Klick
 * @date   10/04/2025 
 **/

#include <Arduino.h>

#include "main.h"

void inject_random_echo() {
  const uint8_t random_char = 'A' + random(26);
  const uint8_t cmd[]   = { 0x00, random_char, 'R' };
  const uint8_t resp[]  = { 'W',  '0', random_char };
  
  for(size_t i = 0; i < sizeof(cmd); i++){
    send.buf[send.write] = cmd[i];
    send.write = (send.write + 1) % sizeof(send.buf);
  }
  
  memcpy(detect.resp, resp, sizeof(resp));
  detect.read = 0;        
  detect.write = sizeof(resp);
  detect.status = 1;
}

void inject_read_title() {  
  uint16_t addr = 0x0134;
  uint16_t len  = 0x000f;
  
  const uint8_t cmd[] = { 0x03, (uint8_t)addr, (uint8_t)(addr >> 8), (uint8_t)len, (uint8_t)(len >> 8) };

  for(size_t i = 0; i < sizeof(cmd); i++){
    send.buf[send.write] = cmd[i];
    send.write = (send.write + 1) % sizeof(send.buf);
  }
  for(size_t i = 0; i < len; i++){
    send.buf[send.write] = 'R';
    send.write = (send.write + 1) % sizeof(send.buf);    
  }
}