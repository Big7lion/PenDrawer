#include "gcodeplayer.h"
#include "Dlion.h"	 
#include "sdio_sdcard.h"
#include "usart.h"
#include "exfuns.h"
#include "malloc.h"		  
#include "string.h"
#include "language.h"
#include "temperature.h"
#include "stepper.h"


/*******************************************************本程序开源供3D打印学习使用************************************************
																												Dlion-3D打印主板
																												3D二进制创客---------技术论坛:www.3dbinmaker.com
																												文件说明：gcode处理附加功能 版本：V02
																												Copyright(C)深圳洛众科技有限公司
																												All rights reserved
***********************************************************************************************************************************/


#ifdef SDSUPPORT

void gcode_ls(u8 * path)
{
	u8 res;	  
    u8 *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
 	fileinfo.lfsize = _MAX_LFN * 2 + 1;
	fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
#endif		  

    res = f_opendir(&dir,(const TCHAR*)path);
    if (res == FR_OK) 
	{	

		while(1)
		{
	        res = f_readdir(&dir, &fileinfo);               
	        if (res != FR_OK || fileinfo.fname[0] == 0) break; 
	        //if (fileinfo.fname[0] == '.') continue;       
#if _USE_LFN
        	fn =(u8*)( *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname);
#else							   
        	fn =(u8*)( fileinfo.fname);
#endif	                                         
		//	printf("%s/", path);
	 	    res=f_typetell(fn);	
			if((res&0XF0)==0X60)
		  	{ printf("%s\n",  fn);
			}
		} 
    }	  
	myfree(SRAMIN,fileinfo.lfname);
  //  return res;	  
}

void card_ls(void)
{ 
  if(card.lsAction==LS_Count)
 // nrFiles=0;
  card.lsAction=LS_SerialPrint;
  gcode_ls("0:\\GCODE");
}

void card_initsd(void)
{ card.cardOK = false;
  SERIAL_ECHO_START;
  if(SD_Init())	
	{ printf(MSG_SD_INIT_FAIL);	
	}
	 else 
	{ 	card.cardOK = true;
		printf(MSG_SD_CARD_OK);
	}
}
void card_release(void)
{ card.sdprinting = false;
  card.cardOK = false;
}
void card_openFile(char* fname,bool read) 
{
  u8 res;
  u8 *pname;		

 pname=mymalloc(SRAMIN,_MAX_LFN*2+1);
  if(!card.cardOK)
    return;
 // file.close();
// printf("--");
  card.sdprinting = false;

  if(read)
  {	
  strcpy((char*)pname,"0:/GCODE/");
  strcat((char*)pname,(const char*)fname); 
  res=f_open(&card.fgcode,(const TCHAR*)pname,FA_READ);
    if (res==FR_OK)
    {
      printf(MSG_SD_FILE_OPENED);
      printf(fname);
      printf(MSG_SD_SIZE);
      printf("%ld",f_size(&card.fgcode));
	  printf("\n");
//      card.sdpos = 0;    
      printf(MSG_SD_FILE_SELECTED);
      //lcd_setstatus(fname);
    }
    else
    {
      printf(MSG_SD_OPEN_FILE_FAIL);
      printf(fname);
      printf(".");
    }
	printf("\n");
  }
  else 
  { res=f_open(&card.fgcode,(const TCHAR*)fname,FA_WRITE|FA_OPEN_ALWAYS);
    if (res!=FR_OK)
    {
      printf(MSG_SD_OPEN_FILE_FAIL);
      printf(fname);
      printf(".");
    }
    else
    {
      card.saving = true;
      printf(MSG_SD_WRITE_TO_FILE);
      printf(fname);
     // lcd_setstatus(fname);
    }
	 printf("\n");
  }
   myfree(SRAMIN,pname);		
  //myfree(SRAMIN,fgcode);
}

void card_removeFile(char* fname)
{  u8 res;
  res = f_unlink(fname);
  if (res==FR_OK) 
    {
      printf("File deleted:");
      printf(fname);
//      card.sdpos = 0;
    }
    else
    {
      printf("Deletion failed, File: ");
      printf(fname);
      printf(".\n");
    }
}

void card_startFileprint(void)
{
  if(card.cardOK)
  {
    card.sdprinting = true; 
  }

}

void card_pauseSDPrint(void)
{
  if(card.sdprinting)
  {
    card.sdprinting = false;
  }
}

void card_setIndex(long index)
{//card.sdpos = index;
 f_lseek(&card.fgcode,index);
}

void card_getStatus(void)
{  if(card.cardOK)
  {
    printf(MSG_SD_PRINTING_BYTE);
    printf("%d",f_tell(&card.fgcode));
    printf("/");
    printf("%d",f_size(&card.fgcode));
  }
  else{
    printf(MSG_SD_NOT_PRINTING);
  }
  printf("\n");
}

void card_closefile(void)
{ f_close(&card.fgcode);
  card.saving = false; 
}

void card_write_command(char *buf)
{ 
}

void card_checkautostart()
{
  if((!card.cardOK)&&(!SD_CD))
  {
    card_initsd();
    if(!card.cardOK) //fail
      return;
  }
  
}
void card_printingHasFinished(void)
{
    st_synchronize();
    quickStop();
    card_closefile();
	starttime=0;
	card.sdprinting = false;
    if(SD_FINISHED_STEPPERRELEASE)
    {
      //  finishAndDisableSteppers();
        enquecommand(PSTR(SD_FINISHED_RELEASECOMMAND));
    }
    autotempShutdown();
//	 printf("5.\r\n");
}

bool card_eof(void)
{ return f_eof(&card.fgcode);
}

int16_t card_get(void) 
{ //card.sdpos = f_tell(&card.fgcode);
  return  file_read();
}

int16_t file_read(void)
{ 	u8 buffer[128]; 
	u8 res;
	UINT br;
	res = f_read(&card.fgcode, buffer, 1, &br);
	if (res == FR_OK )
	{// printf("%s",buffer);
	return *buffer;//
	}else
	{ return -1;// printf("f_read() fail .. \r\n");  
	}
}
#endif //SDSUPPORT
