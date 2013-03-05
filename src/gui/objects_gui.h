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



#ifndef OBJECTS_GUI_H
#define OBJECTS_GUI_H

#include <unistd.h>
#include "../linux/ext3_fs.h"



struct Window {
  GtkWidget *window;
  GdkPixbuf *icon;
  GtkWidget *hPaned;
  GtkWidget *vBox;
  GtkWidget *hBox;
  GtkWidget *menuBar;
  GtkWidget *menu;
  GtkWidget *menuItem;
  GtkWidget *separator;
  GtkWidget *img;
  GtkWidget *statusBar;
  GtkWidget *frame;
};

struct CadreCentre {
  GtkWidget *texte;
  GtkWidget *scrollWin;
  GtkWidget *fileTree;
};

struct filesystem {
  int fd;
  int isopen;
  struct ext3_super_block sb;
};



#endif
