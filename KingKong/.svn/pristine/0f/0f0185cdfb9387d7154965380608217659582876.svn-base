#include <assert.h>
#include "../scene.h"
#include <sys/ioctl.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "../ctrlboard.h"
#include "ite/itp.h"
#include "ite/ith.h"
#include "OTA.h"

#include <stdarg.h>
#include <pthread.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
/***********************************************************************************/
DESC_T tDesc;

/***********************************************************************************/
void sys_err(const char * ptr)
{
	printf("[Copy Error]:%s\r\n", ptr);
	return;
}

void read_write(FILE* Src, FILE *Dst, int filesize)
{
	//char buf[BUF_SIZE];
	unsigned char *readBuf = NULL;
	int readsize, ret_read, ret_write;
	static int TotalSize = 0;
	static int CurSize = 0;
	printf("filesize=%d\n", filesize);
	if (filesize > BUF_SIZE){
		readBuf = (unsigned char*)malloc(BUF_SIZE + 1);
		readsize = BUF_SIZE;
	}
	else{
		readBuf = (unsigned char*)malloc(filesize + 1);
		readsize = filesize;
	}
	//printf("Copy File Start\r\n");
	fseek(Src, 0, SEEK_END);
	TotalSize = ftell(Src);
	fseek(Src, 0, SEEK_SET);
	CurSize = 0;
	while ((ret_read = fread(readBuf, 1, readsize, Src)) > 0){
		
		//printf("read len:[%d]-[%d]\r\n", ret_read, ftell(Src));
		//ret_write = fwrite(readBuf, 1, ret_read, Dst);
		ret_write = 0;               //xx fix the bug of repeat upgrade
		while (ret_write < ret_read)
		{
			ret_write += fwrite(readBuf + ret_write, 1, (ret_read - ret_write), Dst);
			CurSize += ret_write;
			//printf("%d->%d\n", ret_write, ret_read);
		}
		if (ret_write == -1){
			sys_err("write newfile");
		}
		else{
			printf("write len:[%d]\r\n", ret_write);
		}
		tOTA.Percent = CurSize * 100 / TotalSize;
		printf("CurSize=%d,TotalSize=%d,Percent=%d\n", CurSize, TotalSize, tOTA.Percent);
	}
	free(readBuf);
	//printf("Copy File End:[%d]\r\n", ret_read);
}
void deal_copy(const char *old_file, const  char * pathname)
{
	uint32_t tick1, tick2;
	int ret_stat;
	struct stat dstFileMsg, srcFileMsg;

	FILE *fdSrc = NULL;
	FILE *fdDst = NULL;

	ret_stat = stat(old_file, &srcFileMsg);
	if (ret_stat == -1 && errno != ENOENT){
		sys_err("stat in srcFileMsg");
	}

	ret_stat = stat(pathname, &dstFileMsg);
	if (ret_stat == -1 && errno != ENOENT){
		sys_err("stat in dstFileMsg");
	}

	fdSrc = fopen(old_file, "rb");
	if (fdSrc == NULL)

	{
		sys_err("open oldfile in copy_file");
		return;
	}

	ret_stat = 1;

	if (ret_stat){//ret_stat == 1, not the same file, rewrite it.
		//fdDst = fopen(pathname, "wb+");
		fdDst = fopen(pathname, "wb");
		if (fdDst == NULL)
		{
			sys_err("open newfile in copy_file");
		}
	}

	tick1 = SDL_GetTicks();


	if (ret_stat){
		fseek(fdSrc, 0, SEEK_SET);//xx fix the bug of repeat upgrade fw
		read_write(fdSrc, fdDst, srcFileMsg.st_size);
		fclose(fdDst);
	}

	fclose(fdSrc);
}
void mkdir_newdir(const char * new_path)
{
	DIR * ret_opdir = opendir(new_path);
	//if(ret_opdir == NULL)
	{
		int ret_mkdir = mkdir(new_path, DIR_MODE);
		//if(ret_mkdir == -1)
		//sys_err("mkdir newdir");
	}

	if (ret_opdir){
		closedir(ret_opdir);
	}
}
void get_allpath(char * buf_old, char * buf_new, const char * old_path, const char * new_path)
{
	char buf[PATH_SIZE];
	getcwd(buf, PATH_SIZE);
	//printf("getcwd current:%s\n", buf);
	chdir(old_path);
	getcwd(buf_old, PATH_SIZE);

#ifdef _WIN32
	strcat(buf_old, "\\");
#else
	strcat(buf_old, "/");
#endif
	//printf("getcwd old:%s\n", buf_old);

	chdir(buf);
	mkdir_newdir(new_path);
	chdir(new_path);
	getcwd(buf_new, PATH_SIZE);
#ifdef _WIN32
	strcat(buf_new, "\\");
#else
	strcat(buf_new, "/");
#endif
	//printf("getcwd new:%s\n", buf_new);

}
void init_allpath(char * buf_old, char * buf_new, char * buf_oldfile, char * buf_newfile, const char * name)
{
	memset(buf_oldfile, 0, PATH_SIZE);
	memset(buf_newfile, 0, PATH_SIZE);
	strcpy(buf_oldfile, buf_old);
	strcat(buf_oldfile, name);
	strcpy(buf_newfile, buf_new);
	strcat(buf_newfile, name);
}

