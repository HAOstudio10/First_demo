#ifndef _MK60_FLASH_H_
#define _MK60_FLASH_H_

 //k60N512包含512K的程序Flash
 //512K的程序Flash分为256个扇区，每个扇区2K大小
 //K60X256包含256K的程序Flash
 //512K的程序Flash分为128个扇区，每个扇区2K大小
 //    sector（2K）为擦除最小单位
 //    长字（32b）为写的最小单位

#include "common.h"

#define     FLASH_SECTOR_SIZE       (4*1024)                //扇区大小 为 4k 字节
#define     FLASH_SECTOR_NUM        (256)                   //扇区数

#define     FLASH_ALIGN_ADDR        4                       //地址对齐整数倍
typedef     uint32                 FLASH_WRITE_TYPE;       //flash_write 函数写入 的数据类型

void    flash_init();                                              //初始化Flash
#define     flash_read(sectorNo,offset,type)        (*(type *)((uint32)(((sectorNo)*FLASH_SECTOR_SIZE) + (offset))))          //读取扇区

void Flash_read_buf(uint16 sectNo, uint32 *bBuf, uint8 cnt);					//读取Flash

uint8   flash_erase_sector(uint16 sectorNo);                     //擦除指定flash扇区

uint8   flash_write(uint16 sectorNo, uint16 offset, FLASH_WRITE_TYPE data);                //写入flash操作
uint8   flash_write_buf(uint16 sectorNo, uint16 offset, uint16 cnt, uint32 buf[]);              //从缓存区写入flash操作


#endif //_FLASH_H_
