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

#include "tree_menu_gui.h"

extern struct filesystem fs;
extern struct fileTree tree;
void popup_menu_onStat ( GtkWidget *menuitem, gpointer userdata) {
  /* on a passe la ligne selectionnee par userdata lors du connect signal */

  //  GtkTreeView *treeview = GTK_TREE_VIEW(userdata);

  GtkTreeModel *model;
  GtkTreeIter   iter;
  GtkTreeSelection *selection;
  gchar *inode;
  struct ext3_inode i;  

  if ( userdata == NULL ) {
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree.treeView));
  }
  else {
    selection = GTK_TREE_SELECTION(userdata);
  }

  if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
      gtk_tree_model_get (model, &iter, INODE_NUM, &inode, -1);
      
/*      g_print ("You selected inode #%lu to show its structure\n",
	       strtoul(inode, NULL, 10));
*/
      read_inode( fs.fd, &fs.sb, strtoul(inode, NULL, 10), &i );

      print_inode_gui ( fs.fd, &fs.sb, &i, strtoul(inode, NULL, 10) );
      
      g_free (inode);
    }
}

void popup_menu_onCatSymlink ( GtkWidget *menuitem, gpointer userdata) {
  
  GtkTreeModel *model;
  GtkTreeIter   iter;
  GtkTreeSelection *selection;
  gchar *inode;
  struct ext3_inode i;  

  if ( userdata == NULL ) {
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree.treeView));
  }
  else {
    selection = GTK_TREE_SELECTION(userdata);
  }

  if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
      gtk_tree_model_get (model, &iter, INODE_NUM, &inode, -1);
      
      read_inode( fs.fd, &fs.sb, strtoul(inode, NULL, 10), &i );

      print_symlink_gui ( fs.fd, &fs.sb, strtoul(inode, NULL, 10) );
      
      g_free (inode);
    }

}

void popup_menu_onTree ( GtkWidget *menuitem, gpointer userdata) {
  /* on a passe la ligne selectionnee par userdata lors du connect signal */

  //  GtkTreeView *treeview = GTK_TREE_VIEW(userdata);

  GtkTreeModel *model;
  GtkTreeIter   iter;
  GtkTreeSelection *selection = GTK_TREE_SELECTION(userdata);
  gchar *inode;
  
  if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
      gtk_tree_model_get (model, &iter, INODE_NUM, &inode, -1);
      
/*      g_print ("You selected inode #%lu to show its inode allocation\n",
	       strtoul(inode, NULL, 10));
  */    
      g_free (inode);
    }
}

