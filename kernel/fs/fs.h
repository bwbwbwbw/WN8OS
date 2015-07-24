#ifndef __WN8OS_FS_H__
#define __WN8OS_FS_H__

#include "fs_definition.h"

s8 * read_sector(u64 sector_id);

void write_sector(s8 *buffer);

void read_sector_by_address(u64 address, u8 *dist, u64 size);

s32 initialize_fs();

WN8FileSystem * fsInstance       = NULL;
u64 *           fsInodeBitmap    = NULL;
u64 *           fsBlockBitmap    = NULL;
u64             fsInodeBitmapLen = 0;
u64             fsBlockBitmapLen = 0;



#endif

