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

#include "tree_gui.h"

struct fileTree tree;
extern struct filesystem fs;

GtkWidget* create_tree() {

  /* on cree la structure de notre arbre */
  tree.treeStore = gtk_tree_store_new( N_COLUMN, G_TYPE_STRING,
				       G_TYPE_INT, GDK_TYPE_PIXBUF, 
				       G_TYPE_STRING, G_TYPE_STRING,
				       G_TYPE_STRING, G_TYPE_STRING );

 
  /* creation de la vue */
  tree.treeView = gtk_tree_view_new_with_model(GTK_TREE_MODEL(tree.treeStore));
  //gtk_tree_view_set_headers_clickable ( GTK_TREE_VIEW(tree.treeView), TRUE); 
  //  gtk_tree_view_set_show_expanders( tree.treeView, FALSE );
  g_object_set( G_OBJECT(tree.treeView), "enable-tree-lines", TRUE, NULL);
  //g_object_set( G_OBJECT(tree.treeView), "show-expanders", FALSE, NULL);
  //g_object_set( G_OBJECT(tree.treeView), "level-indentation", 40, NULL);


  /* signal pour le double click sur un ligne */
  g_signal_connect( G_OBJECT(tree.treeView), "row-activated",
  		    G_CALLBACK(on_row_double_click), NULL );
  /* permet d'afficher le menu sur click droit */
  g_signal_connect( G_OBJECT(tree.treeView), "button_press_event",
  		    G_CALLBACK(onButtonPressed), NULL );
  /* signal emis lors du click droit au clavier : shift+F10 
     A tester ==> ne marche pas sous enlightenment
  */
  g_signal_connect( G_OBJECT(tree.treeView),  "popup-menu",
		    G_CALLBACK(onPopupMenu), NULL );
 
  /*  GtkTreeSelection  *selection;
  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree.treeView));
  
  gtk_tree_selection_set_select_function( selection, 
					  on_row_double_click, NULL, NULL);
  */

  /* permet de taper un texte pour chercher dans l'arbre */
  gtk_tree_view_set_enable_search ( GTK_TREE_VIEW(tree.treeView), TRUE );
  /* alterne les couleurs entre 2 lignes */
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW(tree.treeView), TRUE);

  // gtk_tree_view_set_hover_expand      (GTK_TREE_VIEW(tree.treeView), TRUE);



  tree.cellRenderer = gtk_cell_renderer_pixbuf_new();
  tree.column = gtk_tree_view_column_new_with_attributes("Type",
							 tree.cellRenderer,
							 "pixbuf", TYPE_PIX,
							 NULL);
  gtk_tree_view_column_set_resizable  ( tree.column, TRUE );
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree.treeView), tree.column);

  /* creation du modele de la colone nom de fichier */
  tree.cellRenderer = gtk_cell_renderer_text_new();
  tree.column = 
    gtk_tree_view_column_new_with_attributes(FILE_NAME_TXT,
					     tree.cellRenderer,
					     "text", FILE_NAME,
					     "foreground", ROW_COLOR,
				 	     "weight", ROW_WEIGHT, 
					     NULL);
  gtk_tree_view_column_set_resizable  ( tree.column, TRUE );
  g_object_set(tree.cellRenderer, "weight-set", TRUE, NULL );
  g_object_set(tree.cellRenderer, "foreground-set", TRUE, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree.treeView), tree.column);


  tree.cellRenderer = gtk_cell_renderer_text_new();
  tree.column = 
    gtk_tree_view_column_new_with_attributes("Inode #",
					     tree.cellRenderer,
					     "text", INODE_NUM,
					     "foreground", ROW_COLOR,
					     NULL);
  gtk_tree_view_column_set_resizable  ( tree.column, TRUE );
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree.treeView), tree.column);

  tree.cellRenderer = gtk_cell_renderer_text_new();
  tree.column = 
    gtk_tree_view_column_new_with_attributes("rec_len",
					     tree.cellRenderer,
					     "text", REC_LEN,
					     "foreground", ROW_COLOR,
					     NULL);
  gtk_tree_view_column_set_resizable  ( tree.column, TRUE );
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree.treeView), tree.column);

  tree.cellRenderer = gtk_cell_renderer_text_new();
  tree.column = 
    gtk_tree_view_column_new_with_attributes("name_len",
					     tree.cellRenderer,
					     "text", NAME_LEN,
					     "foreground", ROW_COLOR,
					     NULL);
  gtk_tree_view_column_set_resizable  ( tree.column, TRUE );
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree.treeView), tree.column);


  return tree.treeView;
}


