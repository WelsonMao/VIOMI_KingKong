/**
 *  @file
 *
 *  @brief      Standard Sound Module
 *
 *  @details    Play sounds over Pwm or External player
 *
 *  $Header: Sound.c 1.31 2012/05/04 22:23:04CST Manoel G De Souza (SOUZAMG) Exp  $
 *
 *  @copyright  Copyright 2012-$Date: 2012/05/04 22:23:04CST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------

//-------------------------------------- Include Files ----------------------------------------------------------------

#include <sys/ioctl.h>
#include <unistd.h>
#include "libxml/parser.h"
#include "SDL/SDL.h"
#include "ite/itp.h"
#include "../ctrlboard.h"
#include "../scene.h"

#include "Sound.h"
#include "Sound_prv.h"          //New Style using Sound_prm.h and Sound_prv.h
#include "Sound_prm.h"

//-------------------------------------- PUBLIC (Variables) -----------------------------------------------------------

//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------
typedef enum
{
	TONE_IDLE,
	TONE_START,
	TONE_TON,
	TONE_TOFF
} TONE_HANDLER_DEF;

static SOUND_VOLUME_DEF Sound_Volume;
static SOUND_VOLUME_DEF Current_Sound_Volume;

#ifdef SOUND_SPECIFIC_VOLUME_TABLE
    const unsigned short int VOL_TABLE[SOUND_VOLUME_TABLE_MAX] =
    {
#if 0        
        0 ,                 // VOL_OFF
        10,                 // VOL_01
        15,                 // VOL_02
        25,                 // VOL_03
        42,                 // VOL_04
        85,                 // VOL_05
        175,                // VOL_06
        350,                // VOL_07
        700,                // VOL_08
        1200,               // VOL_09
        1800                // VOL_MAX
#else
        0 ,                 // VOL_OFF
        10,                 // VOL_01
        20,                 // VOL_02
        30,                 // VOL_03
        40,                 // VOL_04
        50,                 // VOL_05
        60,                // VOL_06
        70,                // VOL_07
        80,                // VOL_08
        90,               // VOL_09
        95                // VOL_MAX
#endif
    };
#else
    const unsigned short int VOL_TABLE[SOUND_VOLUME_TABLE_MAX] = VOL_TABLE_LIST;
#endif

static TONE_HANDLER_DEF Sound_Tone_State;
SEQUENCE_HANDLER_DEF Sound_Sequence_State;
static unsigned char Sequence_Index;
static unsigned char Sequence_Repeat_Counter;
static unsigned short int Tone_Counter;
static SOUND_SEQUENCE_DEF * Current_Sequence;
static SOUND_SEQUENCE_DEF * Current_Sound;
static unsigned char Silence_Step;

#ifdef SOUND_QUEUE_FEATURE

	typedef struct
	{
		SOUND_SEQUENCE_DEF * Sequence;
		SOUND_PRIORITY_DEF Priotity;
		SOUND_PREEMPTION_DEF Percistence;
#ifdef SOUND_VOLUME_QUEUE
		SOUND_VOLUME_DEF Volume;
#endif
		unsigned char Active;
	} SOUND_QUEUE_DEF;

	static SOUND_QUEUE_DEF Sound_Queue[SOUND_QUEUE_SIZE];
	static SOUND_QUEUE_DEF Sound_Aux;
#else
	static SOUND_PRIORITY_DEF Sound_Priority;
#endif

static SOUND_SEQUENCE_DEF * Last_Sequence;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

#ifndef	SOUND_DEFAULT_PRIORITY
    #define SOUND_DEFAULT_PRIORITY      2           //!< By default High Priority
#endif

#ifndef SOUND_DEFAULT_PERSISTENCE
    #define SOUND_DEFAULT_PERSISTENCE   0           //!< by default Keep last
#endif


#ifdef SOUND_QUEUE_FEATURE
	static void SoundPopSequence(unsigned char index);
	static void SoundQueueOpenSpace(unsigned char index);
	static void SoundInvertFirst2Sound(void);
#endif

#ifndef SOUND_HANDLER_5MS
	#define SOUND_TIMER_DIVIDER	1 //1ms
#else
	#define SOUND_TIMER_DIVIDER	5 //5ms
#endif

int BuzzerIoctl(int file, unsigned long request, int pwm, int duty)
{
	//printf("########[%s], [%d][%d][%d], [%d][%d][%d]\r\n", __FUNCTION__, BUZZER_PWM, BUZZER_PWM_PIN, BUZZER_GPIO_MODE, request, pwm, duty);
	switch (request){
	case ITP_PWM_INIT:
		ithPwmInit(BUZZER_PWM, 1500, 50);
		break;
	case ITP_PWM_RESET:
		ithPwmReset(BUZZER_PWM, BUZZER_PWM_PIN, BUZZER_GPIO_MODE);
		break;
	case ITP_PWM_ON:
		ithPwmEnable(BUZZER_PWM, BUZZER_PWM_PIN, BUZZER_GPIO_MODE);
		break;
	case ITP_PWM_OFF:
		ithPwmDisable(BUZZER_PWM, BUZZER_PWM_PIN);
		break;
	case ITP_PWM_GET_MAX_LEVEL:
		break;
	case ITP_PWM_SET_OUTPUT:
		ithPwmInit(BUZZER_PWM, pwm, duty);
		ithPwmReset(BUZZER_PWM, BUZZER_PWM_PIN, BUZZER_GPIO_MODE);
		ithPwmEnable(BUZZER_PWM, BUZZER_PWM_PIN, BUZZER_GPIO_MODE);
		break;
	default:
		break;
	}
	return 0;
}
uint8_t port_pin_set_output_level(uint8_t port,uint8_t onoff)
{
    if(onoff){
		ithGpioSetMode(port, ITH_GPIO_MODE0);
		ithGpioSetOut(port);
		ithGpioSet(port);
		ithGpioEnable(port);
    }
    else{
		ithGpioSetMode(port, ITH_GPIO_MODE0);
		ithGpioSetOut(port);
		ithGpioClear(port);
		ithGpioEnable(port);   
    }   
}

void Pwm__Initialize(void)
{
    //printf("[%s], [%d][%d]\r\n", __FUNCTION__, 0, 0);
	BuzzerIoctl(0, ITP_PWM_INIT, 0, 0);//init
}


void Pwm__Start(int freq, int duty)
{
    //printf("[%s], [%d][%d]\r\n", __FUNCTION__, freq, duty);
	BuzzerIoctl(0, ITP_PWM_SET_OUTPUT, freq, duty);//on
}


void Pwm__Stop(void)
{
    //printf("[%s], [%d][%d]\r\n", __FUNCTION__, 0, 0);
	BuzzerIoctl(0, ITP_PWM_OFF, 0, 0);//on
}


//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It initialize the whole Sound system as the Charge pump and other circuits as well as the variables.
 *
 */
