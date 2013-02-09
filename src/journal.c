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


#include "journal.h"
//#include <utils.h> /* be32_to_cpu defined here */


#ifdef EFENCE
#include <efence.h>
#endif

/* ALL JOURNAL FIELDS A STORE IN BIG ENDIAN */

int read_journal_superblock(int fd, int block_size, __u32 jsb_block_num, struct journal_superblock_s *jsb ) {

  /* read the journal superblock, given the size of a block, the first block
   * allocated to the journal's inode
   */

  off_t lret;
  ssize_t sret;

  __u64 jsb_pos=(__u64)(jsb_block_num) * block_size;

  lret = lseek( fd, jsb_pos, SEEK_SET);

  if ( lret == -1 ) {
    perror("ext3Viewer - lseek error: journal superblock");
    return -1;
  }
  sret = read( fd, jsb, sizeof(struct journal_superblock_s) );
  if ( sret == -1 ) {
    perror("ext3Viewer - read error: journal superblock");
    return -1;
  }
  return 0; 

}

int read_journal_header( int fd, int block_size, 
    __u32 journal_block_num, struct journal_header_s *header ) {

  /* Actually, this functions is not used since the next one
   * is ok for normal journal header as well. Revoke headers only contain
   * one more field in its structure ...
   */

  /* reads the journal standard header for all descriptor blocks */
  off_t lret;
  ssize_t sret;

  __u64 journal_offset = (__u64)(journal_block_num) * block_size;
  
  lret = lseek( fd, journal_offset, SEEK_SET );

  if ( lret == -1 ) {
    perror("ext3Viewer - lseek error: journal header");
    return -1;
  }
  sret = read( fd, header, sizeof(struct journal_header_s) );
  if ( sret == -1 ) {
    perror("ext3Viewer - read error: journal header");
    return -1;
  }
  return 0; 
}

int read_journal_revoke_header(int fd, int block_size,
    __u32 journal_block, int offset, struct journal_revoke_header_s *header ) {
  /* reads the journal revoke header */
  
  off_t lret;
  ssize_t sret;
  __u64 journal_offset = (__u64)(journal_block) * block_size;
  
  lret = lseek( fd, journal_offset + offset , SEEK_SET );
  if ( lret == -1 ) {
    perror("ext3Viewer - lseek error: journal header");
    return -1;
  }
  sret = read( fd, header, sizeof(struct journal_revoke_header_s) );
  if ( sret == -1 ) {
    perror("ext3Viewer - read error: journal header");
    return -1;
  }
  return 0; 
}



/**************************************************************************
 *                                                                        *
 * This section deeply lacks of documentation about the journal           *
 * I'n not very sure of the reading manner :(                             *
 * where could we get more info about write action on journal ? buffers ? *
 *                                                                        *
 **************************************************************************/

int dump_journal ( int fd, int block_size, struct ext3_inode *i ) {

  int cc = 0;

  while( i->i_block[cc] != 0 && cc < EXT3_NDIR_BLOCKS) {
    /* dump all directs blocks */
    dump_journal_block_rec( fd, block_size, i->i_block[cc], 0 );
    cc++;
  }

  /* dump indirect blocks, double indirect and triple indirect blocks */
  if ( i->i_block[EXT3_IND_BLOCK] != 0 )
    dump_journal_block_rec( fd, block_size, i->i_block[EXT3_IND_BLOCK], 1 );
  if ( i->i_block[EXT3_DIND_BLOCK] != 0 )
    dump_journal_block_rec( fd, block_size, i->i_block[EXT3_DIND_BLOCK], 2 );
  if ( i->i_block[EXT3_TIND_BLOCK] != 0 )
    dump_journal_block_rec( fd, block_size, i->i_block[EXT3_TIND_BLOCK], 3 );

  return 0;
}

