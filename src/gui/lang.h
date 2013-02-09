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



#ifdef LANG_FR

#define OPEN_ERR_TXT "Erreur d'ouverture du système de fichier !\n\nCe fichier n'existe pas ou vous n'avez pas les droits de l'ouvrir.\n\n(vous devez être root pour ouvrir /dev/hdXX)"

#define NON_EXT_ERR_TXT "Erreur d'ouverture du système de fichier !\n\nLe système de fichier n'est pas un système de fichier ext3."

#define MOUNT_WARNING_TXT "Ce systeme de fichier est monté ou corrompu. Demontez le ou bien réparez le avant d'utiliser ext3Viewer. L'utilisation d'ext3Viewer sur une partition montée est déconseillée. Voulez continuer ?"

#define FILE_NAME_TXT "Nom de fichier"

#else


#define OPEN_ERR_TXT "Filesystem opening error !\n\nThis file does not exist or you are not allowed to open it.\n\n(you must be root powered to open /dev/hdXX)"

#define NON_EXT_ERR_TXT "Filesystem opening error !\n\nThis filesystem is not a valid ext3. (Bad superblock)"

#define MOUNT_WARNING_TXT "This filesystem is mounted or corrupted. Please umount it or fsck it before using ext3Viewer. Using ext3Viewer on mounted partitions is not recommended. Continue anyway ?"

#define FILE_NAME_TXT "filename"
#endif