void init_tree() {

  tree.pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/directory.png", NULL);

  /* creation de la ligne a inserer */
  gtk_tree_store_append( tree.treeStore, &tree.iter, NULL);
  
  /* on remplit */
  gtk_tree_store_set( tree.treeStore, &tree.iter,
		      TYPE_PIX, tree.pixBuf,
		      FILE_NAME, "/",
		      ROW_COLOR, "black",
	 	      ROW_WEIGHT, 400, 
		      INODE_NUM, "2",
		      REC_LEN, "12",
		      NAME_LEN, "1",
		      -1);
  
  g_object_unref(tree.pixBuf);

}

void explore_dir ( int fd, struct ext3_super_block *sb, __u32 i_num,
		   GtkTreeIter clicked_dir )
{
  
  struct ext3_inode i;
  unsigned int c=0; /* counter */

  read_inode( fd, sb, i_num, &i); /* read the file inode */

  /* now we'll read the file's blocks */

  /*
    first part :
    let's read the 12 direct blocks from block n0 to (EXT3_NDIR_BLOCKS-1)
  */
  
  while( i.i_block[c]  && c < EXT3_NDIR_BLOCKS)
    { /* we read the 0-11 first file's blocks which are direct blocks
	 DEBUG_PRINT("lecture du bloc %u",i.i_block[c]); */
      
      explore_dir_blocks ( fd, sb, i.i_block[c], 0, clicked_dir );

      c++;
    }
  
  /*
    second part :
    let's read the indirect blocks

    organisation : le i_block[12] contient un pointeur vers une table de blocks
    il suffit de lire ce block 4octets par 4octets ( =sizeof(u_32) ) pour avoir
    l'adresse des blocks a lire
  */
  explore_dir_blocks ( fd, sb, i.i_block[EXT3_IND_BLOCK], 1, clicked_dir );
  
  /* And so on ...
     double indirect block
  */
  explore_dir_blocks ( fd, sb, i.i_block[EXT3_DIND_BLOCK], 2, clicked_dir );

  /* triple indirect block */
  explore_dir_blocks ( fd, sb, i.i_block[EXT3_TIND_BLOCK], 3, clicked_dir );


  //  tree.path = gtk_tree_path_new_from_indices ( 0, -1 );
  tree.path = gtk_tree_path_new_from_string("0");
  gtk_tree_view_expand_to_path ( GTK_TREE_VIEW(tree.treeView), 
				 tree.path );
  gtk_tree_path_free(tree.path); 

  //  putchar('\n');
}


