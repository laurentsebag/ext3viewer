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



#ifndef TREE_GUI_H
#define TREE_GUI_H

#include <sys/stat.h>
#include <linux/ext3_fs.h>
#include <string.h>
#include <stdlib.h>


#include "objects_gui.h"
#include "tree_menu_gui.h"
#include "lang.h"

/* sert a definir les colones de l'arbre */
enum {
  ROW_COLOR,
  ROW_WEIGHT, 
  TYPE_PIX,
  FILE_NAME,
  INODE_NUM,
  REC_LEN,
  NAME_LEN,
  N_COLUMN
};

struct fileTree {
  GtkWidget *treeView;
  GtkTreeStore *treeStore;
  GtkTreeViewColumn *column;
  GtkCellRenderer *cellRenderer;
  GdkPixbuf *pixBuf;
  GtkTreeIter iter;
  GtkTreeIter iter2;
  GtkTreePath *path;
  gint index;
  // rajouter socket pipe symlink char block ...
};



GtkWidget* create_tree(); /* on cree l'arbre et sa structure */
void init_tree(); /*on y met les premieres valeurs : contenu de */
void explore_dir ( int fd, struct ext3_super_block *sb, __u32 i_num, 
		   GtkTreeIter clicked_dir );
void explore_dir_blocks ( int fd, struct ext3_super_block *sb,
			  __u32 block_num, short rec_level,
			  GtkTreeIter clicked_dir );
void on_row_double_click(GtkTreeView *treeview, GtkTreePath *path,
			 GtkTreeViewColumn  *col, gpointer userdata);
gboolean onButtonPressed (GtkWidget *treeview, GdkEventButton *event,
			       gpointer userdata);

//extern gboolean onPopupMenu (GtkWidget *treeview, gpointer userdata);
/* definit dans tree_menu_gui.h */

#endif
