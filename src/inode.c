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

#include "inode.h"


void read_inode( int fd, struct ext3_super_block *sb,
    __u32 num, struct ext3_inode *i ) {

  __u32 i_num_g; /* inode number in the group */
  __u16 gb_num; /* group block  number where our inode is */
  struct ext3_group_desc gd;
  off_t lret; /* return value for lseek and read calls */
  size_t sret;

  i_num_g = (num-1) % EXT3_INODES_PER_GROUP(sb); /* num-1 because inode 0 */
  gb_num = (num-1) / EXT3_INODES_PER_GROUP(sb);  /* exists and ls starts at 1*/
  /* when you type in a terminal ls -i it shows the first inode as 1 instead
     of 0 */

  read_group_desc( fd, sb, gb_num, &gd );
  DEBUG_PRINT("lseek to reach inode table = %llu oct",
      ((off_t) gd.bg_inode_table)*EXT3_BLOCK_SIZE(sb) +
      ((off_t) i_num_g)*EXT3_INODE_SIZE(sb));

  lret = lseek( fd, ((off_t) gd.bg_inode_table)*EXT3_BLOCK_SIZE(sb) +
      ((off_t) i_num_g)*EXT3_INODE_SIZE(sb), SEEK_SET );
  if( lret <= 0 ) {
    perror("ext3Viewer error - lseek error inode");
    exit(lret);
  }

  // Let's go at the beginning of the inode table thanks to gb_inode_table,
  // then we seek of i_num_g inodes
  sret = read( fd, i, sizeof(struct ext3_inode) );

  if( sret <= 0 ) {
    perror("ext3Viewer error - inode read error");
    exit(1);
  }
  //  printf("@@@@@@2%u\n", num);

  DEBUG_PRINT_V("struct ext3_inode %u {", num);
  //  printf("@@@@@@2%u\n", num);

  DEBUG_PRINT_V("%hu   i_mode", i->i_mode);
  DEBUG_PRINT_V("%hu   i_uid", i->i_uid);
  DEBUG_PRINT_V("%u   i_size", i->i_size);
  DEBUG_PRINT_V("%u   i_atime", i->i_atime);
  DEBUG_PRINT_V("%u   i_ctime", i->i_ctime);
  DEBUG_PRINT_V("%u   i_mtime", i->i_mtime);
  DEBUG_PRINT_V("%u   i_dtime", i->i_dtime);
  DEBUG_PRINT_V("%hu   i_gid", i->i_gid);
  DEBUG_PRINT_V("%hu   i_links_count", i->i_links_count);
  DEBUG_PRINT_V("%u   i_blocks", i->i_blocks);
  DEBUG_PRINT_V("%u   i_flags", i->i_flags);
  DEBUG_PRINT_V("%u   i_generation", i->i_generation);
  DEBUG_PRINT_V("%u   i_file_acl", i->i_file_acl);
  DEBUG_PRINT_V("%u   i_dir_acl", i->i_dir_acl);
  DEBUG_PRINT_V("%u   i_faddr", i->i_faddr);
#ifdef DEBUG_VERBOSE
  short index;
  DEBUG_PRINT_V("i_block[] =%s","");
  for (index = 0; index < EXT3_N_BLOCKS; index++)
  {
    printf("%3u", i->i_block[index]);
  }
  printf("\n");

#endif
  DEBUG_PRINT_V("}%s","");
}


