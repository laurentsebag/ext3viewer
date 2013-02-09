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
#include "close_gui.h"

extern struct filesystem fs;
extern struct CadreCentre centre;
extern struct fileTree tree;

void onQuit(GtkWidget *pWidget, gpointer pData) {
  if ( fs.isopen ) {
    close_fs(fs.fd);
    //printf("fermeture du systeme de fichier\n");
  }
  gtk_main_quit();
}

void onClose( GtkWidget *widget, gpointer data) {

  GtkTextBuffer *buffer;
  GtkTextIter start, end;
  //  GtkTreePath *path;

  if ( fs.isopen ) {
    //printf("fermeture du systeme de fichier et de l'arbre\n");
    /* on recupere le buffer */
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));
    
    /* on vide la zone de texte */
    gtk_text_buffer_get_bounds ( buffer, &start, &end);
    gtk_text_buffer_delete ( buffer, &start, &end );
    
    /* on vide l'arbre */
    gtk_tree_store_clear ( tree.treeStore );
    fs.isopen = 0;
    close_fs(fs.fd);
  }

}
