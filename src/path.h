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



#ifndef PATH_H
#define PATH_H

#include "linux/ext3_fs.h"
#include <stdio.h>
#include <string.h>

struct path_filo
{
  char name[EXT3_NAME_LEN];
  __u32 inode;
  struct path_filo *prev;
} path_filo ;

int push ( struct path_filo **p, const char * name, __u32 inode );
int remove_top(struct path_filo **p);
void free_path(struct path_filo **p);
void view(const struct path_filo *p);
void fill_path_name( struct path_filo *p, char *string );

#endif