void print_inode(int fd, const struct ext3_super_block *sb, const struct ext3_inode *in)
  /*Prints a given inode from the inode table*/
{
  __u64 real_size;
  printf("File mode = %ho\n", in->i_mode);
  print_filemode(in->i_mode);
  printf("UID (low 16 bits) = %hu  \n", in->i_uid);
  printf("Last access time on the file (timestamp) = %u\n", in->i_atime);
  print_time(in->i_atime);
  printf("Creation timestamp =  %u  \n", in->i_ctime  );
  print_time(in->i_ctime);
  printf("Last inode modification time (timestamp) = %u\n", in->i_mtime);
  print_time(in->i_mtime);
  printf("File Deletion time (timestamp) = %u \n", in->i_dtime  );
  print_time(in->i_dtime);
  printf("GID (low 16 bits) = %hu\n", in->i_gid  );
  printf("Hard links count = %hu\n", in->i_links_count  );
  printf("File data blocks count = %u\n", in->i_blocks  );
  printf("File flags = %u\n", in->i_flags);
  print_fileflags(in->i_flags);
  switch(sb->s_creator_os)
  {
    case EXT3_OS_HURD:
      printf("Translator:%u.\n", in->osd1.hurd1.h_i_translator);
      break;
    default: /*Reserved zone, nothing to print*/
      ;
  }

  if ( in->i_blocks != 0 ) {
    print_blockpointers( (__u32 *) in->i_block );
  }

  printf("File version (used for NFS) = %u  \n", in->i_generation);
  if(mask(in->i_mode>>13, EXT3_FT_DIR))
  {/*If the inode refers to a directory, the size can be printed normally*/

    printf("File Access Control List = %#x\n", in->i_file_acl);
    print_acl(fd, sb, in->i_file_acl);
    printf("Directory Access Control List = %#x\n", in->i_dir_acl);
    print_acl(fd, sb, in->i_dir_acl);
    printf("File size in bytes (octets) =  %u\n", in->i_size);
  }
  else
  {/*If the inode is not a directory, the size must be printed using a special 'trick'*/
    printf("File Access Control List = %#x\n", in->i_file_acl);/* File ACL   __u32  */
    print_acl(fd, sb, in->i_file_acl);
    __u64 diracl =((__u64)(in->i_size_high)<<32);
    //i_size_high is a macro to i_dir_acl
    real_size =(__u64)(in->i_size)+diracl; 
    // We mask the highest order bit of i_size since it's not used, and we
    // "concatenate" the rest of the size contained in i_dir_acl to the real
    // size of the file.
    printf("File size in bytes (octets) =  %llu\n", real_size);/* Size in bytes*/
  }
  printf("Fragment address: %x\n", in->i_faddr);  /* Fragment address __u32 */


  switch(sb->s_creator_os)
  {
    case EXT3_OS_LINUX:

      printf("Fragment number:%hu \n", in->osd2.linux2.l_i_frag);
      printf("Fragment size:%hu \n", in->osd2.linux2.l_i_fsize);
      printf("UID (high 16 bits):%hu \n", in->osd2.linux2.l_i_uid_high);
      printf("GID (high 16 bits):%hu \n", in->osd2.linux2.l_i_gid_high);
      break;
    case EXT3_OS_HURD:

      printf("Fragment number:%hu \n", in->osd2.hurd2.h_i_frag);
      printf("Fragment size:%hu \n", in->osd2.hurd2.h_i_fsize);
      printf("MODE (high 16 bits):%hu \n", in->osd2.hurd2.h_i_mode_high);
      printf("UID (high 16 bits):%hu \n", in->osd2.hurd2.h_i_uid_high);
      printf("GID (high 16 bits):%hu \n", in->osd2.hurd2.h_i_gid_high);
      printf("Author:%u \n", in->osd2.hurd2.h_i_author);
      break;
    case EXT3_OS_MASIX:
      printf("Fragment number:%hu \n", in->osd2.masix2.m_i_frag);
      printf("Fragment size:%hu \n", in->osd2.masix2.m_i_fsize);
      break;

    default:
      ;
  }
}

void print_file ( int fd, struct ext3_super_block *sb, __u32 i_num, int mode )
{

  struct ext3_inode i;
  unsigned int c = 0; /* counter */


  read_inode( fd, sb, i_num, &i); /* read the file inode */

  /* now we'll read the file's blocks */


  /*
   * First part:
   * Let's read the 12 direct blocks from block n0 to (EXT3_NDIR_BLOCKS-1)
   */

  while( i.i_block[c] > 0 && c < EXT3_NDIR_BLOCKS)
  { /* we read the 0-11 first file's blocks which are direct blocks */

    if ( mode == HEXA_DATA ) {
      print_block_hexa_noline ( fd, sb, i.i_block[c] );
    }
    else if ( mode == PLAIN_DATA )
      print_blocks ( fd, sb, i.i_block[c], 0, mode );

    c++;
  }

  /*
   * Second part:
   * Let's read the indirect blocks
   *
   * layout: i_block[12] contains a pointer to a block table
   * We can read that block 4 octets by 4 octest ( = sizeof(u_32) to get
   * the address of the blocks to read
   */
  print_blocks ( fd, sb, i.i_block[EXT3_IND_BLOCK], 1, mode );

  /* And so on ...
     double indirect block
     */
  print_blocks ( fd, sb, i.i_block[EXT3_DIND_BLOCK], 2, mode );

  /* triple indirect block */
  print_blocks ( fd, sb, i.i_block[EXT3_TIND_BLOCK], 3, mode );
}

