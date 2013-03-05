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



#include <gtk/gtk.h>
#include <string.h>

#include "objects_gui.h"
#include "open_gui.h"
#include "lang.h"

extern struct Window wMain;
extern struct CadreCentre centre;
extern struct fileTree tree;
struct filesystem fs;

void open_file( gchar *path )
{

  int ret;

  fs.fd = open_fs ( (char*)path );

  if ( fs.fd < 0 ) {
    open_error_dialog();
    // close_fs( fd );
    return;
  }


  ret = read_superblock_gui( fs.fd, &fs.sb );
  if ( ret == -1 ) {
    // Error: bad superblock
    superblock_error_dialog();
    fs.isopen = 0;
    close(fs.fd);
  }
  else if ( ret == 1 ) {
    // The filesystem is mounted
    ret = superblock_warning_dialog();
    if ( ret == -1 ) {
      // User doesn't want to continue
      fs.isopen = 0;
      close_fs( fs.fd );
    }
    else {
      fs.isopen = 1;
      init_tree();
      explore_dir ( fs.fd, &fs.sb, 2, tree.iter);

    }
  }
  else {

    fs.isopen = 1;
    init_tree();

    explore_dir ( fs.fd, &fs.sb, 2, tree.iter );
  }
}

void open_error_dialog() {
  GtkWidget *error;

  error = gtk_message_dialog_new( GTK_WINDOW(wMain.window),
      GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
      GTK_BUTTONS_OK,
      OPEN_ERR_TXT);

  gtk_dialog_run( GTK_DIALOG(error));
  gtk_widget_destroy( error );

}

void superblock_error_dialog() {
  GtkWidget *error;

  error = gtk_message_dialog_new( GTK_WINDOW(wMain.window),
      GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
      GTK_BUTTONS_OK,
      NON_EXT_ERR_TXT);

  gtk_dialog_run( GTK_DIALOG(error));
  gtk_widget_destroy( error );
}

int superblock_warning_dialog() {

  GtkWidget *question;

  question = gtk_message_dialog_new( GTK_WINDOW(wMain.window),
      GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
      GTK_BUTTONS_YES_NO,
      MOUNT_WARNING_TXT);

  switch ( gtk_dialog_run( GTK_DIALOG(question) ) ) {
    case GTK_RESPONSE_YES:
      gtk_widget_destroy( question );
      return 0;
      break;
    case GTK_RESPONSE_NO:
      gtk_widget_destroy( question );
      return -1;
      break;
  }
  return 0;
}


void show_warning_dialog( char *string ) {
  GtkWidget *error;

  error = gtk_message_dialog_new( GTK_WINDOW(wMain.window),
      GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING,
      GTK_BUTTONS_OK,
      string);

  gtk_dialog_run( GTK_DIALOG(error));
  gtk_widget_destroy( error );
}

