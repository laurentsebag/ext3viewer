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
#include "output_gui.h"

extern struct CadreCentre centre;
extern struct filesystem fs;
void init_tags ( GtkTextBuffer *buffer ) {

  /* Create tags associated with the buffer. */
  /* Tag with weight bold and tag name "bold" . */
  gtk_text_buffer_create_tag (buffer, "bold",
      "weight", PANGO_WEIGHT_BOLD,
      NULL);
  /* Tag with style italic and tag name "italic". */
  gtk_text_buffer_create_tag (buffer, "italic",
      "style", PANGO_STYLE_ITALIC,
      NULL);
  /* Tag with style italic&center and tag name "citalic". */
  gtk_text_buffer_create_tag (buffer, "citalic",
      "style", PANGO_STYLE_ITALIC,
      "justification", GTK_JUSTIFY_CENTER,
      "foreground", "#0f681b",
      NULL);
  /* Tag with font fixed and tag name "fixed". */
  gtk_text_buffer_create_tag (buffer, "fixed",
      "font", "fixed",
      "size", 9*PANGO_SCALE,
      NULL);
  /* Tag with big size and blue colored : title */
  gtk_text_buffer_create_tag (buffer, "title",
      "foreground", "blue",
      "size", 13*PANGO_SCALE,
      "justification", GTK_JUSTIFY_CENTER,
      NULL);
  /* Tag with weight bold centered and tag name "title2" . */
  gtk_text_buffer_create_tag (buffer, "title2",
      "weight", PANGO_WEIGHT_BOLD,
      "justification", GTK_JUSTIFY_CENTER,
      NULL);
  /* Tag for normal text just in case we'd like to change it */
  gtk_text_buffer_create_tag (buffer, "text",
      NULL);
}

int ask_group_number( char *string ) {

  unsigned int number = 0;

  GtkWidget *question;
  GtkWidget *parent;
  GtkWidget *dialog;
  GtkWidget *entry;
  int group_num_max;

  group_num_max = (fs.sb.s_blocks_count/EXT3_BLOCKS_PER_GROUP(&fs.sb));
  group_num_max = group_num_max>0?group_num_max-1:0;
  gchar *texte;

  texte = g_strdup_printf( "number between 0 and %d ?", group_num_max);

  parent = GTK_WIDGET(wMain.window);

  /* Create the widgets */

  dialog = gtk_dialog_new_with_buttons ("Message",
      GTK_WINDOW(wMain.window),
      GTK_DIALOG_MODAL,
      GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
      GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
      NULL);

  question = gtk_label_new (string);

  /* Add the label, and show everything we've added to the dialog. */

  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox),
      question);

  entry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(entry), texte);
  gtk_box_pack_start( GTK_BOX(GTK_DIALOG(dialog)->vbox), entry, TRUE, FALSE, 0);

  gtk_widget_show_all (dialog);

  switch ( gtk_dialog_run( GTK_DIALOG(dialog) ) )
  {
    case GTK_RESPONSE_ACCEPT:
      number = atoi( gtk_entry_get_text(GTK_ENTRY(entry)) );
      break;
    default:
      number = -1;
      break;
  }
  gtk_widget_destroy (dialog);

  /*
     switch ( gtk_dialog_run( GTK_DIALOG(question) ) ) {
     case GTK_RESPONSE_YES:
     gtk_widget_destroy( question );
     break;
     case GTK_RESPONSE_NO:
     gtk_widget_destroy( question );
     return;
     break;
     }
     */
  return number;
}


