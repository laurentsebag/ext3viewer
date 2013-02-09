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



#include <gtk/gtk.h>

#include "search_gui.h"

GtkWidget *create_search ( struct SearchBar *search ) {


  search->frame = gtk_frame_new("Search :") ;
  gtk_container_set_border_width(GTK_CONTAINER(search->frame), 10);

  search->hBox = gtk_hbox_new(FALSE, 10);

 search->entry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(search->hBox), search->entry, FALSE, FALSE, 0);
  
  search->list = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(search->list),"find a file from a pattern");
  gtk_combo_box_append_text(GTK_COMBO_BOX(search->list),"find a file from its inode");
  gtk_combo_box_set_active ( GTK_COMBO_BOX(search->list), 0);
  gtk_combo_box_set_active ( GTK_COMBO_BOX(search->list), 0);
  gtk_box_pack_start(GTK_BOX(search->hBox), search->list, FALSE, FALSE, 0);



  
  search->button = gtk_button_new_with_label("find it !");
  g_signal_connect ( G_OBJECT(search->button), "clicked", G_CALLBACK(onFind),
      search );
  gtk_box_pack_start(GTK_BOX(search->hBox), search->button, FALSE, FALSE, 0);

  gtk_container_add(GTK_CONTAINER(search->frame),search->hBox);

  return search->frame;

}

void onFind ( GtkWidget *widget, struct SearchBar *data ) {

  GtkTextIter iter, start, end;
  GtkTextBuffer *buffer;
  gchar *texte;

  gchar *pattern;
  pattern = g_strdup_printf("%s",gtk_entry_get_text(GTK_ENTRY(data->entry)));
  //printf("EEEEEEEEEEEE%s\n",pattern);

  regex_t preg;
  int ret;
  struct path_filo *path = NULL;
  char path_name[EXT3_NAME_LEN]="";

  
  if ( !fs.isopen ) {
    show_warning_dialog("You cannot access to this functionality :\nno filesystem is open.");
  }
  else if ( strlen(pattern) == 0 ) {
    show_warning_dialog("Please enter a non empty regex pattern or number");
  }
  else {


    DEBUG_PRINT("debut de la recherche %s", "");
    ret = regcomp ( &preg, pattern, REG_EXTENDED );
    if ( ret != 0 ) return;
//    while ( ret != 0 ) {
///	printf("votre pattern n'est pas valide\n");
//	printf("retapez : ");
//	scanf("%s",pattern);
//	ret = regcomp ( &preg, pattern, REG_EXTENDED );
//    }

    /* on recupere le buffer */
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));
  
    /* on vide la zone de texte */
    gtk_text_buffer_get_bounds ( buffer, &start, &end);
    gtk_text_buffer_delete ( buffer, &start, &end );

    /* ecritures dans la zone */
    gtk_text_buffer_get_end_iter ( buffer, &iter);
    if ( gtk_combo_box_get_active(GTK_COMBO_BOX(data->list)) == 0  ) {
    
      texte = g_strdup_printf("Searching for inodes that match \"%s\"\n\n",pattern);
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter,  texte, -1, "title", NULL);
      free(texte);

      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter,  "Searching consists in exploring each directory entries of the filesystem recursively. It could take some time, depending on the filesystem size.\n\n", -1, "citalic", NULL);
      search_file_rec_gui ( fs.fd, &fs.sb, 2, &preg , path, path_name );

      DEBUG_PRINT("fin de la recherche %s", "");
    }
    else {

//printf("aaaaaaaa%d\n",gtk_combo_box_get_active(GTK_COMBO_BOX(data->list)));

      texte = g_strdup_printf("Searching for files that have inode #%u\n\n", (unsigned int )strtoul( pattern, NULL, 10 ));
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter,  texte, -1, "title", NULL);
      free(texte);

      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter,  "Searching consists in exploring each directory entries of the filesystem recursively. It could take some time, depending on the filesystem size.\n\n", -1, "citalic", NULL);
    DEBUG_PRINT("debut de la recherche %s", "");

    search_inode_rec_gui ( fs.fd, &fs.sb, 2, strtoul( pattern, NULL, 10 ) , path, path_name );

    DEBUG_PRINT("fin de la recherche %s", "");


    }

  }
}


