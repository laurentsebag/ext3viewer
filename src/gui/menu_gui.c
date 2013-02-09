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

#include "menu_gui.h"

void onAbout(GtkWidget *pWidget, gpointer data) {
  GtkWidget *pAbout;
  GdkPixbuf *pixBuf;
  
  const gchar *authors[4] = { "Laurent Sebag - laurentsebag@free.fr\n\t- author of ext3Viewer GUI\n\t- co-author of ext3Viewer",
    "Nathan Periana - nathan.periana@yahoo.com\n\t- co-author of ext3Viewer",
    "\nThanks to Julien Poitrat for the original idea", NULL };
  
  pAbout = gtk_about_dialog_new();
  pixBuf = gdk_pixbuf_new_from_file( IMG_PATH "img/icon.png", NULL );

  gtk_about_dialog_set_name( GTK_ABOUT_DIALOG(pAbout), ABOUT_NAME );
  gtk_about_dialog_set_version( GTK_ABOUT_DIALOG(pAbout), VERSION );
  gtk_about_dialog_set_copyright( GTK_ABOUT_DIALOG(pAbout), COPYRIGHT );
  gtk_about_dialog_set_comments( GTK_ABOUT_DIALOG(pAbout), COMMENTS );
  gtk_about_dialog_set_license( GTK_ABOUT_DIALOG(pAbout), LICENSE );
//  gtk_about_dialog_set_wrap_license( GTK_ABOUT_DIALOG(pAbout), TRUE );
  gtk_about_dialog_set_website( GTK_ABOUT_DIALOG(pAbout), WEBSITE );
  gtk_about_dialog_set_website_label( GTK_ABOUT_DIALOG(pAbout), WEBSITE_LABEL );
  gtk_about_dialog_set_authors( GTK_ABOUT_DIALOG(pAbout), authors );
//  gtk_about_dialog_set_artists( GTK_ABOUT_DIALOG(pAbout), artists );
//  gtk_about_dialog_set_documenters( GTK_ABOUT_DIALOG(pAbout), documentors );
// gtk_about_dialog_set_translator_credits( GTK_ABOUT_DIALOG(pAbout), translators );
  gtk_about_dialog_set_logo  ( GTK_ABOUT_DIALOG(pAbout), pixBuf );

  


  gtk_dialog_run(GTK_DIALOG(pAbout));

  gtk_widget_destroy(pAbout);

  g_object_unref(pixBuf);
}


void onOpenBtn(GtkWidget *pWidget, gpointer data) {
  
  GtkWidget *fileS;
  GtkWidget *parent;
  gchar *path;
  GtkWidget *question;
  
  parent = GTK_WIDGET(data);

  if ( fs.isopen ) {
    /* si le systeme de fichier est deja ouvert on en ouvre pas d'autre */
    
    question = gtk_message_dialog_new( GTK_WINDOW(parent), 
				       GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION,
				       GTK_BUTTONS_YES_NO,
				       "You cannot open multiple filesystems at once.\n\nDo you want to close the current filesystem to open an other one ?");
//		       "Vous ne pouvez pas ouvrir plusieurs systèmes de fichiers à la fois. Voulez vous fermer celui en cours pour en ouvrir un nouveau ?");
    
    switch ( gtk_dialog_run( GTK_DIALOG(question) ) ) {
    case GTK_RESPONSE_YES:
      gtk_widget_destroy( question );
      onClose ( pWidget, data );
      break;
    case GTK_RESPONSE_NO:
      gtk_widget_destroy( question );
      return;
      break;
    }
    
  }
  
    
    /* Creation de la fenetre de selection */
    fileS = gtk_file_chooser_dialog_new("Please choose an ext3 filesystem",
					GTK_WINDOW(parent),
					GTK_FILE_CHOOSER_ACTION_OPEN,
					GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_OK,
					NULL);
    /* On limite les actions a cette fenetre */
    gtk_window_set_modal( GTK_WINDOW(fileS), TRUE );
    
    switch( gtk_dialog_run(GTK_DIALOG(fileS)) )
      {
      case GTK_RESPONSE_OK:
	/* Recuperation du chemin */
	path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileS));
	gtk_widget_destroy(fileS);
	open_file( path );
	g_free(path);
	break;
      default:
	gtk_widget_destroy(fileS);
	break;
      }
  
}