int ask_block_number( char *string ) {

  unsigned int number = 0;

  GtkWidget *question;
  GtkWidget *parent;
  GtkWidget *dialog;
  GtkWidget *entry;
  int group_num_max;

  group_num_max = (fs.sb.s_blocks_count/EXT3_BLOCKS_PER_GROUP(&fs.sb));
  group_num_max = group_num_max>0?group_num_max-1:0;
  gchar *texte;

  texte = g_strdup_printf( "block number ?");

  parent = GTK_WIDGET(wMain.window);

  /* Create the widgets */

  dialog = gtk_dialog_new_with_buttons ("Message",
      GTK_WINDOW(wMain.window),
      GTK_DIALOG_MODAL,
      GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
      GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
      NULL);

  question = gtk_label_new (string);

  /* Add the label, and show everything we've added to the dialog. */

  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox),
      question);

  entry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(entry), texte);
  gtk_box_pack_start( GTK_BOX(GTK_DIALOG(dialog)->vbox), entry, TRUE, FALSE, 0);

  gtk_widget_show_all (dialog);

  switch ( gtk_dialog_run( GTK_DIALOG(dialog) ) )
  {
    case GTK_RESPONSE_ACCEPT:
      number = atoi( gtk_entry_get_text(GTK_ENTRY(entry)) );
      break;
    default:
      number = -1;
      break;
  }
  gtk_widget_destroy (dialog);

  /*
     switch ( gtk_dialog_run( GTK_DIALOG(question) ) ) {
     case GTK_RESPONSE_YES:
     gtk_widget_destroy( question );
     break;
     case GTK_RESPONSE_NO:
     gtk_widget_destroy( question );
     return;
     break;
     }
     */
  return number;
}
void print_inode_gui ( int fd, const struct ext3_super_block *sb,
    const struct ext3_inode *in, __u32 inum )  {
  //  gtk_text_buffer_create_child_anchor ( buffer, &iter );

  GtkTextIter iter, start, end;
  GtkTextBuffer *buffer;
  gchar *texte;
  int i;


  // Get the buffer
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

  // Empty the text area
  gtk_text_buffer_get_bounds ( buffer, &start, &end);
  gtk_text_buffer_delete ( buffer, &start, &end );


  // Write in the text area
  gtk_text_buffer_get_end_iter ( buffer, &iter);

  //gtk_widget_show_all(wMain.window);

  texte = g_strdup_printf("ext3_inode structure of inode #%u\n\n", inum);
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
      -1, "title", NULL);
  free( texte );

  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Inodes store administrative informations about files, such as the file size, or the file name...\n\n", -1, "citalic", NULL);

  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "File mode :  ",
      -1, "bold", NULL);

  texte = g_strdup_printf("%ho\n", in->i_mode);
  // gtk_text_buffer_insert ( buffer, &iter, texte, -1 );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
      -1, "text", NULL);
  free( texte );


  //  print_filemode_gui( texte, in->i_mode );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "File type : ",
      -1, "bold", NULL);

  if ( S_ISDIR(in->i_mode) ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "directory\n",
        -1, "text", NULL);
  }
  else if ( S_ISCHR(in->i_mode) ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,
        "character device\n",
        -1, "text", NULL);
  }
  else if ( S_ISBLK(in->i_mode) ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"block device\n",
        -1, "text", NULL);
  }
  else if ( S_ISFIFO(in->i_mode) ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "FIFO file\n",
        -1, "text", NULL);
  }
  else if ( S_ISLNK(in->i_mode) ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"symbolic link\n",
        -1, "text", NULL);
  }
  else if ( S_ISSOCK(in->i_mode) ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"socket file\n" ,
        -1, "text", NULL);
  }
  else if ( S_ISREG(in->i_mode) ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "regular file\n",
        -1, "text", NULL);
  }
  else {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "unknown",
        -1, "text", NULL);
  }
  if(mask(in->i_mode, 0x800))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "this file is "
        "SUID\n", -1, "text", NULL);
  }
  if(mask(in->i_mode, 0x400))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "this file is"
        " SGID\n", -1, "text", NULL);
  }
  if(mask(in->i_mode, 0x200))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "this file has"
        " sticky bit enable\n",
        -1, "text", NULL);
  }
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Permissions : ",
      -1, "bold", NULL);

  g_strconcat_permissions( &texte, (in->i_mode&0700)>>6 );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
      -1, "text", NULL);
  free( texte );
  g_strconcat_permissions( &texte, (in->i_mode&070)>>3 );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
      -1, "text", NULL);
  free( texte );
  g_strconcat_permissions( &texte, (in->i_mode&07) );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
      -1, "text", NULL);
  free( texte );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "\n",
      -1, "text", NULL);


  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "UID (low 16 "
      "bits) : ", -1, "bold", NULL);
  texte = g_strdup_printf("%hu\n", in->i_uid);
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
      -1, "text", NULL);
  free( texte );


  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Last access time "
      "on the file (timestamp) : ", -1,
      "bold", NULL);
  if ( in->i_atime == 0 ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "never\n", -1,
        "text", NULL);
  }
  else {
    gtk_text_buffer_insert_with_tags_by_name
      ( buffer, &iter, ctime((const time_t *)(&in->i_atime)), -1,"text", NULL);
  }


  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Creation "
      "timestamp : ", -1, "bold", NULL);
  if ( in->i_ctime == 0 ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "never\n", -1,
        "text", NULL);
  }
  else {
    gtk_text_buffer_insert_with_tags_by_name
      ( buffer, &iter, ctime((const time_t *)(&in->i_ctime)), -1,"text", NULL);
  }

  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Last inode "
      "modification time : ", -1,
      "bold", NULL);
  if ( in->i_mtime == 0 ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "never\n", -1,
        "text", NULL);
  }
  else {
    gtk_text_buffer_insert_with_tags_by_name
      ( buffer, &iter, ctime((const time_t *)(&in->i_mtime)), -1,"text", NULL);
  }

  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "File deletion "
      " time : ", -1, "bold", NULL);
  if ( in->i_dtime == 0 ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "never\n", -1,
        "text", NULL);
  }
  else {
    gtk_text_buffer_insert_with_tags_by_name
      ( buffer, &iter, ctime((const time_t *)(&in->i_dtime)), -1,"text", NULL);
  }

  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "GID (low 16 bits)"
      " : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%hu\n", in->i_gid );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
      -1, "text", NULL);
  free( texte );

  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Hard links count"
      " : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%hu\n", in->i_links_count );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
      -1, "text", NULL);
  free( texte );

  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"File data blocks"
      " count : " , -1, "bold", NULL);
  texte = g_strdup_printf( "%u\n", in->i_blocks  );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
      -1, "text", NULL);
  free( texte );

  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"File flags : ",
      -1, "bold", NULL);
  texte = g_strdup_printf( "%u\n", in->i_flags);
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
      -1, "text", NULL);
  free( texte );


  if(mask(in->i_flags, EXT3_SECRM_FL))//properties to check
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_SECRM_FL - File must be securely erased\n" , -1, "text", NULL);
  }
  if(mask(in->i_flags, EXT3_UNRM_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_UNRM_FL - File can be undeleted\n", -1, "text", NULL);
  }
  if(mask(in->i_flags, EXT3_COMPR_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_COMPR_FL - File must be compressed\n", -1, "text", NULL);

  }
  if(mask(in->i_flags, EXT3_SYNC_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_SYNC_FL - File must be synchronized\n", -1, "text", NULL);
  }
  if(mask(in->i_flags, EXT3_IMMUTABLE_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_IMMUTABLE_FL - File cannot be modified\n", -1, "text", NULL);
  }
  if(mask(in->i_flags, EXT3_APPEND_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_APPEND_FL - File is in appending mode, writing is made at the end of the file\n" , -1, "text", NULL);
  }
  if(mask(in->i_flags, EXT3_NODUMP_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_NODUMP_FL - File must not be deleted or dumped\n", -1, "text", NULL);
  }
  if(mask(in->i_flags, EXT3_NOATIME_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_NOATIME_FL - File access time must not be updated\n", -1, "text", NULL);
  }
  if(mask(in->i_flags, EXT3_DIRTY_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_DIRTY_FL - This file has been marked as dirty\n", -1, "text", NULL);

  }
  if(mask(in->i_flags, EXT3_COMPRBLK_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_COMPRBLK_FL - This file is composed of one or more compressed 'clusters'\n", -1, "text", NULL);
  }
  if(mask(in->i_flags, EXT3_NOCOMPR_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_NOCOMPR_FL - This file must not be compressed\n", -1, "text", NULL);
  }
  if(mask(in->i_flags, EXT3_ECOMPR_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_ECOMPR_FL - This file has been corrupted during the compression\n", -1, "text", NULL);
  }

  if(mask(in->i_flags, EXT3_INDEX_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_INDEX_FL - Hash-indexed directory\n", -1, "text", NULL);
  }
  if(mask(in->i_flags, EXT3_IMAGIC_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_IMAGIC_FL -AFS directory\n", -1, "text", NULL);
  }
  if(mask(in->i_flags, EXT3_JOURNAL_DATA_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_JOURNAL_DATA_FL - File data must be journalized\n", -1, "text", NULL);
  }
  if(mask(in->i_flags, EXT3_RESERVED_FL))
  {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "EXT3_RESERVED_FL - Reserved for the ext3 library\n", -1, "text", NULL);
  }

  if ( sb->s_creator_os == EXT3_OS_HURD ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Translator : ",
        -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", in->osd1.hurd1.h_i_translator);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );
  }

  if ( in->i_blocks != 0 ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Data block "
        "pointers  : \n", -1, "bold",
        NULL);

    for ( i = 0 ; i < EXT3_N_BLOCKS ; i++ ) {
      texte = g_strdup_printf("\ti_block[%2u] = %u\n", i, in->i_block[i]);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "fixed", NULL);
      free( texte );
    }
  }


  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"File version "
      "(used for NFS) : ",
      -1, "bold", NULL);
  texte = g_strdup_printf("%u\n", in->i_generation);
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
      -1, "text", NULL);
  free( texte );

  if(mask(in->i_mode>>13, EXT3_FT_DIR)) {

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"File Access "
        "Control List : ", -1,
        "bold", NULL);
    texte = g_strdup_printf("%#x\n", in->i_file_acl);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Directory Acc"
        "ess Control List : ",
        -1, "bold", NULL);
    texte = g_strdup_printf("%#x\n", in->i_dir_acl);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"File size in "
        "bytes : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", in->i_size);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

  }
  else {


    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"File Access "
        "Control List : ",
        -1, "bold", NULL);
    texte = g_strdup_printf("%#x\n", in->i_file_acl);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );


    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"File size in "
        "bytes : ",
        -1, "bold", NULL);
    texte = g_strdup_printf("%llu\n",(__u64)(in->i_size)+((__u64)(in->i_size_high)<<32)); // see ../output.c for more details
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

  }


  switch(sb->s_creator_os)
  {
    case EXT3_OS_LINUX:

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Fragment number : ", -1, "bold", NULL);
      texte = g_strdup_printf("%hu\n", in->osd2.linux2.l_i_frag);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "text", NULL);
      free( texte );

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Fragment size : ", -1, "bold", NULL);
      texte = g_strdup_printf("%hu\n", in->osd2.linux2.l_i_fsize);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "text", NULL);
      free( texte );

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"UID (high 16 bits) : ", -1, "bold", NULL);
      texte = g_strdup_printf("%hu\n", in->osd2.linux2.l_i_uid_high);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "text", NULL);
      free( texte );

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"GID (high 16 bits) : ", -1, "bold", NULL);
      texte = g_strdup_printf("%hu\n", in->osd2.linux2.l_i_gid_high);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "text", NULL);
      free( texte );

      break;
    case EXT3_OS_HURD:

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Fragment number : ", -1, "bold", NULL);
      texte = g_strdup_printf("%hu\n", in->osd2.hurd2.h_i_frag);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "text", NULL);
      free( texte );

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Fragment size : ", -1, "bold", NULL);
      texte = g_strdup_printf("%hu\n", in->osd2.hurd2.h_i_fsize);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "text", NULL);
      free( texte );

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"MODE (high 16 bits) : ", -1, "bold", NULL);
      texte = g_strdup_printf("%hu\n", in->osd2.hurd2.h_i_mode_high);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "text", NULL);
      free( texte );

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"UID (high 16 bits) : ", -1, "bold", NULL);
      texte = g_strdup_printf("%hu\n", in->osd2.hurd2.h_i_uid_high);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "text", NULL);
      free( texte );

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"GID (high 16 bits) : ", -1, "bold", NULL);
      texte = g_strdup_printf("%hu\n", in->osd2.hurd2.h_i_gid_high);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "text", NULL);
      free( texte );

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Author : ",
          -1, "bold", NULL);
      texte = g_strdup_printf("%u\n", in->osd2.hurd2.h_i_author);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "text", NULL);
      free( texte );

      break;
    case EXT3_OS_MASIX:

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Fragment number : ", -1, "bold", NULL);
      texte = g_strdup_printf("%hu\n", in->osd2.masix2.m_i_frag);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "text", NULL);
      free( texte );

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Fragment size : ", -1, "bold", NULL);
      texte = g_strdup_printf("%hu\n", in->osd2.masix2.m_i_fsize);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "text", NULL);
      free( texte );
      break;

    default:
      ;
  }

}