__u32 search_file_rec_gui ( int fd, struct ext3_super_block *sb,
			__u32 i_num, const regex_t *preg ,
			struct path_filo *path, char *path_name) {

    struct ext3_inode i;
    unsigned int c=0; //counter

    read_inode( fd, sb, i_num, &i); //read the file inode


    while( i.i_block[c] != 0 && c < EXT3_NDIR_BLOCKS)
    { //we read the 0-11 first file's blocks which are direct blocks 
   
	search_file_rec_block_gui ( fd, sb, preg, i.i_block[c], 0,
				path, path_name );

	c++;
    } 
  
    search_file_rec_block_gui ( fd, sb, preg, i.i_block[EXT3_IND_BLOCK], 1, path, 
			    path_name );
  
    search_file_rec_block_gui ( fd, sb, preg, i.i_block[EXT3_DIND_BLOCK], 2, path,
			    path_name );
  
    search_file_rec_block_gui ( fd, sb, preg, i.i_block[EXT3_TIND_BLOCK], 3, path,
			    path_name );
  
    return 0;
}


__u32 search_file_rec_block_gui ( int fd, struct ext3_super_block *sb, 
			      const regex_t *preg, __u32 block_num, 
			      short rec_level, struct path_filo *path,
			      char *path_name ) {


    off_t lret; size_t sret; int ret;
    struct ext3_dir_entry_2 dir_entry;
    struct ext3_inode i;
    __u32 iblock_num;
    unsigned int c=0,cc=0; //counter
    char file_name[EXT3_NAME_LEN];
    __u64 next_dir_entry=0;//offset to the next dir_entry

    /************************/
    size_t nmatch=1;
    regmatch_t pmatch[1];
    //utilise par regex.
    /************************/
    GtkTextIter iter;
    GtkTextBuffer *buffer;
    gchar *texte;

    
    if ( rec_level == 0 ) {
         
	next_dir_entry = 0;
    
	lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);
	if ( lret < 0 ) {
	    perror ("ext3Viewer - erreur de lseek search");
	    return 1;
	}

	sret = read( fd, &dir_entry, sizeof( struct ext3_dir_entry_2 ) );
	if ( sret < 0 ) {
	    perror ("ext3Viewer - erreur de lecture dir_entry search");
	    return 1;
	}


	DEBUG_PRINT("find: lecture du bloc %u",block_num);

	while( next_dir_entry < EXT3_BLOCK_SIZE(sb) && dir_entry.inode != 0 )
	{  
	    for ( c=0 ; c != dir_entry.name_len ; c++ ) 
		file_name[c] = dir_entry.name[c];
	
	    file_name[c] = '\0';
	    //this  null-terminated string contains the file_name
	
	    ret = regexec ( preg, file_name, nmatch, pmatch, REG_NOTEOL );
	    //printf("file - %s\n",file_name);
	    if ( ret == 0 ) {
//printf("l'inode %u correspond a votre recherche\n",
//    dir_entry.inode);

    /* on recupere le buffer */
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));
  
    /* ecritures dans la zone */
    gtk_text_buffer_get_end_iter ( buffer, &iter);
		texte = g_strdup_printf("The inode number %u matches :\n",dir_entry.inode);
		gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, texte,
		    -1, "italic", NULL);
		free(texte);

		push ( &path, file_name, dir_entry.inode );
		fill_path_name ( path, path_name );
