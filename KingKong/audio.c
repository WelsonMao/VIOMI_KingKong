#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "ite/audio.h"
#include "audio_mgr.h"
#include "ctrlboard.h"
#include "buzzer/Sound.h"

static bool                       audioInited;
extern bool                       audioKeySoundPaused;
static AudioPlayCallback          audioPlayCallback;
static pthread_mutex_t            audioPlayMutex;

extern MMP_INT smtkAudioMgrPlayNetwork(SMTK_AUDIO_PARAM_NETWORK* pNetwork);

static int AudioPlayCallbackFunc(int state);

static void *               AL_Local_player_http_handle = NULL;

#define AUDIOLINK_LOCAL_PLAYER_BUFFER_LEN            (64 * 1024)
#define AUDIO_WAIT_INIT_I2S

void AudioKeySound(uint8_t index)
{
	Sound__Stop();
	Sound__SetVolume(tConfig.KeyVolumeVal / 10);//tConfig.KeyVolumeVal  
	//printf("play sound [%d],vol[%d],enable[%d-%d]\n", index, tConfig.KeyVolumeVal, tConfig.Enable_KeyVoice, bit_Config_KeyVoice);//theConfig.keylevel
	if (!tConfig.Enable_KeyVoice) return;// bit_Config_KeyVoice
	switch (index)
	{
	case SOUND_START:
		Sound__SimplePlay(SOUND_START_S);
		break;
	case SOUND_STOP:
		Sound__SimplePlay(SOUND_STOP_S);
		break;
	case SOUND_LOCK_IN:
		Sound__SimplePlay(SOUND_LOCK_IN_S);
		break;
	case SOUND_LOCK_OUT:
		Sound__SimplePlay(SOUND_LOCK_OUT_S);
		break;
	case SOUND_WATEROUT_START:
		Sound__SimplePlay(SOUND_WATEROUT_START_S);
		break;
	case SOUND_WATEROUT_STOP:
		Sound__SimplePlay(SOUND_WATEROUT_STOP_S);
		break;
	case SOUND_LOCK_ALARM:
		Sound__SimplePlay(SOUND_LOCK_ALARM_S);
		break;
	case SOUND_WATERLOW_ALARM:
		Sound__SimplePlay(SOUND_WATERLOW_ALARM_S);
		break;
	case SOUND_FUNCTION_IN:
		Sound__SimplePlay(SOUND_FUNCTION_IN_S);
		break;
	case SOUND_FUNCTION_OUT:
		Sound__SimplePlay(SOUND_FUNCTION_OUT_S);
		break;
	case SOUND_WARNING:
		Sound__SimplePlay(SOUND_WARNING_S);
		break;
	case SOUND_ERROR:
		Sound__SimplePlay(SOUND_ERROR_S);
		break;
	case SOUND_REMEMBER:
		Sound__SimplePlay(SOUND_REMEMBER_S);
		break;
	case SOUND_CLICK:
		Sound__SimplePlay(SOUND_CLICK_S);
		break;
	case SOUND_CLICK2:
		Sound__SimplePlay(SOUND_CLICK2_S);
		break;
	default:
		break;
	}
	
}
void AudioInit(void)
{
    smtkAudioMgrInitialize();
    //smtkAudioMgrSetCallbackFunction((int*)AudioPlayCallbackFunc);

    audioKeySoundPaused = false;
    audioInited         = false;

    pthread_mutex_init(&audioPlayMutex, NULL);

    audioInited = true;
    //smtkAudioMgrOpenEngine(SMTK_AUDIO_WAV);
}

void AudioExit(void)
{
    if (!audioInited)
    {
        return;
    }

    if (AudioIsPlaying())
    {
        AudioStop();
    }

    smtkAudioMgrTerminate();
    pthread_mutex_destroy(&audioPlayMutex);
}

static int AudioPlayCallbackFunc(int state)
{
    if (audioPlayCallback)
    {
        return audioPlayCallback(state);
    }
    return 0;
}

static int AudioPlayCallbackFuncEmpty(int state)
{
    // DO NOTHING
    return 0;
}

