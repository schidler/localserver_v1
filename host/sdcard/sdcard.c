#include "comshead.h"
#include "config.h"
#include "systools.h"

#define SEC			1
#define MIN			60*SEC
#define HOUR		60*MIN
#define DAY			24*HOUR
#define MONTH		30*DAY
#define FILETIME	1*MONTH
#define REMOVE_MP3_NUM 10

#define SD_MIN		50

//是否小于SD剩余空间下限大小
void CheckSdcardInfo(char * sdpath)
{
	int Capacity =0;
	GetStorageInfo(sdpath,&Capacity,FREE);
	if(Capacity<SD_MIN)
		return -1;
	else
		return 0;
}
//删除长时间不用的文件
void DelSdcardMp3file(char * sdpath)
{
	char filepath[128]={0};
	int delmp3Num=0;
	struct stat Mp3info;
	DIR *dirptr = NULL;  
	struct dirent *entry;
	time_t timep;
	
	time(&timep);
#ifdef LOG_DELMP3
	writeLog("/home/mp3filename.txt",sdpath);
#endif
	if((dirptr = opendir(sdpath)) == NULL)	
	{  
		printf("open dir !\n"); 
		return ;	
	}
	while (entry = readdir(dirptr))  
	{  
		//去除当前目录和上一级目录
		if( !strcmp(entry->d_name,".")||!strcmp(entry->d_name,"..") ){
			usleep(1000);
			continue;
		}
#ifdef LOG_DELMP3
		writeLog("/home/mp3filename.txt",entry->d_name);
#endif
		printf("filename: %s\n",entry->d_name);
		snprintf(filepath,128,"%s%s",sdpath,entry->d_name);
		if( stat(filepath, &Mp3info) != 0 ){
			break;
		}
		if((timep-Mp3info.st_ctime)<FILETIME){
			usleep(1000);
			continue;
		}else{			//删除长时间不用的文件
			if( remove(filepath) == 0){
				printf("Removed %s.\n", filepath);
				delmp3Num++;
				if(delmp3Num>REMOVE_MP3_NUM){
					break;
				}
			}
			else
				perror("remove");
			usleep(1000);
		}
		usleep(1000);
	}
	printf("del file end ... \n");
}
#ifdef TEST_SDK
void WavtoAmrfile(char * sdpath)
{
	char filepath[128]={0};
	char wavfilepath[128]={0};
	char amrfilepath[128]={0};
	DIR *dirptr = NULL;  
	struct dirent *entry;  

	if((dirptr = opendir(sdpath)) == NULL)	
	{  
		printf("open dir !\n"); 
		return ;	
	}
	sleep(1);
	while (entry = readdir(dirptr))  
	{  
		//去除当前目录和上一级目录
		if( !strcmp(entry->d_name,".")||!strcmp(entry->d_name,"..") ){
			usleep(1000);
			continue;
		}
		printf("filename: %s\n",entry->d_name);
		if(!strstr(entry->d_name,".wav")){
			usleep(1000);
		}
		sscanf(entry->d_name,"%[^.].%*s",filepath);
		snprintf(wavfilepath,128,"%s%s",sdpath,entry->d_name);
		snprintf(amrfilepath,128,"%s%s%s","/mnt/tang/",filepath,".amr");
		//WavToAmr8kFile(wavfilepath,amrfilepath);
#ifdef	SPEEK_VOICES
		playspeekVoices(amrfilepath);
#endif
		memset(wavfilepath,0,128);
		memset(amrfilepath,0,128);
		usleep(10000);
	}
	printf("wav to amr file end ... \n");
}
#endif
#if 0
int main(int argc,char **argv){
	int Capacity =0;
	GetStorageInfo(argv[1],&Capacity,ALL);
	printf("%s  size %d Mb\n",argv[1],Capacity);
	return 0;
}
#endif

