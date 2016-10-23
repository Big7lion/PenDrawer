#ifndef __SRAM_H
#define __SRAM_H															    
#include "sys.h" 
/*******************************************************本程序开源供3D打印学习使用************************************************
																												Dlion-3D打印主板
																												3D二进制创客---------技术论坛:www.3dbinmaker.com
																												文件说明：内存管理  版本：V02
																												Copyright(C)深圳洛众科技有限公司
																												All rights reserved
***********************************************************************************************************************************/
											  
void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 NumHalfwordToRead);


void fsmc_sram_test_write(u8 data,u32 addr);
u8 fsmc_sram_test_read(u32 addr);

#endif

