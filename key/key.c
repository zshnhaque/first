
#include "key.h"

const EatKey_enum EatKey[KEY_NUM] = {
     EAT_KEY_C1R1 // KEY_0,
    ,EAT_KEY_C3R4 // KEY_1,
    ,EAT_KEY_C3R3 // KEY_2,
    ,EAT_KEY_C2R0 // KEY_3,
    ,EAT_KEY_C2R4 // KEY_4,
    ,EAT_KEY_C2R1 // KEY_5,
    ,EAT_KEY_C1R2 // KEY_6,
    ,EAT_KEY_C2R3 // KEY_7,
    ,EAT_KEY_C1R3 // KEY_8,
    ,EAT_KEY_C1R0 // KEY_9,
    ,EAT_KEY_C1R4 // KEY_STAR,
    ,EAT_KEY_C0R0 // KEY_HASH,
    ,EAT_KEY_C4R2 // KEY_UP,
    ,EAT_KEY_C2R2 // KEY_DOWN,
    ,EAT_KEY_C3R0 // KEY_LEFT,
    ,EAT_KEY_C3R2 // KEY_RIGHT,
    ,EAT_KEY_C3R1 // KEY_OK,
    ,EAT_KEY_POWER // KEY_POWER,
};

typedef void (*key_handler_func)(Key_enum, eat_bool pressed);
static key_handler_func KeyHandlerTable[KEY_NUM];
void key_proc(EatEvent_st *event)
{
    Key_enum key;
    for(key= KEY_0; key<KEY_NUM; key++)
    {
        if(event->data.key.key_value == EatKey[key])
            break;
    }
    eat_trace("EAT_value: %d", event->data.key.key_value);
    eat_trace("EAT_Key: %d", key);
    if(key >= KEY_NUM)
        return;
    if(KeyHandlerTable[key])
    {
        (*KeyHandlerTable[key])(key, event->data.key.is_pressed);
    }
}
void key_register_hander(Key_enum key, void (*handler)(Key_enum, eat_bool pressed))
{
    if(key >= KEY_NUM)
        return;
    KeyHandlerTable[key] = handler;
}