void popup_menu (GtkWidget *treeview, GdkEventButton *event, 
		      gpointer userdata) {
  
  GtkWidget *menu, *submenu;
  GtkWidget  *menuitem;
  GtkTreeSelection *selection = GTK_TREE_SELECTION(userdata);
  GdkPixbuf *pixBuf;
  GtkWidget *image;
  GtkWidget *separator;

  menu = gtk_menu_new();

  //  g_object_set ( menu, "vertical-padding", 10, TRUE, NULL );
  gtk_container_set_border_width ( GTK_CONTAINER(menu), 2 ); 


  /***********************************
   *  create the "File" menu     *
   ***********************************/
  submenu = gtk_menu_new();
  /* bouton ouvrir */
  menuitem = gtk_image_menu_item_new_with_label("Open");
  pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/open.png", NULL);
  image = gtk_image_new_from_pixbuf ( pixBuf );
  g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(onOpenBtn),
		   NULL);
  gtk_image_menu_item_set_image ( GTK_IMAGE_MENU_ITEM(menuitem), image );
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
  g_object_unref(pixBuf);


  /* bouton fermer le systeme de fichier */
  menuitem = gtk_image_menu_item_new_with_label("Close");
  pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/close.png", NULL);
  image = gtk_image_new_from_pixbuf ( pixBuf );
  g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(onClose),
		   NULL);
  gtk_image_menu_item_set_image ( GTK_IMAGE_MENU_ITEM(menuitem), image );
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);  
  g_object_unref(pixBuf);


  menuitem = gtk_image_menu_item_new_with_label("Exit"); 
  pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/exit.png", NULL);
  image = gtk_image_new_from_pixbuf ( pixBuf );
  gtk_image_menu_item_set_image ( GTK_IMAGE_MENU_ITEM(menuitem), image );
  g_signal_connect(G_OBJECT(menuitem),"activate",G_CALLBACK(onQuit),
		   NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);  
  g_object_unref(pixBuf);


  /* ajout de Fichier dans "titre du menu" */
  menuitem = gtk_image_menu_item_new_with_label("File");
  pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/tree_menu/file.png", NULL);
  image = gtk_image_new_from_pixbuf ( pixBuf );
  gtk_image_menu_item_set_image ( GTK_IMAGE_MENU_ITEM(menuitem), image );
  //  gtk_container_set_border_width ( GTK_CONTAINER(menuitem), 3 ); 
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), submenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
  g_object_unref(pixBuf);


  separator = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), separator);

  /**********************************
   *      Create the inode menu     *
   **********************************/
  menuitem = gtk_image_menu_item_new_with_label("Show superblock structure");
  pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/tree_menu/superblock.png", NULL);
  image = gtk_image_new_from_pixbuf ( pixBuf );
  gtk_image_menu_item_set_image ( GTK_IMAGE_MENU_ITEM(menuitem), image );
  g_signal_connect(menuitem, "activate",
		   (GCallback) print_superblock_gui, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
  g_object_unref(pixBuf);
  
  menuitem = gtk_image_menu_item_new_with_label("Show group descriptors");
  pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/tree_menu/group_desc.png", NULL);
  image = gtk_image_new_from_pixbuf ( pixBuf );
    gtk_image_menu_item_set_image ( GTK_IMAGE_MENU_ITEM(menuitem), image );
  g_signal_connect(menuitem, "activate",
		   (GCallback) print_group_desc, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
  g_object_unref(pixBuf);


  separator = gtk_separator_menu_item_new(); 
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), separator); 


  menuitem = gtk_image_menu_item_new_with_label("Show the inode's structure");
  pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/tree_menu/stat.png", NULL);
  image = gtk_image_new_from_pixbuf ( pixBuf );
  gtk_image_menu_item_set_image ( GTK_IMAGE_MENU_ITEM(menuitem), image );
  g_signal_connect(menuitem, "activate",
		   (GCallback) popup_menu_onStat, selection);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
  g_object_unref(pixBuf);

  menuitem = gtk_image_menu_item_new_with_label("Show a block contents");
  pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/tree_menu/contents.png", NULL);
  image = gtk_image_new_from_pixbuf ( pixBuf );
    gtk_image_menu_item_set_image ( GTK_IMAGE_MENU_ITEM(menuitem), image );
  g_signal_connect(menuitem, "activate",
		   (GCallback) print_block_hexa_gui, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
  g_object_unref(pixBuf);
 
  
  menuitem = gtk_image_menu_item_new_with_label("Show a symlink contents");
  pixBuf = gdk_pixbuf_new_from_file(IMG_PATH "img/symlink.png", NULL);
  image = gtk_image_new_from_pixbuf ( pixBuf );
    gtk_image_menu_item_set_image ( GTK_IMAGE_MENU_ITEM(menuitem), image );
  g_signal_connect(menuitem, "activate",
		   (GCallback) popup_menu_onCatSymlink, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
  g_object_unref(pixBuf);
  
  gtk_widget_show_all(menu);
  
  /* Note: event can be NULL here when called from onPopupMenu;
   *  gdk_event_get_time() accepts a NULL argument */
  gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL,
		 (event != NULL) ? event->button : 0,
		 gdk_event_get_time((GdkEvent*)event));
}


gboolean onButtonPressed (GtkWidget *treeview, GdkEventButton *event,
			       gpointer userdata) {

  GtkTreeSelection *selection;
  GtkTreePath *path;
  
  if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) {
    /* si le bouton droit de la souris est clique */
    
    // g_print ("Single right click on the tree view.\n");
    
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    
    if (gtk_tree_selection_count_selected_rows(selection)  <= 1)
      {    
	/*  on retrouve le path pour la ligne selectionnee */
	if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
					  (gint) event->x, 
					  (gint) event->y,
					  &path, NULL, NULL, NULL)) {
	  gtk_tree_selection_unselect_all(selection);
	  gtk_tree_selection_select_path(selection, path);
	  gtk_tree_path_free(path);
	}
      }
	
    popup_menu(treeview, event, selection);
    
    return TRUE; /* we handled this */
    
  }
  
  return FALSE; /* we did not handle this */
}

gboolean onPopupMenu (GtkWidget *treeview, gpointer userdata) {
  
  popup_menu(treeview, NULL, userdata);
  
  return TRUE; /* we handled this */
}
