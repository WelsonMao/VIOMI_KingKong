/**
 *  @file
 *
 *  @brief      Private parameters for the Sound module
 *
 *
 *  $Header: Sound_prv.h 1.4 2012/05/04 06:21:36CST Manoel G De Souza (SOUZAMG) Exp  $
 *
 *  @copyright  Copyright 2007-$Date: 2012/05/04 06:21:36CST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
#ifndef __SOUND_PRV_H_
#define __SOUND_PRV_H_
//#include "typdefs.h"

//=====================================================================================================================
//-------------------------------------- PUBLIC (Extern Variables, Constants & Defines) -------------------------------
//=====================================================================================================================

/**
 * @brief The SOUND_HANDLER_5MS directive when present defines that the Sound__Handler function should be called every 5ms.
 * When the define is commented out the Sound__Handler should be called every 1ms
 */
//#define SOUND_HANDLER_5MS

/**
 * @brief Defines if the system uses a charge pump or not to generate higher voltage to supply the Chime Circuit
 * @details The change pump is a circuit based on a Pwm which elevates the voltage supplied to the Chime circuit.
 * @note
 * <pre>
 *      #define SOUND_CHARGE_PUMP_CONTROL              // Charge pump feature enabled
 *      #define SOUND_CHARGE_PUMP_CONTROL              // Charge pump feature disabled
 * </pre>
 */
//#define SOUND_CHARGE_PUMP_CONTROL

/**
 * @brief Defines if the system uses the chime 1 feature
 * @details The Chime feature controls a RC circuit which created modulation in amplitude for the signal being played in the buzzer.
 * @note
 * <pre>
 *      #define SOUND_CHIME1_FEATURE                   // Chime 1 is present in the system
 *      #define SOUND_CHIME1_FEATURE                   // Chime 1 is absent in the system
 * </pre>
 */
#define SOUND_CHIME1_FEATURE

/**
 * @brief Defines if the system uses a queue for sounds or it just play the requested sound
 * @details The queue feature allows the system to be able to queue several sounds and play all them sequence. Also it allows priority for the sounds
 * @note
 * <pre>
 *      #define SOUND_QUEUE_FEATURE                    // Sound queue feature disabled
 *      #define SOUND_QUEUE_FEATURE                    // Sound queue feature enabled
 * </pre>
 */
//#define SOUND_QUEUE_FEATURE


/**
 * @brief Defines if the volume will be queue along with the sound.
 * @details IF present this feature allows the system to set the volume and queue a sound and in the very next line change the sound and
 * queue another sound, each sound will be executed with the volume configured by the time they were queued. If disabled the volume which
 * all sounds are played is the current volume configured in the system.
 * @note
 * <pre>
 *      #define SOUND_VOLUME_QUEUE                     // The system uses the sound setup in the system by the time the sound is being executed
 *      #define SOUND_VOLUME_QUEUE                     // Volume is queued along with the sound
 * </pre>
 */
//#define SOUND_VOLUME_QUEUE

/**
 * @brief Allows preemption between sounds of different priorities.
 * @details When this feature is enabled by calling Sound__Play and selecting the parameter preemption to define the behavior
 * when a preemption happens.
 * @note
 * <pre>
 *      #define SOUND_PRIORITY_PREEMPTION_FEATURE      // Preemption feature disabled
 *      #define SOUND_PRIORITY_PREEMPTION_FEATURE      // Preemption feature enabled
 * </pre>
 */
#define SOUND_PRIORITY_PREEMPTION_FEATURE


/**
 * @brief Allows any system to have a customized volume table
 * @details
 * @note
 * <pre>
 *      #define SOUND_SPECIFIC_VOLUME_TABLE        // Customized volume table is enabled
 *      #define SOUND_SPECIFIC_VOLUME_TABLE        // Customized volume table is disabled
 * </pre>
 */
#define SOUND_SPECIFIC_VOLUME_TABLE

