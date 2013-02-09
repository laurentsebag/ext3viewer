##################
# DEFAULT CONFIG #
##################
INSTALL_DIR= /usr/local/share/ext3Viewer/
BIN_DIR= /usr/local/bin/
MAN_DIR=/usr/share/man/man1/
USER=root
GROUP=root


#######################################
##EXEMPLE OF CONFIG FOR NON ROOT USERS#
#######################################
##Path must be absolute !
#INSTALL_DIR=/home/heavenman/test/ext3Viewer
#BIN_DIR=/home/heavenman/test/bin/
## must be different that INSTALL_DIR and should be in your
## path
#MAN_DIR=/home/heavenman/test/man/
#USER=heavenman
#GROUP=users

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
	test -d $(BIN_DIR) || install -g $(GROUP) -o $(USER) -m 0755 -d $(BIN_DIR)/
	install -g $(GROUP) -o $(USER) -m 0555 ./ext3Viewer $(INSTALL_DIR)/bin/;
	install -g $(GROUP) -o $(USER) -m 0555 ./gext3Viewer $(INSTALL_DIR)/bin/;
	install -g $(GROUP) -o $(USER) -m 0444 ./COPYING $(INSTALL_DIR)/;
	install -g $(GROUP) -o $(USER) -m 0444 ./README $(INSTALL_DIR)/;
	install -g $(GROUP) -o $(USER) -m 0444 ./VERSION $(INSTALL_DIR)/;
	install -g $(GROUP) -o $(USER) -m 0444 ./man/ext3Viewer.1 $(MAN_DIR)/;
	install -g $(GROUP) -o $(USER) -m 0444 ./src/gui/img/about.png \$(INSTALL_DIR)/img/;
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
	ln -f -s $(INSTALL_DIR)/bin/ext3Viewer $(BIN_DIR)/ext3Viewer;
	ln -f -s $(INSTALL_DIR)/bin/gext3Viewer $(BIN_DIR)/gext3Viewer;

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

.PHONY: clean all doc gui depend man install