int AudioPlay(char* filename, AudioPlayCallback func)
{
    void *                   local_player_http_handle = NULL;
    SMTK_AUDIO_PARAM_NETWORK audiomgr_local;
    int                      nResult                  = 0;
    char*                    ext;

    ext = strrchr(filename, '.');
    if (!ext)
    {
        printf("Invalid file name: %s\n", filename);
        return -1;
    }
    ext++;


    pthread_mutex_lock(&audioPlayMutex);

#ifdef AUDIO_WAIT_INIT_I2S

    #ifdef __OPENRTOS__
    smtkAudioMgrQuickStop();
    #endif

    // close handler (if any)
    if (AL_Local_player_http_handle)
    {
        fclose(AL_Local_player_http_handle);
        AL_Local_player_http_handle = NULL;
    }
    
#endif
    
    
    audiomgr_local.audioMgrCallback = AudioPlayCallbackFunc;

    if (stricmp(ext, "wav") == 0)
    {
        audiomgr_local.nType            = SMTK_AUDIO_WAV;
        audioPlayCallback  = func;
        if(func == NULL)
            audiomgr_local.audioMgrCallback = AudioPlayCallbackFuncEmpty;        
    }
    else if (stricmp(ext, "mp3") == 0)
    {
        audiomgr_local.nType = SMTK_AUDIO_MP3;
        audioPlayCallback  = func;
    }
    else if (stricmp(ext, "wma") == 0)
    {
        audiomgr_local.nType = SMTK_AUDIO_WMA;
        audioPlayCallback  = func;
    }
    else
    {
        printf("Unsupport file format: %s\n", ext);
        pthread_mutex_unlock(&audioPlayMutex);
        return -1;
    }
#ifdef AUDIO_WAIT_INIT_I2S
    if (filename)
    {
        AL_Local_player_http_handle = fopen(filename, "rb");
    }
    if (!AL_Local_player_http_handle)
    {
        printf("[Main]%s() L#%ld: fopen error \r\n", __FUNCTION__, __LINE__);
        pthread_mutex_unlock(&audioPlayMutex);
        return -1;
    }
#endif
    audiomgr_local.pHandle     = AL_Local_player_http_handle;
    audiomgr_local.LocalRead   = fread;
    audiomgr_local.nReadLength = AUDIOLINK_LOCAL_PLAYER_BUFFER_LEN;
    audiomgr_local.bSeek       = 0;
    audiomgr_local.nM4A        = 0;
    audiomgr_local.bLocalPlay  = 1;
    audiomgr_local.pFilename   = filename;
    
    printf("[Main]%s() L#%ld:  %s success \r\n", __FUNCTION__, __LINE__, filename);
#ifdef CFG_AUDIO_FILTER_ENABLE
    audioFilterProcessEnable();
#endif
#ifdef AUDIO_WAIT_INIT_I2S
    nResult                    = smtkAudioMgrPlayNetwork(&audiomgr_local);
#else
    nResult                    = smtkSetFileQueue(audiomgr_local);
#endif
    
    pthread_mutex_unlock(&audioPlayMutex);
    return 0;
}

void AudioStop(void)
{
    audioPlayCallback = NULL;
    smtkAudioMgrQuickStop();
#ifdef CFG_AUDIO_FILTER_ENABLE
    audioFilterProcessDisable();
#endif
}

int AudioPlayMusic(char* filename, AudioPlayCallback func)
{
    bool muted = AudioIsMuted();
    //printf("muted=%d\n", muted);
    smtkAudioMgrSetVolume(theConfig.audiolevel);
    return AudioPlay(filename, func);
}

void AudioPlayKeySound(void)
{
#if 0
    char filepath[PATH_MAX];

    if (theConfig.keylevel > 0 && smtkAudioMgrGetState() != SMTK_AUDIO_PLAY && audioKeySoundPaused == false)
    {
        smtkAudioMgrSetVolume(theConfig.keylevel);
        strcpy(filepath, CFG_PRIVATE_DRIVE ":/sounds/");
        strcat(filepath, theConfig.keysound);
        AudioPlay(filepath, NULL);

    }
#else
	//AudioKeySound(SOUND_CLICK);
#endif
}

void AudioPauseKeySound(void)
{
    audioKeySoundPaused = true;
}

void AudioResumeKeySound(void)
{
    audioKeySoundPaused = false;
}

void AudioSetKeyLevel(int level)
{
    if (level < 0 || level > 100)
    {
        return;
    }

    theConfig.keylevel = level;
}

void AudioMute(void)
{
    smtkAudioMgrMuteOn();
}

void AudioUnMute(void)
{
    smtkAudioMgrMuteOff();
}

bool AudioIsMuted(void)
{
    return smtkAudioMgrIsMuteOn() ? true : false;
}

bool AudioIsPlaying(void)
{
    return smtkAudioMgrGetState() == SMTK_AUDIO_PLAY;
}

void AudioSetVolume(int level)
{
    if (level < 0 || level > 100)
    {
        return;
    }

    theConfig.audiolevel = level;
	
    smtkAudioMgrSetVolume(theConfig.audiolevel);
}

void AudioSetLevel(int level)
{
    if (level < 0 || level > 100)
        return;

    theConfig.audiolevel = level; 
}

int AudioGetVolume(void)
{
    return theConfig.audiolevel;
}