void Sound__Initialize(void)
{
	//unsigned char count;
    Pwm__Initialize();
	// Configure the GPIO used for the Chime 1 circuit
	#ifdef SOUND_CHIME1_FEATURE
	//port_pin_set_output_level(BUZZER_EN_PIN,false);
	port_pin_set_output_level(BUZZER_EN_PIN,false);
	#endif

	// Configure the Charge pump PWM
	#ifdef SOUND_CHARGE_PUMP_CONTROL
		Pwm__Initialize();

		#ifdef CHARGE_ENERGY_SAVE
			Pwm__Start(CHARGE_PUMP_FREQUENCY,CHARGE_PUMP_ENERGY_DUTY_CYCLE);
		#else
			Pwm__Start(CHARGE_PUMP_FREQUENCY,CHARGE_PUMP_DUTY_CYCLE);
		#endif
	#endif

	Sound_Tone_State = TONE_IDLE;					// Initialize the Tone generation state machine
	Sound_Sequence_State = SEQUENCE_IDLE;			// Initialize the Sound Sequence state machine
	Sound_Volume = SOUND_VOLUME_MAX;				// Initialize the Volume
	Sound_Priority = SOUND_PRIOTITY_LOW;

	Current_Sound_Volume = SOUND_VOLUME_MAX;        // Initialize the Volume

	// If there's queue, initialize it
	#ifdef SOUND_QUEUE_FEATURE
		for (count=0 ; count<SOUND_QUEUE_SIZE ; count++)
		{
			Sound_Queue[count].Active = 0;
		}
	#endif

}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      This method handles the Sound Sequence and the Tone generation state machines. (5ms)
 *  @image html Sequence_Hanlder.jpg
 */
