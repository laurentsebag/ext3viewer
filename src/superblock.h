/*
 *  ext3Viewer, ext3Viewer GUI / an ext3 filesystem low level viewer
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



#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

//#include <linux/ext3_fs.h>
//#include <sys/types.h>
#include <unistd.h>

#include "filesystem.h"
#include "inode.h"
#include "journal.h"

int read_superblock ( int fd, struct ext3_super_block *sb );

int read_superblock_backup ( int fd, struct ext3_super_block *sb,
    __u32 block_num, int block_size );

int read_superblock_gui ( int fd, struct ext3_super_block *sb );

void print_sb_copy (const struct ext3_super_block *sb);

void print_superblock ( const struct ext3_super_block *sb );

#endif
