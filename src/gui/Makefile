TARGET=../../gext3Viewer
OFILES= ../inode.o ../journal.o ../search.o ../groups.o ../acl.o ../block.o \
	../path.o ../filesystem.o ../superblock.o \
	output_gui.o close_gui.o menu_gui.o open_gui.o \
        tree_gui.o tree_menu_gui.o search_gui.o main_gui.o
CFLAGS=-Wall `pkg-config gtk+-2.0 --cflags` -DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGDK_PIXBUF_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED -D_FILE_OFFSET_BITS=64 -DIMG_PATH=\"$(INSTALL_DIR)/\"   
LDFLAGS=`pkg-config gtk+-2.0 --libs`
CC=gcc
all: $(TARGET)

$(TARGET): $(OFILES)
	$(CC) $(CFLAGS) -o $@ $(OFILES) $(LDFLAGS)

%.o : %.c %.h Makefile 
	$(CC) $(CFLAGS) -c -o $@ $<

depend:
	makedepend *.c
	rm -f Makefile.bak

clean: 
	rm -f *.o *~ $(TARGET)

mrproper: clean

.PHONY: clean mrproper depend all