#ifdef SOUND_SPECIFIC_VOLUME_TABLE

/**
 *  @brief Customized volume table.
 *  @details This table should contains 11 values.
 *  @note
 *  <pre>
 *      VOL_TABLE Has to have VOL_OFF,VOL_01 to VOL_09 and VOL_MAX
 *      The volume is related to the duty cycle applied to the PWM.
 *      The duty cycle is given *100, for instance
 *          10 means   0.1%
 *          175 means  1.75%
 *          1200 means 12%
 *  The duty cycle is a logarithm value when related to volume
 *      50% of duty cycle (5000) doens't means maximum volume in most cases.
 *  </pre>
 */
    #define VOL_TABLE_LIST          {       \
                                    0,     /* VOL_OFF */ \
                                    10,    /* VOL_01  */ \
                                    15,    /* VOL_02  */ \
                                    25,    /* VOL_03  */ \
                                    42,    /* VOL_04  */ \
                                    85,    /* VOL_05  */ \
                                    175,   /* VOL_06  */ \
                                    350,   /* VOL_07  */ \
                                    700,   /* VOL_08  */ \
                                    1200,  /* VOL_09  */ \
                                    1800   /* VOL_MAX  */ \
                                    }

#endif

//--- Sound define -------------------------------------------------
// Options for the Chime
// SOUND_CHIME1 (Low volume)
// SOUND_CHIME2(High volume)

// Options for Action
// SOUND_ACTION_END,                // Last tone of the sequence                            //!< Last beep from the sequence
// SOUND_ACTION_REPEAT,             // Repeat the sequence X times
// SOUND_ACTION_CONTINUE            // Play the next sound

/**
 *  @brief Sound definition.
 *  @details Each sound should have a const definition
 *  @note
 *  <pre>
 *      Each line correlates to a tone in the sound.
 *      The composition of each line is:
 *          1- Frequency in Hertz
 *          2- Chime Charge time in ms
 *          3- Decay time in ms
 *          4- Chime circuit used (SOUND_CHIME1 or SOUND_CHIME2)
 *          5- Action (SOUND_ACTION_CONTINUE, SOUND_ACTION_END or SOUND_ACTION_REPEAT)
 *          6- Parameter
 *
 *      For example:
 *              1480,       93,     93,     SOUND_CHIME1,   SOUND_ACTION_CONTINUE,      0,
 *
 *              It will play a tone of 1480Hz which charge time of 93ms and Decay time of 93ms
 *              This tone will use the Chime 1 circuit.
 *              The action indicates that there are more tones in the sound
 *              parameter 0 indicate volume maximum (when set from 1 to 10 controls the volume of
 *              this tone proportional to the volume in the system
 *
 *      All the sequences should finish with action  SOUND_ACTION_END (that means last tone in this sound)
 *      or SOUND_ACTION_REPEAT where the parameter defines how many times the whole sequence will be repeated.
 *  </pre>
 */
#define		OCTAVE				1
#define 	S5DH				(622 * OCTAVE)
#define		S5E					(659 * OCTAVE)
#define		S5F					(698 * OCTAVE)
#define		S5FH				(739 * OCTAVE)
#define		S5G					(783 * OCTAVE)
#define		S5GH				(830 * OCTAVE)
#define		S5A					(880 * OCTAVE)
#define		S5AH				(932 * OCTAVE)
#define		S5B					(987 * OCTAVE)
#define		S6C					(1056 * OCTAVE)
#define		S6CH				(1108 * OCTAVE)
#define		S6D					(1174 * OCTAVE)
#define		S6DH				(1244 * OCTAVE)
#define		S6E					(1318 * OCTAVE)
#define		S6F					(1396 * OCTAVE)
#define		S6FH				(1479 * OCTAVE)
#define		S6G					(1567 * OCTAVE)
#define		S6GH				(1661 * OCTAVE)
#define		S6A					(1760 * OCTAVE)
#define		S6AH				(1864 * OCTAVE)
#define		S6B					(1975 * OCTAVE)
#define		S7C					(2093 * OCTAVE)
#define		S7CH				(2217 * OCTAVE)
#define		S7D					(2349 * OCTAVE)