void explore_dir_blocks ( int fd, struct ext3_super_block *sb,
			__u32 block_num, short rec_level, 
			  GtkTreeIter clicked_dir) {

  /*
    This function reads and print recursively the entries of a directory

    a rec_level of 1 means to consider block_num as an index of blocks
                   2                   block_num as an index of index of blocks
    and so on ...
  */

  unsigned int c=0;
  struct ext3_dir_entry_2 dir_entry;
  struct ext3_inode i;
  __u32 iblock_num;
  __u64 next_dir_entry=0;/* offset for the next dir_entry */
  off_t lret; size_t sret;
  char file_name[EXT3_NAME_LEN];
  char inode_num[16];
  char rec_len[16];
  char name_len[4];
  char tmp[4];
  int weight;  
  char color_rvb[8];
  if ( rec_level == 0 ) {
    
    DEBUG_PRINT("lecture du bloc %u",block_num);
    
    next_dir_entry = 0;
    
    lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);
    if ( lret <= 0 ) {
      perror("ext3Viewer - lseek 2 print_dir_blocks");
      exit (-1);
    }
    
    sret = read( fd, &dir_entry, sizeof( struct ext3_dir_entry_2 ) );
    if ( sret <= 0 ) {
      perror("ext3Viewer - read print_dir_blocks");
      exit (-1);
    }
    
    while( next_dir_entry < EXT3_BLOCK_SIZE(sb) && dir_entry.inode != 0 )
      {
	/* on ajoute une ligne dans l'arbre contenant les info de dir_entry */
	c=0;
	bzero( file_name, sizeof(file_name) );
	bzero( inode_num, sizeof(inode_num) );
	bzero( rec_len, sizeof(rec_len) );
	bzero( name_len, sizeof(name_len) );
	bzero( tmp, sizeof(tmp) );
	weight = 400;

	while ( c != dir_entry.name_len && c <= 25 ) {
	  /* not a printf cause that provoke display bugs ... */
	  //putchar(dir_entry.name[c]);
	  //snprintf( file_name, 1, "%s%c", file_name, dir_entry.name[c]);
	  sprintf( tmp, "%c", dir_entry.name[c] );
	  strcat( file_name, tmp );
	  c++;
	}
	

	if ( c != dir_entry.name_len ) {
	  while ( c < 30 ) {
	    if ( c < 27 )
	      //putchar(' ');
	      strcat( file_name, " "); 
	    else
	      //putchar('.');
	      strcat( file_name, "." );
	    c++;
	  }
	}
	else {
	  while ( c < 30 ) {
	    //putchar(' ');
	    strcat( file_name, " ");
	    c++;
	  }
	}

	sprintf( inode_num, "%u", dir_entry.inode );
	sprintf( rec_len, "%hd", dir_entry.rec_len );
	sprintf( name_len, "%hd", dir_entry.name_len );
	
	/* storing file type in the dir_entry.imode field is not always
	 * implemented :( so we have to read each inode
	 */
	read_inode( fd, sb, dir_entry.inode, &i);

	if ( S_ISDIR(i.i_mode) ) {
	  // putchar('d');
	  tree.pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/directory.png", NULL);
	  weight=PANGO_WEIGHT_BOLD;
	  sprintf(color_rvb, "black");
	}
	else if ( S_ISCHR(i.i_mode) ) {
	  // putchar('c');
	  tree.pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/chardevice.png", NULL);
	  sprintf(color_rvb, "#f1006c");
	}
	else if ( S_ISBLK(i.i_mode) ) {
	  //  putchar('b');
	  tree.pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/blockdevice.png", NULL);
	  sprintf(color_rvb, "#e47a1d");
	}
	else if ( S_ISFIFO(i.i_mode) ) {
	  // putchar('p'); /* pipe file */
	  tree.pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/pipe.png", NULL);
	  sprintf(color_rvb,"#cfd100");
	}
	else if ( S_ISLNK(i.i_mode) ) {
	  //  putchar('l'); /* symlink */
	  tree.pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/symlink.png", NULL);
	  sprintf(color_rvb, "#05bc01");
	}
	else if ( S_ISSOCK(i.i_mode) ) {
	  //  putchar('s'); /* socket file */
	  tree.pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/socket.png", NULL);
	  sprintf(color_rvb, "#00ccce");
	}
	else if ( S_ISREG(i.i_mode) ) {
	  //  putchar('-');
	  sprintf(color_rvb, "#170089");
	  if (  (((i.i_mode&0700)>>6) & 01) == 01 ) {
	    //if the file is executable
	    tree.pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/exec.png", NULL);
	  }
	  else {
	    tree.pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/file.png", NULL);
	  }
	}
	else {
	  //	  putchar('?');
	  tree.pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/unknown.png", NULL);
	  sprintf(color_rvb, "red");
	}

	/* creation d'une nouvelle ligne enfant */
	gtk_tree_store_append( tree.treeStore, &tree.iter2, &clicked_dir
			       /*&tree.iter*/ );
	

	gtk_tree_store_set( tree.treeStore, &tree.iter2,
			    TYPE_PIX, tree.pixBuf,
			    FILE_NAME, file_name,
			    ROW_COLOR, color_rvb,
			    ROW_WEIGHT, weight, 
			    INODE_NUM, inode_num,
			    REC_LEN, rec_len,
			    NAME_LEN, name_len,
			    -1);

	g_object_unref(tree.pixBuf);
  
	next_dir_entry += dir_entry.rec_len; /* rec_len points to the next
						dir_entry */
	
	lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb)+
		      next_dir_entry, SEEK_SET);
	if ( lret <= 0 ) {
	  perror("ext3Viewer - lseek 3 print_dir_blocks");
	  exit (-1);
	}

	sret = read( fd, &dir_entry, sizeof( struct ext3_dir_entry_2 ) );
	if (sret == 0) {
	  perror("ext3Viewer - erreur de lecture de dir_entry "
		 "print_dir_blocks");
	  exit(1);
	}
      }
  }
  else {
    
    if ( block_num > 0 ) {
      DEBUG_PRINT("lecture du bloc index %u",block_num);
      
      lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb), SEEK_SET);
      if ( lret <= 0 ) {
	perror("ext3Viewer - lseek 4 print_dir_blocks");
	exit (-1);
      }
      
      sret = read ( fd, &iblock_num, 4 ); /*we read the block number of the
					    indexed block */
      if ( sret <= 0 ) {
	perror("ext3Viewer - read 2 print_dir_blocks");
	exit (-1);
      }
      while ( iblock_num != 0 && c < (EXT3_BLOCK_SIZE(sb)/4) )
	{ /* on parcours la table de blocks */
	  
	  /* pour chaque block on l'affiche ou on le parcours si c une table */
	  explore_dir_blocks ( fd, sb, iblock_num, rec_level-1, clicked_dir );
	  
	  c++;
	  lret = lseek( fd, ((off_t) block_num)*EXT3_BLOCK_SIZE(sb)+(c*4),SEEK_SET);
	  if ( lret <= 0 ) {
	    perror("ext3Viewer - lseek 5 print_dir_blocks");
	    exit (-1);
	  }
	  /* on se deplace de 4 en 4octets car c'est la taille de u_32 sur
	     laquelle on stoque un numero de block */
	  sret = read ( fd, &iblock_num, 4 );
	  if ( sret <= 0 ) {
	    perror("ext3Viewer - read 3 print_dir_blocks");
	    exit (-1);
	  }
	  
	}
      
    }
    
  }
  

}


