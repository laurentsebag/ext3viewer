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



#ifndef INODE_H
#define INODE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "linux/ext3_fs.h"
#include <time.h>

#include "groups.h"
#include "debug.h"
#include "acl.h"
#include "block.h"

void read_inode( int fd, struct ext3_super_block *sb,
    __u32 num, struct ext3_inode *i );

void print_inode (int fd, const struct ext3_super_block *sb,
    const struct ext3_inode *in);


void print_file ( int fd, struct ext3_super_block *sb, __u32 i_num, int mode );

void print_symlink ( int fd, struct ext3_super_block *sb, __u32 i_num);


void print_dir ( int fd, struct ext3_super_block *sb, __u32 i_num );

void print_dir_blocks ( int fd, struct ext3_super_block *sb,
    __u32 block_num, short rec_level );

void print_tree( int fd, struct ext3_super_block *sb, struct ext3_inode *i );

/*Various functions used to 'resolve' informations from field values*/
int  mask(const __u32 value, const __u32 macro);
void print_binary(const __u32 value, const int size);
void print_time(const __u32 timestamp);
void print_state(__u16 s_state);
void print_errorbehavior(__u16 s_errors);
void print_revlev(__u32 s_minor_rev_level);
void print_creatorOS(__u32 s_creator_os);
void print_featcompat(__u32 s_feature_compat);
void print_featincompat(__u32 s_feature_incompat);
void print_featROcompat(__u32 s_feature_ro_compat);
void print_UUID(const __u8 * s_journal_uuid);
void print_mountoptions(__u32 s_default_mount_opts);
void print_filemode(__u16 i_mode);
void print_fileflags(__u32 i_flags);
void print_blockpointers( __u32 i_block[] );

#endif