void Sound__Handler(void)
{
	SoundToneHandler();								// Tone Generation State machine

	// Sound Sequence State Machine
	switch (Sound_Sequence_State)
	{
		case SEQUENCE_IDLE:							// Waiting, no sound being played
			if (Sound_Tone_State == TONE_IDLE)
			{
				#ifdef SOUND_QUEUE_FEATURE          // If the Queue is enabled
					if (Sound_Queue[0].Active)		// If the head of the queue is active
					{
						Sound_Sequence_State = SEQUENCE_PLAYING;	// Set next state

						// It tests if the second sound in the queue has highest priority than the head
						if ((Sound_Queue[1].Active == 1) &&
							(Sound_Queue[1].Priotity > Sound_Queue[0].Priotity))
						{
							// If yes, invert them
							SoundInvertFirst2Sound();
						}

						// Catch the sound in the queue's head
						Current_Sequence = Sound_Queue[0].Sequence;
						Last_Sequence = Current_Sequence;

#ifdef SOUND_VOLUME_QUEUE
						Current_Sound_Volume = Sound_Queue[0].Volume;
#else
						Current_Sound_Volume = Sound_Volume;
#endif
					}
                #else
					Current_Sound_Volume = Sound_Volume;
				#endif
				Sequence_Index = 0;					//< Initialize the tone index for the sequence
				Sequence_Repeat_Counter = 0;		//< Initialize the Sequence repeat counter
			}
			break;
		case SEQUENCE_PLAYING:						//< Play a sound
			if (Sound_Tone_State == TONE_IDLE)
			{
				Current_Sound = Current_Sequence + Sequence_Index;	// Get the current tone of the sequence
				Sound_Tone_State =	TONE_START;		//< start the Tone generation state machine
				Sound_Sequence_State = SEQUENCE_NEXT;	//< Go to the next state
			}
			break;
		case SEQUENCE_NEXT:							//< In this state, the method decides about the next step
			if (Sound_Tone_State == TONE_IDLE)
			{
				switch (Current_Sound->Action)
				{
					case SOUND_ACTION_REPEAT:		//< Repeat the sequence
						Sequence_Repeat_Counter++;
						if ((Sequence_Repeat_Counter < Current_Sound->Parameter) ||
							(Current_Sound->Parameter == 0))
						{
							Sequence_Index = 0;
							Current_Sound = Current_Sequence + Sequence_Index;   // Get the current tone of the sequence
  		                    Sound_Tone_State =  TONE_START;     //< start the Tone generation state machine
						}
						else
						{
							#ifdef SOUND_QUEUE_FEATURE
								SoundPopSequence(0);
							#endif
							Sound_Sequence_State = SEQUENCE_IDLE;
						}
						break;
					case SOUND_ACTION_CONTINUE:		// Play the next tone
						Sequence_Index++;
						Current_Sound = Current_Sequence + Sequence_Index;    // Get the current tone of the sequence
						Sound_Tone_State =  TONE_START;     // start the Tone generation state machine
						break;
					case SOUND_ACTION_END:			// Ends the sequence
					default:
						#ifdef SOUND_QUEUE_FEATURE
							SoundPopSequence(0);
						#endif
						Sound_Sequence_State = SEQUENCE_IDLE;
						break;
				}
				if (Sound_Sequence_State != SEQUENCE_IDLE)
				{
				    SoundToneHandler();
				}
			}
			else
			{
				// The following  code takes care of the preemption feature which allows a high priority soun to interrupt
				// a low priority sound while it is in a silence step
				#ifdef SOUND_QUEUE_FEATURE
                #ifdef SOUND_PRIORITY_PREEMPTION_FEATURE
					if ((Sound_Queue[1].Active == 1) &&
						(Sound_Queue[1].Priotity > Sound_Queue[0].Priotity) &&
						(Silence_Step == 1))
					{
						if (Sound_Queue[0].Percistence == SOUND_PREEMPTION_KEEP)
						{
							// Exchange the current song with the next
							SoundInvertFirst2Sound();
						}
						else
						{
							// Discard the current song
							SoundPopSequence(0);
						}
						Sound_Sequence_State = SEQUENCE_IDLE;
						Sound_Tone_State = TONE_IDLE;
					}
				#endif
                #endif
			}
			break;
		default:
			Sound_Sequence_State = SEQUENCE_IDLE;
			break;
	}
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      This method start to play a sound or push it in the queue, depending on the module configuration.
 *
 *  @param      sequence    : Pointer to a sound structure
 *  @return     unsigned char
 *      @retval   TRUE: the requested sound will be played
 *      @retval   FALSE: It's no possible to play the requested sound
 */
unsigned char Sound__SimplePlay(SOUND_SEQUENCE_DEF * sequence)
{
    return (Sound__Play(sequence,(SOUND_PRIORITY_DEF)SOUND_DEFAULT_PRIORITY,(SOUND_PREEMPTION_DEF)SOUND_DEFAULT_PERSISTENCE));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      This method start to play a sound or push it in the queue, depending on the module configuration.
 *
 *  @param 		sequence	: Pointer to a sound structure
 *  @param 		priority	: Priority to play this sound
 * 	@param 		preemption	: It set the preemption mode
 *  @return     unsigned char
 *   	@retval   TRUE: the requested sound will be played
 *   	@retval   FALSE: It's no possible to play the requested sound
 */
unsigned char Sound__Play(SOUND_SEQUENCE_DEF * sequence,
						  SOUND_PRIORITY_DEF priority,
						  SOUND_PREEMPTION_DEF preemption)
{
	unsigned char response = 0;
	//unsigned char count;
	//unsigned char index;

	#ifdef SOUND_QUEUE_FEATURE
		index = 0xFF;

		// Is the Queue empty
		if (Sound_Queue[0].Active == 0)
		{
			// Queue empty
			index = 0;								// The first position is available
		}
		else
		{
			// Queue not empty
			// Look for a position in the Queue for the requested sound
			for (count=1 ; count<SOUND_QUEUE_SIZE ; count++)
			{
				if ( (Sound_Queue[count].Active == 0) ||	// If Queue position is empty OR
					 (Sound_Queue[count].Priotity < priority))	// The sound in the position has lower priority
				{
					index = count;					// St the current position as the destination to the sound
					break;
				}
			}
		}

		if (index < 0xFF)							// If a position was found in the Queue
		{
			SoundQueueOpenSpace(index);				// Adjust the queue to open a space in the selected position

			// Fill the selected position with the requested data
			Sound_Queue[index].Sequence = sequence;
			Sound_Queue[index].Priotity = priority;
			Sound_Queue[index].Percistence = preemption;
			Sound_Queue[index].Active = 1;
#ifdef SOUND_VOLUME_QUEUE
			Sound_Queue[index].Volume = Sound_Volume;
#endif
			response = 1;
		}
	#else
		//Systems without Queues
		if (
			((Sound_Tone_State == TONE_IDLE) &&		// the module is not playing a sound
			 (Sound_Sequence_State == SEQUENCE_IDLE)) ||	// OR
			(Sound_Priority <= priority)				// the requested sound has higher priority
		    )
		{
		    Current_Sound_Volume = Sound_Volume;
		    response = 1;
			Current_Sequence = sequence;			// Load the Sound sequence
			Last_Sequence = Current_Sequence;
			Sound_Priority = priority;
			Sequence_Index = 0;

			// Frequency = 0 => Silence
            Pwm__Stop();
            Silence_Step = 1;

			Sequence_Repeat_Counter = 0;
			Sound_Tone_State = TONE_IDLE;

			Sound_Sequence_State  = SEQUENCE_PLAYING;
		}
	#endif

	return (response);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      This method stops the current sound.
 *
 *  @return     unsigned char
 *   	@retval		TRUE:  A sound which was be played stops
 *   	@retval  	FALSE: No sound being played
 */
unsigned char Sound__Stop(void)
{
	unsigned char result = 0;
    #ifdef SOUND_QUEUE_FEATURE
        #if (SOUND_CLEAR_QUEUE_WHEN_STOPPED == ENABLED)
            unsigned char count;
        #endif
    #endif

	if (Sound_Sequence_State != SEQUENCE_IDLE)
	{
		#ifdef SOUND_QUEUE_FEATURE
			#if (SOUND_CLEAR_QUEUE_WHEN_STOPPED == ENABLED)
				// Clear Queue
				for (count=0 ; count<SOUND_QUEUE_SIZE ; count++)
				{
					Sound_Queue[count].Active = 0;
				}
			#else
				// Remove the the sound from the head of the Queue
				SoundPopSequence(0);
			#endif

		#endif

        Pwm__Stop();
        #ifdef CHARGE_ENERGY_SAVE
            Pwm__Start(CHARGE_PUMP_FREQUENCY,CHARGE_PUMP_ENERGY_DUTY_CYCLE);
        #endif

        #ifdef SOUND_CHIME1_FEATURE
            port_pin_set_output_level(BUZZER_EN_PIN,false);
        #endif

		Sound_Sequence_State = SEQUENCE_IDLE;
		Sound_Tone_State = TONE_IDLE;
		result = 1;
	}
	return (result);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      This method is used to set the sound Volume.
 *
 *  @param      volume: Requested volume
 */
void Sound__SetVolume(SOUND_VOLUME_DEF volume)
{
	if (volume < SOUND_VOLUME_TABLE_MAX)
	{
		Sound_Volume = volume;
	}
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      This method is used to get current status from the Sound module.
 *
 *  @param      index: Specify which status is being required
 *  @return		unsigned char The current status requested
 */
unsigned char Sound__GetStatus(SOUND_STATUS_DEF index)
{
	unsigned char response = 0;

	switch (index)
	{
		case SOUND_HANDLER_STATE:					// Current Sound Sequencer State
			response = Sound_Sequence_State;
			break;
		case SOUND_CURRENT_VOLUME:					// Current Sound Volume
			response = Sound_Volume;
			break;
		case SOUND_CURRENT_TONE_STATE:
		    response = SOUND_TONE_OFF;
		    if (Silence_Step == 0)
            {
                response = SOUND_TONE_ON;
            }
		    break;
		default:
			break;
	}
	return (response);
}

#ifdef SOUND_PAUSE_RESUME_FEATURE

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Pause the current sound.
 *
 *  @return     unsigned char
 */
unsigned char Sound__Pause(void)
{
	return 0;
}
#endif

#ifdef SOUND_PAUSE_RESUME_FEATURE
//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Resume a paused sound
 *
 *  @return		unsigned char
 */
unsigned char Sound__Resume(void)
{
	unsigned char response = 0;


	return (response);
}
#endif

/**
 * @brief This method exposes the sound being played.
 * @return returns a pointer the sound being played. If no sounds is being player returns NULL.
 */
SOUND_SEQUENCE_DEF * Sound__GetCurrentSound(void)
{
    if (Sound_Sequence_State != SEQUENCE_IDLE)
    {
        return (Last_Sequence);
    }
    else
    {
        return ((SOUND_SEQUENCE_DEF *)NULL);
    }
}

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

#ifdef SOUND_QUEUE_FEATURE
//---------------------------------------------------------------------------------------------------------------------
/**
 *   @brief 		This method is used to open a space in the Sound Queue
 *
 *   @details   	The sounds from the position requested to the end of the Queue are shifted one position backward
 *				If there was Sound in the last position of the Queue this sound will be lost
 *   @param			index : specify the position to be opened
 *
 */
static void SoundQueueOpenSpace(unsigned char index)
{
	unsigned char count;
	if (index < SOUND_QUEUE_SIZE)
	{
		for (count=SOUND_QUEUE_SIZE-1 ; count > index ; count--)
		{
			Sound_Queue[count].Active = Sound_Queue[count-1].Active;
			Sound_Queue[count].Sequence = Sound_Queue[count-1].Sequence;
			Sound_Queue[count].Priotity = Sound_Queue[count-1].Priotity;
#ifdef SOUND_VOLUME_QUEUE
			Sound_Queue[count].Volume = Sound_Queue[count-1].Volume;
#endif
		}
	}
}
#endif

#ifdef SOUND_QUEUE_FEATURE
//---------------------------------------------------------------------------------------------------------------------
/**
 *   @brief		This method removes a sound from the Queue
 *
 *   @details   	The Sounds after the sound selected to be deleted will be moved one position forward in the Queue
 *
 *   @param 		index : Specifies the Sound to be deleted from the Queue
 *
 */
static void SoundPopSequence(unsigned char index)
{
	unsigned char count;
	if (index < SOUND_QUEUE_SIZE)
	{
		for (count=index ; count< (SOUND_QUEUE_SIZE-1) ; count++)
		{
			Sound_Queue[count].Active = Sound_Queue[count+1].Active;
			Sound_Queue[count].Sequence = Sound_Queue[count+1].Sequence;
			Sound_Queue[count].Priotity = Sound_Queue[count+1].Priotity;
#ifdef SOUND_VOLUME_QUEUE
			Sound_Queue[count].Volume = Sound_Queue[count+1].Volume;
#endif
		}
		Sound_Queue[SOUND_QUEUE_SIZE-1].Active = 0;
	}
}
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
 *   @brief    	This method handles one tone, included Chime Charge and Discharge control
 *
 *   @details 	Local Function  called every 1ms
 *   @image html Tone_Hanlder.jpg
 */
void SoundToneHandler(void)
{

    SOUND_VOLUME_DEF instant_volume;
	#ifdef SOUND_VOLUME_QUEUE
		instant_volume = Current_Sound_Volume;
	#else
		instant_volume = Sound_Volume;
	#endif

	if (Tone_Counter)								// Local Time Base Counter
	{
		Tone_Counter--;
	}

	switch (Sound_Tone_State)
	{
		case TONE_IDLE:								// Idle State (don't playing sound)
			Silence_Step = 1;
			break;
		case TONE_START:
			if (Current_Sound->Frequency != 0)
			{
			    //Sound_Volume
			    if (Current_Sound->Action != SOUND_ACTION_REPEAT)
			    {
                    if ((Current_Sound->Parameter > 0) &&
                        (Current_Sound->Parameter <= 10))
                    {
                        instant_volume = (SOUND_VOLUME_DEF)(Current_Sound->Parameter * (unsigned char)instant_volume);
                        instant_volume = (SOUND_VOLUME_DEF)((unsigned char)instant_volume / 10);
                    }
			    }

			    // Configure the Charge pump PWM
				#ifdef CHARGE_ENERGY_SAVE
				    Pwm__Start(CHARGE_PUMP_FREQUENCY,CHARGE_PUMP_DUTY_CYCLE);
				#endif

				// Valid frequency
				Pwm__Start(Current_Sound->Frequency, VOL_TABLE[instant_volume]);

				// Handles the Chime circuit
				if (Current_Sound->Chime == SOUND_CHIME1)
				{
					#ifdef SOUND_CHIME1_FEATURE
						port_pin_set_output_level(BUZZER_EN_PIN,true);
				    #endif
				}

				Silence_Step = 0;
			}
			else
			{
				// Frequency = 0 => Silence
			    Pwm__Stop();
				Silence_Step = 1;
			}

			Tone_Counter = Current_Sound->TOn / SOUND_TIMER_DIVIDER;		// Reload Counter
			Sound_Tone_State = TONE_TON;
			break;
		case TONE_TON:
			if (!Tone_Counter)
			{
				Tone_Counter = Current_Sound->TOff  / SOUND_TIMER_DIVIDER;
				Sound_Tone_State = TONE_TOFF;

				// Handles the Chime Circuits
				if (Current_Sound->Chime == SOUND_CHIME1)
				{
					#ifdef SOUND_CHIME1_FEATURE
						port_pin_set_output_level(BUZZER_EN_PIN,false);
					#endif
				}
				// No Break, it falls trough
			}
			else
			{
			    break;
			}
			//break; The break depends on the condition above..
		case TONE_TOFF:
			if (!Tone_Counter)
			{
				Sound_Tone_State = TONE_IDLE;
				if (Current_Sound->Action != SOUND_ACTION_CONTINUE)
				{
                    Pwm__Stop();
                    #ifdef CHARGE_ENERGY_SAVE
                        Pwm__Start(CHARGE_PUMP_FREQUENCY,CHARGE_PUMP_ENERGY_DUTY_CYCLE);
                    #endif
				}
				Silence_Step = 1;
			}
			break;
		default:
			break;
	}
}

#ifdef SOUND_QUEUE_FEATURE
//---------------------------------------------------------------------------------------------------------------------
/**
 *   @brief		This method swaps the sound Queue position 0 and 1
 *
 *   @details   	Called every time that it's necessary to swap queue position 0 and 1
 *
 */
static void SoundInvertFirst2Sound(void)
{
	// Move the Queue head to an auxiliary Sound
	Sound_Aux.Sequence = Sound_Queue[0].Sequence;
	Sound_Aux.Priotity = Sound_Queue[0].Priotity;
	Sound_Aux.Percistence = Sound_Queue[0].Percistence;
#ifdef SOUND_VOLUME_QUEUE
	Sound_Aux.Volume = Sound_Queue[0].Volume;
#endif


	// move the sound from Queue position 1 to position 0
	Sound_Queue[0].Sequence = Sound_Queue[1].Sequence;
	Sound_Queue[0].Priotity = Sound_Queue[1].Priotity;
	Sound_Queue[0].Percistence = Sound_Queue[1].Percistence;
#ifdef SOUND_VOLUME_QUEUE
	Sound_Queue[0].Volume = Sound_Queue[1].Volume;
#endif

	// Move the sound from the auxiliary Sound to the position 1
	Sound_Queue[1].Sequence = Sound_Aux.Sequence;
	Sound_Queue[1].Priotity = Sound_Aux.Priotity;
	Sound_Queue[1].Percistence = Sound_Aux.Percistence;
#ifdef SOUND_VOLUME_QUEUE
	Sound_Queue[1].Volume = Sound_Aux.Volume;
#endif
}
#endif

void *BuzzerTaskTimerFunc(void* arg)
{
	int Soundtime = 1000;//us//*50
	static uint32_t SoundTick = 0;
	static uint16_t Cnt = 0;
	SoundTick = itpGetTickCount();
	printf("=======Start SoundTaskTimerFunc!=======\n");
	while (1){
		usleep(Soundtime);
		Sound__Handler();
	}
	printf("=======end SoundTaskTimerFunc!=======\n");
	return 0;
}
void Buzzer_TimeInit(void)
{
	static  volatile bool  AcitonOnce = false;
	pthread_t Soundtask;
	pthread_attr_t Soundattr;
	//struct sched_param param;
	//param.sched_priority = 3;
	if ((!ScreenIsOff()) && (AcitonOnce == false))
	{
		printf("=======Start Buzzer_TimeInit!=======\n");
		pthread_attr_init(&Soundattr);
		//pthread_attr_setschedparam(&Soundattr, &param);
		pthread_create(&Soundtask, &Soundattr, BuzzerTaskTimerFunc, NULL);//
		AcitonOnce = true;
	}
}