//		printf("---> %s\n\n",path_name);//file_name);
		
		texte = g_strdup_printf("---> %s\n\n", path_name );
		gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, texte,
		    -1, "bold", NULL);
		free(texte);
		
		for ( cc = 0 ; cc < EXT3_NAME_LEN ; cc++ )
		    path_name[cc] = '\0';
		remove_top ( &path );
		//	    return dir_entry.inode;
	    }
	
	    /////////////////////////////////////////////////
	    read_inode( fd, sb, dir_entry.inode, &i );
	    if ( S_ISDIR(i.i_mode) && strcmp(file_name,".") &&
		 strcmp(file_name,"..") ) {
		DEBUG_PRINT("->parcours du dossier %s .",file_name);
		push ( &path, file_name, dir_entry.inode );
		search_file_rec_gui( fd, sb, dir_entry.inode, preg, path, path_name );
		remove_top ( &path );
	    }
	    /////////////////////////////////////////////////
	
	    next_dir_entry += dir_entry.rec_len;
	    //rec_len points to the next dir_entr
	
	    lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb)+
			  next_dir_entry, SEEK_SET);
	    if ( lret < 0 ) {
		perror ("ext3Viewer - erreur 2 de lseek search");
		return 1;
	    }

	    sret = read( fd, &dir_entry, sizeof( struct ext3_dir_entry_2 ) );
	    if (sret == 0) {
		perror("ext3Viewer - erreur 2 de lecture de dir_entry search");
		exit(1);
	    }
	
	}
    }
    else {
    
	if ( block_num > 0 ) {
	    DEBUG_PRINT("find: lecture du bloc index %u",block_num);      
      
	    lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);
	    if ( lret < 0 ) {
		perror ("ext3Viewer - erreur 3 de lseek search");
		return 1;
	    }
      
	    sret = read ( fd, &iblock_num, 4 ); /*we read the block number of the 
						  indexed block */
	    if ( sret < 0 ) {
		perror ("ext3Viewer - erreur de lecture iblock search");
		return 1;
	    }
      
	    while ( iblock_num != 0 && c < (EXT3_BLOCK_SIZE(sb)/4) )
	    { //on parcours la table de blocks 
	  

		//pour chaque block on l'affiche ou on le parcours si c une table
		search_file_rec_block_gui ( fd, sb, preg, iblock_num, rec_level-1, path,
					path_name );
	  
		c++;
		lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb)+(c*4),SEEK_SET);
		/* on se deplace de 4 en 4octets car c'est la taille de u_32 sur
		   laquelle on stoque un numero de block */
		if ( lret < 0 ) {
		    perror ("ext3Viewer - erreur 4 de lseek search");
		    return 1;
		}
		sret = read ( fd, &iblock_num, 4 );
		if ( sret < 0 ) {
		    perror ("ext3Viewer - erreur 2 de lecture iblock search");
		    return 1;
		}
	  
	    }
      
	}
    
    } 
  
    return 0;
}





int search_inode_gui ( int fd, struct ext3_super_block *sb, __u32 i_num,
		   __u32 i_tofind ) {

    struct path_filo *path = NULL;
    char path_name[EXT3_NAME_LEN]="";

    DEBUG_PRINT("debut de la recherche %s", "");

    search_inode_rec_gui ( fd, sb, i_num, i_tofind , path, path_name );

    DEBUG_PRINT("fin de la recherche %s", "");

    return 0;
}

int search_inode_rec_gui ( int fd, struct ext3_super_block *sb, __u32 i_num,
		       __u32 i_tofind, struct path_filo *path, char *path_name ) {

    struct ext3_inode i;
    unsigned int c=0; //counter

    GtkTextIter iter;
    GtkTextBuffer *buffer;
    //gchar *texte;

    if ( i_tofind == 2 ) {
    /* on recupere le buffer */
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));
  
    /* ecritures dans la zone */
    gtk_text_buffer_get_end_iter ( buffer, &iter);

      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "---> /",
			-1, "bold", NULL);
	return 0;
    }

    DEBUG_PRINT("debut de la recherche %s", "");

    read_inode( fd, sb, i_num, &i); //read the file inode


    while( i.i_block[c] != 0 && c < EXT3_NDIR_BLOCKS)
    { //we read the 0-11 first file's blocks which are direct blocks 
   
	search_inode_rec_block_gui ( fd, sb, i_tofind, i.i_block[c], 0, path, 
				 path_name );

	c++;
    } 
  
    search_inode_rec_block_gui ( fd, sb, i_tofind, i.i_block[EXT3_IND_BLOCK], 1,
			     path, path_name );
  
    search_inode_rec_block_gui ( fd, sb, i_tofind, i.i_block[EXT3_DIND_BLOCK], 2,
			     path, path_name );
  
    search_inode_rec_block_gui ( fd, sb, i_tofind, i.i_block[EXT3_TIND_BLOCK], 3,
			     path, path_name );

    return 0;
}


