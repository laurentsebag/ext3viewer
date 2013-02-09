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
#include <stdlib.h>


#include "groups.h"

int read_inodebitmap( int fd, const struct ext3_super_block *sb,
		      const struct ext3_group_desc *gd,char * inodebitmap) {
    //Reads the inode bitmap
    __u64 block_num = gd->bg_inode_bitmap;
    int ret;

    bzero(inodebitmap, sizeof(inodebitmap));

    //inodebitmap->size = EXT3_INODES_PER_GROUP(sb)/8; // 1oct for 8 inodes
    ret = lseek( fd, block_num * EXT3_BLOCK_SIZE(sb), SEEK_SET);
    if ( ret == -1 ) {
	perror("ext3Viewer - lseek error: Inode Bitmap");
	return -1;
    }
    ret = read( fd, inodebitmap, (EXT3_INODES_PER_GROUP(sb)/8) );
    if ( ret == -1 ) {//8 inodes can be 'described' in a byte
	perror("ext3Viewer - read error: Inode Bitmap");
	return -1;
    }



    return 0;
}


int read_datablockbitmap( int fd, const struct ext3_super_block *sb,
			  const struct ext3_group_desc *gd,
			  const __u32 group_num, char *datablockbitmap) 
{
    //Reads the data block bitmap
    __u64 block_num = gd->bg_block_bitmap;
    int ret;
    int group_count = sb->s_blocks_count/EXT3_BLOCKS_PER_GROUP(sb);
    int otherblockscount;//If last group, will be used to count the number of 
//Blocks not belonging to this group
//(EXT3_BLOCKS_COUNT() - otherblockscount )/8
  
    memset(datablockbitmap,0,sizeof(datablockbitmap));
  
//Same as for inodes, 1oct for 8 BLOCKS
    ret = lseek( fd, block_num * EXT3_BLOCK_SIZE(sb), SEEK_SET);
    if ( ret == -1 ) {
	perror("ext3Viewer - lseek error: Data Block Bitmap");
	return -1;
    }
    if( group_num == group_count )//We are in the last group,
    {//which can be filled only partially
	otherblockscount=(group_count * EXT3_BLOCKS_PER_GROUP(sb));
	ret = read( fd, datablockbitmap,(sb->s_blocks_count
					 - otherblockscount )/8 );
	if ( ret == -1 ) {//8 blocks can be 'described' in a byte
	    perror("ext3Viewer - read error: Data Block Bitmap");
	    return -1;
	} 
    }
    else
    {//We proceed with reading the normal number of blocks
	ret = read( fd, datablockbitmap, (EXT3_BLOCKS_PER_GROUP(sb)/8) );
	if ( ret == -1 ) {//8 blocks can be 'described' in a byte
	    perror("ext3Viewer - read error: Data Block Bitmap");
	    return -1;
	} 
    }

    return 0;
}

int bitmap_test_free(char content )
{//If the first bit is 0 (free inode/block), return 1
    if(!(content&1)){return 1;}
    if(!((content&2)>>1)){return 2;}
    if(!((content&4)>>2)){return 3;}
    if(!((content&8)>>3)){return 4;}
    if(!((content&16)>>4)){return 5;}
    if(!((content&32)>>5)){return 6;}
    if(!((content&64)>>6)){return 7;}
    if(!((content&128)>>7)){return 8;}
    return 0;
}

void read_group_desc(int fd,const struct ext3_super_block *sb, 
		     __u16 group_num, struct ext3_group_desc *gd) {

  off_t lret;
  size_t sret;

  lret = lseek ( fd, (sb->s_first_data_block+1) * EXT3_BLOCK_SIZE(sb) +
		sizeof(struct ext3_group_desc) * group_num, SEEK_SET );

  if ( lret <= 0) {
    perror("ext3Viewer error - erreur lseek group_desc");
    exit(1);
  }

  DEBUG_PRINT_V("s_first data block %u", sb->s_first_data_block);
  DEBUG_PRINT_V("EXT3_BLOCK_SIZE(sb) %u",EXT3_BLOCK_SIZE(sb));
  DEBUG_PRINT_V("sizeof(struct ext3_group_desc) = %u",
		sizeof(struct ext3_group_desc) );
  DEBUG_PRINT_V("group_num = %u", group_num);
  DEBUG_PRINT("lseek for group_desc = %u oct", 
	      (sb->s_first_data_block+1) * EXT3_BLOCK_SIZE(sb) +
	      sizeof(struct ext3_group_desc) * group_num);
  sret=read ( fd, gd, sizeof(struct ext3_group_desc) );

  if( sret <= 0 ) {
    perror("ext3Viewer error - erreur de lecture group_desc");
    exit(1);
  }
  DEBUG_PRINT_V("->struct ext3_group_desc %hu {",group_num);
  DEBUG_PRINT_V("%u bg_block_bitmap",gd->bg_block_bitmap);
  DEBUG_PRINT_V("%u bg_inode_bitmap",gd->bg_inode_bitmap);
  DEBUG_PRINT_V("%u bg_inode_table",gd->bg_inode_table);
  DEBUG_PRINT_V("%hu bg_free_blocks_count",gd->bg_free_blocks_count);
  DEBUG_PRINT_V("%hu bg_free_inodes_count",gd->bg_free_inodes_count);
  DEBUG_PRINT_V("%hu bg_used_dirs_count",gd->bg_used_dirs_count);
  DEBUG_PRINT_V("}%s","");
}


