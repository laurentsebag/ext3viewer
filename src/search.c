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



#include "search.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef EFENCE
#include <efence.h>
#endif

/***************************************************************************
 *             searching a file from a regex pattern                       *
 ***************************************************************************/

int search_file ( int fd, struct ext3_super_block *sb, __u32 i_num,
    char *pattern ) {

  regex_t preg;
  int ret;
  struct path_filo *path = NULL;
  char path_name[EXT3_NAME_LEN]="";

  DEBUG_PRINT("begining of the search %s", "");
  ret = regcomp ( &preg, pattern, REG_EXTENDED );
  while ( ret != 0 ) {
    printf("Your pattern isn't valid\n");
    printf("Type pattern again: ");
    scanf("%s", pattern);
    ret = regcomp ( &preg, pattern, REG_EXTENDED );
  }

  ret = search_file_rec ( fd, sb, i_num, &preg , path, path_name );
  if ( ret == -1 ) {
    fprintf( stderr, "!!error searching for a file!!\n");
    return -1;
  }


  DEBUG_PRINT("End of the search %s", "");

  return 0;
}

int search_file_rec ( int fd, struct ext3_super_block *sb,
    __u32 i_num, const regex_t *preg ,
    struct path_filo *path, char *path_name) {

  struct ext3_inode i;
  unsigned int c = 0; //counter
  int ret;

  read_inode( fd, sb, i_num, &i); //read the file inode


  while( i.i_block[c] != 0 && c < EXT3_NDIR_BLOCKS)
  { //we read the 0-11 first file's blocks which are direct blocks

    ret = search_file_rec_block ( fd, sb, preg, i.i_block[c], 0,
        path, path_name );
    if ( ret == -1 ) {
      return -1;
    }

    c++;
  }

  if ( i.i_block[EXT3_IND_BLOCK] != 0 ) {
    ret = search_file_rec_block ( fd, sb, preg, i.i_block[EXT3_IND_BLOCK], 1,
        path, path_name );
    if ( ret == -1 ) {
      return -1;
    }
  }

  if ( i.i_block[EXT3_DIND_BLOCK] != 0 ) {
    ret = search_file_rec_block ( fd, sb, preg, i.i_block[EXT3_DIND_BLOCK], 2,
        path, path_name );
    if ( ret == -1 ) {
      return -1;
    }
  }

  if ( i.i_block[EXT3_TIND_BLOCK] != 0 ) {
    ret = search_file_rec_block ( fd, sb, preg, i.i_block[EXT3_TIND_BLOCK], 3,
        path, path_name );
    if ( ret == -1 ) {
      return -1;
    }
  }

  return 0;
}