void deal_dir(const char *old_path, const char * new_path)
{
	DIR * ret_opdir = opendir(old_path);//闂佺懓鐏氶幐鍝ユ閹达附鍎庢い鏃囧亹缁?
	if (ret_opdir == NULL)
		sys_err("opendir");
	struct dirent * ret_redir;

	char buf_old[PATH_SIZE] = { 0 };
	char buf_new[PATH_SIZE] = { 0 };
	char buf_oldfile[PATH_SIZE] = { 0 };
	char buf_newfile[PATH_SIZE] = { 0 };

	//printf("old_path:[%s]\r\n", old_path);
	//printf("new_path:[%s]\r\n", new_path);
	get_allpath(buf_old, buf_new, old_path, new_path);
	//printf("buf_old:[%s]\r\n", buf_old);
	//printf("buf_new:[%s]\r\n", buf_new);
	while ((ret_redir = readdir(ret_opdir))){
		if (!strcmp(ret_redir->d_name, ".") || !strcmp(ret_redir->d_name, ".."))
			continue;
		else{
			init_allpath(buf_old, buf_new, buf_oldfile, buf_newfile, ret_redir->d_name);
			if ((ret_redir->d_type & DT_REG) == DT_REG)
				deal_copy(buf_oldfile, buf_newfile);
			else if ((ret_redir->d_type & DT_DIR) == DT_DIR)
				deal_dir(buf_oldfile, buf_newfile);
		}
	}
	if (ret_opdir){
		closedir(ret_opdir);
	}
}

int get_file_num(char* root)
{
	int total = 0;
	DIR* dir = NULL;
	dir = opendir(root);
	char path[128];
	struct dirent* ptr = NULL;
	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
		{
			continue;
		}
		if (ptr->d_type == DT_DIR)
		{
			sprintf(path, "%s/%s", root, ptr->d_name);
			total += get_file_num(path);
		}
		if (ptr->d_type == DT_REG)
		{
			total++;
		}
	}
	closedir(dir);
	return total;
}
void func_file_copy(char *srcDir, char *dstDir)
{
	uint8_t ret_stat;
	struct stat get_message;

	ret_stat = stat(srcDir, &get_message);

	if (S_ISDIR(get_message.st_mode))
		deal_dir(srcDir, dstDir);
	else{
		if (strcmp(srcDir, dstDir) == 0){
			printf("The same filename of parameter is error!\n");
			exit(EXIT_FAILURE);
		}
		deal_copy(srcDir, dstDir);
	}
}
int WashOption_search_dir(void)
{
	int ret_stat = 0;
	struct stat get_message;
#ifdef _WIN32
	if (-1 != stat(WIN32_WASHOPTION_SRC, &get_message)) ret_stat = 1;
	if (ret_stat == 0)  printf("%s stat failed\r\n", WIN32_WASHOPTION_SRC);
#else
	if (-1 != stat(WASHOPTION_SRC_PATH, &get_message)){
		ret_stat = 1;
	}
	if (ret_stat == 0){
		printf("%s stat failed\r\n", WASHOPTION_SRC_PATH);
	}
#endif
	return ret_stat;
}

int WashOption_start_copy(void)
{
	int ret_stat;
	struct stat get_message;

#ifdef _WIN32
	char * srcDir = WIN32_WASHOPTION_SRC;
	char * dstDir = WIN32_WASHOPTION_DST;
#else
	char * srcDir = WASHOPTION_SRC_PATH;
	char * dstDir = WASHOPTION_DST_PATH;
#endif

	ret_stat = 0;

	if (-1 != stat(srcDir, &get_message)){
		ret_stat = 1;
	}

	if (ret_stat == 0){
		sys_err(srcDir);
	}
	else{
		printf("[%s] total File num:[%d]\r\n", srcDir, get_file_num(srcDir));

		func_file_copy(srcDir, dstDir);
	}
	return 0;
}

