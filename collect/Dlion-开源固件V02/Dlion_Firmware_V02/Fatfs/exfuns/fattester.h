#ifndef __FATTESTER_H
#define __FATTESTER_H 			   
#include <stm32f10x.h>	   
#include "ff.h"
/*******************************************************本程序开源供3D打印学习使用************************************************
																												Dlion-3D打印主板
																												3D二进制创客---------技术论坛:www.3dbinmaker.com
																												文件说明：文件系统   版本：V02
																												Copyright(C)深圳洛众科技有限公司
																												All rights reserved
***********************************************************************************************************************************/
 
u8 mf_mount(u8 drv);
u8 mf_open(u8*path,u8 mode);
u8 mf_close(void);
u8 mf_read(u16 len);
u8 mf_write(u8*dat,u16 len);
u8 mf_opendir(u8* path);
u8 mf_readdir(void);
u8 mf_scan_files(u8 * path);
u32 mf_showfree(u8 *drv);
u8 mf_lseek(u32 offset);
u32 mf_tell(void);
u32 mf_size(void);
u8 mf_mkdir(u8*pname);
u8 mf_fmkfs(u8 drv,u8 mode,u16 au);
u8 mf_unlink(u8 *pname);
u8 mf_rename(u8 *oldname,u8* newname);
void mf_gets(u16 size);
u8 mf_putc(u8 c);
u8 mf_puts(u8*c);
u8 mf_cpymsg(u8*pname,u8 pct,u8 mode);
u32 mf_fdsize(u8*pname);
u8 mf_fcopy(u8 *psrc,u8 *pdst);
u8 mf_fdcopy(u8 *psrc,u8 *pdst);
#endif





