int search_file_rec_block ( int fd, struct ext3_super_block *sb,
    const regex_t *preg, __u32 block_num,
    short rec_level, struct path_filo *path,
    char *path_name ) {

  off_t lret; size_t sret; int ret;
  struct ext3_dir_entry_2 dir_entry;
  struct ext3_inode i;
  __u32 iblock_num;
  unsigned int c = 0, cc = 0; /* counter */
  char file_name[EXT3_NAME_LEN];
  __u64 next_dir_entry = 0; /* offset to the next dir_entry */

  /* used by regex */
  size_t nmatch = 1;
  regmatch_t pmatch[1];

  if ( rec_level == 0 ) {

    next_dir_entry = 0;

    lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);
    if ( lret < 0 ) {
      return -1;
    }

    sret = read( fd, &dir_entry, sizeof( struct ext3_dir_entry_2 ) );
    if ( sret < 0 ) {
      return -1;
    }


    DEBUG_PRINT("find: reading block %u", block_num);

    while( next_dir_entry < EXT3_BLOCK_SIZE(sb) && dir_entry.inode != 0 )
    {
      for ( c = 0 ; c != dir_entry.name_len ; c++ )
        file_name[c] = dir_entry.name[c];

      file_name[c] = '\0';
      //this  null-terminated string contains the file_name

      ret = regexec ( preg, file_name, nmatch, pmatch, REG_NOTEOL );

      if ( ret == 0 ) {
        /* if we're here, then we found a file */
        printf("The inode %u matches your search\n",
            dir_entry.inode);
        push ( &path, file_name, dir_entry.inode );
        /* we concatenate the last part of the path*/
        fill_path_name ( path, path_name );

        printf("---> %s\n\n", path_name);

        for ( cc = 0 ; cc < EXT3_NAME_LEN ; cc++ )
          path_name[cc] = '\0';
        remove_top ( &path );
      }

      /* else we search for a dir to explore */
      read_inode( fd, sb, dir_entry.inode, &i );
      if ( S_ISDIR(i.i_mode) && strcmp(file_name,".")>0 &&
          strcmp(file_name,"..")>0 ) {
        /* we don't go through . and .. to avoid infinite loop */

        DEBUG_PRINT("->Going through folder %s .", file_name);
        push ( &path, file_name, dir_entry.inode );
        search_file_rec( fd, sb, dir_entry.inode, preg, path, path_name );
        remove_top ( &path );
      }

      next_dir_entry += dir_entry.rec_len;
      //rec_len points to the next dir_entr

      lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb)+
          next_dir_entry, SEEK_SET);
      if ( lret < 0 ) {
        return -1;
      }

      sret = read( fd, &dir_entry, sizeof( struct ext3_dir_entry_2 ) );
      if (sret == 0) {
        return -1;
      }

    }
  }
  else {

    if ( block_num > 0 ) {
      DEBUG_PRINT("find: reading block index %u", block_num);

      lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);
      if ( lret < 0 ) {
        return -1;
      }

      sret = read ( fd, &iblock_num, 4 ); /*we read the block number of the
                                            indexed block */
      if ( sret < 0 ) {
        return -1;
      }

      while ( iblock_num != 0 && c < (EXT3_BLOCK_SIZE(sb)/4) )
      { // Going through the block table


        // For each block, we display it or we go through it if it's a table
        ret = search_file_rec_block ( fd, sb, preg, iblock_num, rec_level-1, path,
            path_name );
        if ( ret == -1 ) {
          return -1;
        }

        lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb)+(c*4), SEEK_SET);
        // We move 4 by 4 octets because it's the size of u_32 on which the
        // block number is stored
        if ( lret < 0 ) {
          perror ("ext3Viewer error - error 4 lseek search");
          return -1;
        }
        sret = read ( fd, &iblock_num, 4 );
        if ( sret < 0 ) {
          perror ("ext3Viewer error - error 2 read iblock search");
          return -1;

        }

        c++;
      }

    }

  }

  return 0;
}

/*****************************************************************************
 *                  searching a file by its inode number                     *
 *****************************************************************************/

int search_inode ( int fd, struct ext3_super_block *sb, __u32 i_num,
    __u32 i_tofind ) {

  struct path_filo *path = NULL;
  char path_name[EXT3_NAME_LEN]="";
  int ret;

  DEBUG_PRINT("beginning of search %s", "");

  ret = search_inode_rec ( fd, sb, i_num, i_tofind , path, path_name );
  if ( ret == -1 )
    return -1;

  DEBUG_PRINT("end of search %s", "");

  return 0;
}

int search_inode_rec ( int fd, struct ext3_super_block *sb, __u32 i_num,
    __u32 i_tofind, struct path_filo *path, char *path_name ) {

  struct ext3_inode i;
  unsigned int c = 0; //counter
  int ret;

  if ( i_tofind == 2 ) {
    printf("---> /\n");
    return 0;
  }

  DEBUG_PRINT("beginning of search %s", "");

  read_inode( fd, sb, i_num, &i); //read the file inode


  while( i.i_block[c] != 0 && c < EXT3_NDIR_BLOCKS)
  { //we read the 0-11 first file's blocks which are direct blocks

    ret = search_inode_rec_block ( fd, sb, i_tofind, i.i_block[c], 0, path,
        path_name );
    if ( ret == -1 )
      return -1;

    c++;
  }

  if ( i.i_block[EXT3_IND_BLOCK] != 0 ) {
    ret = search_inode_rec_block ( fd, sb, i_tofind, i.i_block[EXT3_IND_BLOCK], 1, path, path_name );
    if ( ret == -1 )
      return -1;
  }

  if ( i.i_block[EXT3_DIND_BLOCK] != 0 ) {
    ret = search_inode_rec_block ( fd, sb, i_tofind, i.i_block[EXT3_DIND_BLOCK], 2, path, path_name );
    if ( ret == -1 )
      return -1;
  }
  if ( i.i_block[EXT3_TIND_BLOCK] != 0 ) {
    ret = search_inode_rec_block ( fd, sb, i_tofind, i.i_block[EXT3_TIND_BLOCK], 3, path, path_name );
    if ( ret == -1 )
      return -1;
  }

  return 0;
}