void print_symlink ( int fd, struct ext3_super_block *sb, __u32 i_num) {

  struct ext3_inode i;
  unsigned int c = 0; /* counter */
  char l = 'a';
  char *dest;
  read_inode( fd, sb, i_num, &i); /* read the file inode */

  /* now we'll read the symlink : if the size of the string is less than 60,
     we read i.i_block[] as a char[]. Else we can read the block as for a
     regular file */
  printf("inode %u  --> \"", i_num);
  if ( i.i_size < 60 ) {
    dest = (char *)i.i_block;
    while ( l != '\0' && c <= 60 ) {
      l = putchar( dest[c] );
      c++;
    }
    printf("\"\n");
  }
  else {
    print_file( fd, sb, i_num, PLAIN_DATA );
    printf("\"");
  }

}

void print_dir ( int fd, struct ext3_super_block *sb, __u32 i_num )
{

  struct ext3_inode i;
  unsigned int c = 0; /* counter */

  read_inode( fd, sb, i_num, &i); /* read the file inode */

  /* now we'll read the file's blocks */

  /*
   * First part :
   * Let's read the 12 direct blocks from block n0 to (EXT3_NDIR_BLOCKS-1)
   */
  printf("+-----------------------------------"
      "-----------------------------------+\n");
  printf("|          file name           | inode num | type | "
      "rec len | name len |\n");
  printf("+-----------------------------------"
      "-----------------------------------+\n");

  while( i.i_block[c]  && c < EXT3_NDIR_BLOCKS)
  { /* we read the 0-11 first file's blocks which are direct blocks
       DEBUG_PRINT("reading block %u", i.i_block[c]); */

    print_dir_blocks ( fd, sb, i.i_block[c], 0 );

    c++;
  }

  /*
   * Second part :
   * Let's read the indirect blocks
   *
   * layout: i_block[12] contains a pointer to a block table. We can read this
   * block 4 octets by 4 octets ( = sizeof(u_32) ) to get the address of the
   * block to read
   */
  print_dir_blocks ( fd, sb, i.i_block[EXT3_IND_BLOCK], 1 );

  /* And so on ...
     double indirect block
     */
  print_dir_blocks ( fd, sb, i.i_block[EXT3_DIND_BLOCK], 2 );

  /* triple indirect block */
  print_dir_blocks ( fd, sb, i.i_block[EXT3_TIND_BLOCK], 3 );

  putchar('\n');
}


