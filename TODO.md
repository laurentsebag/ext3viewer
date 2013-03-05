There is still a few things to correct and some features could be added:

 - cleaning code : a lot of the source code should be cleaned ; output
   functions have to be rewritten so that the same functions for the gui and
   the console could be called

 - find more details and specs about jbd

 - implement a complete acl functionality

 - implement help functions in the gui : it could explain how the filesystem
   works... like lesson for IT students

 - improve interactivity in the gui. eg.: block number could be links for
   bpoint

 - implement input of files as a string. ex: /etc/foo.conf

 - internationalization

 - see if it is possible to use e2fs headers instead of ext3_fs.h like
   described in linux 3.x commit (ext3_fs.h is no longer exported by the
   kernel).

 - more information about journaling

 - support for ext4

 - add english devel doc (French version on sourceforge.net repo)

Laurent Sebag <ext3viewer@free.fr>