void g_strconcat_permissions( gchar **text, __u8 perm)
{
  //  gchar *tmp;
  *text = g_strdup_printf(" ");
  if(mask(perm, 04))
  {
    *text = g_strconcat(*text, "r", NULL);
  }
  else
  {
    *text = g_strconcat(*text, "-", NULL);
  }
  if(mask(perm, 02))
  {
    *text = g_strconcat(*text, "w", NULL);

  }
  else
  {
    *text = g_strconcat(*text, "-", NULL);

  }
  if(mask(perm, 01))
  {
    *text = g_strconcat(*text, "x", NULL);
  }
  else
  {
    *text = g_strconcat(*text, "-", NULL);

  }
  // printf("%s\n",*text);
  //    text = g_strconcat(text, " ", NULL);
}


void print_superblock_gui ( ) {

  if ( !fs.isopen ) {
    show_warning_dialog("You cannot access to this functionality : no "
        "filesystem open");
  }
  else {
    GtkTextIter iter, start, end;
    GtkTextBuffer *buffer;
    gchar *texte;

    /* get the buffer */
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

    /* empty the text area */
    gtk_text_buffer_get_bounds ( buffer, &start, &end);
    gtk_text_buffer_delete ( buffer, &start, &end );

    // Write in the text area
    gtk_text_buffer_get_end_iter ( buffer, &iter);

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Superblock structure display\n\n", -1, "title", NULL);

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "The superblock contains values describing the filesystem's geometry, behavior, and tunable parameters. It also contains states flags (for data recovery, etc.)\n\n", -1, "citalic", NULL);

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Total inode count : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_inodes_count);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Block count : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_blocks_count);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );


    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Reserved block count : " , -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_r_blocks_count );
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );


    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Free block count : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_free_blocks_count);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );


    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Free inode count : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_free_inodes_count);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"First data block : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_first_data_block);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );



    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Block size : ", -1, "bold", NULL);
    texte = g_strdup_printf("1024 * 2^%u\n", fs.sb.s_log_block_size);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Fragment size : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_log_frag_size);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );


    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Blocks per group count : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_blocks_per_group);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Fragments per group count : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_frags_per_group);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Inodes per group count : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_inodes_per_group);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "\n", -1, "text", NULL);

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Last mounting operation timestamp : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_mtime);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

    if (fs.sb.s_mtime == 0 ) {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Never\n", -1, "text", NULL);
    }
    else {
      gtk_text_buffer_insert_with_tags_by_name
        ( buffer, &iter, ctime((const time_t *)(&fs.sb.s_mtime)), -1,"text", NULL);
    }
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Last writing operation timestamp : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_wtime);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

    if (fs.sb.s_mtime == 0 ) {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Never\n", -1, "text", NULL);
    }
    else {
      gtk_text_buffer_insert_with_tags_by_name
        ( buffer, &iter, ctime((const time_t *)(&fs.sb.s_wtime)), -1,"text", NULL);
    }

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Counter (times mounted) : ", -1, "bold", NULL);
    texte = g_strdup_printf("%hu\n", fs.sb.s_mnt_count);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Authorized mount count before integrity checking : ", -1, "bold", NULL);
    texte = g_strdup_printf("%hu\n", fs.sb.s_max_mnt_count);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Magic Number : ", -1, "bold", NULL);
    texte = g_strdup_printf("%#.4hx\n", fs.sb.s_magic);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"Status flag : ", -1, "bold", NULL);
    texte = g_strdup_printf("%hx\n", fs.sb.s_state);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
        -1, "text", NULL);
    free( texte );



    switch(fs.sb.s_state)
    {
      case 0://No consta linux-extfs OREILLY

        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,"The filesystem was not unmounted cleanly\n" , -1, "text", NULL);
        break;
      case EXT3_VALID_FS:
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "The filesystem is clean\n", -1, "text", NULL);
        break;
      case EXT3_ERROR_FS:
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "The filesystem contains errors\n", -1, "text", NULL);
        break;
      case EXT3_ORPHAN_FS:
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Orphan inodes are being recovered\n", -1, "text", NULL);
        break;
      default:
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "The superblock is corrupt.\n", -1, "text", NULL);

    }


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Behaviour when detecting errors : ", -1, "bold", NULL);
    texte = g_strdup_printf("%hu\n", fs.sb.s_errors);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    switch(fs.sb.s_errors)
    {
      case EXT3_ERRORS_CONTINUE:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "If an error is detected, the filesystem will continue to be used.\n", -1, "text", NULL);
        break;
      case EXT3_ERRORS_RO:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "If an error is detected, the filesystem must be remounted in read-only mode.\n", -1, "text", NULL);
        break;
      case EXT3_ERRORS_PANIC:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "If an error is detected, a KERNEL PANIC must be triggered.\n", -1, "text", NULL);
        break;
      default:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Unknown behaviour.\n", -1, "text", NULL);

    }

    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Minimum compatible revision level (version) : ", -1, "bold", NULL);
    texte = g_strdup_printf("%hu\n", fs.sb.s_minor_rev_level);//Traduire
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);



    switch(fs.sb.s_minor_rev_level)
    {
      case EXT3_GOOD_OLD_REV:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "(Version using the original format.)\n", -1, "text", NULL);
        break;
      case EXT3_DYNAMIC_REV:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "(Second version using dynamically sized inodes.)\n", -1, "text", NULL);
        break;
      default:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "(Unknown filesystem version.)", -1, "text", NULL);
    }

    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Last check timestamp : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_lastcheck);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    if(fs.sb.s_lastcheck == 0 )
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Never\n", -1, "text", NULL);
    }
    else
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, (ctime((const time_t *)&fs.sb.s_lastcheck)), -1, "text", NULL);
    }

    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Maximum interval between checks : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u secondes\n", fs.sb.s_checkinterval);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Creator OS : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_creator_os);//Determiner OS
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    switch(fs.sb.s_creator_os)
    {
      case EXT3_OS_LINUX:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Partition created by Linux.\n", -1, "text", NULL);
        break;
      case EXT3_OS_HURD:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Partition created by HURD.\n", -1, "text", NULL);
        break;
      case EXT3_OS_MASIX:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Partition created by MASIX.\n", -1, "text", NULL);
        break;
      case EXT3_OS_FREEBSD:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Partition created by FreeBSD.\n", -1, "text", NULL);
        break;
      case EXT3_OS_LITES:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Partition created by Lites.\n", -1, "text", NULL);
        break;
      default:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Unknown creator OS.\n", -1, "text", NULL);

    }


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Current revision level (version) : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_rev_level);//Traduire
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    switch(fs.sb.s_rev_level)
    {
      case EXT3_GOOD_OLD_REV:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "(Version using the original format.)\n", -1, "text", NULL);
        break;
      case EXT3_DYNAMIC_REV:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "(Second version using dynamically sized inodes.)\n", -1, "text", NULL);
        break;
      default:
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "(Unknown filesystem version.)", -1, "text", NULL);
    }



    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "\n", -1, "text", NULL);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Default UID for reserved blocks : ", -1, "bold", NULL);
    texte = g_strdup_printf("%hu\n", fs.sb.s_def_resuid);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Default GID for reserved blocks : ", -1, "bold", NULL);
    texte = g_strdup_printf("%hu\n", fs.sb.s_def_resgid);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);



    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "First non-reserved inode number : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_first_ino);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Inode size : ", -1, "bold", NULL);
    texte = g_strdup_printf("%hu\n", fs.sb.s_inode_size);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Superblock block number (being read) : ", -1, "bold", NULL);
    texte = g_strdup_printf("%hu\n", fs.sb.s_block_group_nr);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);



    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "\n", -1, "text", NULL);



    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Compatible features set : ", -1, "bold", NULL);
    texte = g_strdup_printf("%#.4x\n", fs.sb.s_feature_compat);//Verifier
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    if(mask(fs.sb.s_feature_compat, EXT3_FEATURE_COMPAT_DIR_PREALLOC))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports directories preallocation.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_feature_compat, EXT3_FEATURE_COMPAT_IMAGIC_INODES))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports magical inodes.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_feature_compat, EXT3_FEATURE_COMPAT_HAS_JOURNAL))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports journalling.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_feature_compat, EXT3_FEATURE_COMPAT_EXT_ATTR))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports extended user attirbutes.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_feature_compat, EXT3_FEATURE_COMPAT_RESIZE_INODE))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports variable-sized inodes.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_feature_compat, EXT3_FEATURE_COMPAT_DIR_INDEX))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports directory indexing.\n", -1, "text", NULL);
    }


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Incompatible features set : ", -1, "bold", NULL);
    texte = g_strdup_printf("%#.4x\n", fs.sb.s_feature_incompat);//Verifier
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    if(mask(fs.sb.s_feature_incompat, EXT3_FEATURE_COMPAT_DIR_PREALLOC))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports directories preallocation.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_feature_incompat, EXT3_FEATURE_COMPAT_IMAGIC_INODES))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports magical inodes.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_feature_incompat, EXT3_FEATURE_COMPAT_HAS_JOURNAL))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports journalling.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_feature_incompat, EXT3_FEATURE_COMPAT_EXT_ATTR))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports extended user attirbutes.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_feature_incompat, EXT3_FEATURE_COMPAT_RESIZE_INODE))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports variable-sized inodes.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_feature_incompat, EXT3_FEATURE_COMPAT_DIR_INDEX))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports directory indexing.\n", -1, "text", NULL);
    }

    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Read-Only Compatible features set : ", -1, "bold", NULL);
    texte = g_strdup_printf("%#.4x\n", fs.sb.s_feature_ro_compat);//Verifier
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    if(mask(fs.sb.s_feature_ro_compat, EXT3_FEATURE_RO_COMPAT_SPARSE_SUPER))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports sparsefiles in read-only mode.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_feature_ro_compat, EXT3_FEATURE_RO_COMPAT_LARGE_FILE))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports large files in read-only mode.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_feature_ro_compat, EXT3_FEATURE_RO_COMPAT_BTREE_DIR))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "This partition supports directory B-trees in read only mode.\n", -1, "text", NULL);
    }



    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "\n", -1, "text", NULL);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "UUID: ", -1, "bold", NULL);
    texte = g_strdup_printf("%hx%hx%hx%hx-%hx%hx-%hx%hx-%hx%hx-%hx%hx%hx%hx%hx%hx\n", fs.sb.s_uuid[0], fs.sb.s_uuid[1], fs.sb.s_uuid[2], fs.sb.s_uuid[3], fs.sb.s_uuid[4], fs.sb.s_uuid[5], fs.sb.s_uuid[6], fs.sb.s_uuid[7], fs.sb.s_uuid[8], fs.sb.s_uuid[9], fs.sb.s_uuid[10], fs.sb.s_uuid[11], fs.sb.s_uuid[12], fs.sb.s_uuid[13], fs.sb.s_uuid[14], fs.sb.s_uuid[15]);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free( texte );

    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Volume name = \"", -1, "bold", NULL);
    texte = g_strdup_printf("%s\"\n", fs.sb.s_volume_name);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Last mounted in directory \"", -1, "bold", NULL);
    texte = g_strdup_printf("%s\"\n", fs.sb.s_last_mounted);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);



    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Compression usage bitmap : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_algorithm_usage_bitmap);//Verifier
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Number of blocks to try to preallocate : ", -1, "bold", NULL);
    texte = g_strdup_printf("%hu\n", fs.sb.s_prealloc_blocks); // 8bits /!\  //Verifier
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Number of blocks to preallocate for directories : ", -1, "bold", NULL);
    texte = g_strdup_printf("%hu\n", fs.sb.s_prealloc_dir_blocks);// 8bits
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "\n", -1, "text", NULL);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Journal UUID: ", -1, "bold", NULL);
    texte = g_strdup_printf("%hx%hx%hx%hx-%hx%hx-%hx%hx-%hx%hx-%hx%hx%hx%hx%hx%hx\n", fs.sb.s_journal_uuid[0], fs.sb.s_journal_uuid[1], fs.sb.s_journal_uuid[2], fs.sb.s_journal_uuid[3], fs.sb.s_journal_uuid[4], fs.sb.s_journal_uuid[5], fs.sb.s_journal_uuid[6], fs.sb.s_journal_uuid[7], fs.sb.s_journal_uuid[8], fs.sb.s_journal_uuid[9], fs.sb.s_journal_uuid[10], fs.sb.s_journal_uuid[11], fs.sb.s_journal_uuid[12], fs.sb.s_journal_uuid[13], fs.sb.s_journal_uuid[14], fs.sb.s_journal_uuid[15]);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free( texte );


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Journal Inode number : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_journal_inum);  /* inode number of journal file */
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);





    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Journal device number : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_journal_dev);  /* device number of journal file */
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Start of list of orphaned inodes to delete : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_last_orphan);  /* start of list of inodes to delete */
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "HTREE default hash seed : ", -1, "bold", NULL);
    texte = g_strdup_printf("%x|%x|%x|%x\n", fs.sb.s_hash_seed[0], fs.sb.s_hash_seed[1], fs.sb.s_hash_seed[2], fs.sb.s_hash_seed[3]);

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Hash default version : ", -1, "bold", NULL);
    texte = g_strdup_printf("%hd\n", fs.sb.s_def_hash_version);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);

    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "s_reserved_word_pad : ", -1, "bold", NULL);
    texte = g_strdup_printf("%hd\n", fs.sb.s_reserved_word_pad);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Default mount options : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_default_mount_opts);//Traduire
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);



    if(mask(fs.sb.s_default_mount_opts, EXT3_DEFM_DEBUG))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Debug information displayed when mounting.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_default_mount_opts, EXT3_DEFM_BSDGROUPS))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "The default group ID of a new file is its parent directory's one.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_default_mount_opts, EXT3_DEFM_XATTR_USER))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Support for extended user attributes.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_default_mount_opts, EXT3_DEFM_ACL))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Support for Access Control Lists.\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_default_mount_opts, EXT3_DEFM_UID16))
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Desactivation of the 32-bits mode for UIDs and GIDs (retrocompatibility with older kernels).\n", -1, "text", NULL);
    }
    if(mask(fs.sb.s_default_mount_opts, EXT3_DEFM_JMODE))//Meme signature que WBACK?
    {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Activated journalling.\n", -1, "text", NULL);

      if(mask(fs.sb.s_default_mount_opts, EXT3_DEFM_JMODE_DATA))
      {
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Journal is used in complete journaling mode (data+metadata).\n", -1, "text", NULL);
      }
      else
      {
        if(mask(fs.sb.s_default_mount_opts, EXT3_DEFM_JMODE_ORDERED))
        {
          gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Journal is used in ORDERED mode.\n", -1, "text", NULL);
        }
        else
        {
          if(mask(fs.sb.s_default_mount_opts, EXT3_DEFM_JMODE_WBACK))
          {
            gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Journal is used in WRITEBACK mode.\n", -1, "text", NULL);
          }
        }

      }
    }



    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "First metablock block group : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n", fs.sb.s_first_meta_bg);//??
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);

  }

}


