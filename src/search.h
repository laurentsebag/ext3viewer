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



#ifndef SEARCH_H
#define SEARCH_H

//#include <linux/ext3_fs.h>
//#include <sys/types.h>
#include <regex.h>
#include <string.h>

#include "path.h"
#include "debug.h"
#include "inode.h"

int search_file ( int fd, struct ext3_super_block *sb, __u32 i_num, 
		    char *pattern );

int search_file_rec ( int fd, struct ext3_super_block *sb,
			__u32 i_num, const regex_t *preg ,
			struct path_filo *path, char *path_name );

int search_file_rec_block ( int fd, struct ext3_super_block *sb, 
			      const regex_t *preg, __u32 block_num, 
			      short rec_level , struct path_filo *path,
			      char *path_name );


int search_inode ( int fd, struct ext3_super_block *sb, __u32 i_num,
		   __u32 i_tofind );

int search_inode_rec ( int fd, struct ext3_super_block *sb, __u32 i_num,
		     __u32 i_tofind, struct path_filo *path, char *path_name);

int search_inode_rec_block ( int fd, struct ext3_super_block *sb, 
			     __u32 i_tofind, __u32 block_num, 
			     short rec_level, struct path_filo *path,
			     char *path_name );

__u32 search_first_free_inode(int fd, const struct ext3_super_block *sb,
				char * inodebitmap);
__u32 search_first_free_block(int fd, const struct ext3_super_block *sb,
				char * datablockbitmap);


#endif
