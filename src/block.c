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

#include "block.h"

int read_block ( int fd, struct ext3_super_block *sb,
    __u32 num, char *block ) {
  off_t lret;
  size_t sret;

  lret = lseek( fd, ((off_t) num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);
  if ( lret <= 0 ) {
    perror("ext3Viewer error - lseek read_block");
    return -1;
  }

  sret = read ( fd, block, EXT3_BLOCK_SIZE(sb) );

  if ( sret <= 0 ) {
    perror("ext3Viewer error - read read_block");
    return -1;
  }
  return 0;
}

int read_block_pointer ( int fd, int block_size,
    __u32 num, __u32 *block ) {
  off_t lret;
  size_t sret;

  lret = lseek( fd, ((off_t) num)*block_size, SEEK_SET);
  if ( lret <= 0 ) {
    perror("ext3Viewer error - lseek read_block");
    return -1;
  }

  sret = read ( fd, block, block_size );

  if ( sret <= 0 ) {
    perror("ext3Viewer error - read read_block");
    return -1;
  }
  return 0;
}

/*void print_block ( int fd, struct ext3_super_block *sb, __u32 block_num )
  {
  print_blocks ( fd, sb, block_num, 0, PLAIN_DATA );
  }
  */
void print_blocks ( int fd, struct ext3_super_block *sb,
    __u32 block_num, short rec_level, int mode ) {

  /*
     This function reads and print recursively the indexed blocks :

     a rec_level of 1 means to consider block_num as an index of blocks
     2                   block_num as an index of index of blocks
     and so on ...
     */

  unsigned int c = 0;
  char octet;
  __u32 iblock_num;
  off_t lret;
  size_t sret;

  if ( rec_level == 0 ) {

    DEBUG_PRINT("Reading block %u", block_num);

    lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);

    if ( lret <= 0 ) {
      perror("ext3Viewer error - lseek block print_blocks");
      exit (-1);
    }

    if ( mode == HEXA_DATA ) {

      print_block_hexa_noline ( fd, sb, block_num );

    }
    else if ( mode == PLAIN_DATA ) {

      for ( c = 0 ; c < EXT3_BLOCK_SIZE(sb) ; c++ )
      {
        sret = read ( fd, &octet, sizeof(char) );
        if ( sret <= 0 ) {
          perror("ext3Viewer error - read octet print_blocks");
          exit (-1);
        }
        putchar(octet);
      }

    }

  }
  else {

    if ( block_num > 0 ) {
      DEBUG_PRINT("Reading block index %u", block_num);

      lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);
      if ( lret <= 0 ) {
        perror("ext3Viewer error - lseek 2 print_blocks");
        exit (-1);
      }

      sret = read ( fd, &iblock_num, 4 ); /*we read the block number of the
                                            indexed block */
      if ( sret <= 0 ) {
        perror("ext3Viewer error - read 2 print_blocks");
        exit (-1);
      }

      while ( iblock_num != 0 && c < (EXT3_BLOCK_SIZE(sb)/4) )
      {
        // Go through the block table

        // For each block, display it or go through it if it's a table
        print_blocks ( fd, sb, iblock_num, rec_level-1, mode );

        c++;
        lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb)+(c*4),
            SEEK_SET);
        if ( lret <= 0 ) {
          perror("ext3Viewer error - lseek 3 print_blocks");
          exit (-1);
        }
        // Move 4 by 4 octets because it's the size of u_32 on which the block
        // number are stored
        sret = read ( fd, &iblock_num, 4 );
        if ( sret <= 0 ) {
          perror("ext3Viewer error - read 3 print_blocks");
          exit (-1);
        }

      }

    }

  }


}


void print_block_as_bpointer( int fd, struct ext3_super_block *sb,
    __u32 block_num ) {
  int i = 0;
  int max = EXT3_BLOCK_SIZE(sb)/sizeof(__u32);
  __u32 block[EXT3_BLOCK_SIZE(sb)];
  int ret;

  ret = read_block_pointer ( fd, EXT3_BLOCK_SIZE(sb), block_num, block );
  if ( ret == -1 )
    exit(1);
  printf("  NUM                           POINTERS TO BLOCK N\n");
  printf(A_LINE);
  for ( i = 0 ; i < max ; i+= 6 ) {
    printf( "|0x%.3x||%11u|%11u|%11u|%11u|%11u|%11u|\n", i,
        block[i], block[i+1], block[i+2], block[i+3], block[i+4],
        block[i+5] );
  }
  printf(A_LINE);

}


void print_block_hexa ( int fd, struct ext3_super_block *sb, __u32 block_num ) {

  int cc, c;
  int blocksize = EXT3_BLOCK_SIZE(sb);
  char *block;
  int ret;

  block = malloc ( blocksize );
  ret = read_block( fd, sb, block_num, block );
  if ( ret == -1 )
    exit(1);

  for ( cc = 0 ; cc < blocksize ; cc+= 16) {

    printf(" %.8x  ", cc);

    for ( c = 0 ; c < 8 ; c++ ) //print numbers in hexa
      printf("%.2x ", (__u8) block[cc+c]);
    printf(" ");
    for (  ; c < 16 ; c++ )
      printf("%.2x ",  (__u8)block[cc+c]);
    printf(" |");

    for ( c = 0 ; c < 16 ; c++ ) //print ascii characters
      if ( block[c+cc] >= 32 && block[c+cc] <= 126 ) //viewable chars:man ascii
        putchar(block[c+cc]);
      else
        putchar('.');
    printf("|\n");

  }
  free ( block );
}


void print_block_hexa_noline ( int fd, struct ext3_super_block *sb,

    __u32 block_num ) {

  int cc, c;
  int blocksize = EXT3_BLOCK_SIZE(sb);
  char *block;
  int ret;

  block = malloc ( blocksize );
  ret = read_block( fd, sb, block_num, block );
  if ( ret == -1 )
    exit(1);

  for ( cc = 0 ; cc < blocksize ; cc+= 16) {

    printf("     ");

    for ( c = 0 ; c < 8 ; c++ ) //print numbers in hexa
      printf("%.2x ", (__u8) block[cc+c]);
    printf(" ");
    for (  ; c < 16 ; c++ )
      printf("%.2x ",  (__u8)block[cc+c]);
    printf(" |");

    for ( c = 0 ; c < 16 ; c++ ) //print ascii characters
      if ( block[c+cc] >= 32 && block[c+cc] <= 126 ) //viewable chars:man ascii
        putchar(block[c+cc]);
      else
        putchar('.');
    printf("|\n");

  }
  free ( block );
}



void print_t_bin(char content)//modified, Tested, working
{ // ex : 4 -> 00100000
  printf("%d", content&1);
  printf("%d",(content&2)>>1);
  printf("%d",(content&4)>>2);
  printf("%d",(content&8)>>3);
  printf("%d",(content&16)>>4);
  printf("%d",(content&32)>>5);
  printf("%d",(content&64)>>6);
  printf("%d",(content&128)>>7);
}