void print_group_desc () {

  unsigned int group_count;
  unsigned int c;
  struct ext3_group_desc gd;

  GtkTextIter iter, start, end;
  GtkTextBuffer *buffer;
  gchar *texte;

  if ( !fs.isopen ) {

    show_warning_dialog("You cannot access to this functionality : no "
        "filesystem open");

  }
  else {


    // Get the buffer
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

    // Empty the text area
    gtk_text_buffer_get_bounds ( buffer, &start, &end);
    gtk_text_buffer_delete ( buffer, &start, &end );

    // Write in the text area
    gtk_text_buffer_get_end_iter ( buffer, &iter);

    group_count = fs.sb.s_blocks_count/EXT3_BLOCKS_PER_GROUP(&fs.sb);


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Details about the group descriptors\n\n", -1, "title", NULL);

    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Group descriptors store information about block groups, such as the inode bitmap block number.\n\n", -1, "citalic", NULL);

    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Group descriptor cout : ", -1, "bold", NULL);
    texte = g_strdup_printf("%u\n\n", group_count);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free(texte);

    for ( c = 0 ; c < group_count || c == 0 ; c++ ) {

      texte = g_strdup_printf("[ group #%u ]\n", c);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "bold", NULL);
      free(texte);

      read_group_desc( fs.fd, &fs.sb, c, &gd);

      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Block bitmap block (number) : ", -1, "text", NULL);
      texte = g_strdup_printf("%u\n", gd.bg_block_bitmap);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
      free(texte);

      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Inode bitmap block number : ", -1, "text", NULL);
      texte = g_strdup_printf("%u\n", gd.bg_inode_bitmap);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
      free(texte);

      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "First Inode table block number : ", -1, "text", NULL);
      texte = g_strdup_printf("%u\n", gd.bg_inode_table);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
      free(texte);

      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Free blocks count (in this group) : ", -1, "text", NULL);
      texte = g_strdup_printf("%hd\n", gd.bg_free_blocks_count);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
      free(texte);

      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Free inodes count (in this group) : ", -1, "text", NULL);
      texte = g_strdup_printf("%hd\n", gd.bg_free_inodes_count);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
      free(texte);

      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Directories count (in this group) : ", -1, "text", NULL);
      texte = g_strdup_printf("%hd\n\n", gd.bg_used_dirs_count);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
      free(texte);


    }

  }
}



