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

#include "acl.h"

///*ACL and xattr related functions are currently "broken"... */
//int read_aclh(int fd,const struct ext3_super_block *sb, __u32 acl_position,
//    struct ext3_acl_header * aclh)
//{/*Reads an acl header*/
//  int ret;
//  ret= lseek(fd,acl_position,SEEK_SET);//All ACL must fit in the same block
//  if ( ret == -1 ) {
//    perror("ext3Viewer - lseek error: ACL_H");
//    return -1;
//  }
//  //	ret= read( fd, aclh, sizeof(struct ext3_acl_header) );
//  if ( ret == -1 ) {
//    perror("ext3Viewer - read error: ACL");
//    return -1;
//  }
//  return 0; 
//}
//
//
//int read_acl(int fd,const struct ext3_super_block *sb, __u32 acl_position,
//    struct ext3_acl_entry * acl)//Reading the first ACL for an inode
//{
//  int ret;
//  ret= lseek(fd,acl_position,SEEK_SET);//All ACL must fit in the same block
//  if ( ret == -1 ) {
//    perror("ext3Viewer - lseek error: ACL");
//    return -1;
//  }
//  //   ret= read( fd, acl, sizeof(struct ext3_acl_entry) );
//  if ( ret == -1 ) {
//    perror("ext3Viewer - read error: ACL");
//    return -1;
//  }
//  return 0; 
//}



int read_xattrh(int fd,const struct ext3_super_block *sb, __u32 header_pos,
    struct ext3_xattr_header * header )//Reading the first ACL for an inode
{
  int ret;
  ret= lseek(fd,header_pos,SEEK_SET);//All ACL must fit in the same block
  if ( ret == -1 ) {
    perror("ext3Viewer - lseek error: ACL");
    return -1;
  }
  ret= read( fd, header, sizeof(struct ext3_xattr_header) );
  if ( ret == -1 ) {
    perror("ext3Viewer - read error: ACL");
    return -1;
  }
  return 0; 
}


int read_xattre(int fd,const struct ext3_super_block *sb, __u32 entry_blocknum,
    __u16 offset, struct ext3_xattr_entry * entry )
{//Reads an entry of an inode extended attribute, given the superblock, the block number,and an empty ext3_xattr_entry structure
  int ret;
  entry_blocknum=entry_blocknum*EXT3_BLOCK_SIZE(sb);
  ret= lseek(fd,entry_blocknum+offset,SEEK_SET);
  if ( ret == -1 ) {
    perror("ext3Viewer - lseek error: xattr_entry");
    return -1;
  }
  ret= read( fd, entry, sizeof(struct ext3_xattr_entry) );
  if ( ret == -1 ) {
    perror("ext3Viewer - read error: xattr_entry");
    return -1;
  }
  return 0;
  //REMINDER:All ACLs must fit in the same block
}



void print_acl(int fd,const struct ext3_super_block *sb,__u32 i_type_acl)    
  //http://www.suse.de/~agruen/acl/linux-acls/online/
{//This function only prints extended attributes.
  /*ACLs are implemented under ext3 using extended attributes: see kernel source http://lxr.linux.no/source/fs/ext3/xattr.h*/
  //struct ext3_acl_entry  entry;
  //struct ext3_acl_header header;
  struct ext3_xattr_header xattr_header;
  struct ext3_xattr_entry xattr_entry;

  __u16 position=0,offset=0;//ACL position
  if(i_type_acl==0)
  {
    printf("No ACL entry associated.\n");
  }
  else
  {
    printf("This is the number of the file system block on which\n " 
        "the Extended Attributes associated with this inode are stored.\n");
    //	acl_get_entry(acl_t acl, int entry_id, acl_entry_t *entry_p);
    position=i_type_acl;

    read_xattrh(fd,sb,position,&xattr_header);
    offset=sizeof(struct ext3_xattr_header);
    read_xattre(fd,sb,position,offset,&xattr_entry);


    /*Extended attributes header display*/
    printf("Header of the extended attributes list\n");
    printf("The magic number should be %#x, is %#x\n",EXT3_XATTR_MAGIC,xattr_header.h_magic);
    printf("This set of attributes is referenced %u times\n",xattr_header.h_refcount);
    printf("%u blocks are used\n",xattr_header.h_blocks);
    printf("Hash value of all attributes:%#x\n",xattr_header.h_hash);

    /*Extended attributes entry display*/
    printf("Name length: %hhu\n",xattr_entry.e_name_len);     
    printf("Attribute name index: %hhu\n",xattr_entry.e_name_index);   			
    printf("Disk block offset of value: %u\n",le16_to_cpu(xattr_entry.e_value_offs));       
    printf("Disk block attribute is stored on: %u\n",le32_to_cpu(xattr_entry.e_value_block)); 
    printf("Size of attribute value: %u\n",le32_to_cpu(xattr_entry.e_value_size));   			
    printf("Hash value of name and value: %#x\n",le32_to_cpu(xattr_entry.e_hash));        
    printf("Attribute name: %#x\n",xattr_entry.e_name[0]);  
    offset=le16_to_cpu(xattr_entry.e_value_offs);
    //Here should go the actual reading of the access control lists.
  }
}