int search_inode_rec_block ( int fd, struct ext3_super_block *sb,
    __u32 i_tofind, __u32 block_num,
    short rec_level, struct path_filo *path,
    char *path_name ) {

  off_t lret; size_t sret; //int ret;
  struct ext3_dir_entry_2 dir_entry;
  struct ext3_inode i;
  __u32 iblock_num;
  unsigned int c = 0, cc; //counter
  char file_name[EXT3_NAME_LEN];
  __u64 next_dir_entry = 0;//offset to the next dir_entry
  int ret;

  if ( rec_level == 0 ) {

    next_dir_entry = 0;

    lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);
    if ( lret < 0 ) {
      perror ("ext3Viewer error - lseek iname error");
      return -1;
    }

    sret = read( fd, &dir_entry, sizeof( struct ext3_dir_entry_2 ) );
    if ( sret < 0 ) {
      perror ("ext3Viewer error - dir_entry iname reading error");
      return -1;
    }


    DEBUG_PRINT("find: reading block %u", block_num);

    while( next_dir_entry < EXT3_BLOCK_SIZE(sb) && dir_entry.inode != 0 )
    {

      for ( c = 0 ; c != dir_entry.name_len ; c++ )
        file_name[c] = dir_entry.name[c];

      file_name[c] = '\0';
      //this  null-terminated string contains the file_name

      if ( dir_entry.inode == i_tofind ) {

        if ( strcmp(file_name,".")>0 && strcmp(file_name,"..")>0 ) {
          // If the inode that we're looking for is a directory, then we do not
          // display . or .. but just the directory
          /*      printf("--->name = %s\n", file_name);
                  view ( path );
                  */

          push ( &path, file_name, dir_entry.inode );
          fill_path_name ( path, path_name );
          printf("---> %s\n", path_name);
          //view(path);
          for ( cc = 0 ; cc < EXT3_NAME_LEN ; cc++ )
            path_name[cc] = '\0';
          remove_top ( &path );


          // return dir_entry.inode;
        }
      }

      read_inode ( fd, sb, dir_entry.inode, &i );
      if ( S_ISDIR(i.i_mode) && strcmp(file_name,".")>0 &&
          strcmp(file_name,"..")>0 ) {

        DEBUG_PRINT("->going through directory %s .", file_name);

        push ( &path, file_name, dir_entry.inode );
        ret = search_inode_rec ( fd, sb, dir_entry.inode, i_tofind, path,
            path_name );
        if ( ret == -1 )
          return -1;

        remove_top ( &path );

      }

      next_dir_entry += dir_entry.rec_len;
      //rec_len points to the next dir_entr

      lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb)+
          next_dir_entry, SEEK_SET);
      if ( lret < 0 ) {
        perror ("ext3Viewer error - error 2 lseek iname");
        return -1;
      }

      sret = read( fd, &dir_entry, sizeof( struct ext3_dir_entry_2 ) );
      if (sret == 0) {
        perror("ext3Viewer error - error 2 reading dir_entry iname");
        exit(1);
      }

    }
  }
  else {

    if ( block_num > 0 ) {
      DEBUG_PRINT("find: reading block index %u", block_num);

      lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);
      if ( lret < 0 ) {
        perror ("ext3Viewer error - error 3 lseek iname");
        return -1;
      }

      sret = read ( fd, &iblock_num, 4 ); /*we read the block number of the
                                            indexed block */
      if ( sret < 0 ) {
        perror ("ext3Viewer error - error reading iblock iname");
        return -1;
      }

      while ( iblock_num != 0 && c < (EXT3_BLOCK_SIZE(sb)/4) )
      { // We go through the block table

        // For each block, we either display it or go through it if it is
        // a table
        ret = search_inode_rec_block ( fd, sb, i_tofind, iblock_num,
            rec_level-1, path, path_name );
        if ( ret == -1 )
          return -1;

        c++;
        lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb)+(c*4), SEEK_SET);
        // We move 4 by 4 octets because it's the size of u_32 on which the
        // block number is stored
        if ( lret < 0 ) {
          perror ("ext3Viewer error - error 4 lseek iname");
          return -1;
        }
        sret = read ( fd, &iblock_num, 4 );
        if ( sret < 0 ) {
          perror ("ext3Viewer error - iblock iname read error 2");
          return -1;
        }

      }

    }

  }
  return 0;
}