void print_superblock_backups () {


  int group_count;
  int i = 1;
  GtkTextIter iter, start, end;
  GtkTextBuffer *buffer;
  gchar *texte;

  if ( !fs.isopen ) {

    show_warning_dialog("You cannot access to this functionality : no "
        "filesystem open");

  }
  else {


    // Get the buffer
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

    // Empty the text area
    gtk_text_buffer_get_bounds ( buffer, &start, &end);
    gtk_text_buffer_delete ( buffer, &start, &end );

    // Write in the text area
    gtk_text_buffer_get_end_iter ( buffer, &iter);


    group_count = fs.sb.s_blocks_count/EXT3_BLOCKS_PER_GROUP(&fs.sb);


    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Superblock backups\n\n", -1, "title", NULL);

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "The superblock contains very important data, ext3 maintains several copies of it.\n\n", -1, "citalic", NULL);

    if (! mask(  fs.sb.s_feature_ro_compat, EXT3_FEATURE_RO_COMPAT_SPARSE_SUPER)) {
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "On your disk, a copy exists at the start of every block group\n\n", -1, "bold", NULL);

      if ( group_count > 0 ) {

        texte = g_strdup_printf("There is a copy of the superblock at group #%d block #%d\n", i, i*EXT3_BLOCKS_PER_GROUP(&fs.sb) );
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
        free(texte);

        for ( i = 3 ; i < group_count || i == 0; i++ ) {

          texte = g_strdup_printf("There is a copy of the superblock at group #%d block #%d\n", i, i == 0?EXT3_BLOCK_SIZE(&fs.sb):i*EXT3_BLOCKS_PER_GROUP(&fs.sb) );
          gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
          free(texte);
        }
      }
      else {
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "There is no copy of the superblock\n", -1, "text", NULL);
      }
    }
    else {
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Your filesystem uses the sparse superblock feature :\n\n", -1, "bold", NULL);

      if ( group_count > 0 ) {
        texte = g_strdup_printf("There is a copy of the superblock at group #%d block #%d\n", 1, i*EXT3_BLOCKS_PER_GROUP(&fs.sb) );
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
        free(texte);

        for ( i = 3 ; i < group_count ; i++ ) {

          if ( is_pow_of( i, 3 ) || is_pow_of( i, 5 ) || is_pow_of( i, 7 ) ) {
            texte = g_strdup_printf("There is a copy of the superblock at group #%d block #%d\n", i, i*EXT3_BLOCKS_PER_GROUP(&fs.sb) );
            gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
            free(texte);
          }
        }
      }
      else {
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "There is no copy of the superblock\n", -1, "text", NULL);

      }


    }

  }

}





void print_ffi () {

  unsigned int inode_num;
  char *inodebitmap;

  GtkTextIter iter, start, end;
  GtkTextBuffer *buffer;
  gchar *texte;

  if ( !fs.isopen ) {

    show_warning_dialog("You cannot access to this functionality :\nno filesystem is open.");

  }
  else {

    // Get the buffer
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

    // Empty the text area
    gtk_text_buffer_get_bounds ( buffer, &start, &end);
    gtk_text_buffer_delete ( buffer, &start, &end );

    // Write in the text area
    gtk_text_buffer_get_end_iter ( buffer, &iter);
    inodebitmap = malloc( EXT3_INODES_PER_GROUP(&fs.sb)/8 );
    inode_num = search_first_free_inode( fs.fd, &fs.sb, inodebitmap );


    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Inode bitmaps\n\n", -1, "title", NULL);


    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "To find the first free inode, we probe the inode bitmaps of each group until we find the first zero-valued bit.\n\n", -1, "citalic", NULL);


    if ( inode_num > 0 ) {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "First free Inode found: ", -1, "bold", NULL);
      texte = g_strdup_printf("%u\n", inode_num);
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, texte, -1, "text", NULL);
      free(texte);
    }
    else {
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "No free inode found\n", -1, "text", NULL);
    }

    free( inodebitmap );

  }

}


void print_ffb () {

  unsigned int block_num;
  char *blockbitmap;

  GtkTextIter iter, start, end;
  GtkTextBuffer *buffer;
  gchar *texte;

  if ( !fs.isopen ) {

    show_warning_dialog("You cannot access to this functionality :\nno filesystem is open.");

  }
  else {

    // Get the buffer
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

    // Empty the text area
    gtk_text_buffer_get_bounds ( buffer, &start, &end);
    gtk_text_buffer_delete ( buffer, &start, &end );

    // Write in the text area
    gtk_text_buffer_get_end_iter ( buffer, &iter);

    blockbitmap = malloc( EXT3_BLOCKS_PER_GROUP(&fs.sb)/8 );
    block_num = search_first_free_block( fs.fd, &fs.sb, blockbitmap );


    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Block bitmaps\n\n", -1, "title", NULL);


    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "To find the first free block, we probe the block bitmap of each group until we find the first zero-valued bit.\n\n", -1, "citalic", NULL);


    if ( block_num > 0 ) {
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "First free block found: ", -1, "bold", NULL);
      texte = g_strdup_printf("%u\n", block_num);
      gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, texte, -1, "text", NULL);
      free(texte);
    }
    else {
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "No free block found\n", -1, "text", NULL);
    }

    free( blockbitmap );

  }

}


void print_imap () {

  int group_num;
  int group_num_max;
  char *inodebitmap;
  struct ext3_group_desc gd;
  __u32 imax;
  __u32 i_num;
  unsigned long long int i = 0;//Counter used to print the content
  int charcount = 0;//Printed characters, used for formatting purposes

  GtkTextIter iter, start, end;
  GtkTextBuffer *buffer;
  gchar *texte;

  if ( !fs.isopen ) {

    show_warning_dialog("You cannot access to this functionality :\nno filesystem is open.");

  }
  else {


    group_num_max = (fs.sb.s_blocks_count/EXT3_BLOCKS_PER_GROUP(&fs.sb))-1;
    group_num = ask_group_number("Show the inode bitmap of group :");
    if ( group_num < 0 || (group_num > group_num_max && group_num != 0) )
      return;

    // Get the buffer
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

    // Empty the text area
    gtk_text_buffer_get_bounds ( buffer, &start, &end);
    gtk_text_buffer_delete ( buffer, &start, &end );

    // Write in the text area
    gtk_text_buffer_get_end_iter ( buffer, &iter);
    inodebitmap = malloc( EXT3_INODES_PER_GROUP(&fs.sb)/8 );

    read_group_desc( fs.fd, &fs.sb, group_num, &gd );

    read_inodebitmap( fs.fd, &fs.sb, &gd, inodebitmap);


    imax = ((__u32) EXT3_INODES_PER_GROUP(&fs.sb)) / 8;



    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Inode bitmap of group ", -1, "title", NULL);
    texte = g_strdup_printf("#%u\n\n", group_num );
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, texte, -1, "title", NULL);
    free(texte);

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "In each block group, there is an inode bitmap. It is used to know whether a particular inode is free or not\n\n", -1, "citalic", NULL);
    gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, "Inode bitmap: 0 means free inode, 1 means inode in use\n", -1, "italic", NULL);

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, A_LINE_BITMAP_GUI, -1, "fixed", NULL);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "|Inode num.|                      Bits values                    |\n", -1, "fixed", NULL);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, A_LINE_BITMAP_GUI, -1, "fixed", NULL);
    while ( i < imax )
    {

      if(i%6 == 0)
      {
        if(i == 0) {
          gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, " ", -1, "fixed", NULL);
        }
        else {
          gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "\n ", -1, "fixed", NULL);
        }

        // We look at the inode number in the group, we want to find the real
        // inode number: we add the number of inodes per group * the group
        // number
        i_num = i*8+(EXT3_INODES_PER_GROUP(&fs.sb)*group_num);
        texte = g_strdup_printf("0x%08x", i_num+1);
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "fixed", NULL);
        free(texte);//Can go up to 32 bits
        //We add 1 because the inode number starts at 1

      }
      if( charcount%8 == 0)
      {
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "|", -1, "fixed", NULL);
      }
      //  print_t_bin( inodebitmap[i] );

      texte = g_strdup_printf("%d%d%d%d%d%d%d%d", inodebitmap[i]&1,(inodebitmap[i]&2)>>1,(inodebitmap[i]&4)>>2,(inodebitmap[i]&8)>>3,(inodebitmap[i]&16)>>4,(inodebitmap[i]&32)>>5,(inodebitmap[i]&64)>>6,(inodebitmap[i]&128)>>7);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "fixed", NULL);
      free(texte);

      i++;//We print bits eight by eight
      charcount = charcount+8;
    }
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "\n"A_LINE_BITMAP_GUI, -1, "fixed", NULL);
    //    printf("charcount =%u\n", charcount);


    free( inodebitmap );




  }

}