void print_dir_blocks ( int fd, struct ext3_super_block *sb,
    __u32 block_num, short rec_level ) {

  /*
     This function reads and print recursively the entries of a directory

     a rec_level of 1 means to consider block_num as an index of blocks
     2                   block_num as an index of index of blocks
     and so on ...
     */

  unsigned int c = 0;
  struct ext3_dir_entry_2 dir_entry;
  struct ext3_inode i;
  __u32 iblock_num;
  __u64 next_dir_entry = 0;/* offset for the next dir_entry */
  off_t lret; size_t sret;

  if ( rec_level == 0 ) {

    DEBUG_PRINT("Reading block %u", block_num);

    next_dir_entry = 0;

    lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);
    if ( lret <= 0 ) {
      perror("ext3Viewer error - lseek 2 print_dir_blocks");
      exit (-1);
    }

    sret = read( fd, &dir_entry, sizeof( struct ext3_dir_entry_2 ) );
    if ( sret <= 0 ) {
      perror("ext3Viewer error - read print_dir_blocks");
      exit (-1);
    }

    while( next_dir_entry < EXT3_BLOCK_SIZE(sb) && dir_entry.inode != 0 )
    {
      c = 0;

      while ( c != dir_entry.name_len && c <= 25 ) {
        /* not a printf cause that provoke display bugs ... */
        putchar(dir_entry.name[c]);
        c++;
      }
      if ( c != dir_entry.name_len ) {
        while ( c < 30 ) {
          if ( c < 27 )
            putchar(' ');
          else
            putchar('.');
          c++;
        }
      }
      else {
        while ( c < 30 ) {
          putchar(' ');
          c++;
        }
      }
      printf(" | %9u", dir_entry.inode);


      printf(" | ");
      /*
         storing file type in the dir_entry.imode field is not always
        implemented :( .So we have to read each inode to know their file type

        switch ( dir_entry.file_type ) {  we show the file type in
        the ls style
        case EXT3_FT_DIR:
        putchar('d');
        break;
        case EXT3_FT_FIFO:
        putchar('x');     i don' t how is it in ls
        break;
        case EXT3_FT_SOCK:
        putchar('x');     i don' t how is it in ls
        break;
        case EXT3_FT_SYMLINK:
        putchar('l');
        break;
        case EXT3_FT_REG_FILE:
        putchar('-');
        break;
        case EXT3_FT_CHRDEV:
        putchar('c');
        break;
        case EXT3_FT_BLKDEV:
        putchar('b');
        break;

        default:
        putchar('?');  unknown type
        }
      */
      read_inode( fd, sb, dir_entry.inode, &i);

      if ( S_ISDIR(i.i_mode) ) {
        putchar('d');
      } else if ( S_ISCHR(i.i_mode) ) {
        putchar('c');
      } else if ( S_ISBLK(i.i_mode) ) {
        putchar('b');
      } else if ( S_ISFIFO(i.i_mode) ) {
        putchar('p'); /* pipe file */
      } else if ( S_ISLNK(i.i_mode) ) {
        putchar('l'); /* symlink */
      } else if ( S_ISSOCK(i.i_mode) ) {
        putchar('s'); /* socket file */
      } else if ( S_ISREG(i.i_mode) ) {
        putchar('-');
      } else {
        putchar('?');
      }

      printf(" %2o", dir_entry.file_type);
      printf(" | %7hd", dir_entry.rec_len);
      printf(" | %8hd\n", dir_entry.name_len);

      next_dir_entry += dir_entry.rec_len; /* rec_len points to the next
                                              dir_entry */

      lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb)+
          next_dir_entry, SEEK_SET);
      if ( lret <= 0 ) {
        perror("ext3Viewer error - lseek 3 print_dir_blocks");
        exit (-1);
      }

      sret = read( fd, &dir_entry, sizeof( struct ext3_dir_entry_2 ) );
      if (sret == 0) {
        perror("ext3Viewer error - reading error on dir_entry "
            "print_dir_blocks");
        exit(1);
      }
    }
  }
  else {

    if ( block_num > 0 ) {
      DEBUG_PRINT("Reading block index %u", block_num);

      lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);
      if ( lret <= 0 ) {
        perror("ext3Viewer error - lseek 4 print_dir_blocks");
        exit (-1);
      }

      sret = read ( fd, &iblock_num, 4 ); /*we read the block number of the
                                            indexed block */
      if ( sret <= 0 ) {
        perror("ext3Viewer error - read 2 print_dir_blocks");
        exit (-1);
      }
      while ( iblock_num != 0 && c < (EXT3_BLOCK_SIZE(sb)/4) )
      { // we go through the table of blocks

        // For each block, we display it or we go through it if it is a table
        print_dir_blocks ( fd, sb, iblock_num, rec_level-1 );

        c++;
        lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb)+(c*4), SEEK_SET);
        if ( lret <= 0 ) {
          perror("ext3Viewer error - lseek 5 print_dir_blocks");
          exit (-1);
        }
        // Let's move 4 by 4 octets because it's the size of u_32 on which the
        // number of a block is stored
        sret = read ( fd, &iblock_num, 4 );
        if ( sret <= 0 ) {
          perror("ext3Viewer error - read 3 print_dir_blocks");
          exit (-1);
        }

      }

    }

  }

}

