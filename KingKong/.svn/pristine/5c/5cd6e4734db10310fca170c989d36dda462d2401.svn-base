/**
 *  @file
 *
 *  @brief      Standard interface for the Sound module
 *
 *
 *  $Header: Sound.h 1.14 2012/05/04 06:21:38CST Manoel G De Souza (SOUZAMG) Exp  $
 *
 *  @copyright  Copyright 2012-$Date: 2012/05/04 06:21:38CST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef __SOUND_H                                       //!< Ensure this file is not compiled again
#define __SOUND_H                                   //!< after we compile it the first time
#include "../scene.h"
//#include "typdefs.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

typedef enum
{
	SOUND_CHIME1,
	SOUND_CHIME2
} SOUND_CHIME_DEF ;


typedef enum
{
	SOUND_ACTION_END,								//!< Last beep from the sequence
	SOUND_ACTION_REPEAT,							//!< Repeat X numbers
	SOUND_ACTION_CONTINUE							//!< when end ,play the next sound
}SOUND_ACTION_DEF;

typedef struct
{
	unsigned short int Frequency;					//!< Frequency in Hertz
	unsigned short int TOn;							//!< Chime charge time in ms
	unsigned short int TOff;						//!< Chime discharge time in ms
	SOUND_CHIME_DEF Chime;
	SOUND_ACTION_DEF Action;						//!< Action related to the Event
	unsigned char Parameter;						//!< Parameter defined by the  Action
} SOUND_SEQUENCE_DEF ;

#include "Sound_prm.h"

typedef enum
{
	SOUND_HANDLER_STATE,
	SOUND_CURRENT_VOLUME,
	SOUND_CURRENT_TONE_STATE
} SOUND_STATUS_DEF;


typedef enum
{
	SOUND_VOLUME_OFF,
	SOUND_VOLUME_1,
	SOUND_VOLUME_2,
	SOUND_VOLUME_3,
	SOUND_VOLUME_4,
	SOUND_VOLUME_5,
	SOUND_VOLUME_6,
	SOUND_VOLUME_7,
	SOUND_VOLUME_8,
	SOUND_VOLUME_9,
	SOUND_VOLUME_MAX
}SOUND_VOLUME_DEF;

#define SOUND_VOLUME_TABLE_MAX      11


typedef enum
{
	SOUND_PREEMPTION_KEEP,
	SOUND_PREEMPTION_DISCART
} SOUND_PREEMPTION_DEF ;



typedef enum
{
	SOUND_PRIOTITY_LOW,								//!< The lowest priority for a sound to be played
	SOUND_PRIOTITY_MID,								//!< The midle priotity for a sound to be played
	SOUND_PRIORITY_HIGH								//!< The Highest priority for a sound to be played
}SOUND_PRIORITY_DEF;


typedef enum
{
	SEQUENCE_IDLE,
	SEQUENCE_PLAYING,
	SEQUENCE_NEXT
} SEQUENCE_HANDLER_DEF;

typedef enum
{
    SOUND_TONE_OFF,
    SOUND_TONE_ON
} SOUND_TONE_STATE_TYPE;

#ifdef ENABLE_BUZZER
typedef enum{
	ITP_PWM_INIT=0,
	ITP_PWM_RESET,
	ITP_PWM_ON,
	ITP_PWM_OFF,
	ITP_PWM_GET_MAX_LEVEL,
	ITP_PWM_SET_OUTPUT,
}ITP_PWM_ACTION;
#define BUZZER_EN_PIN           ITH_GPIO_PIN22//58
#define BUZZER_PWM_PIN          ITH_GPIO_PIN23//59
#define BUZZER_GPIO_MODE        0
#define BUZZER_PWM              ITH_PWM1
extern int BuzzerIoctl(int file, unsigned long request, int pwm, int duty);
#endif
//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

void Sound__Initialize(void);
void Sound__Handler(void);
unsigned char Sound__Play(	SOUND_SEQUENCE_DEF * sequence,
							SOUND_PRIORITY_DEF priority,
							SOUND_PREEMPTION_DEF preemption);

unsigned char Sound__SimplePlay(SOUND_SEQUENCE_DEF * sequence);

unsigned char Sound__GetStatus(SOUND_STATUS_DEF index);
void Sound__SetVolume(SOUND_VOLUME_DEF volume);
#ifdef SOUND_PAUSE_RESUME_FEATURE
	unsigned char Sound__Pause(void);
	unsigned char Sound__Resume(void);
#endif
unsigned char Sound__Stop(void);

SOUND_SEQUENCE_DEF * Sound__GetCurrentSound(void);

extern SEQUENCE_HANDLER_DEF Sound_Sequence_State;
extern void Buzzer_TimeInit(void);
#endif

