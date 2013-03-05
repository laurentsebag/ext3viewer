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



#ifndef LANG_H
#define LANG_H


/*Formatting constants*/
#define A_LINE "+------------------------------------------------------------------------------+\n"
#define A_LCOL "\n|"

#define HELP_TXT "Usage: %s <fd>  \n\
               [ -stats   | -stat | -printgd | -imap | -bmap | -ffi   |\n\
                 -ffb     | -cat  | -bcat    | -ls   | -find | -iname |\n\
                 -backups | -jbd  | -bpoint  | -tree ]\n\
'%s -help' for help.\n\
version: ext3Viewer 1.0.2\n"

#define HELP_TXT_VERBOSE "Usage: %s <FILESYSTEM> OPTION ARG1 [ ARG2 ... ]\n\n\
 Dumps and interprets informations about ext3 filesystems.\n\
 (in all this help, is the ext3 filesystem. ex: /dev/hda2)\n\n\
\
 <FILESYSTEM> is the ext3 filesystem to examinate, could be a device or\n\
                 a image file. eg: /dev/hda2, /dev/sda3, ./image.ext3\n\n\
\
 OPTION and ARGS are shown below :\n\
\
\t-stats [ <block num> <block size> ]\n\
\t                           info about the superblock and the group\n\
\t                           descriptors. if you specify a block number\n\
\t                           and a block size, it will read an alternate \n\
\t                           superblock. see -backups\n\n\
\t-stat  <inode num>         show the inode structure contents\n\
\t-bpoint  <block num>       display a block as a pointer of blocks\n\
\t-printgd  <group num>      dump the group_desc structure of a group\n\
\n\n\
\t-tree  <inode num>         show the form of an inode block allocation\n\
\t-imap  <group_desc num>    print the inode bitmap as an array\n\
\t-bmap  <group_desc num>    print the block bitmap as an array\n\
\t-ffi                       find the first free inode thanks to the\n\
\t                           bitmap\n\
\t-ffb                       find the first free block\n\
\n\n\
\t-cat  <inode num>  [ -x ]  dump the contents of a file.(read all the\n\
\t                           blocks). -x for hexa.\n\
\t-bcat  <block num> [ -x ]  dump the contents of a block.(read just one\n\
\t                           block) -x for hexa.\n\
\n\n\
\t-ls  <inode num>           print the directory entries of an inode.\n\
\t                           the root has inode number 2.\n\
\n\
\t-find  <inode_dir starting number> <regex pattern>\n\
\t                           find files recursively from a given directory\n\
\t                           that matches a regex pattern. for / type 2\n\n\
\t-iname  <inode num>        find all the files that have the given inode\n\
\n\
\t-backups                   calculate where the copies of the\n\
\t                           superblock are\n\
\n\n\
\t-jbd                       dump the journal contents.\n\
\n\n\
\t-help                      display this help. see also the man page\n\n\
 See the man page for more details.\n\n\
ext3Viewer 1.0.2 GNU public license\n\
Copyright (C) 2013 Laurent Sebag\n\
Copyright (C) 2007 Laurent Sebag & Nathan Periana\n\
e2view (2003) Julien Poitrat, original idea\n"

#endif
