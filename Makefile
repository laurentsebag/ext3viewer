##################
# DEFAULT CONFIG #
##################
#INSTALL_DIR= /usr/local/share/ext3Viewer/
#BIN_DIR= /usr/local/bin/
#MAN_DIR=/usr/share/man/man1/
#USER=root
#GROUP=root


#######################################
##EXEMPLE OF CONFIG FOR NON ROOT USERS#
#######################################
INSTALL_DIR=~/.local/share/ext3Viewer
# BIN_DIR: optional path to put binaries into. Must be different than
# INSTALL_DIR and should be in your path.
BIN_DIR=~/roro/
MAN_DIR=~/.local/share/man/
USER=laurent
GROUP=users

all: ext3Viewer gui #man doc

ext3Viewer: src/*.c src/*.h src/Makefile
	make -C src/ all
	
doc:
	make -C doc/devel/
	make -C doc/rapport/
gui:
	INSTALL_DIR=$(INSTALL_DIR) make -e -C src/gui/
man:
	make -C man/
install: ext3Viewer gui
	install -g $(GROUP) -o $(USER) -m 0755 -d $(INSTALL_DIR)/;
	install -g $(GROUP) -o $(USER) -m 0755 -d $(INSTALL_DIR)/bin/;
	install -g $(GROUP) -o $(USER) -m 0755 -d $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0755 -d $(INSTALL_DIR)/img/tree_menu/;
	test -d $(MAN_DIR) || install -g $(GROUP) -o $(USER) -m 0755 -d $(MAN_DIR)/
	if [ -n "$(BIN_DIR)" ];then \
		test -d $(BIN_DIR) || install -g $(GROUP) -o $(USER) -m 0755 -d $(BIN_DIR)/;\
	fi
	install -g $(GROUP) -o $(USER) -m 0555 ./ext3Viewer $(INSTALL_DIR)/bin/;
	install -g $(GROUP) -o $(USER) -m 0555 ./gext3Viewer $(INSTALL_DIR)/bin/;
	install -g $(GROUP) -o $(USER) -m 0444 ./COPYING $(INSTALL_DIR)/;
	install -g $(GROUP) -o $(USER) -m 0444 ./README $(INSTALL_DIR)/;
	install -g $(GROUP) -o $(USER) -m 0444 ./VERSION $(INSTALL_DIR)/;
	install -g $(GROUP) -o $(USER) -m 0444 ./man/ext3Viewer.1 $(MAN_DIR)/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/about.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/blockdevice.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/chardevice.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/close.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/directory.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/exec.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/exit.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/file.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/help.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/icon.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/info.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/open.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/pipe.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/socket.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/symlink.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/unknown.png $(INSTALL_DIR)/img/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/tree_menu/contents.png $(INSTALL_DIR)/img/tree_menu;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/tree_menu/superblock.png $(INSTALL_DIR)/img/tree_menu;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/tree_menu/group_desc.png $(INSTALL_DIR)/img/tree_menu;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/tree_menu/cat.png $(INSTALL_DIR)/img/tree_menu;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/tree_menu/file.png $(INSTALL_DIR)/img/tree_menu;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/tree_menu/stat.png $(INSTALL_DIR)/img/tree_menu;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/tree_menu/tree.png $(INSTALL_DIR)/img/tree_menu;
	if [ -n "$(BIN_DIR)" ];then \
		ln -f -s $(INSTALL_DIR)/bin/ext3Viewer $(BIN_DIR)/ext3Viewer;\
		ln -f -s $(INSTALL_DIR)/bin/gext3Viewer $(BIN_DIR)/gext3Viewer;\
	fi

uninstall:
	rm -f $(BIN_DIR)/ext3Viewer
	rm -f $(BIN_DIR)/gext3Viewer
	rm -f $(INSTALL_DIR)/img/tree_menu/tree.png
	rm -f $(INSTALL_DIR)/img/tree_menu/stat.png
	rm -f $(INSTALL_DIR)/img/tree_menu/file.png
	rm -f $(INSTALL_DIR)/img/tree_menu/cat.png
	rm -f $(INSTALL_DIR)/img/tree_menu/group_desc.png
	rm -f $(INSTALL_DIR)/img/tree_menu/superblock.png
	rm -f $(INSTALL_DIR)/img/tree_menu/contents.png
	rm -f $(INSTALL_DIR)/img/unknown.png
	rm -f $(INSTALL_DIR)/img/symlink.png
	rm -f $(INSTALL_DIR)/img/socket.png
	rm -f $(INSTALL_DIR)/img/pipe.png
	rm -f $(INSTALL_DIR)/img/open.png
	rm -f $(INSTALL_DIR)/img/info.png
	rm -f $(INSTALL_DIR)/img/icon.png
	rm -f $(INSTALL_DIR)/img/help.png
	rm -f $(INSTALL_DIR)/img/file.png
	rm -f $(INSTALL_DIR)/img/exit.png
	rm -f $(INSTALL_DIR)/img/exec.png
	rm -f $(INSTALL_DIR)/img/directory.png
	rm -f $(INSTALL_DIR)/img/close.png
	rm -f $(INSTALL_DIR)/img/chardevice.png
	rm -f $(INSTALL_DIR)/img/blockdevice.png
	rm -f $(INSTALL_DIR)/img/about.png
	rm -f $(MAN_DIR)/man/ext3Viewer.1
	rm -f $(INSTALL_DIR)/VERSION
	rm -f $(INSTALL_DIR)/README
	rm -f $(INSTALL_DIR)/COPYING
	rm -f $(INSTALL_DIR)/bin/gext3Viewer
	rm -f $(INSTALL_DIR)/bin/ext3Viewer
	if [ -n "$(BIN_DIR)" ];then \
		if [ -z "$(ls -A $(BIN_DIR) 2>/dev/null)" ]; then \
			rmdir $(BIN_DIR)/;\
		fi;\
	fi
	if [ -z "$$(ls -A $(MAN_DIR) 2>/dev/null)" ]; then rmdir $(MAN_DIR);fi
	if [ -z "$$(ls -A $(INSTALL_DIR)/img/tree_menu/ 2>/dev/null)" ]; then \
		rmdir $(INSTALL_DIR)/img/tree_menu;\
	fi
	if [ -z "$$(ls -A $(INSTALL_DIR)/img 2>/dev/null)" ]; then rmdir $(INSTALL_DIR)/img;fi
	if [ -z "$$(ls -A $(INSTALL_DIR)/bin 2>/dev/null)" ]; then rmdir $(INSTALL_DIR)/bin;fi
	if [ -z "$$(ls -A $(INSTALL_DIR) 2>/dev/null)" ]; then rmdir $(INSTALL_DIR);fi

depend:
	make -C src/ depend
	make -C src/gui/ depend
clean:
	rm -f *~
	make -C src/ clean
	make -C doc/devel clean
	make -C doc/rapport clean
	make -C src/gui/ clean
	make -C man/ clean

.PHONY: clean all doc gui depend man install uninstall

