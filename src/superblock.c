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

#include "superblock.h"

int read_superblock ( int fd, struct ext3_super_block *sb ) {

  char c;

  if( (lseek( fd, BOOT_SECTOR_OFFSET, SEEK_SET )) == -1 ) {
    perror("ext3Viewer error - lseek superblock error");
    return -1;
  }
  if( (read(fd, sb, sizeof(struct ext3_super_block) )) == -1 ) {
    perror("ext3Viewer error - superblock read error");
    return -1;
  }
  if ( sb->s_magic != EXT3_SUPER_MAGIC ) {
    fprintf( stderr, "!!ERROR!! : This filesystem is not a valid ext. "
        "Bad magic number\n");
    return -1;
  }
  if( mask(sb->s_feature_incompat, EXT3_FEATURE_INCOMPAT_RECOVER) ) {

    printf("WARNING: This filesystem is mounted or corrupted.\n"
        "         unmount it or fsck before using ext3viewer.\n"
        "         Using ext3viewer on mounted partition may\n"
        "         be unsafe.\n"
        "continue anyway (y/n)? ");
    c = getchar();
    while ( getchar()!='\n');
    if ( c != 'y' )
      return -2; //no error: quit cleanly the prog

    return 1;
  }
  return 0;
}

int read_superblock_backup ( int fd, struct ext3_super_block *sb,
    __u32 block_num, int block_size ) {

  char c;
  if( (lseek( fd, block_num*block_size, SEEK_SET )) == -1 ) {
    perror("ext3Viewer error - superblock lseek error");
    return -1;
  }
  if( (read(fd, sb, sizeof(struct ext3_super_block) )) == -1 ) {
    perror("ext3Viewer error - superblock read error");
    return -1;
  }

  if (  sb->s_magic != EXT3_SUPER_MAGIC ) {
    fprintf( stderr, "!!ERROR!! : This filesystem is not a valid ext. "
        "Bad magic number\n");
    return -1;
  }
  if( mask(sb->s_feature_incompat, EXT3_FEATURE_INCOMPAT_RECOVER) ) {
    printf("WARNING: This filesystem is mounted or corrupted.\n"
        "         unmount it or fsck before using ext3viewer.\n"
        "         Using ext3viewer on mounted partition may\n"
        "         be unsafe.\n"
        "continue anyway (y/n)? ");
    c = getchar();
    while ( getchar()!='\n');
    if ( c != 'y' )
      return -2;
    return 1;
  }
  return 0;

}
int read_superblock_gui ( int fd, struct ext3_super_block *sb ) {

  if( (lseek( fd, BOOT_SECTOR_OFFSET, SEEK_SET )) == -1 ) {
    perror("ext3Viewer error - superblock lseek error");
    return -1;
  }
  if( (read(fd, sb, sizeof(struct ext3_super_block) )) == -1 ) {
    perror("ext3Viewer error - superblock read error");
    return -1;
  }
  if ( sb->s_magic != EXT3_SUPER_MAGIC ) {
    /*printf("!!ERROR!! : This filesystem is not a valid ext. "
      "Bad magic number\n");*/
    return -1;
  }
  if( mask(sb->s_feature_incompat, EXT3_FEATURE_INCOMPAT_RECOVER) )
    return 1;
  else
    return 0;
}