void print_tree( int fd, struct ext3_super_block *sb, struct ext3_inode *i ) {

  int c;
  int ret;

  __u32 *block = malloc ( EXT3_BLOCK_SIZE(sb) );//[EXT3_BLOCK_SIZE(sb)];
  __u32 *block2 = malloc ( EXT3_BLOCK_SIZE(sb) );//[EXT3_BLOCK_SIZE(sb)];
  __u32 *block3 = malloc ( EXT3_BLOCK_SIZE(sb) );//[EXT3_BLOCK_SIZE(sb)];


  printf("Form of the inode block allocation. use -bpoint");
  printf("to explore in more details.\n\n");

  for ( c = 0 ; c < EXT3_NDIR_BLOCKS ; c++ ) {
    printf(" - i_block[%2u]=%u\n", c, i->i_block[c]);
  }

  printf(" - i_block[12]=%u\n", i->i_block[12]);
  if ( i->i_block[12] != 0 ) {
    ret = read_block_pointer ( fd, EXT3_BLOCK_SIZE(sb), i->i_block[12], block );
    if ( ret == -1 )
      exit(1);

    printf("  |\n");
    printf("   \\--- i_block[12][0]=%u\n", block[0]);
    printf("    |-- i_block[12][1]=%u\n", block[1]);
    printf("    |-- i_block[12][2]=%u\n", block[2]);
    printf("    |--    ... ... ...\n");

  }

  printf(" - i_block[13]=%u\n", i->i_block[13]);
  if ( i->i_block[13] != 0 ) {
    ret = read_block_pointer ( fd, EXT3_BLOCK_SIZE(sb), i->i_block[13], block );
    if ( ret == -1 )
      exit(1);

    printf("  |\n");
    printf("   \\--- i_block[13][0]=%u\n", block[0]);
    if ( block[0] != 0 ) {
      ret = read_block_pointer ( fd, EXT3_BLOCK_SIZE(sb), block[0], block2 );
      if ( ret == -1 )
        exit(1);

      printf("    |  |\n");
      printf("    |   \\--- i_block[13][0][0]=%u\n", block2[0]);
      printf("    |    |-- i_block[13][0][1]=%u\n", block2[1]);
      printf("    |    |-- i_block[13][0][2]=%u\n", block2[2]);
      printf("    |    |--    ... ... ...\n");

    }
    printf("    |-- i_block[13][1]=%u\n", block[1]);
    if ( block[1] != 0 ) {
      ret = read_block_pointer ( fd, EXT3_BLOCK_SIZE(sb), block[1], block2 );
      if ( ret == -1 )
        exit(1);

      printf("    |  |\n");
      printf("    |   \\--- i_block[13][1][0]=%u\n", block2[0]);
      printf("    |    |-- i_block[13][1][1]=%u\n", block2[1]);
      printf("    |    |-- i_block[13][1][2]=%u\n", block2[2]);
      printf("    |    |--    ... ... ...\n");

    }
    printf("    |-- i_block[13][2]=%u\n", block[2]);
    if ( block[2] != 0 ) {
      ret = read_block_pointer ( fd, EXT3_BLOCK_SIZE(sb), block[2], block2 );
      if ( ret == -1 )
        exit(1);
      printf("    |  |\n");
      printf("    |   \\--- i_block[13][2][0]=%u\n", block2[0]);
      printf("    |    |-- i_block[13][2][1]=%u\n", block2[1]);
      printf("    |    |-- i_block[13][2][2]=%u\n", block2[2]);
      printf("    |    |--    ... ... ...\n");

    }
    printf("    |--    ... ... ...\n");

  }

  printf(" - i_block[14]=%u\n", i->i_block[14]);

  if ( i->i_block[14] != 0 ) {
    ret = read_block_pointer ( fd, EXT3_BLOCK_SIZE(sb), i->i_block[14], block );
    if ( ret == -1 )
      exit(1);

    printf("  |\n");
    printf("   \\--- i_block[14][0]=%u\n", block[0]);

    if ( block[0] != 0 ) {
      ret = read_block_pointer ( fd, EXT3_BLOCK_SIZE(sb), block[0], block2 );
      if ( ret == -1 )
        exit(1);

      block3[0]= 1;
      printf("    |   |\n");
      printf("    |    \\--- i_block[14][0][0]=%u\n", block2[0]);
      if ( block2[0] != 0 ) {
        ret = read_block_pointer ( fd, EXT3_BLOCK_SIZE(sb), block2[0], block3 );
        if ( ret == -1 )
          exit(1);

        printf("    |        |\n");
        printf("    |         \\--- i_block[14][0][0][0]=%u\n", block3[0]);
        printf("    |           |--     ...  ...  ...\n");
      }
      /*      printf("  |     |-- i_block[14][0][1]=%u\n", block2[1]);
              printf("  |     |-- i_block[14][0][2]=%u\n", block2[2]);
              printf("  |     |--    ... ... ...\n");
              */
    }

  }
  free( block );
  free( block2 );
  free( block3 );
}



/*
 * This function serves as a mask for value
 */
