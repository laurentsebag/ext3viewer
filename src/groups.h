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



#ifndef GROUPS_H  
#define GROUPS_H

#include "linux/ext3_fs.h"
#include <strings.h>
//#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "groups.h"
#include "debug.h"
#include "text.h"
#include "groups.h"

int read_inodebitmap( int fd, const struct ext3_super_block *sb,
		      const struct ext3_group_desc *gd,char *inodebitmap);
int read_datablockbitmap( int fd, const struct ext3_super_block *sb,
		       const struct ext3_group_desc *gd,const __u32 group_num, char *datablockbitmap);

int bitmap_test_free(char content );

void read_group_desc(int fd,const struct ext3_super_block *sb, 
		     __u16 group_num,struct ext3_group_desc *gd);


void print_groupdesc ( const struct ext3_group_desc *gd) ;

void print_inodebitmap(const struct ext3_super_block *sb, char *inodebitmap,
 __u16 gb_num);

void print_inodeblocknumber(const struct ext3_super_block *sb,__u32 inode_num_g
, __u16 gb_num);

void print_datablockbitmap(const struct ext3_super_block *sb, 
			   char *datablockbitmap, __u16 gb_num);

void print_datablock_blocknumber(const struct ext3_super_block *sb,
			     __u32 blck_num_g, __u16 gb_num);

void print_t_bin( char content );
#endif 
