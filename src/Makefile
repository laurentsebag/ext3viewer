TARGET=../ext3Viewer
OFILES=acl.o filesystem.o inode.o search.o block.o groups.o journal.o path.o \
       superblock.o main.o 
CFLAGS=-D_FILE_OFFSET_BITS=64 -Wall #-DEFENCE#-gstabs #-DDEBUG -DDEBUG_VERBOSE #-pedantic
LDFLAGS= #-lefence
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

.PHONY: clean mrproper depend all doc

