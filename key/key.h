#if !defined(__KEY_H__)
#define __KEY_H__

#include "eat_interface.h"

typedef enum {
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
  KEY_STAR,
  KEY_HASH,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_OK,
  KEY_POWER,
  KEY_NUM
} Key_enum;
extern void key_proc(EatEvent_st *event);
extern void key_register_hander(Key_enum key, void (*handler)(Key_enum, eat_bool pressed));

#endif