const SOUND_SEQUENCE_DEF    tSOUND_START[] = {
	//  Hz                On      Off     Chime           Action                  Parameter
	S5B, 10, 230, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0,
	S6D, 10, 230, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0,
	S6G, 10, 250, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S6FH,10, 460, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S7D, 10, 1000, SOUND_CHIME1,SOUND_ACTION_END, 0,
};
const SOUND_SEQUENCE_DEF    tSOUND_STOP[] = {
	S6G, 10, 460, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S6E, 10, 192, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S6FH, 10, 192, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0,
	S6D, 10, 1000, SOUND_CHIME1, SOUND_ACTION_END, 0,
};
const SOUND_SEQUENCE_DEF    tSOUND_LOCK_IN[] = {
	S6G, 10, 191, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S7D, 10, 1000, SOUND_CHIME1, SOUND_ACTION_END, 0,
};
const SOUND_SEQUENCE_DEF    tSOUND_LOCK_OUT[] = {
	S6B, 10, 191, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0,
	S6GH, 10, 1000, SOUND_CHIME1, SOUND_ACTION_END, 0,
};
const SOUND_SEQUENCE_DEF    tSOUND_WATEROUT_START[] = {
	S6B, 10, 240, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S7C, 10, 240, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0,
	S7D, 10, 1000, SOUND_CHIME1, SOUND_ACTION_END, 0,
};
const SOUND_SEQUENCE_DEF    tSOUND_WATEROUT_STOP[] = {
	S6A, 10, 240, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0,
	S6FH, 10, 240, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S6D, 10, 1000, SOUND_CHIME1, SOUND_ACTION_END, 0,
};
const SOUND_SEQUENCE_DEF    tSOUND_LOCK_ALARM[] = {
	S6D, 10, 120, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S7D, 10, 360, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S6D, 10, 120, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S7D, 10, 1000, SOUND_CHIME1, SOUND_ACTION_END, 0,
};
const SOUND_SEQUENCE_DEF    tSOUND_WATERLOW_ALARM[] = {
	S6A, 10, 163, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S6A, 10, 163, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S6A, 10, 1000, SOUND_CHIME1, SOUND_ACTION_END, 0,
};
const SOUND_SEQUENCE_DEF    tSOUND_FUNCTION_IN[] = {
	S6FH, 10, 240, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S7D, 10, 480, SOUND_CHIME1, SOUND_ACTION_END, 0,
};
const SOUND_SEQUENCE_DEF    tSOUND_FUNCTION_OUT[] = {
	S6FH, 10, 240, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S6A, 10, 240, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S6D, 10, 1000, SOUND_CHIME1, SOUND_ACTION_END, 0,
};
const SOUND_SEQUENCE_DEF    tSOUND_WARNING[] = {
	S6GH, 10, 236, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S6GH, 10, 1000, SOUND_CHIME1, SOUND_ACTION_END, 0,
};
const SOUND_SEQUENCE_DEF    tSOUND_ERROR[] = {
	S6A, 10, 326, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S6A, 10, 326, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S6A, 10, 1000, SOUND_CHIME1, SOUND_ACTION_END, 0,
};
const SOUND_SEQUENCE_DEF    tSOUND_REMEMBER[] = {
	S6FH, 10, 180, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S6GH, 10, 180, SOUND_CHIME1, SOUND_ACTION_CONTINUE, 0, 
	S6AH, 10, 1000, SOUND_CHIME1, SOUND_ACTION_END, 0,
};
const SOUND_SEQUENCE_DEF    tSOUND_CLICK[] = {
	S6AH, 10, 1000, SOUND_CHIME1, SOUND_ACTION_END, 0,
};
const SOUND_SEQUENCE_DEF    tSOUND_CLICK2[] = {
	S5F, 10, 500, SOUND_CHIME1, SOUND_ACTION_END, 0,
};
////////////////////////////////////////////////////////////////////////
// Sound System Configuration