int dump_journal_block_rec ( int fd, int block_size, __u32 block_num,
    int rec_level ) {

  __u32 *block; /* array to browse an indirect block */
  int i;
  int ret;

  if (  rec_level == 0 ) {
  
    /* if the current block is a direct block then we dump it */
    ret = dump_journal_block( fd, block_size, block_num);
    if ( ret == -1 ) {
      fprintf( stderr, "!!error dumping the journal!!\n" );
      return -1;
    }
 
  }
  else {
    /* else we read the block and for each block in it we browse the block
     * or we dump it depending on rec_level
     */
    block = malloc( block_size );
//fprintf( stderr, "->allocation %.8x niveau %d\n", block, rec_level );
    ret = read_block_pointer ( fd, block_size, block_num, block );
    if ( ret == -1 ) 
      exit(1);

    i = 0;
    while ( i < block_size/4  && block[i] != 0 ) {
      /* each block number is a __u32 */

      ret = dump_journal_block_rec( fd, block_size, block[i], rec_level-1 );
      if ( ret == -1 ) {
	return -1;
      }
      i++;
    }

//fprintf(stderr, "--->free %.8x niveau %d\n", block, rec_level);
    free( block );

  }
  return 0;

}

int dump_journal_block ( int fd, int block_size, __u32 block_num ) {

  /* dump a given block of the journal ( called by dump_journal_block_rec()
   * which will read recursively each block, indirect block, double ind... 
   */

  int offset=0;
  struct journal_block_tag_s jb_tag; 

  //struct journal_superblock_s jsb;
  struct journal_revoke_header_s j_rheader;
  off_t lret;
  ssize_t sret;

  /*
   * In the journal superblock is a a field named block_size i don't know if
   * it could be different that the ext3_super_block blocksize it could be
   * use just for compatibility => jbd can be used with other fs than
   * ext3.
   */

  /* we only read revoke headers because it's the same structure that a normal
   * header but it contains one more field. We just ignore it if it is not 
   * a revoke header but just a normal header 
   */
  read_journal_revoke_header( fd, block_size, block_num, 0, &j_rheader );

  if ( be32_to_cpu((j_rheader.r_header.h_magic)) == JFS_MAGIC_NUMBER ) { 

    /* if the journal block header we red is correct let's dump it */
    print_journal_revoke_header(&j_rheader);

    if ( be32_to_cpu(j_rheader.r_header.h_blocktype) == 
	JFS_DESCRIPTOR_BLOCK ) {

      /* if the readed block is actually a descriptor block, then we have
       * to read all the block tags that are close to it.
       */

      offset = sizeof( struct journal_header_s ); /* used to read the next
						     journal block tag */

      do {

	lret = lseek( fd, (__u64)(block_num)*block_size + offset, SEEK_SET );
	if ( lret == -1 ) {
	  perror("lseek dump journal block");
	  return -1;
	}
	sret = read( fd, &jb_tag, sizeof(struct journal_block_tag_s ) );
	if ( lret == -1 ) {
	  perror("read dump journal block");
	  return -1;
	}

	printf("Concerned block number : %u\n", be32_to_cpu(jb_tag.t_blocknr));
	printf("flag: %u\n",be32_to_cpu(jb_tag.t_flags));

	if ( be32_to_cpu(jb_tag.t_flags) & JFS_FLAG_ESCAPE ) {
	  printf("JFS_FLAG_ESPACE: on-disk block is escaped\n");
	}
	if ( be32_to_cpu(jb_tag.t_flags) & JFS_FLAG_SAME_UUID ) {
	  printf("JFS_FLAG_SAME_UUID: block has same uuid as previous\n");
	}  
	else 
	  offset += 16;
	/* i read that somewhere but don't know why, maybe some datas are
	 * stored after the block when it does not have the same uuid
	 */	  
	if ( be32_to_cpu(jb_tag.t_flags) & JFS_FLAG_DELETED) {
	  printf("JFS_FLAG_DELETED: block deleted by this transaction\n");

      	} 
	if ( be32_to_cpu(jb_tag.t_flags) & JFS_FLAG_LAST_TAG ) {
	  printf("JFS_FLAG_LAST_TAG: last tag in this descriptor block\n");
	}


	offset += sizeof( struct journal_block_tag_s );
      }
      while (  !((be32_to_cpu(jb_tag.t_flags) & JFS_FLAG_LAST_TAG))  && 
	  offset < block_size );
      /* we stop dumping tags as soon as we lseek a complete block or
       * if we met the JFS_FLAG_LAST_TAG
       */
    }
  printf(A_LINE);
  }
  return 0;
}



