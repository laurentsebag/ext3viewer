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



#ifndef OUTPUT_GUI_H
#define OUTPUT_GUI_H

#include <sys/stat.h>
#include "../linux/ext3_fs.h"

#include "../inode.h"
#include "../journal.h"
#include "../search.h"

#include "open_gui.h"
#include "objects_gui.h"

#define A_LINE_BITMAP_GUI "+----------------------------------------------------------------+\n"
#define A_LINE_GUI "+------------------------------------------------------------------------+\n"

extern struct Window wMain;

void init_tags ( GtkTextBuffer *buffer ); //init tags for text formatting


void g_strconcat_permissions( gchar **text, __u8 perm);
void print_filemode_gui ( gchar *texte, __u16 mode );
void print_group_desc ();
void print_inode_gui ( int fd, const struct ext3_super_block *sb,
           const struct ext3_inode *in, __u32 inum );

void print_superblock_backups ();
void print_superblock_gui ();

void print_ffi ();
void print_imap ();
void print_bmap ();
void print_ffb ();

void print_block_as_bpointer_gui( );
void print_block_hexa_gui (  );

void print_symlink_gui ();

int ask_block_number( char *string );

void print_journal ();
void print_journal_superblock_gui(struct journal_superblock_s * j);
void print_journal_header_gui( struct journal_header_s *header );

void  print_UUID_gui(const __u8 * tab_uuid);


int dump_journal_gui ( int fd, int block_size, struct ext3_inode *i );
int dump_journal_block_rec_gui ( int fd, int block_size, __u32 block_num,
    int rec_level );
int dump_journal_block_gui ( int fd, int block_size, __u32 block_num );
void print_journal_revoke_header_gui( struct journal_revoke_header_s *header );

#endif