void print_groupdesc ( const struct ext3_group_desc *gd) 
{
    printf("Block bitmap block (number)= %u\n",gd->bg_block_bitmap);		/* Blocks bitmap block */
    printf("Inode bitmap block number =  %u\n",gd->bg_inode_bitmap);		/* Inodes bitmap block */
    printf("First Inode table block number= %u\n",gd->bg_inode_table);		/* Inodes table block */
    printf("Free blocks count (in this group) = %hd\n",gd->bg_free_blocks_count);	/* Free blocks count */
    printf("Free inodes count (in this group) = %hd\n",gd->bg_free_inodes_count);	/* Free inodes count */
    printf("Directories count (in this group) = %hd\n",gd->bg_used_dirs_count);	/* Directories count */

}

void print_inodebitmap(const struct ext3_super_block *sb, char *inodebitmap,
		       __u16 gb_num)
{
    unsigned long long int i=0;//Counter used to print the content
    int charcount=0;//Printed characters, used for formatting purposes
    __u32 imax = ((__u32) EXT3_INODES_PER_GROUP(sb)) / 8;
    printf("Inode Bitmap: 0 means free Inode, 1 means Inode in use\n");
    printf("%s",A_LINE);
    printf("|Inode num.|    Bits values\n");
    printf("%s",A_LINE);
    while ( i < imax )
    {

	if(i%6==0)
	{
	    if(i==0) { 
		printf("|");
	    }
	    else {
		printf("\n|");
	    }
	    print_inodeblocknumber(sb,i*8,gb_num);
	}
	if( charcount%8 ==0)
	{
	    printf("|");
	}
	print_t_bin( inodebitmap[i] );
	i++;//We print bits eight by eight
	charcount=charcount+8;
    }
    printf("\n%s",A_LINE);
}



void print_inodeblocknumber(const struct ext3_super_block *sb,
			    __u32 inode_num_g, __u16 gb_num)
{
//We want to retrieve the true inode number, from the inode number in the group:
//We add inode count per group*group num.
    __u32 i_num;
    i_num=inode_num_g+(EXT3_INODES_PER_GROUP(sb)*gb_num);
    printf("0x%08x",i_num+1);//Can go up to 32 bits
//We add 1 because the inode number starts at 1
}

void print_datablockbitmap(const struct ext3_super_block *sb,
			   char *datablockbitmap, __u16 gb_num)
{
    unsigned long long int b=0;//Counter used to print the content
    int charcount=0;//Printed characters, used for formatting purposes
    __u32 bmax;//size of the datablockbitmap array
    int group_count = sb->s_blocks_count/EXT3_BLOCKS_PER_GROUP(sb);
//Total number of blocks
    if(gb_num == group_count)//We are in the last group
    {//which can be filled only partially
	bmax = ((__u32)
		(sb->s_blocks_count -  (group_count * EXT3_BLOCKS_PER_GROUP(sb)))/8);
    }
    else
    {
	bmax = ((__u32)
		EXT3_BLOCKS_PER_GROUP(sb)) / 8;
    }
    printf("Data blocks Bitmap: 0 means free Data Block, 1 means Data Block in use\n");
    printf("%s",A_LINE);
    printf("|Databl. n.|    Bits values\n");
    printf("%s",A_LINE);
    while ( b < bmax )
    {

	if(b%6==0)
	{
	    if(b==0) { 
		printf("|");
	    }
	    else {
		printf("\n|");
	    }
	    print_datablock_blocknumber(sb,b*8,gb_num);
	
	}
	if( charcount%8 ==0)
	{
	    printf("|");
	}
	print_t_bin(datablockbitmap[b]);//!\counter starts from 1; so the lastnumber is never used, should not be printed?
      
	b++;//We print bits eight by eight
	charcount=charcount+8;
    }
    printf("\n%s",A_LINE);
}

void print_datablock_blocknumber(const struct ext3_super_block* sb,
				 __u32 block_num_g, __u16 gb_num)
{
//We want to retrieve the true block number, from the block number in the group:
//We add block count per group*group num.
    __u32 b_num;
    b_num=block_num_g+(EXT3_BLOCKS_PER_GROUP(sb)*gb_num);
    printf("0x%08x",b_num+sb->s_first_data_block);//Can go up to 32 bits
//The block number can start at 1, or 0 (s_first_data_block)
}