int search_inode_rec_block_gui ( int fd, struct ext3_super_block *sb, 
			     __u32 i_tofind, __u32 block_num, 
			     short rec_level, struct path_filo *path,
			     char *path_name ) {

    GtkTextIter iter;
    GtkTextBuffer *buffer;
    gchar *texte;

    off_t lret; size_t sret; //int ret;
    struct ext3_dir_entry_2 dir_entry;
    struct ext3_inode i;
    __u32 iblock_num;
    unsigned int c=0,cc; //counter
    char file_name[EXT3_NAME_LEN];
    __u64 next_dir_entry=0;//offset to the next dir_entry

 
    if ( rec_level == 0 ) {
         
	next_dir_entry = 0;
    
	lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);
	if ( lret < 0 ) {
	    perror ("ext3Viewer - erreur de lseek iname");
	    exit (1);
	}

	sret = read( fd, &dir_entry, sizeof( struct ext3_dir_entry_2 ) );
	if ( sret < 0 ) {
	    perror ("ext3Viewer - erreur de lecture dir_entry iname");
	    exit (1);
	}


	DEBUG_PRINT("find: lecture du bloc %u",block_num);

	while( next_dir_entry < EXT3_BLOCK_SIZE(sb) && dir_entry.inode != 0 )
	{  

	    for ( c=0 ; c != dir_entry.name_len ; c++ ) 
		file_name[c] = dir_entry.name[c];
	
	    file_name[c] = '\0';
	    //this  null-terminated string contains the file_name
	
	    if ( dir_entry.inode == i_tofind ) {
	  
		if ( strcmp(file_name,".") && strcmp(file_name,"..") ) {
		    /* si l'inode que l'on cherche est un dossier alors on n'affiche
		       pas les . ou .. mais juste le dossier/ */
		    /*	    printf("--->name = %s\n",file_name);
			    view ( path );
		    */


		    push ( &path, file_name, dir_entry.inode );
		    fill_path_name ( path, path_name );
		//    printf("---> %s\n",path_name);   

    /* on recupere le buffer */
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));
  
    /* ecritures dans la zone */
    gtk_text_buffer_get_end_iter ( buffer, &iter);
		    texte = g_strdup_printf("---> %s\n\n", path_name );
		    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, texte,
			-1, "bold", NULL);
		    free(texte);
		    
		    
		    //view(path);
		    for ( cc = 0 ; cc < EXT3_NAME_LEN ; cc++ )
			path_name[cc] = '\0';
		    remove_top ( &path );
	    
	    
		    // return dir_entry.inode;
		}
	    }
	
	    read_inode ( fd, sb, dir_entry.inode, &i );
	    if ( S_ISDIR(i.i_mode) && strcmp(file_name,".") &&
		 strcmp(file_name,"..") ) {

		DEBUG_PRINT("->parcours du dossier %s .",file_name);
	    
		push ( &path, file_name, dir_entry.inode );
		search_inode_rec_gui ( fd, sb, dir_entry.inode, i_tofind, path, path_name );
		remove_top ( &path );

	    }
	
	    next_dir_entry += dir_entry.rec_len;
	    //rec_len points to the next dir_entr
	
	    lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb)+
			  next_dir_entry, SEEK_SET);
	    if ( lret < 0 ) {
		perror ("ext3Viewer - erreur 2 de lseek iname");
		exit (1);
	    }

	    sret = read( fd, &dir_entry, sizeof( struct ext3_dir_entry_2 ) );
	    if (sret == 0) {
		perror("ext3Viewer - erreur 2 de lecture de dir_entry iname");
		exit(1);
	    }
	
	}
    }
    else {
    
	if ( block_num > 0 ) {
	    DEBUG_PRINT("find: lecture du bloc index %u",block_num);      
      
	    lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);
	    if ( lret < 0 ) {
		perror ("ext3Viewer - erreur 3 de lseek iname");
		exit (1);
	    }
      
	    sret = read ( fd, &iblock_num, 4 ); /*we read the block number of the 
						  indexed block */
	    if ( sret < 0 ) {
		perror ("ext3Viewer - erreur de lecture iblock iname");
		exit (1);
	    }
      
	    while ( iblock_num != 0 && c < (EXT3_BLOCK_SIZE(sb)/4) )
	    { //on parcours la table de blocks 
	  
		//pour chaque block on l'affiche ou on le parcours si c une table
		search_inode_rec_block_gui ( fd, sb, i_tofind, iblock_num, rec_level-1,
					 path, path_name );
	 
		c++;
		lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb)+(c*4),SEEK_SET);
		/* on se deplace de 4 en 4octets car c'est la taille de u_32 sur
		   laquelle on stoque un numero de block */
		if ( lret < 0 ) {
		    perror ("ext3Viewer - erreur 4 de lseek iname");
		    exit (1);
		}
		sret = read ( fd, &iblock_num, 4 );
		if ( sret < 0 ) {
		    perror ("ext3Viewer - erreur 2 de lecture iblock iname");
		    exit (1);
		}
	  
	    }
      
	}
    
    } 
    //printf("aucun fichier ne correspond a votre recherche\n");
    return 0;
}