GtkWidget *createMenu ( GtkWidget *window ) {

  GtkWidget *menuBar;
  GtkWidget *menu;
  GtkWidget *submenu;
  GtkWidget *menuItem;
  GtkWidget *image;
  GdkPixbuf *pixBuf;
  menuBar = gtk_menu_bar_new();
  
  /***********************************
   *     create the "File" menu      *
   ***********************************/
  menu = gtk_menu_new();
  /* permet de detacher le menu de la fenetre */
  // menuItem = gtk_tearoff_menu_item_new();
  //gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);  

  /* bouton ouvrir */
  menuItem = gtk_image_menu_item_new_with_mnemonic("_Open");
  pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/open.png", NULL);
  image = gtk_image_new_from_pixbuf ( pixBuf );
  g_signal_connect(G_OBJECT(menuItem),"activate",G_CALLBACK(onOpenBtn),
		   NULL);
  gtk_image_menu_item_set_image ( GTK_IMAGE_MENU_ITEM(menuItem), image );
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
  g_object_unref(pixBuf);


  /* bouton fermer le systeme de fichier */
  menuItem = gtk_image_menu_item_new_with_mnemonic("Close");
  pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/close.png", NULL);
  image = gtk_image_new_from_pixbuf ( pixBuf );
  gtk_image_menu_item_set_image ( GTK_IMAGE_MENU_ITEM(menuItem), image );
  g_signal_connect(G_OBJECT(menuItem),"activate",G_CALLBACK(onClose),
		   NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);  
  g_object_unref(pixBuf);


  menuItem = gtk_image_menu_item_new_with_mnemonic("_Exit");
  pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/exit.png", NULL);
  image = gtk_image_new_from_pixbuf ( pixBuf );
  gtk_image_menu_item_set_image ( GTK_IMAGE_MENU_ITEM(menuItem), image );
  g_signal_connect(G_OBJECT(menuItem),"activate",G_CALLBACK(onQuit),
		   NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);  
  g_object_unref(pixBuf);


  /* ajout de Fichier dans "titre du menu" */
  menuItem = gtk_menu_item_new_with_mnemonic("_File");
  gtk_container_set_border_width ( GTK_CONTAINER(menuItem), 3 ); 
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), menuItem);

  /*******************************
   *   create the "Tools" menu   *
   *******************************/
  menu = gtk_menu_new();
  menuItem = gtk_tearoff_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);  


  /* create the "superblock" sub-menu */
  submenu = gtk_menu_new();
  menuItem = gtk_image_menu_item_new_with_label("Show superblock structure");
  g_signal_connect(menuItem, "activate", (GCallback) print_superblock_gui, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);
  menuItem = gtk_image_menu_item_new_with_label("Show superblock backups");
  g_signal_connect(menuItem, "activate", G_CALLBACK(print_superblock_backups), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);

  menuItem = gtk_image_menu_item_new_with_mnemonic("_Superblock");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), submenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
  

  /* create the "Group desc" sub-menu */
  submenu = gtk_menu_new();
  menuItem = gtk_image_menu_item_new_with_label("Show the group descriptors");
  g_signal_connect(menuItem, "activate", G_CALLBACK(print_group_desc), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);

  menuItem = gtk_image_menu_item_new_with_mnemonic("_Group descriptors");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), submenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);


  /* create the "bitmaps" sub-menu */
  submenu = gtk_menu_new();
  menuItem = gtk_image_menu_item_new_with_label("Show the inode bitmap");
  g_signal_connect(menuItem, "activate", (GCallback) print_imap, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);
  menuItem = gtk_image_menu_item_new_with_label("Find the first free inode");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);
  g_signal_connect(menuItem, "activate", (GCallback) print_ffi, NULL);
  menuItem = gtk_image_menu_item_new_with_label("Show the block bitmap");
  g_signal_connect(menuItem, "activate", (GCallback) print_bmap, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);
  menuItem = gtk_image_menu_item_new_with_label("Find the first free block");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);
  g_signal_connect(menuItem, "activate", (GCallback) print_ffb, NULL);
  menuItem = gtk_image_menu_item_new_with_mnemonic("Bit_maps");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), submenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);


  /* creat the "Inode" sub-menu */
  submenu = gtk_menu_new();
  menuItem = gtk_image_menu_item_new_with_label("Print the selected inode's structure");
  g_signal_connect(menuItem, "activate", (GCallback) popup_menu_onStat, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);

  /*menuItem = gtk_image_menu_item_new_with_label("Print the inode's block allocation tree");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);
  menuItem = gtk_image_menu_item_new_with_label("Show the inode's contents");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);
  menuItem = gtk_image_menu_item_new_with_label("Show the inode's contents in hexadecimal");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);
*/

  menuItem = gtk_image_menu_item_new_with_label("Show a symlink contents");
  g_signal_connect(menuItem, "activate", (GCallback) popup_menu_onCatSymlink, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);


  menuItem = gtk_image_menu_item_new_with_mnemonic("_Inode");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), submenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);


  /* create the "Block" sub-menu */
  submenu = gtk_menu_new();
