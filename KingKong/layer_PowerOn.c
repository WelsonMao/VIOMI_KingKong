#include <assert.h>
#include "scene.h"
#include "ite/itp.h"
#include "castor3player.h"
#include "ite/itv.h"
#include "bsp/COM.h"
#include "bsp/uart_process.h"
static ITULayer*PowerOnLayer, *LogoLayer;
static ITUBackground*PowerOnLayerBG;
static ITUVideo*PowerOnVideo;

char* filenames[] = {
	"B:/media/PowerOn.mkv"
};
/***************************************************************************/
static void PowerOnVideoOnStop(ITUVideo* video)
{
	printf("**********************PowerOnVideoOnStop=%d************\n", uTransmitDev.pkt.bPowerOnOff);
	if (uTransmitDev.pkt.bPowerOnOff)  ProcessLayerJump(LAYER_MAIN_MENU, 1);
	else                               ProcessLayerJump(LAYER_LOGO, 1);
}
/***************************************************************************/
void VideoSelectPlay(ITUVideo*video, uint8_t index)
{
	strcpy(video->filePath, filenames[index]);
	ituVideoPlay(video, 0);
}
bool PowerOnLayerOnEnter(ITUWidget* widget, char* param)
{
	if (!PowerOnLayer){
		PowerOnLayer = ituSceneFindWidget(&theScene, "PowerOnLayer");
		LogoLayer = ituSceneFindWidget(&theScene, "LogoLayer");
		PowerOnLayerBG = ituSceneFindWidget(&theScene, "PowerOnLayerBG");
		PowerOnVideo = ituSceneFindWidget(&theScene, "PowerOnVideo");
	}
	TimeBaseThreadInit();
#ifdef _WIN32
	ituLayerGoto(LogoLayer);
#else
	VideoSelectPlay(PowerOnVideo, 0);
	ituVideoSetOnStop(PowerOnVideo, PowerOnVideoOnStop);//PowerOnVideoOnStop
#endif
	OperatorParam.LayerID = LAYER_POWER_ON;
	AudioKeySound(SOUND_START);
    return true;
}
bool PowerOnLayerOnTimer(ITUWidget* widget, char* param)
{
	bool ret = false;
	return ret;
}
bool PowerOnLayerOnLeave(ITUWidget* widget, char* param)
{
    return true;
}