void print_journal_header( struct journal_header_s *header ) {
  if ( be32_to_cpu((header->h_magic)) != JFS_MAGIC_NUMBER ) {
    printf("ERROR ---------------------\n");
  }
  printf("Journal superblock header:\n");
  printf("Journal magic number:%#x, should be %#x \n",be32_to_cpu((header->h_magic)),JFS_MAGIC_NUMBER);
  printf("Journal block type:%u    ",be32_to_cpu((header->h_blocktype)));
  switch(be32_to_cpu(header->h_blocktype)) {

  case JFS_DESCRIPTOR_BLOCK:
    printf("JFS_DESCRIPTOR_BLOCK\n");
    break;
    
  case JFS_COMMIT_BLOCK:
    printf("JFS_COMMIT_BLOCK\n");
    break;
    
  case JFS_REVOKE_BLOCK:
    printf("JFS_REVOKE_BLOCK\n");
    break;

  case JFS_SUPERBLOCK_V1:
    printf("JFS_SUPERBLOCK_V1\n");
    break;

  case JFS_SUPERBLOCK_V2:
    printf("JFS_SUPERBLOCK_V2\n");
    break;

  default:
    printf("Unexpected block type\n");
    return;
  }
  printf("Journal sequence:%u    \n\n",be32_to_cpu((header->h_sequence)));
}

void print_journal_revoke_header( struct journal_revoke_header_s *header ) {
  
  print_journal_header( &header->r_header );
  if ( be32_to_cpu(header->r_header.h_blocktype) == JFS_REVOKE_BLOCK )
    printf("Count of bytes used in the block: %u\n",be32_to_cpu(header->r_count));
}



int is_pow_of ( int num, int pow ) {

  float nb = (float)num;
  float nb2=0;
  if ( num == pow ) 
    return 1;
  else {
    while ( nb > 1.0f ) {
      nb = nb/pow;
      nb2 = (int)nb;
      if ( nb2 != nb )
	return 0;
    }
    return 1;

  }
}


void print_journal_superblock(struct journal_superblock_s * j)
{
  print_journal_header( &j->s_header );
    printf("Journal device blocksize:%u    \n",be32_to_cpu((j->s_blocksize)));		
  printf("Total blocks in journal size:%u   \n",be32_to_cpu((j->s_maxlen)));		
  printf("First block of log information:%u\n", be32_to_cpu((j->s_first)));		
  printf("First commit ID expected in the log:%u\n",be32_to_cpu((j->s_sequence)));	
  printf("Block number of the log start:%u  \n",be32_to_cpu((j->s_start)));		
  printf("Error value (if any):%u\n",be32_to_cpu((j->s_errno)));
  printf("The following fields are only valid in a version 2 superblock.\n");
  printf("Compatible features set:%#x\n" ,be32_to_cpu((j->s_feature_compat))); 	
  printf("Incompatible features set:%#x\n",be32_to_cpu((j->s_feature_incompat))); 	
  printf("Read-only compatible feature set:%#x\n",be32_to_cpu((j->s_feature_ro_compat))); 
  print_UUID( j->s_uuid );
  printf("Number of filesystem sharing log:%u\n",be32_to_cpu((j->s_nr_users)));		/* Nr of filesystems sharing log */
  printf("Block number of dynamic superblock copy:%u\n",be32_to_cpu((j->s_dynsuper)));		/* Blocknr of dynamic superblock copy*/
  printf("Limit of journal blocks per transaction:%u\n",be32_to_cpu((j->s_max_transaction)));	/* Limit of journal blocks per trans.*/
  printf("Limit of data blocks per transaction:%u\n",be32_to_cpu((j->s_max_trans_data)));	/* Limit of data blocks per trans. */
  putchar('\n');
}