uint8_t Udisk_Get_DescFile(char*path, DESC_T*data)
{
	char getstr[100];
	int index = 0;
	FILE*fp = NULL;
	char*temp = NULL;

	fp = fopen(path, "r");
	if (fp == NULL)
	{
		printf("desc.txt open fail !\n");
		return 0;
	}
	memset(data, 0, sizeof(DESC_T));
	while (fgets(getstr, MAX_LINE, fp) != NULL)
	{
		getstr[strlen(getstr) - 1] = '\0';
		temp = strtok(getstr, "=");
		while (temp != NULL)
		{
			printf("index[%d]:%s\n",index,temp);
			if (index % 2) strcpy(data->c_str[index / 2], temp);
			else        strcpy(data->t_str[index / 2], temp);
			temp = strtok(NULL, "=");
			index++;
		}
	}
	fclose(fp);
	for (int i = 0; i < 3; i++){
		//printf("%s\r%s\n", data->c_str[i], data->t_str[i]);
	}

	return 1;
}

uint8_t Udisk_Check_FileDesc(void)
{
	uint8_t ret = 0;
	uint8_t uDiskFile = UDISK_FILE_NONE;
#if 1//def _WIN32
	if (WashOption_search_dir())
	{
		ret |= UDISK_FILE_WASHOPTION;
	}
	return ret;
#else
#ifdef _WIN32
	ret = Udisk_Get_DescFile(WIN32_DESC_FILE_PATH, &tDesc);
#else
	ret = Udisk_Get_DescFile(DESC_FILE_PATH, &tDesc);
#endif
	if (ret)
	{
		printf("=====udiskDescFileType:%s\n", tDesc.c_str[DESC_INDEX_TYPE]);
		/*if (!stricmp(tDesc.c_str[DESC_INDEX_TYPE], UP_DESC_MDATA_STR))
		{
			printf("=======Check descfile:MiscData\n");
			uDiskFile = Udisk_Check_File();
		}
		if (!stricmp(tDesc.c_str[DESC_INDEX_TYPE], UP_DESC_POWER_STR))
		{
			printf("=======Check descfile:PowerData\n");
			if (firmware_search_newfile(FW_SRC_PATH_2))
			{
				printf("=======Find new fw in uDiskDesc\r\n");
				uDiskFile |= UDISK_FILE_FW;
			}
		}
		if (!stricmp(tDesc.c_str[DESC_INDEX_TYPE], UP_DESC_TOUCH_STR))
		{
			printf("=======Check descfile:TouchData\n");
			if (fwTouch_search_newfile(TOUCHFW_SRC_PATH_1))
			{
				tOTA.Percent = 0;
				uDiskFile |= UDISK_FILE_FWTOUCH;
			}
		}
		if (!stricmp(tDesc.c_str[DESC_INDEX_TYPE], UP_DESC_TP_STR))
		{
			printf("=======Check descfile:TPData\n");
			if (st_read_version(TP_SRC_PATH))
			{
				tOTA.tpPercent = 0;
				uDiskFile |= UDISK_FILE_TP;
			}
		}
		if (!stricmp(tDesc.c_str[DESC_INDEX_TYPE], UP_DESC_OPTION_STR))
		{
			printf("=======Check descfile:CookBookData\n");
			if (cookbook_search_dir())
			{
				uDiskFile |= UDISK_FILE_WASHOPTION;
			}
		}
		return uDiskFile;
		*/
	}
	return ret;
#endif
}

void* FileCopyFunc(void* arg)
{
	printf("=======start FileCopyFunc======\n");
	pthread_detach(pthread_self());
	if (tOTA.TypeIndex&UDISK_FILE_WASHOPTION)
		WashOption_start_copy();
	printf("=======end FileCopyFunc======\n");
	return 0;
}

void FileCopyTaskInit(void)
{
#if 1//ndef _WIN32
	pthread_t CopyTask;
	pthread_attr_t Copy_attr;

	pthread_attr_init(&Copy_attr);
	pthread_create(&CopyTask, &Copy_attr, FileCopyFunc, NULL);
#endif
}