void on_row_double_click(GtkTreeView *treeview, GtkTreePath *path,
			 GtkTreeViewColumn  *col, gpointer userdata) {
  
    GtkTreeModel *model;
    GtkTreeIter   iter;
    char test[4];
    gchar *name;
    gchar *inode_num;
    //    g_print ("A row has been double-clicked!\n");
    struct ext3_inode i;

    model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    
    if (gtk_tree_model_get_iter(model, &iter, path))
    {

       gtk_tree_model_get(model, &iter, FILE_NAME, &name, -1);
       gtk_tree_model_get(model, &iter, INODE_NUM, &inode_num, -1);
       snprintf ( test, 3, "%s", name );

       //       g_print ("Double-clicked row contains name %s\n", name );

       if ( strcmp( test, ".." )>0 && strcmp( test, "." )>0 ) {
	 /* on affiche pas le contenu des repertoire . et .. */
	 read_inode( fs.fd, &fs.sb, strtoull(inode_num, NULL, 10), &i);

	 if ( S_ISDIR(i.i_mode) ) {  
	   /* on verifie que l'inode selectionne est bien un dossier 
	    *  avant de le parcourir
	    */
	   if ( gtk_tree_view_row_expanded ( treeview, path ) == TRUE ) {
	     /* si deja deplie alors on replie */
	     gtk_tree_view_collapse_row ( treeview, path );
	   }
	   else {
	     if (gtk_tree_view_expand_row( treeview, path, FALSE ) == FALSE) {
	       /* si le dossier n'as pas de fils alors on le parcours et on 
		*  le deplie
		*/
	       explore_dir ( fs.fd, &fs.sb, strtoull(inode_num, NULL, 10), 
			     iter);
	       gtk_tree_view_expand_row ( treeview, path, FALSE );
	     }
	   }
	   
	 }

       }
       g_free(name);
       g_free(inode_num);

    }
}