#ifdef SOUND_CHIME1_FEATURE
    /**
     *  @brief Gpio Port used by the Chime 1 feature
     *  @note
     *  <pre>
     *      example:
     *      #define BUZZER_EN_PIN                 PORTA
     *  </pre>
     */
   // #define BUZZER_EN_PIN                 22//OUT_23_PIN //BuzzerEN

    /**
     *  @brief Definition of the state which will be applied to the gpio when the Chime should be charging the capacitor.
     *  @note
     *  <pre>
     *      example:
     *      #define CHIME1_ACTIVE               1  // chime 1 charges the capacitor when the portbit is set to 1
     *      #define CHIME1_ACTIVE               0  // chime 1 charges the capacitor when the portbit is set to 0
     *  </pre>
     */
    #define CHIME1_ACTIVE               1

    /**
     *  @brief Definition of the state which will be applied to the gpio when the Chime should be decay capacitor.
     *  @note
     *  <pre>
     *      example:
     *      #define CHIME1_DEACTIVE               1  // chime 1 decays the capacitor when the portbit is set to 1
     *      #define CHIME1_DEACTIVE               0  // chime 1 decays the capacitor when the portbit is set to 0
     *  </pre>
     */
    #define CHIME1_DEACTIVE             0
#endif

#ifdef SOUND_CHARGE_PUMP_CONTROL
    /**
     *  @brief Definition of PWM used for the Charge-pump
     *  @note
     *  <pre>
     *      example:
     *      #define CHARGE_PUMP_PWM                 PWM14
     *      #define CHARGE_PUMP_PWM                 PWM11
     *  </pre>
     */
    #define CHARGE_PUMP_PWM                 PWM14

    /**
     *  @brief Definition of frequency of operation for the charge pump Pwm
     *  @note The Chime GDM requires 50000 Hz
     *  <pre>
     *      example:
     *      #define CHARGE_PUMP_FREQUENCY           50000
     *  </pre>
     */
    #define CHARGE_PUMP_FREQUENCY           50000

    /**
     *  @brief Definition of the duty cycle of operation for the charge pump
     *  @note The Chime GDM requires 8750 which means 87.5% active. The number is giving time 100
     *  <pre>
     *      example:
     *      #define CHARGE_PUMP_DUTY_CYCLE          8750
     *  </pre>
     */
    #define CHARGE_PUMP_DUTY_CYCLE          8750

    /**
     *  @brief The charge pump energy save feature allows the system to reduce the duty cycle of the charge pump
     *  when i's not playing a sound.
     *  @note If this feature is disabled the charge pump Pwm will be working the the playing conditions wll the time
     *  <pre>
     *      example:
     *      #define CHARGE_ENERGY_SAVE
     *  </pre>
     */
    //#define CHARGE_ENERGY_SAVE

    /**
     *  @brief Definition of the duty cycle of operation for the charge pump when no sound is being played.
     *  @note The number is giving time 100, 300 means 3%
     *  <pre>
     *      example:
     *      #define CHARGE_PUMP_ENERGY_DUTY_CYCLE   300
     *  </pre>
     */
    #define CHARGE_PUMP_ENERGY_DUTY_CYCLE   300
#endif

/**
 *  @brief Definition how big is the sound queue
 *  @note
 *  <pre>
 *      example:
 *      #define SOUND_QUEUE_SIZE                5
 *  </pre>
 */
#define SOUND_QUEUE_SIZE                10
//=====================================================================================================================
//-------------------------------------- PUBLIC (Function Prototypes) -------------------------------------------------
//=====================================================================================================================

#endif // SOUND_PRV_H_