void print_superblock ( const struct ext3_super_block *sb ) {
  printf(A_LINE);
  printf("\t\t\t\tEXT3_SUPER_BLOCK\n");
  printf(A_LINE);
  printf("Total inode count    = %u\n", sb->s_inodes_count);
  printf("Block count    = %u\n", sb->s_blocks_count);
  printf("Reserved block count  = %u\n", sb->s_r_blocks_count);
  printf("Free block count = %u\n", sb->s_free_blocks_count);
  printf("Free inode count = %u\n", sb->s_free_inodes_count);
  printf("First data block = %u\n", sb->s_first_data_block);
  printf("Block size = 1024 * 2^%u octets\n", sb->s_log_block_size);
  printf("Fragment size =%u\n", sb->s_log_frag_size);
  printf("Blocks per group count =%u\n", sb->s_blocks_per_group);
  printf("Fragments per group count =%u\n", sb->s_frags_per_group);
  printf("Inode per group count =%u\n", sb->s_inodes_per_group);
  printf(A_LINE);

  printf("Last mounting operation timestamp =%u\n", sb->s_mtime);
  print_time(sb->s_mtime);
  printf("Last writing operation timestamp =%u\n", sb->s_wtime);
  print_time(sb->s_wtime);
  printf("Counter (times mounted)=%hu\n", sb->s_mnt_count);
  printf("Authorized mount count before integrity checking =%hu\n", sb->s_max_mnt_count);
  printf("Magic Number =%#.4hx\n", sb->s_magic);
  printf("Status flag =%hx\n", sb->s_state);
  print_state(sb->s_state);
  printf("Behaviour when detecting errors =%hu\n", sb->s_errors);
  print_errorbehavior(sb->s_errors);
  printf("Minimum compatible revision level (version)=%hu\n", sb->s_minor_rev_level);
  print_revlev(sb->s_minor_rev_level);
  printf("Last check timestamp =%u\n", sb->s_lastcheck);
  print_time(sb->s_lastcheck);
  printf("Maximum interval between checks = %u secondes\n", sb->s_checkinterval);
  printf("Creator OS =%u\n", sb->s_creator_os);
  print_creatorOS(sb->s_creator_os);
  printf("Current revision level (version)=%u\n", sb->s_rev_level);
  print_revlev(sb->s_rev_level);


  printf("%s", A_LINE);

  printf("Default UID for reserved blocks =%hu\n", sb->s_def_resuid);
  printf("Default GID for reserved blocks =%hu\n", sb->s_def_resgid);
  printf("First non-reserved inode number =%u\n", sb->s_first_ino);
  printf("Inode size = %hu\n", sb->s_inode_size);
  printf("Superblock block number (being read)= %hu\n", sb->s_block_group_nr);


  printf(A_LINE);

  printf("Compatible features set = %#.4x\n", sb->s_feature_compat);
  print_featcompat(sb->s_feature_compat);
  printf("Incompatible features set = %#.4x\n", sb->s_feature_incompat);
  print_featincompat(sb->s_feature_incompat);
  printf("Read-Only Compatible features set = %#.4x\n", sb->s_feature_ro_compat);
  print_featROcompat(sb->s_feature_ro_compat);
  printf("%s", A_LINE);


  print_UUID( sb->s_uuid );
  printf("Volume name = \"%s\"\n", sb->s_volume_name);
  printf("Last mounted in directory \"%s\"\n", sb->s_last_mounted);

  printf("Compression usage bitmap = %u\n", sb->s_algorithm_usage_bitmap);
  printf("Number of blocks to try to preallocate = %hu\n", sb->s_prealloc_blocks);
  printf("Number of blocks to preallocate for directories = %hu\n", sb->s_prealloc_dir_blocks);

  printf(A_LINE);

  //See     http://en.wikipedia.org/wiki/UUID
  //550e8400-e29b-41d4-a716-446655440000
  printf("Journal ");
  print_UUID( sb->s_journal_uuid );

  printf("Journal Inode number = %u\n", sb->s_journal_inum);  /* inode number of journal file */
  printf("Journal device number = %u\n", sb->s_journal_dev);  /* device number of journal file */
  printf("Start of list of orphaned inodes to delete = %u\n", sb->s_last_orphan);  /* start of list of inodes to delete */

  printf("HTREE default hash seed = %x|%x|%x|%x\n", sb->s_hash_seed[0], sb->s_hash_seed[1], sb->s_hash_seed[2], sb->s_hash_seed[3]);       //http://www.linuxsymposium.org/2002/view_txt.php?talk = 3&text = abstract

  printf("Hash default version = %hd\n", sb->s_def_hash_version);
  printf("Default mount options = %u\n", sb->s_default_mount_opts);
  print_mountoptions(sb->s_default_mount_opts);
  printf("First metablock block group = %u\n", sb->s_first_meta_bg);

}






void print_sb_copy ( const struct ext3_super_block *sb ) {
  /*
     Because this can consume a considerable amount of space for large
     filesystems, later revisions can optionally reduce the number of backup
     copies by only putting backups in specific groups (this is the sparse
     superblock feature).  The groups chosen are 0, 1 and powers of 3, 5 and 7.
     */
  int group_count = sb->s_blocks_count/EXT3_BLOCKS_PER_GROUP(sb);
  int i = 1;

  printf("The superblock contains very important datas, ext3 maintains "
      "several copies of \nit. ");

  if (! mask(  sb->s_feature_ro_compat, EXT3_FEATURE_RO_COMPAT_SPARSE_SUPER)) {
    printf("a copy exists at the start of every block group\n");

    if ( group_count > 0 ) {

      printf("\tThere is a copy of the superblock at group #%d block #%d\n\n",
          i, i*EXT3_BLOCKS_PER_GROUP(sb) );

      for ( i = 3 ; i < group_count || i == 0; i++ )

        printf("\tThere is a copy of the superblock at group #%d block #%d\n\n",
            i, i == 0?EXT3_BLOCK_SIZE(sb):i*EXT3_BLOCKS_PER_GROUP(sb) );
    }
    else {
      printf("There is no copy of the superblock\n");
    }
  }
  else {
    printf("Your filesystem uses the sparse superblock feature :\n\n");

    if ( group_count > 0 ) {
      printf("\tThere is a copy of the superblock at group #%d block #%d\n\n",
          1, i*EXT3_BLOCKS_PER_GROUP(sb) );

      for ( i = 3 ; i < group_count ; i++ ) {

        if ( is_pow_of( i, 3 ) || is_pow_of( i, 5 ) || is_pow_of( i, 7 ) ) {
          printf("\tThere is a copy of the superblock at group #%d block #%d\n\n",
              i, i*EXT3_BLOCKS_PER_GROUP(sb) );
        }
      }
    }
    else {
      printf("There is no copy of the superblock\n");

    }


  }
}
