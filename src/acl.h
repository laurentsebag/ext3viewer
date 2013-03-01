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



#ifndef ACL_H
#define ACL_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "linux/ext3_fs.h"

#ifndef UTILS_H
#define UTILS_H
//#include <utils.h>
#endif

#ifndef le16_to_cpu
#include <linux/byteorder/little_endian.h>
#define le16_to_cpu __le16_to_cpu
#endif

#ifndef le32_to_cpu
#define le32_to_cpu __le32_to_cpu
#endif

#include "filesystem.h"

/*
 * Taken from kernel 2.6.17.13 sources: fs/ext3/xattr.h 
 */
#define EXT3_XATTR_MAGIC                0xEA020000

struct ext3_xattr_header {
    __le32  h_magic;        /* magic number for identification */
    __le32  h_refcount;     /* reference count */
    __le32  h_blocks;       /* number of disk blocks used */
    __le32  h_hash;         /* hash value of all attributes */
    __u32   h_reserved[4];  /* zero right now */
};
   
//struct ext3_xattr_ibody_header {
//    __le32  h_magic;        /* magic number for identification */
//};

struct ext3_xattr_entry {
    __u8    e_name_len;     /* length of name */
    __u8    e_name_index;   /* attribute name index */
    __le16  e_value_offs;   /* offset in disk block of value */
    __le32  e_value_block;  /* disk block attribute is stored on (n/i) */
    __le32  e_value_size;   /* size of attribute value */
    __le32  e_hash;         /* hash value of name and value */
    char    e_name[0];      /* attribute name */
};

//#define ACL_USER_OBJ		(0x01)
//#define ACL_USER		(0x02)
//#define ACL_GROUP_OBJ		(0x04)
//#define ACL_GROUP		(0x08)
//#define ACL_MASK		(0x10)
//#define ACL_OTHER		(0x20)

//int read_aclh(int fd,const struct ext3_super_block *sb, __u32 acl_position,
//	      struct ext3_acl_header * aclh);
//int read_acl(int fd,const struct ext3_super_block *sb, __u32 acl_position,
//	     struct ext3_acl_entry * acl);
int read_xattrh(int fd,const struct ext3_super_block *sb,
      __u32 header_blocknum, struct ext3_xattr_header * header );
int read_xattre(int fd,const struct ext3_super_block *sb,
      __u32 entry_blocknum, __u16 offset, struct ext3_xattr_entry * entry );

void print_acl(int fd,const struct ext3_super_block *sb,__u32 i_type_acl);
// void print_aclentrytype(__u16 acle_type)//???

#endif 