void print_bmap () {

  int group_num;
  int group_num_max;
  char *datablockbitmap;
  struct ext3_group_desc gd;
  __u32 bmax;
  __u32 i_num;
  unsigned long long int i = 0;//Counter used to print the content
  int charcount = 0;//Printed characters, used for formatting purposes
  int group_count;
  GtkTextIter iter, start, end;
  GtkTextBuffer *buffer;
  gchar *texte;

  if ( !fs.isopen ) {

    show_warning_dialog("You cannot access to this functionality :\nno filesystem is open.");

  }
  else {


    group_num_max = (fs.sb.s_blocks_count/EXT3_BLOCKS_PER_GROUP(&fs.sb))-1;
    group_num = ask_group_number("Show the block bitmap of group :");
    if ( group_num < 0 || (group_num > group_num_max && group_num != 0) )
      return;

    // Get the buffer
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

    // Empty the text area
    gtk_text_buffer_get_bounds ( buffer, &start, &end);
    gtk_text_buffer_delete ( buffer, &start, &end );

    // Write in the text area
    gtk_text_buffer_get_end_iter ( buffer, &iter);
    datablockbitmap = malloc( EXT3_BLOCKS_PER_GROUP(&fs.sb)/8 );

    read_group_desc( fs.fd, &fs.sb, group_num, &gd );

    group_count =  fs.sb.s_blocks_count/EXT3_BLOCKS_PER_GROUP(&fs.sb);
    if(group_num == group_count)//We are in the last group
    {//which can be filled only partially
      bmax = ((__u32)
          (fs.sb.s_blocks_count -  (group_count * EXT3_BLOCKS_PER_GROUP(&fs.sb)))/8);
    }
    else
    {
      bmax = ((__u32)
          EXT3_BLOCKS_PER_GROUP(&fs.sb)) / 8;
    }

    read_datablockbitmap( fs.fd, &fs.sb, &gd, group_num, datablockbitmap);

    //print_datablockbitmap(&fs.sb, datablockbitmap, group_num);

    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Blocck bitmap of group ", -1, "title", NULL);
    texte = g_strdup_printf("#%u\n\n", group_num );
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, texte, -1, "title", NULL);
    free(texte);

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "In each block group, there is an block bitmap. It is used to know whether a particular block is free or not\n\n", -1, "citalic", NULL);
    gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, "Block bitmap: 0 means free inode, 1 means inode in use\n", -1, "italic", NULL);

    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, A_LINE_BITMAP_GUI, -1, "fixed", NULL);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "|Block num.|                      Bits values                    |\n", -1, "fixed", NULL);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, A_LINE_BITMAP_GUI, -1, "fixed", NULL);
    while ( i < bmax )
    {

      if(i%6 == 0)
      {
        if(i == 0) {
          gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, " ", -1, "fixed", NULL);
        }
        else {
          gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "\n ", -1, "fixed", NULL);
        }

        // We look at the inode number in the group, we want to find the real
        // inode number: we add the number of inodes per group * the group
        // number
        i_num = i*8+(EXT3_BLOCKS_PER_GROUP(&fs.sb)*group_num);
        texte = g_strdup_printf("0x%08x", i_num+1);
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "fixed", NULL);
        free(texte);//Can go up to 32 bits
        //We add 1 because the block number starts at 1

      }
      if( charcount%8 == 0)
      {
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "|", -1, "fixed", NULL);
      }
      //  print_t_bin( blockbitmap[i] );

      texte = g_strdup_printf("%d%d%d%d%d%d%d%d", datablockbitmap[i]&1,( datablockbitmap[i]&2)>>1,( datablockbitmap[i]&4)>>2,( datablockbitmap[i]&8)>>3,( datablockbitmap[i]&16)>>4,( datablockbitmap[i]&32)>>5,( datablockbitmap[i]&64)>>6,( datablockbitmap[i]&128)>>7);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "fixed", NULL);
      free(texte);

      i++;//We print bits eight by eight
      charcount = charcount+8;
    }
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "\n"A_LINE_BITMAP_GUI, -1, "fixed", NULL);
    //    printf("charcount =%u\n", charcount);


    free(  datablockbitmap );




  }

}



void print_block_as_bpointer_gui( ) {

  int i = 0;
  int max = EXT3_BLOCK_SIZE(&fs.sb)/sizeof(__u32);
  __u32 *block;
  long long int block_num;
  int ret;

  GtkTextIter iter, start, end;
  GtkTextBuffer *buffer;
  gchar *texte;

  if ( !fs.isopen ) {

    show_warning_dialog("You cannot access to this functionality :\nno filesystem is open.");

  }
  else {

    block = malloc ( EXT3_BLOCK_SIZE(&fs.sb) );

    block_num = ask_block_number("Show a block as a block pointer :");
    if ( block_num > 0 ) {

      ret = read_block_pointer ( fs.fd, EXT3_BLOCK_SIZE(&fs.sb), block_num, block );
      if ( ret == -1 ) {
        show_warning_dialog("Wrong block number !");
        return;
      }

      // Get the buffer
      buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

      // Empty the text area
      gtk_text_buffer_get_bounds ( buffer, &start, &end);
      gtk_text_buffer_delete ( buffer, &start, &end );

      // Write in the text area
      gtk_text_buffer_get_end_iter ( buffer, &iter);

      texte = g_strdup_printf("Showing block #%u as a block pointer\n\n",(__u32)block_num);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "title", NULL);
      free( texte );

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "An inode has\
          blocks allocated to it, i_blocks[] from index 12 to 14 are block\
          pointers. Index 12 is an indirect block, index 13 is a double indirect\
          block and index 14 is a triple indirect block\n\n", -1, "citalic", NULL);

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "  NUM            "
          "               POINTERS TO BLOCK N\n"A_LINE_GUI, -1, "fixed", NULL );
      for ( i = 0 ; i < max ; i+= 6 ) {

        texte =  g_strdup_printf("|0x%.3x||%10u|%10u|%10u|%10u|%10u|%10u|\n", i,
            block[i], block[i+1], block[i+2], block[i+3], block[i+4], block[i+5]);
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
            -1, "fixed", NULL);
        free( texte );
      }

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter,  A_LINE_GUI, -1,
          "fixed", NULL );

      free(block);
    }
  }
}

