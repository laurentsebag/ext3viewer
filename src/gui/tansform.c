
 gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Behaviour when detecting errors : ", -1, "bold", NULL);
texte = g_strdup_printf("%hu\n",sb->s_errors);//Traduire!
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    print_errorbehavior(sb->s_errors);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Minimum compatible revision level (version) : ", -1, "bold", NULL);
texte = g_strdup_printf("%hu\n",sb->s_minor_rev_level);//Traduire
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    print_revlev(sb->s_minor_rev_level);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Last check timestamp : ", -1, "bold", NULL);
texte = g_strdup_printf("%u\n",sb->s_lastcheck);
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    print_time(sb->s_lastcheck);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Maximum interval between checks : ", -1, "bold", NULL);
texte = g_strdup_printf("%u secondes\n",sb->s_checkinterval);
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Creator OS : ", -1, "bold", NULL);
texte = g_strdup_printf("%u\n",sb->s_creator_os);//Determiner OS
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    print_creatorOS(sb->s_creator_os);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Current revision level (version) : ", -1, "bold", NULL);
texte = g_strdup_printf("%u\n",sb->s_rev_level);//Traduire
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    print_revlev(sb->s_rev_level);


    printf("-------------------------------------------------------\n");


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Default UID for reserved blocks : ", -1, "bold", NULL);
texte = g_strdup_printf("%hu\n",sb->s_def_resuid);
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Default GID for reserved blocks : ", -1, "bold", NULL);
texte = g_strdup_printf("%hu\n",sb->s_def_resgid);
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    //  if(sb->s_minor_rev_level==EXT3_DYNAMIC_REV)
    // {

    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "First non-reserved inode number : ", -1, "bold", NULL);
texte = g_strdup_printf("%u\n",sb->s_first_ino);
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Inode size : ", -1, "bold", NULL);
texte = g_strdup_printf("%hu\n", sb->s_inode_size);
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Superblock block number (being read) : ", -1, "bold", NULL);
texte = g_strdup_printf("%hu\n", sb->s_block_group_nr);
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
//  gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "s_block_group_nr : ", -1, "bold", NULL);
texte = g_strdup_printf("%hd\n", sb->s_block_group_nr);//Ecrit deux fois?
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);

    printf("-------------------------------------------------------\n");


    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Compatible features set : ", -1, "bold", NULL);
texte = g_strdup_printf("%#.4x\n", sb->s_feature_compat);//Verifier
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    print_featcompat(sb->s_feature_compat);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Incompatible features set : ", -1, "bold", NULL);
texte = g_strdup_printf("%#.4x\n", sb->s_feature_incompat);//Verifier
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    print_featincompat(sb->s_feature_incompat);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Read-Only Compatible features set : ", -1, "bold", NULL);
texte = g_strdup_printf("%#.4x\n", sb->s_feature_ro_compat);//Verifier
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    print_featROcompat(sb->s_feature_ro_compat);
    /*	__u8	s_uuid[16];*/

    printf("-------------------------------------------------------\n");


    printf("Volume name = \"%s\"\n",sb->s_volume_name);
    printf("Last mounted in directory \"%s\"\n",sb->s_last_mounted);

    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Compression usage bitmap : ", -1, "bold", NULL);
texte = g_strdup_printf("%u\n", sb->s_algorithm_usage_bitmap);//Verifier
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Number of blocks to try to preallocate : ", -1, "bold", NULL);
texte = g_strdup_printf("%hu\n", sb->s_prealloc_blocks); // 8bits /!\  //Verifier
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Number of blocks to preallocate for direcotires : ", -1, "bold", NULL);
texte = g_strdup_printf("%hu\n", sb->s_prealloc_dir_blocks);// 8bits
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    // gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Alignement sur le mot : ", -1, "bold", NULL);
texte = g_strdup_printf("%hd\n", sb->s_padding1);//Pour equilibrer la structure? a  verifier
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);

    printf("-------------------------------------------------------\n");

    // if(mask(sb->s_feature_compat,EXT3_FEATURE_COMPAT_HAS_JOURNAL))//Ces champs ne sont utilises que si la journalisation est activee???
    //{
    /*	__u8	s_journal_uuid[16];*///CF     http://en.wikipedia.org/wiki/UUID

//550e8400-e29b-41d4-a716-446655440000
    print_UUID(&(sb->s_journal_uuid[16]));

    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Journal Inode number : ", -1, "bold", NULL);
texte = g_strdup_printf("%u\n", sb->s_journal_inum);	/* inode number of journal file */
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Journal device number : ", -1, "bold", NULL);
texte = g_strdup_printf("%u\n", sb->s_journal_dev);	/* device number of journal file */
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Start of list of orphaned inodes to delete : ", -1, "bold", NULL);
texte = g_strdup_printf("%u\n", sb->s_last_orphan);	/* start of list of inodes to delete */
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    //}
   
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "HTREE default hash seed : ", -1, "bold", NULL);
texte = g_strdup_gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "%x|%x|%x|%x\n", sb->s_hash_seed[0],sb->s_hash_seed[1],sb->s_hash_seed[2],sb->s_hash_seed[3]);       //http://www.linuxsymposium.org/2002/view_txt.php?talk=3&text=abstract
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
// __u8	s_def_hash_version;
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Hash default version : ", -1, "bold", NULL);
texte = g_strdup_printf("%hd\n", sb->s_def_hash_version);	/*
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
									  __u8	s_reserved_char_pad;
									  __u16	s_reserved_word_pad;
									*/
    // gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "s_reserved_word_pad : ", -1, "bold", NULL);
texte = g_strdup_printf("%hd\n", sb->s_reserved_word_pad);
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "Default mount options : ", -1, "bold", NULL);
texte = g_strdup_printf("%u\n", sb->s_default_mount_opts);//Traduire
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    print_mountoptions(sb->s_default_mount_opts);
    gtk_text_buffer_insert_with_tags_by_name( buffer, &iter, "First metablock block group : ", -1, "bold", NULL);
texte = g_strdup_printf("%u\n", sb->s_first_meta_bg);//??
gtk_text_buffer_insert_with_tags_by_name ( buffer, &iter, texte, -1, "text", NULL);
free(texte);
    /*	__u32	s_reserved[190];*/
    //printf(" = \n", sb->);

//}//Fin des champs 
