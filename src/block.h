/*
 *  ext3Viewer,ext3Viewer GUI / an ext3 filesystem low level viewer
 *
 *  Copyright (C) 2007 Laurent Sebag & Nathan Periana
 *
 *  This file is part of ext3Viewer.
 *
 *  Ext3Viewer is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Ext3Viewer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */



#ifndef BLOCK_H  
#define BLOCK_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <linux/ext3_fs.h>

#include "debug.h"
#include "filesystem.h"
#include "block.h"
#include "text.h"

int read_block ( int fd, struct ext3_super_block *sb, 
		  __u32 num, char *block );

int read_block_pointer ( int fd, int block_size, 
			  __u32 num, __u32 *block );

void print_blocks ( int fd, struct ext3_super_block *sb,
		    __u32 block_num, short rec_level, int mode );

void print_block_as_bpointer( int fd, struct ext3_super_block *sb, 
			      __u32 block_num );

void print_block_hexa ( int fd, struct ext3_super_block *sb, __u32 block_num ); 

void print_block_hexa_noline ( int fd, struct ext3_super_block *sb, __u32 block_num ); 

void print_block ( int fd, struct ext3_super_block *sb, __u32 block_num );

#endif 