int mask(const __u32 value, const __u32 macro)
{
  // If value == 110010 and Macro == 000010, then value has the functionnality
  // described in macro.
  if((value&macro)== macro)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/*
 * Print a number in binary form, 001010 style
 * The size of the data is in bits
 */
void print_binary(const __u32 value, int size)
{

  __u32 val = value;
  __u8 invert[32];//Obvious waste of memory
  int ind = 0, i = 0;
  while(val!= 0)
  {
    invert[ind]= val%2;
    val = val/2;
    ind++;
  }
  for(i = size-1;i>= 0;i--)//We invert the inverted bits array...
  {
    if(invert[i]== 1)
    {
      printf("%u", invert[i]);
    }
    else
    {
      printf("0");
    }
  }

}
void print_time(const __u32 timestamp)
{
  time_t tm22 = timestamp;
  if(tm22 == 0)
  {
    printf("Never\n");
  }
  else
  {
    printf("%s", ctime(&tm22));
  }
}

void print_permissions(__u8 perm)
{
  if(mask(perm, 04))
  {
    printf("r");
  }
  else
  {
    printf("-");
  }
  if(mask(perm, 02))
  {
    printf("w");
  }
  else
  {
    printf("-");
  }
  if(mask(perm, 01))
  {
    printf("x");
  }
  else
  {
    printf("-");
  }
  printf(" ");
}

/*End of general functions*/


/*Superblock-related functions*/
void print_state(__u16 s_state)
{
  switch(s_state)
  {
    case 0://No constant seems to exist, see O'REILLY linux-extfs for info
      printf("The filesystem was not unmounted cleanly.\n");
      break;
    case EXT3_VALID_FS:
      printf("The filesystem is clean.\n");
      break;
    case EXT3_ERROR_FS:
      printf("The filesystem contains errors.\n");
      break;
    case EXT3_ORPHAN_FS:
      printf("Orphan inodes are being recovered.\n");
      break;
    default:
      printf("The superblock is corrupt.\n");
  }

}

void print_errorbehavior(__u16 s_errors)
{
  switch(s_errors)
  {
    case EXT3_ERRORS_CONTINUE:
      printf("If an error is detected, the filesystem will continue to be used.\n");
      break;
    case EXT3_ERRORS_RO:
      printf("If an error is detected, the filesystem must be remounted in read-only mode.\n");
      break;
    case EXT3_ERRORS_PANIC:
      printf("If an error is detected, a KERNEL PANIC must be triggered.\n");
      break;
    default:
      printf("Unknown behaviour.\n");

  }
}

void print_revlev(__u32 s_minor_rev_level)
{

  switch(s_minor_rev_level)
  {
    case EXT3_GOOD_OLD_REV:
      printf("(Version using the original format.)\n");
      break;
    case EXT3_DYNAMIC_REV:
      printf("(Second version using dynamically sized inodes.)\n");
      break;
    default:
      printf("(Unknown filesystem version.)");
  }
}

void print_creatorOS(__u32 s_creator_os)
{
  switch(s_creator_os)
  {
    case EXT3_OS_LINUX:
      printf("Partition created by Linux.\n");
      break;
    case EXT3_OS_HURD:
      printf("Partition created by HURD.\n");
      break;
    case EXT3_OS_MASIX:
      printf("Partition created by MASIX.\n");
      break;
    case EXT3_OS_FREEBSD:
      printf("Partition created by FreeBSD.\n");
      break;
    case EXT3_OS_LITES:
      printf("Partition created by Lites.\n");
      break;
    default:
      printf("Unknown creator OS.\n");
  }
}

void print_featcompat(__u32 s_feature_compat)
{/*Informations here: http://www.charmed.com/txt/ext2.txt*/
  if(mask(s_feature_compat, EXT3_FEATURE_COMPAT_DIR_PREALLOC))
  {
    printf("This partition supports directories preallocation.\n");
  }
  if(mask(s_feature_compat, EXT3_FEATURE_COMPAT_IMAGIC_INODES))
  {
    printf("This partition supports magical inodes.\n");//???
  }
  if(mask(s_feature_compat, EXT3_FEATURE_COMPAT_HAS_JOURNAL))
  {
    printf("This partition supports journalling.\n");
  }
  if(mask(s_feature_compat, EXT3_FEATURE_COMPAT_EXT_ATTR))
  {
    printf("This partition supports extended user attirbutes.\n");
  }
  if(mask(s_feature_compat, EXT3_FEATURE_COMPAT_RESIZE_INODE))
  {
    printf("This partition supports variable-sized inodes.\n");//???
  }
  if(mask(s_feature_compat, EXT3_FEATURE_COMPAT_DIR_INDEX))
  {
    printf("This partition supports directory indexing.\n");//?????
  }
}
void print_featincompat(__u32 s_feature_incompat)
{//Retrieves incompatible features
  if(mask(s_feature_incompat, EXT3_FEATURE_INCOMPAT_COMPRESSION))
  {
    printf("This partition does not support \"transparent\" compression.\n");//????
  }
  if(mask(s_feature_incompat, EXT3_FEATURE_INCOMPAT_FILETYPE))
  {
    printf("This partition does not support filetypes.\n");//??
  }
  if(mask(s_feature_incompat, EXT3_FEATURE_INCOMPAT_RECOVER))
  {
    printf("This partition needs recovery.\n");
  }
  if(mask(s_feature_incompat, EXT3_FEATURE_INCOMPAT_JOURNAL_DEV))
  {
    printf("This partition does not support journalling.\n");//???
  }
  if(mask(s_feature_incompat, EXT3_FEATURE_INCOMPAT_META_BG))
  {
    printf("This partition does not support metainformation block groups .\n");//????
  }
}
void print_featROcompat(__u32 s_feature_ro_compat)
{
  if(mask(s_feature_ro_compat, EXT3_FEATURE_RO_COMPAT_SPARSE_SUPER))
  {
    printf("This partition supports sparsefiles in read-only mode.\n");
  }
  if(mask(s_feature_ro_compat, EXT3_FEATURE_RO_COMPAT_LARGE_FILE))
  {
    printf("This partition supports large files in read-only mode.\n");
  }
  if(mask(s_feature_ro_compat, EXT3_FEATURE_RO_COMPAT_BTREE_DIR))
  {
    printf("This partition supports directory B-trees in read only mode.\n");
    //Cf http://en.wikipedia.org/wiki/B-tree
  }
}

//Example: 550e8400-e29b-41d4-a716-446655440000
void  print_UUID(const __u8 * tab_uuid)
{
  printf("UUID: %hx%hx%hx%hx-%hx%hx-%hx%hx-%hx%hx-%hx%hx%hx%hx%hx%hx\n",
      tab_uuid[0], tab_uuid[1], tab_uuid[2], tab_uuid[3], tab_uuid[4],
      tab_uuid[5], tab_uuid[6], tab_uuid[7], tab_uuid[8], tab_uuid[9],
      tab_uuid[10], tab_uuid[11], tab_uuid[12], tab_uuid[13], tab_uuid[14],
      tab_uuid[15]);
}



void print_mountoptions(__u32 s_default_mount_opts)
{
  if(mask(s_default_mount_opts, EXT3_DEFM_DEBUG))
  {
    printf("Debug information displayed when mounting.\n");
  }
  if(mask(s_default_mount_opts, EXT3_DEFM_BSDGROUPS))
  {
    printf("The default group ID of a new file is its parent directory's one.\n");
  }
  if(mask(s_default_mount_opts, EXT3_DEFM_XATTR_USER))
  {
    printf("Support for extended user attributes.\n");
  }
  if(mask(s_default_mount_opts, EXT3_DEFM_ACL))
  {
    printf("Support for Access Control Lists.\n");
  }
  if(mask(s_default_mount_opts, EXT3_DEFM_UID16))
  {
    printf("Desactivation of the 32-bits mode for UIDs and GIDs "
        "(retrocompatibility with older kernels).\n");
  }
  if(mask(s_default_mount_opts, EXT3_DEFM_JMODE))
  {
    printf("Activated journalling.\n");

    if(mask(s_default_mount_opts, EXT3_DEFM_JMODE_DATA))
    {
      printf("Journal is used in complete journaling mode (data+metadata).\n");
    }
    else
    {
      if(mask(s_default_mount_opts, EXT3_DEFM_JMODE_ORDERED))
      {
        printf("Journal is used in ORDERED mode.\n");
      }
      else
      {
        if(mask(s_default_mount_opts, EXT3_DEFM_JMODE_WBACK))
        {//Macro has same value as EXT3_DEFM_JMODE, must be tested last.
          printf("Journal is used in WRITEBACK mode.\n");
        }
      }

    }
  }
}

/*End of superblock related function*/



/*Inode related functions*/
void print_filemode(__u16 i_mode)
{
  /*
     we use <sys/stat.h> macros like S_ISDIR(mode).
     File types.
     ---> in <bits/stat.h>
#define __S_IFDIR       0040000  Directory.
#define __S_IFCHR       0020000  Character device.
#define __S_IFBLK       0060000  Block device.
#define __S_IFREG       0100000  Regular file.
#define __S_IFIFO       0010000  FIFO.
#define __S_IFLNK       0120000  Symbolic link.
#define __S_IFSOCK      0140000  Socket.
*/
  printf("File type : ");
  if ( S_ISDIR(i_mode) ) {
    printf("directory\n");
  }
  else if ( S_ISCHR(i_mode) ) {
    printf("character device\n");
  }
  else if ( S_ISBLK(i_mode) ) {
    printf("block device\n");
  }
  else if ( S_ISFIFO(i_mode) ) {
    printf("FIFO file\n");
  }
  else if ( S_ISLNK(i_mode) ) {
    printf("Symbolic link\n");
  }
  else if ( S_ISSOCK(i_mode) ) {
    printf("Socket file\n");
  }
  else if ( S_ISREG(i_mode) ) {
    printf("Regular file\n");
  }
  else {
    printf("Unknown?\n");
  }
  //From http://uranus.it.swin.edu.au/~jn/explore2fs/es2fs.htm
  if(mask(i_mode, 0x800))
  {
    printf("SUID\n");
  }
  if(mask(i_mode, 0x400))
  {
    printf("SGID\n");
  }
  if(mask(i_mode, 0x200))
  {
    printf("Sticky bit\n");
  }
  printf("Permissions : ");
  print_permissions((i_mode&0700)>>6);//Only prints 3 bits
  print_permissions((i_mode&070)>>3);
  print_permissions((i_mode&07));
  printf("\n");
}


void print_fileflags(__u32 i_flags)//The flag set has to be 'masked'
{
  if(mask(i_flags, EXT3_SECRM_FL))
  {
    printf("File must be securely erased.\n");
  }
  if(mask(i_flags, EXT3_UNRM_FL))
  {
    printf("File can be undeleted.\n");
  }
  if(mask(i_flags, EXT3_COMPR_FL))
  {
    printf("File must be compressed.\n");
  }
  if(mask(i_flags, EXT3_SYNC_FL))
  {
    printf("File must be synchronized.\n");
  }
  if(mask(i_flags, EXT3_IMMUTABLE_FL))
  {
    printf("File cannot be modified.\n");
  }
  if(mask(i_flags, EXT3_APPEND_FL))
  {
    printf("File is in appending mode, writing is made at the end of the \
        file.\n");
  }
  if(mask(i_flags, EXT3_NODUMP_FL))
  {
    printf("File must not be deleted or dumped.\n");
  }
  if(mask(i_flags, EXT3_NOATIME_FL))
  {
    printf("File access time must not be updated.\n");
  }
  if(mask(i_flags, EXT3_DIRTY_FL))
  {
    printf("This file has been marked as dirty.\n");
  }
  if(mask(i_flags, EXT3_COMPRBLK_FL))
  {
    printf("This file is composed of one or more compressed 'clusters'.\n");
  }
  if(mask(i_flags, EXT3_NOCOMPR_FL))
  {
    printf("This file must not be compressed.\n");
  }
  if(mask(i_flags, EXT3_ECOMPR_FL))
  {
    printf("This file has been corrupted during the compression.\n");
  }

  if(mask(i_flags, EXT3_INDEX_FL))
  {
    printf("Hash-indexed directory.\n");
  }
  if(mask(i_flags, EXT3_IMAGIC_FL))
  {
    printf("AFS directory.\n");
  }
  if(mask(i_flags, EXT3_JOURNAL_DATA_FL))
  {
    printf("File data must be journalized.\n");
  }
  if(mask(i_flags, EXT3_RESERVED_FL))
  {
    printf("Reserved for the ext3 library.\n");
  }
}


void print_blockpointers( __u32 *i_block )
{
  int cmp;
  printf("Data block pointers : ");
  for ( cmp = 0; cmp < EXT3_N_BLOCKS; cmp++ )
    printf("\n\ti_block[%2u] = %u", cmp, i_block[cmp]);

  printf("\n");
}