void print_block_hexa_gui (  ) {

  int cc, c;
  int blocksize = EXT3_BLOCK_SIZE(&fs.sb);
  char *block;
  int ret;

  long long int block_num;

  GtkTextIter iter, start, end;
  GtkTextBuffer *buffer;
  gchar *texte;

  if ( !fs.isopen ) {

    show_warning_dialog("You cannot access to this functionality :\nno "
        "filesystem is open.");

  }
  else {


    block_num = ask_block_number("Show the block contents of :");

    if ( block_num > 0 ) {
      block = malloc ( blocksize );
      ret = read_block( fs.fd, &fs.sb, block_num, block );
      if ( ret == -1 ) {
        show_warning_dialog("Wrong block number !");
        return;
      }

      // Get the buffer
      buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

      // Empty the text area
      gtk_text_buffer_get_bounds ( buffer, &start, &end);
      gtk_text_buffer_delete ( buffer, &start, &end );


      // Write in the text area
      gtk_text_buffer_get_end_iter ( buffer, &iter);


      texte = g_strdup_printf("Showing block #%u contents in hexadecimal\n\n",
          (__u32)block_num);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte,
          -1, "title", NULL);
      free( texte );



      for ( cc = 0 ; cc < blocksize ; cc+= 16) {

        texte = g_strdup_printf(" %.8x  ", cc);
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, texte,
            -1, "fixed", NULL);
        free( texte );

        for ( c = 0 ; c < 8 ; c++ ) {//print numbers in hexa
          texte = g_strdup_printf("%.2x ", (__u8) block[cc+c]);
          gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, texte,
              -1, "fixed", NULL);
          free( texte );
        }
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, " ", -1,
            "fixed", NULL);
        for (  ; c < 16 ; c++ ) {
          texte = g_strdup_printf("%.2x ",  (__u8)block[cc+c]);
          gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, texte,
              -1, "fixed", NULL);
          free( texte );
        }
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, " |", -1,
            "fixed", NULL);

        for ( c = 0 ; c < 16 ; c++ ) //print ascii characters
          if ( block[c+cc] >= 32 && block[c+cc] <= 126 ) { 
            //viewable chars:man ascii
            texte = g_strdup_printf("%c", block[c+cc]);
            gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, texte,
                -1, "fixed", NULL);
            free( texte );
          }
          else
            gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, ".", -1,
                "fixed", NULL);
        gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "|\n", -1,
            "fixed", NULL);

      }
      free ( block );
    }

  }
}


void print_journal () {

  GtkTextIter iter, start, end;
  GtkTextBuffer *buffer;
  //gchar *texte;

  struct journal_superblock_s jsb;
  struct ext3_inode i;

  if ( !fs.isopen ) {

    show_warning_dialog("You cannot access to this functionality :\nno "
        "filesystem is open.");

  }
  else {
    // Get the buffer
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

    // Empty the text area
    gtk_text_buffer_get_bounds ( buffer, &start, &end);
    gtk_text_buffer_delete ( buffer, &start, &end );


    // Write in the text area
    gtk_text_buffer_get_end_iter ( buffer, &iter);


    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Dumping the "
        "internal journal\n\n", -1, "title", NULL);


    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "The journal "
        "(jbd) takes note of each writing operation. When the filesystem is "
        "mounted, the flag s_feature_incompat is set to 1. If any problem "
        "occurs when the filesystem is mounted (eg. power failure), when it "
        "is remounted, the \"mount flag\" value is still set to 1 (mounted). "
        "Then the journal recovery  must be done. When a correct unmount "
        "operation is done, the \"mount flag\" is set to 0.\n\n", -1,
        "citalic", NULL);

    read_inode( fs.fd, &fs.sb, fs.sb.s_journal_inum, &i );
    read_journal_superblock( fs.fd, EXT3_BLOCK_SIZE(&fs.sb), i.i_block[0], &jsb);

    print_journal_superblock_gui(&jsb);

    dump_journal_gui( fs.fd, EXT3_BLOCK_SIZE(&fs.sb), &i);
  }

}



void print_journal_superblock_gui(struct journal_superblock_s * j)
{

  GtkTextIter iter;
  GtkTextBuffer *buffer;
  gchar *texte;

  // Get the buffer
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

  // Write in the text zone
  gtk_text_buffer_get_end_iter ( buffer, &iter);

  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "JOURNAL SUPERBLOCK\n", -1, "title2", NULL );


  print_journal_header_gui( &j->s_header );

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));
  gtk_text_buffer_get_end_iter ( buffer, &iter);

  if ( be32_to_cpu(j->s_start) == 0 ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "The journal is empty ! dumping old transactions\n", -1, "citalic", NULL);
  }


  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Journal device blocksize : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%u    \n", be32_to_cpu((j->s_blocksize)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Total blocks in journal size : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%u   \n", be32_to_cpu((j->s_maxlen)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "First block of log information : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%u\n", be32_to_cpu((j->s_first)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "First commit ID expected in the log : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%u\n", be32_to_cpu((j->s_sequence)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Block number of the log start : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%u  \n", be32_to_cpu((j->s_start)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Error value (if any) : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%u\n\n", be32_to_cpu((j->s_errno)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "The following fields are only valid in a version 2 superblock.\n", -1, "italic", NULL);
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Compatible features set : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%#x\n" , be32_to_cpu((j->s_feature_compat)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Incompatible features set : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%#x\n", be32_to_cpu((j->s_feature_incompat)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Read-only compatible feature set : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%#x\n", be32_to_cpu((j->s_feature_ro_compat)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );


  print_UUID_gui( j->s_uuid );


  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));
  gtk_text_buffer_get_end_iter ( buffer, &iter);


  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Number of filesystem sharing log : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%u\n", be32_to_cpu((j->s_nr_users)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );    /* Nr of filesystems sharing log */
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Block number of dynamic superblock copy : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%u\n", be32_to_cpu((j->s_dynsuper)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );    /* Blocknr of dynamic superblock copy*/
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Limit of journal blocks per transaction : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%u\n", be32_to_cpu((j->s_max_transaction)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );  /* Limit of journal blocks per trans.*/
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Limit of data blocks per transaction : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%u\n", be32_to_cpu((j->s_max_trans_data)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );  /* Limit of data blocks per trans. */

}



void print_journal_header_gui( struct journal_header_s *header ) {

  GtkTextIter iter;
  GtkTextBuffer *buffer;
  gchar *texte;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

  gtk_text_buffer_get_end_iter ( buffer, &iter);


  if ( be32_to_cpu((header->h_magic)) != JFS_MAGIC_NUMBER ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "ERROR ---------------------\n", -1, "text", NULL);
  }
  //  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Journal superblock header\n", -1, "italic", NULL);
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Journal magic number : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%#x, should be %#x \n", be32_to_cpu((header->h_magic)), JFS_MAGIC_NUMBER);
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Journal block type : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%u  -  ", be32_to_cpu((header->h_blocktype)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );
  switch(be32_to_cpu(header->h_blocktype)) {

    case JFS_DESCRIPTOR_BLOCK:
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "JFS_DESCRIPTOR_BLOCK\n", -1, "text", NULL);
      break;

    case JFS_COMMIT_BLOCK:
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "JFS_COMMIT_BLOCK\n", -1, "text", NULL);
      break;

    case JFS_REVOKE_BLOCK:
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "JFS_REVOKE_BLOCK\n", -1, "text", NULL);
      break;

    case JFS_SUPERBLOCK_V1:
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "JFS_SUPERBLOCK_V1\n", -1, "text", NULL);
      break;

    case JFS_SUPERBLOCK_V2:
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "JFS_SUPERBLOCK_V2\n", -1, "text", NULL);
      break;

    default:
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Unexpected block type\n", -1, "text", NULL);
      return;
  }
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Journal sequence : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%u\n\n", be32_to_cpu((header->h_sequence)));
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );
}


//Example: 550e8400-e29b-41d4-a716-446655440000
void  print_UUID_gui(const __u8 * tab_uuid)
{
  GtkTextIter iter;
  GtkTextBuffer *buffer;
  gchar *texte;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

  gtk_text_buffer_get_end_iter ( buffer, &iter);


  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "UUID : ", -1, "bold", NULL);
  texte = g_strdup_printf( "%hx%hx%hx%hx-%hx%hx-%hx%hx-%hx%hx-%hx%hx%hx%hx%hx%hx\n", tab_uuid[0], tab_uuid[1], tab_uuid[2], tab_uuid[3], tab_uuid[4], tab_uuid[5], tab_uuid[6], tab_uuid[7], tab_uuid[8], tab_uuid[9], tab_uuid[10], tab_uuid[11], tab_uuid[12], tab_uuid[13], tab_uuid[14], tab_uuid[15]);
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
  free( texte );

}




int dump_journal_gui ( int fd, int block_size, struct ext3_inode *i ) {

  int cc = 0;

  GtkTextIter iter;
  GtkTextBuffer *buffer;
  //gchar *texte;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

  gtk_text_buffer_get_end_iter ( buffer, &iter);

  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "\nJOURNAL CONTENTS\n", -1, "title2", NULL);
  gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, A_LINE_GUI, -1, "fixed", NULL);

  while( i->i_block[cc] != 0 && cc < EXT3_NDIR_BLOCKS) {
    /* dump all directs blocks */
    dump_journal_block_rec_gui( fd, block_size, i->i_block[cc], 0 );
    cc++;
  }

  /* dump indirect blocks, double indirect and triple indirect blocks */
  if ( i->i_block[EXT3_IND_BLOCK] != 0 )
    dump_journal_block_rec_gui( fd, block_size, i->i_block[EXT3_IND_BLOCK], 1 );
  if ( i->i_block[EXT3_DIND_BLOCK] != 0 )
    dump_journal_block_rec_gui( fd, block_size, i->i_block[EXT3_DIND_BLOCK], 2 );
  if ( i->i_block[EXT3_TIND_BLOCK] != 0 )
    dump_journal_block_rec_gui( fd, block_size, i->i_block[EXT3_TIND_BLOCK], 3 );

  return 0;
}

