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



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linux/ext3_fs.h"

#include "text.h"
#include "superblock.h"
#include "inode.h"
#include "search.h"
#include "acl.h"


#ifdef EFENCE
#include <efence.h>
#endif

int main(int argc, char *argv[]) {

  struct ext3_super_block sb;
  struct ext3_inode i;
  struct ext3_group_desc gd; 
  struct journal_superblock_s jsb; // the journal superblock

  char *inodebitmap,*datablockbitmap; // allocated to print the bitmaps
  char c; // a counter
  char *block; //used to store a block for bcat and cat options
  int cc; //a counter
  int block_size; 
  
  __u16 gd_num; // group_desc number
  __u32 inode_num; //inode 
  __u32 block_num;
  __u16 group_count; /*Total group count, to avoid reading nonexistant 
		       group descriptors*/
  int fd;
  char pattern[EXT3_NAME_LEN]; // pattern for egrep ==> -find option
  long long int ret;
  
  DEBUG_PRINT_V("sizeoff_t %d",sizeof(off_t));
  DEBUG_PRINT_V("sizeof groupdesc = %d",sizeof(struct ext3_group_desc));
  
  if ( argc == 2 && !strcmp( argv[1], "-help" ) ) {
    printf(HELP_TXT_VERBOSE,argv[0]);
    return 0;
  }
  else if ( argc < 3 ) {
    printf(HELP_TXT,argv[0],argv[0]);
    return 1;
  }
  else if ( !strcmp( argv[2], "-stats" ) ) {
    /****************************************
     *  DUMP THE EXT3_SUPER_BLOCK STRUCTURE *
     ****************************************/
    if ( argc != 5 && argc != 3 ) {
      printf("usage %s <filesystem> -stats [ <block number> <block size> ]\n",argv[0]);
      return 1;
    }
    
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
    
    if ( argc == 3 ) {
      ret = read_superblock( fd, &sb ); //rempli la structure superblock 
    }
    else {
      
      block_num = strtoul ( argv[3], NULL, 10 );
      block_size = strtoul ( argv[4], NULL, 10 );
      ret = read_superblock_backup( fd, &sb, block_num, block_size );
      
    }
    if ( ret == -1 )
      return 1;
    
    print_superblock( &sb );  
/*  IT IS TOO VERBOSE SO I MOVED IT TO -printgd   
    int group_count = sb.s_blocks_count/EXT3_BLOCKS_PER_GROUP(&sb);

    int c;

    printf(A_LINE);
    printf(" group_descriptor count = %d\n",group_count);
    for ( c=0 ; c < group_count ; c++ ) {
      printf("[ group #%d ]\n",c);
      read_group_desc( fd, &sb, c, &gd);
      print_groupdesc ( &gd);
    }
    
*/
    close_fs ( fd );
    
  }
  else if( !strcmp( argv[2], "-ls" ) ) {
    /****************************************
     * PRINT THE DIR_ENTRIES OF A DIRECTORY *
     ****************************************/
    if ( argc != 4 ) {
      printf("syntax: %s <filesystem> -ls <inode number>\n",argv[0]);
      exit (1);  
    }
    
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
    
    inode_num = strtoul( argv[3], NULL, 10 );
    ret = read_superblock( fd, &sb );
    if ( ret == -1 )
      exit(1);
    
    read_inode( fd, &sb, inode_num, &i);
    if ( !S_ISDIR(i.i_mode) ) {
      printf("WARNING: This inode does not seem to be a directory\n");
      printf("         seeing it as a directory could lead to impre-\n"
	     "         dictable consequences\n");
      printf("continue anyway (y/n) ? ");
      c = getchar();
      if ( c == 'y' ) {
	print_dir( fd, &sb, inode_num );
      }
    }
    else {
      print_dir( fd, &sb, inode_num );
    }
    
    close_fs ( fd );
    
  }
  else if ( !strcmp( argv[2], "-stat" ) ) {
    /**************************************************
     * DUMP THE EXT3_INODE STRUCTURE OF A GIVEN INODE *
     **************************************************/
      
    if ( argc != 4 ) {
      printf("usage %s <filesystem> -stat <inode number>\n",argv[0]);
      exit(1);
    }
      
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
      
    inode_num = strtoul( argv[3], NULL, 10 );
      
    ret = read_superblock( fd, &sb );
    if ( ret == -1 ) {
      fprintf( stderr, "Superbloc read error\n");
      return 1;
    }
    if ( ret == -2 )
      return 0;
    
    read_inode ( fd, &sb, inode_num, &i );
    
    
    printf(A_LINE);
    printf("\t\t\tEXT3_INODE STRUCTURE OF INODE #%u\n",inode_num);
    printf(A_LINE);
    print_inode(fd,&sb, &i);
      
    close_fs ( fd );
      
  }
  else if ( !strcmp( argv[2], "-tree" ) ) {
    /*********************************************
     * GIVE THE ALLURE OF THE BLOCK'S ALLOCATION *
     *********************************************/
    if ( argc != 4 ) {
      printf("usage %s <filesystem> -tree <inode number>\n",argv[0]);
      exit(1);
    }
      
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
      
    inode_num = strtoul( argv[3], NULL, 10 );
      
    ret = read_superblock( fd, &sb );
    if ( ret == -1 ) {
      fprintf( stderr, "Superbloc read error\n");
      return 1;
    }
    if ( ret == -2 )
      return 0;
      
    read_inode ( fd, &sb, inode_num, &i );
    print_tree(fd,&sb, &i);
      
    close_fs ( fd );
      
  }
  else if ( !strcmp( argv[2], "-bpoint" ) ) {
  /***********************************
   * SHOW A BLOCK AS A BLOCK POINTER *
   ***********************************/
    if ( argc != 4 ) {
      printf("usage %s <filesystem> -bpoint <block num>\n",argv[0]);
      exit(1);
    }
    
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
    
    block_num = strtoul( argv[3], NULL, 10 );
    
    ret = read_superblock( fd, &sb );
    if ( ret == -1 ) {
      fprintf( stderr, "read_superblock\n");
      exit(1);
    }
    
    print_block_as_bpointer( fd, &sb, block_num );
    
    close_fs ( fd );
    
  }
  else if ( !strcmp( argv[2], "-bcat" ) ) {
    /*********************************
     * PRINT THE CONTENTS OF A BLOCK *
     *********************************/
    if ( argc != 4 && argc != 5 ) {
      printf("syntax: %s <filesystem> -bcat <block number> -x\n",argv[0]);
      exit (1);  
    }
    
    block_num = strtoul ( argv[3], NULL, 10 );
    
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
    inode_num = strtoul ( argv[3], NULL, 10 );
    
    read_superblock( fd, &sb );
    
    if ( argc == 5 && !strcmp("-x",argv[4]) ) {
      printf(A_LINE);
      printf("\t\t\tSHOWING BLOCK #%u IN HEXADECIMAL\n", block_num );
      printf(A_LINE);

      print_block_hexa( fd, &sb, block_num );
    
      printf(A_LINE);
    }
    else if ( argc == 4 ) {
      block = malloc ( EXT3_BLOCK_SIZE(&sb) );
      ret = read_block(fd, &sb, block_num, block );
      if ( ret == -1 ) 
	exit(1);

      for ( cc=0 ; cc < EXT3_BLOCK_SIZE(&sb) ; cc++ ) {
	putchar(block[cc]);
      }
      //    putchar('\n');
      free(block);
      close_fs ( fd );
    }
    else {
      printf("syntax: %s <filesystem> -bcat <block number> -x\n",argv[0]);
    }

  }
  else if ( !strcmp( argv[2], "-cat" ) ) {
    /********************************
     * PRINT THE CONTENTS OF A FILE *
     ********************************/
    if ( argc != 4 && argc != 5 ) {
      printf("syntax: %s <filesystem> -cat <inode number> [ -x ]\n",argv[0]);
      exit (1);  
    }
    
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
    inode_num = strtoul ( argv[3], NULL, 10 );
    
    read_superblock( fd, &sb );
    
    read_inode( fd, &sb, inode_num, &i);
    if ( S_ISDIR(i.i_mode) ) {
      printf("This inode is a directory, you should use -ls instead.\n");
      printf("use it anyway (y/n) ? ");
      c = getchar();
      if ( c != 'y') {
	close_fs(fd);
	return 0;
      }
    }
    else if ( S_ISLNK(i.i_mode) ) {
      print_symlink( fd, &sb, inode_num);
      close_fs(fd);
      return 0;
    }
    if ( argc == 5 && !strcmp(argv[4],"-x") ) {

      printf(A_LINE);
      printf("\t\tDUMPING FILE WITH INODE #%u IN HEXADECIMAL\n", inode_num );
      printf(A_LINE);

      print_file ( fd, &sb, inode_num, HEXA_DATA );

      printf(A_LINE);
    }
    else if ( argc == 4 )
      print_file ( fd, &sb, inode_num, PLAIN_DATA );
    else 
      printf("syntax: %s <filesystem> -cat <inode number> [ -x ]\n",argv[0]);


    close_fs ( fd );
    
  }
  else if ( !strcmp( argv[2], "-iname" ) ) {
    /*****************
     * FIND AN INODE *
     *****************/
    if ( argc != 4 ) {
      printf("syntax: %s <filesystem> -iname <inode num>\n",argv[0]);
      exit (1);  
    }
    
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
    
    inode_num = strtoul ( argv[3], NULL, 10 );
    read_superblock( fd, &sb );
    
    search_inode ( fd, &sb, 2, inode_num );
    
    close_fs ( fd );
    
  }
  else if ( !strcmp( argv[2], "-find" ) ) {
    /***************
     * FIND A FILE *
     ***************/
    if ( argc != 5 ) {
      printf("syntax: %s <filesystem> -find <inode num of the parent dir>"
	     " <regex pattern>\n",argv[0]);
      exit (1);  
    }
    
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
    
    
    inode_num = strtoul ( argv[3], NULL, 10);
    strcpy ( pattern, argv[4] );
    read_superblock( fd, &sb );
    
    read_inode( fd, &sb, inode_num, &i);
    if ( !S_ISDIR(i.i_mode) ) {
      printf("usage: -find <inode of the dir where the search begin> "
	     "<pattern>\n");
      printf("The inode you specified is not a directory. Please specify\n");
      printf("an inode of a directory (eg. \"/\" has inode number 2)\n");
    }
    else {    
      search_file ( fd, &sb, inode_num, pattern );
    }
    close_fs ( fd );
    
  }
  else if ( !strcmp( argv[2], "-imap" ) ) {

    /**************************
     * PRINT THE INODE BITMAP *
     **************************/
    if ( argc != 4 ) {
      printf("syntax: %s <filesystem> -imap <group_descriptor_num>\n",argv[0]);
      exit (1);  
    }
    
    gd_num=(__u16) atoi(argv[3]);
    
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
    read_superblock( fd, &sb );
    
    group_count = sb.s_blocks_count/EXT3_BLOCKS_PER_GROUP(&sb);
    if(  gd_num >= group_count && gd_num != 0 )
      {//gd_num is unsigned
	printf("WARNING: Group descriptor %d entered seems to be "
	       "inexistant.\n",gd_num);
	printf("(Existing group descriptors: from num. %d to num. %d,"
	       " total %d)\n",0,group_count==0?0:group_count-1,group_count);
	printf("Proceed anyway? (y/n)");
	if(getchar()!='y')
	  {
	    exit(0);
	  }
      }
    
    read_group_desc( fd, &sb, gd_num, &gd);
    
    inodebitmap = malloc( EXT3_INODES_PER_GROUP(&sb)/8 );
    
    read_inodebitmap( fd, &sb, &gd, inodebitmap);
    
    print_inodebitmap(&sb, inodebitmap, gd_num);
    
    free((void *)inodebitmap);
    close_fs ( fd );
    
  }
  else if ( !strcmp( argv[2], "-bmap" ) ) {
    /**************************
     * PRINT THE BLOCK BITMAP *
     **************************/

    if ( argc != 4 ) {
      printf("syntax: %s <filesystem> -bmap <group_descriptor_num>\n",argv[0]);
      exit (1);  
    }
    
    gd_num=(__u16) atoi(argv[3]);
    
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
    
    read_superblock( fd, &sb );
    
    group_count = sb.s_blocks_count/EXT3_BLOCKS_PER_GROUP(&sb);
    if( /*gd_num < 0 ||*/gd_num >= group_count && gd_num != 0  )
      {//gd_num is unsigned
	printf("WARNING: Group descriptor %u entered seems to be inexistant.\n",gd_num);
	printf("(Existing group descriptors: from num. %u to num. %u,"
	       " total %u)\n",0,group_count==0?0:group_count-1,group_count);
	printf("Proceed anyway? (y/n)");
	if(getchar()!='y')
	  {
	    exit(0);
	  }
      }
    
    
    read_group_desc( fd, &sb, gd_num, &gd);
    
    datablockbitmap = malloc( EXT3_BLOCKS_PER_GROUP(&sb)/8 );
    
    read_datablockbitmap( fd, &sb, &gd,gd_num, datablockbitmap);
    
    print_datablockbitmap(&sb, datablockbitmap, gd_num);
    
    free((void *)datablockbitmap);
    close_fs ( fd );
    
  }
  else if ( !strcmp( argv[2], "-ffi" ) ) {
    /*****************************
     * FIND THE FIRST FREE INODE *
     *****************************/
    
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
    read_superblock( fd, &sb );
    inodebitmap = malloc( (EXT3_INODES_PER_GROUP(&sb)/8) );
    inode_num=search_first_free_inode(fd, &sb,inodebitmap);
    if(inode_num>0)
      {
	printf("First Free Inode found: %u\n",inode_num);
      }
    else
      {
	printf("No free inode found\n");
      }
    free(inodebitmap);
    close_fs(fd);
  }
  else if ( !strcmp( argv[2], "-ffb" ) ) {
    
    /*****************************
     * FIND THE FIRST FREE BLOCK *
     *****************************/
    
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
    read_superblock( fd, &sb );
    datablockbitmap = malloc( (EXT3_BLOCKS_PER_GROUP(&sb)/8) );
    block_num=search_first_free_block(fd, &sb,datablockbitmap);
    if(block_num>0)
      {
	printf("First Free Block found: %u\n",block_num);
      }
    else
      {
	printf("No free block found\n");
      }
    free( datablockbitmap );
    close_fs(fd);
  }
  else if ( !strcmp( argv[2], "-jbd" ) ) {

    /********************
     * DUMP THE JOURNAL *
     ********************/

    /* IT WAS MORE CONVINIENT TO DO THIS FOR THE USER !
      if ( argc != 4 ) {
      printf("syntax: %s <filesystem> -jbd <journal_sb_block_num>\n",argv[0]);
           
      printf("To find the suitable block number,use the -stat option,\n"
      "with the inode num. set to %u (journal inode),\n"
      "see the data block pointers section and take the \n"
      "first block number that comes out, as the journal's\n"
      " superblock is in the journal's first 1024 bytes. \n"
      ,sb.s_journal_inum);
      exit (1);  
      }
    */
    if ( argc != 3 ) {
      printf("syntax: %s <filesystem> -jbd\n", argv[0]);
      return 1;
    }

    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;

    
    ret = read_superblock( fd, &sb );
    if ( ret == -1 ) {
      fprintf( stderr, "erreur de lecture du superblock\n");
      exit(1);
    }
    else if ( ret == -2 ) {
      exit(0);
    }
    read_inode ( fd, &sb, sb.s_journal_inum, &i );
    read_journal_superblock( fd, EXT3_BLOCK_SIZE(&sb), i.i_block[0], &jsb );

    printf(A_LINE);
    printf("\t\t\t\tJOURNAL SUPERBLOCK\n");
    printf(A_LINE"\n");
    
    print_journal_superblock(&jsb);
    
    printf(A_LINE);
    printf("\t\t\t\tJOURNAL CONTENTS\n");
    printf(A_LINE"\n");
    
    if ( jsb.s_start == 0 ) {
      printf("The journal is empty ! Dumping old transactions :\n");
    }
    
    dump_journal( fd, EXT3_BLOCK_SIZE(&sb), &i );
    
    close_fs ( fd );
    
  }
  else if ( !strcmp( argv[2], "-printgd" ) ) {
    /***************************
     * DUMP A GROUP DESCRIPTOR *
     ***************************/

    if ( argc != 4 ) {
      printf("syntax: %s <filesystem> -printgd <group num>\n",argv[0]);
      exit (1);  
    }
    
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
    
    gd_num = (__u16) atoi(argv[3]);
    read_superblock( fd, &sb );
    
    group_count =  sb.s_blocks_count/EXT3_BLOCKS_PER_GROUP(&sb);
    
    if ( gd_num > group_count ) {
      printf("WARNING: Group descriptor %d entered seems to be ",gd_num);
      printf("inexistant.\n");
      printf("(Existing group descriptors: from num. %d to num. %d,"
	     " total %d)\n",0,group_count,group_count+1);
      printf("Proceed anyway? (y/n) ");
      
      c = getchar();
      
      if ( c != 'y' ) 
	return 0;
    }
    
    read_group_desc( fd, &sb, gd_num, &gd);
    
    print_groupdesc ( &gd);
    
    close_fs ( fd );
  }
  else if ( !strcmp( argv[2], "-backups" ) ) {
    
    /***************************************
     * SHOW WHERE ARE SUPERBLOCKS' BACKUPS *
     ***************************************/

    if ( argc != 3 ) {
      printf("syntax: %s <filesystem> -backups\n",argv[0]);
      exit (1);  
    }
    
    fd = open_fs( argv[1] );
    if( fd == -1 )
      return 1;
    
    read_superblock( fd, &sb );
    
    print_sb_copy ( &sb );
    
    close_fs ( fd );
  }
  else {
    printf(HELP_TXT,argv[0],argv[0]);
    return 1;
  }
  
  return EXIT_SUCCESS;
}


    
