#ifndef __MUSIC_H
#define __MUSIC_H

#include "stm32f4xx_conf.h"


extern void BUZZER_Play(u16 frq);
extern void MUSIC_Play(u8 *music,u8 *time,u16 length);
extern void MUSIC1_Play(void);


#endif