int dump_journal_block_rec_gui ( int fd, int block_size, __u32 block_num,
    int rec_level ) {

  __u32 *block; /* array to browse an indirect block */
  int i;
  int ret;

  if (  rec_level == 0 ) {

    /* if the current block is a direct block then we dump it */
    ret = dump_journal_block_gui( fd, block_size, block_num);
    if ( ret == -1 ) {
      fprintf( stderr, "!!error dumping the journal!!\n" );
      return -1;
    }
  }
  else {
    /* else we read the block and for each block in it we browse the block
     * or we dump it depending on rec_level
     */
    block = malloc( block_size );
    //fprintf( stderr, "->allocation %.8x niveau %d\n", block, rec_level );
    ret = read_block_pointer ( fd, block_size, block_num, block );
    if ( ret == -1 )
      return -1;

    i = 0;
    while ( i < block_size/4  && block[i] != 0 ) {
      /* each block number is a __u32 */

      ret = dump_journal_block_rec_gui( fd, block_size, block[i], rec_level-1 );
      if ( ret == -1 ) {
        return -1;
      }
      i++;
    }

    //fprintf(stderr, "--->free %.8x niveau %d\n", block, rec_level);
    free( block );

  }
  return 0;

}

int dump_journal_block_gui ( int fd, int block_size, __u32 block_num ) {

  /* dump a given block of the journal ( called by dump_journal_block_rec()
   * which will read recursively each block, indirect block, double ind...
   */

  int offset = 0;
  struct journal_block_tag_s jb_tag;

  //struct journal_superblock_s jsb;
  struct journal_revoke_header_s j_rheader;
  off_t lret;
  ssize_t sret;

  GtkTextIter iter;
  GtkTextBuffer *buffer;
  gchar *texte;

  /*
   * In the journal superblock is a a field named block_size i don't know if
   * it could be different that the ext3_super_block blocksize it could be
   * use just for compatibility => jbd can be used with other fs than
   * ext3.
   */

  /* we only read revoke headers because it's the same structure that a normal
   * header but it contains one more field. We just ignore it if it is not
   * a revoke header but just a normal header
   */
  read_journal_revoke_header( fd, block_size, block_num, 0, &j_rheader );
  if ( be32_to_cpu((j_rheader.r_header.h_magic)) == JFS_MAGIC_NUMBER ) {

    /* if the journal block header we red is correct let's dump it */
    print_journal_revoke_header_gui(&j_rheader);
    if ( be32_to_cpu(j_rheader.r_header.h_blocktype) ==
        JFS_DESCRIPTOR_BLOCK ) {

      /* if the readed block is actually a descriptor block, then we have
       * to read all the block tags that are close to it.
       */

      offset = sizeof( struct journal_header_s ); /* used to read the next
                                                     journal block tag */

      do {

        lret = lseek( fd, (__u64)(block_num)*block_size + offset, SEEK_SET );
        if ( lret == -1 ) {
          perror("lseek dump journal block");
          return -1;
        }
        sret = read( fd, &jb_tag, sizeof(struct journal_block_tag_s ) );
        if ( lret == -1 ) {
          perror("read dump journal block");
          return -1;
        }

        // Get the buffer
        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

        // Write in the text area
        gtk_text_buffer_get_end_iter ( buffer, &iter);

        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Concerned block number  : ", -1, "bold", NULL);
        texte = g_strdup_printf( "%u\n", be32_to_cpu(jb_tag.t_blocknr));
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
        free( texte );
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "flag : ", -1, "bold", NULL);
        texte = g_strdup_printf( "%u\n", be32_to_cpu(jb_tag.t_flags));
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
        free( texte );

        if ( be32_to_cpu(jb_tag.t_flags) & JFS_FLAG_ESCAPE ) {
          gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "JFS_FLAG_ESPACE : ", -1, "bold", NULL);
          texte = g_strdup_printf( "on-disk block is escaped\n");
          gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
          free( texte );
        }
        if ( be32_to_cpu(jb_tag.t_flags) & JFS_FLAG_SAME_UUID ) {
          gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "JFS_FLAG_SAME_UUID : ", -1, "bold", NULL);
          texte = g_strdup_printf( "block has same uuid as previous\n");
          gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
          free( texte );
        }
        else
          offset += 16;
        /* i read that somewhere but don't know why, maybe some datas are
         * stored after the block when it does not have the same uuid
         */
        if ( be32_to_cpu(jb_tag.t_flags) & JFS_FLAG_DELETED) {
          gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "JFS_FLAG_DELETED : ", -1, "bold", NULL);
          texte = g_strdup_printf( "block deleted by this transaction\n");
          gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
          free( texte );

        }
        if ( be32_to_cpu(jb_tag.t_flags) & JFS_FLAG_LAST_TAG ) {
          gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "JFS_FLAG_LAST_TAG : ", -1, "bold", NULL);
          texte = g_strdup_printf( "last tag in this descriptor block\n");
          gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
          free( texte );
        }


        offset += sizeof( struct journal_block_tag_s );
      }
      while (  !((be32_to_cpu(jb_tag.t_flags) & JFS_FLAG_LAST_TAG))  &&
          offset < block_size );
      /* we stop dumping tags as soon as we lseek a complete block or
       * if we met the JFS_FLAG_LAST_TAG
       */
    }

    // Get the buffer
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

    // Write in the text area
    gtk_text_buffer_get_end_iter ( buffer, &iter);
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, A_LINE_GUI, -1, "fixed", NULL);
  }
  return 0;
}

void print_journal_revoke_header_gui( struct journal_revoke_header_s *header ) {

  GtkTextIter iter;
  GtkTextBuffer *buffer;
  gchar *texte;

  print_journal_header_gui( &header->r_header );

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

  gtk_text_buffer_get_end_iter ( buffer, &iter);

  if ( be32_to_cpu(header->r_header.h_blocktype) == JFS_REVOKE_BLOCK ) {
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "Count of bytes used in the block : ", -1, "bold", NULL);
    texte = g_strdup_printf( "%u\n", be32_to_cpu(header->r_count));
    gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
    free( texte );
  }


}

void print_symlink_gui ( int fd, struct ext3_super_block *sb, __u32 i_num ) {

  struct ext3_inode i;
  unsigned int c = 0; /* counter */
  char l = 'a';
  char *dest;


  GtkTextIter iter, start, end;
  GtkTextBuffer *buffer;
  gchar *texte;

  if ( !fs.isopen ) {

    show_warning_dialog("You cannot access to this functionality :\nno "
        "filesystem is open.");

  }
  else {

    read_inode( fd, sb, i_num, &i); /* read the file inode */

    if ( !S_ISLNK(i.i_mode) ) {

      show_warning_dialog("This is not a symlink.\n\nPlease select a symlink to use this functionality.");

    }
    else {

      buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (centre.texte));

      gtk_text_buffer_get_bounds ( buffer, &start, &end);
      gtk_text_buffer_delete ( buffer, &start, &end );

      gtk_text_buffer_get_end_iter ( buffer, &iter);


      texte = g_strdup_printf( "Showing the contents of symlink (inode #%d)\n\n", i_num);
      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "title", NULL);
      free( texte );

      gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "This inode represents a symlink. If it links to a path < 60 chars, it is stored directly in ext3_inode.i_block. Else it is stored in allocated blocks to the inode\n\n", -1, "citalic", NULL);

      /* now we'll read the symlink : if the size of the string is less than 60,
         we read i.i_block[] as a char[]. Else we can read the block as for a
         regular file */
      if ( i.i_size < 60 ) {
        dest = (char *)i.i_block;
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "This inode points to: ", -1, "bold", NULL);
        while ( l != '\0' && c < 60 ) {
          texte = g_strdup_printf( "%c", dest[c]);
          gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
          free(texte);
          c++;
        }
      }
      else {
        gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, "This symlink points to a path > 60 characters. To show it, you must use the block display instead", -1, "title2", NULL );
      }


    }
  }
}

