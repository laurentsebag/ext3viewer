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


#include "path.h"

#ifdef EFENCE
#include <efence.h>
#endif

int push ( struct path_filo **p, const char * name, __u32 inode ) { 

  struct path_filo *element = malloc(sizeof(path_filo));
  if(!element) {
    perror("path_filo");     /* Si l'allocation a echouee. */
    return -1;
  }
  strcpy ( element->name, name );
  element->inode = inode;
  element->prev = *p;
  *p = element;       /* Le pointeur pointe sur le dernier element. */
  return 0;
}

int remove_top(struct path_filo **p)
{
  struct path_filo *tmp;
  if( *p == NULL ) 
    return -1;     /* Retourne -1 si la pile est vide. */
  tmp = (*p)->prev;
  free(*p);
  *p = tmp;       /* Le pointeur pointe sur le dernier element. */
  return 0;
}

void free_path(struct path_filo **p)
{
  struct path_filo *tmp;
  while( *p != NULL )
    {
      tmp = (*p)->prev;
      free(*p);
      *p = tmp;
    }
}

void view(const struct path_filo *p)
{
  if ( p != NULL ) {
    view ( p->prev) ;
  printf("%s %d ",p->name, p->inode);

  }
}

void fill_path_name( struct path_filo *p, char *string )
{
  if ( p != NULL ) {
    fill_path_name ( p->prev, string );
    strcat ( string, "/");
    strcat ( string, p->name );

  }
}
