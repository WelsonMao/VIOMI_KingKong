#ifndef _OTA_H__
#define __OTA_H__

#define BUF_SIZE                           1024//4096*2//8K
#define PATH_SIZE                          128 
#define FILE_MODE                          0664 
#define DIR_MODE                           0664  

#define MAX_LINE 1024
#define DEC_FILE_TITLE_NUM                 3

#define UP_DESC_MDATA_STR                  "MiscData"
#define UP_DESC_POWER_STR                  "Power"
#define UP_DESC_TP_STR                     "TP"
#define UP_DESC_TP_ITE                     "System"
#define UP_DESC_TOUCH_STR                  "Touch"
#define UP_DESC_OPTION_STR                 "OptionParam"

#define WIN32_DESC_FILE_PATH                "B:/misc-upgrade/desc.txt"
#define WIN32_WASHOPTION_SRC                "A:\sounds\\WashOptionParam.json"
#define WIN32_WASHOPTION_DST                "B:\misc-upgrade\\WashOptionParam1.json"


#define DESC_FILE_PATH                     "E:/misc-upgrade/desc.txt"
#define WASHOPTION_SRC_PATH               "E:/misc-upgrade/miscdata/WashOptionParam.json"
#define WASHOPTION_DST_PATH                 "B:/misc-upgrade/miscdata/WashOptionParam.json"

#define  UDISK_FILE_NONE                    0
#define  UDISK_FILE_FW                      1
#define  UDISK_FILE_WASHOPTION              2
#define  UDISK_FILE_FWTOUCH                 4   
#define  UDISK_FILE_TP                      8

typedef enum
{
	DESC_INDEX_TYPE = 0,
	DESC_INDEX_FILE = 1,
	DESC_INDEX_DESC = 2
};
typedef struct
{
	char t_str[DEC_FILE_TITLE_NUM][100];
	char c_str[DEC_FILE_TITLE_NUM][100];
}DESC_T;


uint8_t Udisk_Check_FileDesc(void);
void FileCopyTaskInit(void);
#endif