/*  menuItem = gtk_image_menu_item_new_with_label("Show a block's contents");
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);*/
  menuItem = gtk_image_menu_item_new_with_label("Show a block's contents");
  g_signal_connect(menuItem, "activate", (GCallback) print_block_hexa_gui, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);
  menuItem = gtk_image_menu_item_new_with_label("Show a block as a block pointer");
  g_signal_connect(menuItem, "activate", (GCallback) print_block_as_bpointer_gui, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);
  menuItem = gtk_image_menu_item_new_with_mnemonic("_Block");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), submenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);

  /* creat the "Journal" sub-menu */
  submenu = gtk_menu_new();
  menuItem = gtk_image_menu_item_new_with_label("Dump the internal journal");
  g_signal_connect(menuItem, "activate", (GCallback) print_journal, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuItem);

  menuItem = gtk_image_menu_item_new_with_mnemonic("_Journal");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), submenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);

 

  menuItem = gtk_menu_item_new_with_mnemonic("_Tools");
  gtk_container_set_border_width ( GTK_CONTAINER(menuItem), 3 ); 

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), menuItem);

  /******************************
   *   create the "Help" menu   *
   ******************************/
  menu = gtk_menu_new();
/*  menuItem = gtk_image_menu_item_new_with_mnemonic("_Help Contents");
  pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/help.png", NULL);
  image = gtk_image_new_from_pixbuf ( pixBuf );
  gtk_image_menu_item_set_image ( GTK_IMAGE_MENU_ITEM(menuItem), image );
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
  g_object_unref(pixBuf);
*/
  menuItem = gtk_image_menu_item_new_with_mnemonic("_About ext3Viewer GUI");
  //gtk_menu_item_new_with_label("A propos");
  pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/about.png", NULL);
  image = gtk_image_new_from_pixbuf ( pixBuf );
  gtk_image_menu_item_set_image ( GTK_IMAGE_MENU_ITEM(menuItem), image );
  g_signal_connect(G_OBJECT(menuItem),"activate",G_CALLBACK(onAbout),
		   (GtkWidget*) window);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
  g_object_unref(pixBuf);

  menuItem = gtk_menu_item_new_with_mnemonic("_Help");
  gtk_container_set_border_width ( GTK_CONTAINER(menuItem), 3 ); 
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), menuItem);

  return menuBar;

}
