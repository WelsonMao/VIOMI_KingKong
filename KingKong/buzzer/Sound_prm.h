/**
 *  @file
 *
 *  @brief      Public parameters for the Sound Module
 *
 *  $Header: Sound_prm.h 1.9 2012/05/04 06:21:37CST Manoel G De Souza (SOUZAMG) Exp  $
 *
 *  @copyright  Copyright 2007-$Date: 2012/05/04 06:21:37CST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef __SOUND_PRM_H                                       //!< Ensure this file is not compiled again
#define __SOUND_PRM_H                                   //!< after we compile it the first time
//#include "typdefs.h"
#include "Sound.h"

#define SOUND_ENABLE
#define ENABLE_BUZZER
//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================
/**
 * @brief Definition of the sounds
 * @details Each sound defined in the Sound_prv.h should have an external prototype casted.
 */
#define SOUND_START_S                (SOUND_SEQUENCE_DEF *)tSOUND_START
#define SOUND_STOP_S                 (SOUND_SEQUENCE_DEF *)tSOUND_STOP
#define SOUND_LOCK_IN_S              (SOUND_SEQUENCE_DEF *)tSOUND_LOCK_IN
#define SOUND_LOCK_OUT_S             (SOUND_SEQUENCE_DEF *)tSOUND_LOCK_OUT
#define SOUND_WATEROUT_START_S       (SOUND_SEQUENCE_DEF *)tSOUND_WATEROUT_START
#define SOUND_WATEROUT_STOP_S        (SOUND_SEQUENCE_DEF *)tSOUND_WATEROUT_STOP
#define SOUND_LOCK_ALARM_S           (SOUND_SEQUENCE_DEF *)tSOUND_LOCK_ALARM
#define SOUND_WATERLOW_ALARM_S       (SOUND_SEQUENCE_DEF *)tSOUND_WATERLOW_ALARM
#define SOUND_FUNCTION_IN_S          (SOUND_SEQUENCE_DEF *)tSOUND_FUNCTION_IN
#define SOUND_FUNCTION_OUT_S         (SOUND_SEQUENCE_DEF *)tSOUND_FUNCTION_OUT
#define SOUND_WARNING_S              (SOUND_SEQUENCE_DEF *)tSOUND_WARNING
#define SOUND_ERROR_S                (SOUND_SEQUENCE_DEF *)tSOUND_ERROR
#define SOUND_REMEMBER_S             (SOUND_SEQUENCE_DEF *)tSOUND_REMEMBER
#define SOUND_CLICK_S                (SOUND_SEQUENCE_DEF *)tSOUND_CLICK
#define SOUND_CLICK2_S               (SOUND_SEQUENCE_DEF *)tSOUND_CLICK2

//=====================================================================================================================
//-------------------------------------- GLOBAL (Constant array prototypes) -------------------------------------------
//=====================================================================================================================
extern const SOUND_SEQUENCE_DEF	tSOUND_START[];
extern const SOUND_SEQUENCE_DEF	tSOUND_STOP[];
extern const SOUND_SEQUENCE_DEF	tSOUND_LOCK_IN[];
extern const SOUND_SEQUENCE_DEF tSOUND_LOCK_OUT[];
extern const SOUND_SEQUENCE_DEF tSOUND_WATEROUT_START[];
extern const SOUND_SEQUENCE_DEF tSOUND_WATEROUT_STOP[];
extern const SOUND_SEQUENCE_DEF tSOUND_LOCK_ALARM[];
extern const SOUND_SEQUENCE_DEF	tSOUND_WATERLOW_ALARM[];
extern const SOUND_SEQUENCE_DEF	tSOUND_FUNCTION_IN[];
extern const SOUND_SEQUENCE_DEF	tSOUND_FUNCTION_OUT[];
extern const SOUND_SEQUENCE_DEF	tSOUND_WARNING[];
extern const SOUND_SEQUENCE_DEF	tSOUND_ERROR[];
extern const SOUND_SEQUENCE_DEF	tSOUND_REMEMBER[];
extern const SOUND_SEQUENCE_DEF	tSOUND_CLICK[];
extern const SOUND_SEQUENCE_DEF	tSOUND_CLICK2[];
extern void Sound__Initialize(void);
extern void SoundToneHandler(void);


#endif