__u32 search_first_free_inode(int fd, const struct ext3_super_block *sb, char * inodebitmap)
{
  struct ext3_group_desc gd;
  __u32 group_count = sb->s_blocks_count/EXT3_BLOCKS_PER_GROUP(sb);
  __u32 curr_group = 0 ;
  __u32 i = 0;//counter: (inode number inside the group - 1)/8
  __u32 real_i_num = 0;//= inode_num_g+(EXT3_INODES_PER_GROUP(sb)*curr_group);
  int flag = 0;//1 when the free inode has been found
  __u32 imax = ((__u32) EXT3_INODES_PER_GROUP(sb)) / 8;//Maximum inode number in the group
  __u8 num = 0;//Number of the free inode in the byte tested; 0 if none

  while((curr_group <= group_count) && (flag == 0))//The counter starts at 0 and goes to group_count  (- 1)??
  {
    //  printf("Entering group %u", curr_group);
    read_group_desc( fd, sb, curr_group, &gd);
    read_inodebitmap( fd, sb, &gd, inodebitmap);
    for(i = 0; ((i < imax) && (flag == 0) );i++)
    {
      num = bitmap_test_free(inodebitmap[i]);
      if(num!= 0)
      {
        flag = 1;
        real_i_num = num+i*8+(EXT3_INODES_PER_GROUP(sb)*curr_group);
      }
      else
      {
        flag = 0;
      }
    }
    curr_group++;
  }
  return real_i_num;
}


__u32 search_first_free_block(int fd, const struct ext3_super_block *sb,
    char * datablockbitmap)
{
  struct ext3_group_desc gd;
  __u32 group_count = sb->s_blocks_count/EXT3_BLOCKS_PER_GROUP(sb);
  __u32 curr_group = 0 ;
  __u32 b = 0;//counter: (block number inside the group - 1)/8
  __u32 real_b_num = 0;//= block_num_g+(EXT3_BLOCKS_PER_GROUP(sb)*curr_group);
  int flag = 0;//1 when the free block has been found
  __u32 bmax ;//Maximum block number in the group
  __u8 num = 0;//Number of the free block in the byte tested; 0 if none

  while((curr_group <= group_count) && (flag == 0))
    //The counter starts at 0 and goes to group_count  (- 1)??
  {
    DEBUG_PRINT_V("Entering group %u\n", curr_group);
    read_group_desc( fd, sb, curr_group, &gd);
    read_datablockbitmap( fd, sb, &gd, curr_group, datablockbitmap);

    if(curr_group == group_count)//We are in the last group
    {//which can be filled only partially
      bmax = ((__u32)(sb->s_blocks_count -
            (group_count * EXT3_BLOCKS_PER_GROUP(sb)))/8);
    }
    else
    {
      bmax = ((__u32)EXT3_BLOCKS_PER_GROUP(sb)) / 8;
    }
    DEBUG_PRINT_V("bmax is %d\n", bmax);
    for(b = 0; ((b < bmax) && (flag == 0) );b++)
    {
      num = bitmap_test_free(datablockbitmap[b]);
      if(num != 0)
      {
        flag = 1;
        real_b_num = num+b*8+(EXT3_BLOCKS_PER_GROUP(sb)*curr_group);
        //The block numbers may start at 0 or at s_first_data_block
        //  printf("real_b_num = %u + %u * 8 + %u  = %u\n", num, b,
        //         (EXT3_BLOCKS_PER_GROUP(sb)*curr_group), real_b_num);
      }
      else
      {
        flag = 0;
      }
    }
    curr_group++;
  }


  return real_b_num -(1 - (sb->s_first_data_block));
  //In order to get the real block number: the first data block can be 0, or 1

}


