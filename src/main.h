/** 
 * @author Sascha Klick
 * @date   10/04/2025 
 **/

#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

enum GBTK_Mode {
  GBTK_SIO,
  GBTK_DUMP  
};

struct {
  uint8_t buf[32];
  size_t  read;
  size_t  write;
  size_t  cnt;
} send = { { 0, }, 0, 0, 0 };

struct {
  uint8_t resp[16];
  size_t  read;    
  size_t  write;
  uint8_t status;
} detect = { { 0, }, 0, 0, 0 };

#endif