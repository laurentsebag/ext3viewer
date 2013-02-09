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
#include <gtk/gtk.h>
#include <libintl.h>


#include "objects_gui.h"
#include "output_gui.h"
#include "close_gui.h"
#include "menu_gui.h"
#include "tree_gui.h"
#include "search_gui.h"
struct Window wMain;
struct CadreCentre centre;

extern struct filesystem fs;

int main ( int argc, char *argv[] )
{

  struct SearchBar search;

  gtk_init ( &argc, &argv );

  /* creation de la fenetre principale */
  wMain.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(wMain.window), GTK_WIN_POS_NONE);
  gtk_window_set_default_size(GTK_WINDOW(wMain.window), 1024, 600);
  gtk_window_set_title(GTK_WINDOW(wMain.window), "ext3Viewer GUI");
  //gtk_container_set_border_width (GTK_CONTAINER (wMain.window), 3);
  /* on associe un icon a notre application */
  wMain.icon = gdk_pixbuf_new_from_file( IMG_PATH "img/icon.png", NULL);
  //gtk_window_set_icon(GTK_WINDOW(wMain.window), wMain.icon);
  gtk_window_set_default_icon(wMain.icon);

  g_object_unref(wMain.icon);

  /* traitement de l'action fermer la fenetre avec croix */
  g_signal_connect(G_OBJECT(wMain.window), "destroy", 
		   G_CALLBACK(onQuit), NULL);


  /* Creation de la GtkVBox */
  wMain.vBox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(wMain.window), wMain.vBox);


  /* creation du menu */
  wMain.menuBar = createMenu( wMain.window );

  /* Ajout des elements dans la vBox */
  gtk_box_pack_start(GTK_BOX(wMain.vBox), wMain.menuBar, FALSE, FALSE, 0);

  wMain.separator = gtk_hseparator_new(); 
  gtk_box_pack_start(GTK_BOX(wMain.vBox), wMain.separator, FALSE, FALSE, 0);


  wMain.frame = create_search( &search );

  gtk_box_pack_start(GTK_BOX(wMain.vBox), wMain.frame, FALSE, FALSE, 0);


  //  wMain.separator = gtk_hseparator_new(); 
  //gtk_box_pack_start(GTK_BOX(wMain.vBox), wMain.separator, FALSE, FALSE, 0);

 
  /* Creation de la separation avec zones etirables pour navig+centre */
  wMain.hPaned = gtk_hpaned_new ();
  /* defini la position de la separation */
  gtk_paned_set_position ( GTK_PANED(wMain.hPaned), 470 );
  
  gtk_box_pack_start(GTK_BOX(wMain.vBox), wMain.hPaned, TRUE, TRUE, 0);

  /* creation de la partie gauche de navigation */
  centre.fileTree = create_tree();
  
  /* on cree une fenetre redimensionable dans laquelle on met l'arbre
     de navigation */
  
  centre.scrollWin = gtk_scrolled_window_new( NULL, NULL );
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(centre.scrollWin),
				 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(centre.scrollWin),
					centre.fileTree);
  
  /*
  wMain.hBox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(wMain.hBox), wMain.fileTree, TRUE, TRUE, 0);

  wMain.separator = gtk_vseparator_new();
  gtk_box_pack_start(GTK_BOX(wMain.hBox), wMain.separator, FALSE, FALSE, 0);
  */

  gtk_paned_add1 (GTK_PANED (wMain.hPaned), centre.scrollWin);

  /* creation de la partie centrale */
  //centre.texte = gtk_label_new(""); 
  //  gtk_label_set_justify( GTK_LABEL(centre.texte), GTK_JUSTIFY_RIGHT );
  //  gtk_misc_set_alignment (GTK_MISC (centre.texte), 0, 0);
  //  gtk_misc_set_padding( GTK_MISC( centre.texte ), 30, 0);

  GtkTextBuffer *buffer;

  centre.texte = gtk_text_view_new ();
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (centre.texte));

  //  gtk_text_buffer_set_text (buffer, "Hello, this is some text", -1);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(centre.texte), GTK_WRAP_WORD);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(centre.texte), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(centre.texte), FALSE);
  gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(centre.texte), 5);
  gtk_text_view_set_left_margin (GTK_TEXT_VIEW(centre.texte), 10);
  gtk_text_view_set_right_margin (GTK_TEXT_VIEW(centre.texte), 10);

  init_tags ( buffer );


  /* creation de la vue avec scrollbar */
  centre.scrollWin = gtk_scrolled_window_new(NULL, NULL);

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(centre.scrollWin),
				 GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  /* on ajoute le texte dans la fenetre scrollbar */
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(centre.scrollWin),
					centre.texte);
  
  // wMain.vBox = gtk_vbox_new(FALSE, 0);
  // gtk_box_pack_start(GTK_BOX(wMain.vBox), centre.scrollWin, TRUE, TRUE, 0);
  //gtk_box_pack_start(GTK_BOX(wMain.vBox), view, TRUE, TRUE, 0);
    
  
  /* on place la fenetre scrollbar dans la partie redimensionnable */
  gtk_paned_add2 (GTK_PANED (wMain.hPaned), centre.scrollWin);

  /* ajout d'une barre de statut tout en bas, il sera possible par la suite
   * d'y ajouter les instructions possibles lors du survol a la souris
   */
  wMain.statusBar = gtk_statusbar_new ();      
  gtk_box_pack_start(GTK_BOX(wMain.vBox), wMain.statusBar, FALSE, FALSE, 0);


  gtk_widget_show_all(wMain.window);

  fs.isopen = 0;

  gtk_main();

  //printf("Bye!\n");
  
  return EXIT_SUCCESS;
}



