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



#ifndef MENU_GUI_H
#define MENU_GUI_H

#include "open_gui.h"
#include "close_gui.h"
#include "output_gui.h"
#include "release.h"


void onAPropos(GtkWidget *pWidget, gpointer pData);
void onOpenBtn(GtkWidget *pWidget, gpointer pData);

/* cree le menu de navigation et le connecte a la fenetre window */
GtkWidget *createMenu( GtkWidget *window ); 

extern struct filesystem fs;

#endif
