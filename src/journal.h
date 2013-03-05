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



#ifndef JOURNAL_H
#define JOURNAL_H

//#include <linux/ext3_fs.h>
//#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
//#include <stdlib.h>

//#ifndef UTILS_H
//#define UTILS_H
//#include <utils.h>
//#endif
#ifndef be32_to_cpu
#include <linux/byteorder/little_endian.h>
#define be32_to_cpu  __be32_to_cpu
#endif

#include "block.h"
#include "inode.h"

/*Taken from kernel 2.6.17.13 sources: include/linux/jbd.h */
#define JFS_MAGIC_NUMBER 0xc03b3998U

/*
 * Standard header for all descriptor blocks:
 */
typedef struct journal_header_s
{
  __be32          h_magic;
  __be32          h_blocktype;
  __be32          h_sequence;
} journal_header_t;
/*
 * The block tag: used to describe a single buffer in the journal
 */
typedef struct journal_block_tag_s
{
  __be32          t_blocknr;      /* The on-disk block number */
  __be32          t_flags;        /* See below */
} journal_block_tag_t;

/*
 * The revoke descriptor: used on disk to describe a series of blocks to
 * be revoked from the log
 */
typedef struct journal_revoke_header_s
{
  journal_header_t r_header;
  __be32           r_count;       /* Count of bytes used in the block */
} journal_revoke_header_t;


typedef struct journal_superblock_s
{
  /* 0x0000 */
  journal_header_t s_header;

  /* 0x000C */
  /* Static information describing the journal */
  __be32  s_blocksize;            /* journal device blocksize */
  __be32  s_maxlen;               /* total blocks in journal file */
  __be32  s_first;                /* first block of log information */

  /* 0x0018 */
  /* Dynamic information describing the current state of the log */
  __be32  s_sequence;             /* first commit ID expected in log */
  __be32  s_start;                /* blocknr of start of log */

  /* 0x0020 */
  /* Error value, as set by journal_abort(). */
  __be32  s_errno;

  /* 0x0024 */
  /* Remaining fields are only valid in a version-2 superblock */
  __be32  s_feature_compat;       /* compatible feature set */
  __be32  s_feature_incompat;     /* incompatible feature set */
  __be32  s_feature_ro_compat;    /* readonly-compatible feature set */
  /* 0x0030 */
  __u8    s_uuid[16];             /* 128-bit uuid for journal */

  /* 0x0040 */
  __be32  s_nr_users;             /* Nr of filesystems sharing log */

  __be32  s_dynsuper;             /* Blocknr of dynamic superblock copy*/

  /* 0x0048 */
  __be32  s_max_transaction;      /* Limit of journal blocks per trans.*/
  __be32  s_max_trans_data;       /* Limit of data blocks per trans. */

  /* 0x0050 */
  __u32   s_padding[44];

  /* 0x0100 */
  __u8    s_users[16*48];         /* ids of all fs'es sharing the log */
  /* 0x0400 */
} journal_superblock_t;


/*
 * Descriptor block types:
 */

#define JFS_DESCRIPTOR_BLOCK    1
#define JFS_COMMIT_BLOCK        2
#define JFS_SUPERBLOCK_V1       3
#define JFS_SUPERBLOCK_V2       4
#define JFS_REVOKE_BLOCK        5

/* Definitions for the journal tag flags word: */
#define JFS_FLAG_ESCAPE         1       /* on-disk block is escaped */
#define JFS_FLAG_SAME_UUID      2       /* block has same uuid as previous */
#define JFS_FLAG_DELETED        4       /* block deleted by this transaction */
#define JFS_FLAG_LAST_TAG       8       /* last tag in this descriptor block */



int read_journal_superblock(int fd, int block_size, __u32 jsb_block_num,
    struct journal_superblock_s *jsb );

int read_journal_header( int fd, int block_size,
    __u32 journal_block_num, struct journal_header_s *header );

int read_journal_revoke_header(int fd, int block_size,
    __u32 journal_block, int offset, struct journal_revoke_header_s *header );


int dump_journal ( int fd, int block_size, struct ext3_inode *i );

int dump_journal_block_rec ( int fd, int block_size, __u32 block_num,
    int rec_level );

int dump_journal_block ( int fd, int block_size, __u32 block_num );


void print_journal_superblock(struct journal_superblock_s * j);

void print_journal_header( struct journal_header_s *header );

void print_journal_revoke_header( struct journal_revoke_header_s *header );


int is_pow_of ( int num, int pow );

#endif
