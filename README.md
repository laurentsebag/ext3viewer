ext3Viewer
==========
ext3viewer is a low-level ext3 explorer.
It lets you study all the ext3 structures and prints them in a more
user-friendly form.

Go to http://github.com/laurentsebag/ext3viewer to find the latest release.
Send all ext3Viewer-related subjects to <ext3viewer@free.fr>.

Building ext3Viewer
-------------------
First get the latest code from the git repository:
  `git clone http://github.com/laurentsebag/ext3viewer`

Then build the project with the following command:
  `cd ext3Viewer; make`

Then install ext3Viewer and gext3Viewer with:
  `sudo make install`

Using ext3Viewer
----------------
_ext3Viewer_ is the console program, and _gext3Viewer_ is the graphical
interface.

Try using `ext3Viewer filesystem -stats` to get the superblock information.
`ext3Viewer -help` will give you a short list of the available options.
More details in the man page: man 1 ext3Viewer

Or run `gext3viewer` without arguments.

Project organisation
--------------------
Current directory is the ext3viewer folder:

  - _./        :_ contains makefile, README, INSTALL files, test filesystem image,
    and executable if compiled but not installed.
  - _./doc/    :_ contains user and developer documentation, and LaTeX sources.
  - _./man/    :_ contains manpages.
  - _./src/    :_ contains source code and internal makefile for the console mode.
  - _./src/gui :_ contains source code, images,
    and internal makefile for the graphical mode.

Libraries and versions needed
-----------------------------
This project requires and is known to work with:

  * GTK+ developement headers (at least version 2.0) and library
   (at least version 2.6)
  * GNU/Linux

