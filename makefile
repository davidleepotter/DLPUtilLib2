SHELL = /bin/sh

CC = g++

#Debug compile
CFLAGS = -g -I./includes
 
#Optimize compile
CFLAGS1 = -O

SRCDIR = ./src
HDRDIR = ./includes

#Profile
CFLAGS2 = -pg

INCLUDE1 = /home/dpotter/netman/netLib/mysqllinux
INCLUDE = /home/dpotter/netman/netLib/mysqllinux

OBJS1 = cstr.o netmutex.o dbase.o dbflatfile.o newcol.o compress.o uncompr.o aracrypt.o dbquery.o dbconpool.o
OBJS2 = nettimer.o ocrc32.o timermanobj.o dbmysql.o debuginfo.o ntoklib.o stringpool.o poolobj.o dlprnd.o
OBJS3 = adler32.o crc32.o deflate.o infblock.o infcodes.o inffast.o inflate.o inftrees.o infutil.o trees.o zutil.o

DLPUtilLib2: DLPUtilLib2.a cpy

DLPUtilLib2.a: $(OBJS1) $(OBJS2) $(OBJS3) netlib1 netlib2 netlib3
		
netlib1: $(OBJS1) 
		ar -rc DLPUtilLib2.a $(OBJS1)

netlib2: $(OBJS2) 
		ar -rc DLPUtilLib2.a $(OBJS2)

netlib3: $(OBJS3) 
		ar -rc DLPUtilLib2.a $(OBJS3)

ntoklib.o:$(SRCDIR)/ntoklib.cpp $(HDRDIR)/ntoklib.h 
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/ntoklib.cpp

ocrc32.o: $(SRCDIR)/ocrc32.cpp $(HDRDIR)/ocrc32.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/ocrc32.cpp

cstr.o: $(SRCDIR)/cstr.cpp $(HDRDIR)/cstr.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/cstr.cpp

nettimer.o: $(SRCDIR)/nettimer.cpp $(HDRDIR)/nettimer.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/nettimer.cpp

timermanobj.o: $(SRCDIR)/timermanobj.cpp $(HDRDIR)/timermanobj.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/timermanobj.cpp

newcol.o: $(SRCDIR)/newcol.cpp $(HDRDIR)/newcol.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/newcol.cpp

netmutex.o: $(SRCDIR)/netmutex.cpp $(HDRDIR)/netmutex.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/netmutex.cpp

dbase.o: $(SRCDIR)/dbase.cpp $(HDRDIR)/dbase.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/dbase.cpp

dbmysql.o: $(SRCDIR)/dbmysql.cpp $(HDRDIR)/dbmysql.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/dbmysql.cpp

dbflatfile.o: $(SRCDIR)/dbflatfile.cpp $(HDRDIR)/dbflatfile.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/dbflatfile.cpp

debuginfo.o: $(SRCDIR)/debuginfo.cpp $(HDRDIR)/debuginfo.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/debuginfo.cpp

adler32.o: $(SRCDIR)/adler32.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/adler32.c
 
crc32.o: $(SRCDIR)/crc32.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/crc32.c

deflate.o: $(SRCDIR)/deflate.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/deflate.c

infblock.o: $(SRCDIR)/infblock.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/infblock.c

infcodes.o: $(SRCDIR)/infcodes.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/infcodes.c

inffast.o: $(SRCDIR)/inffast.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/inffast.c

inflate.o: $(SRCDIR)/inflate.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/inflate.c

inftrees.o: $(SRCDIR)/inftrees.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/inftrees.c

infutil.o: $(SRCDIR)/infutil.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/infutil.c

trees.o: $(SRCDIR)/trees.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/trees.c

zutil.o: $(SRCDIR)/zutil.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/zutil.c

compress.o: $(SRCDIR)/compress.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/compress.c

uncompr.o: $(SRCDIR)/uncompr.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/uncompr.c

aracrypt.o: $(SRCDIR)/aracrypt.cpp
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/aracrypt.cpp

dlprnd.o: $(SRCDIR)/dlprnd.cpp
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/dlprnd.cpp

stringpool.o: $(SRCDIR)/stringpool.cpp
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/stringpool.cpp

poolobj.o: $(SRCDIR)/poolobj.cpp
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/poolobj.cpp

dbquery.o: $(SRCDIR)/dbquery.cpp
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/dbquery.cpp

dbconpool.o: $(SRCDIR)/dbconpool.cpp
	$(CC) -I$(INCLUDE) $(CFLAGS) -c $(SRCDIR)/dbconpool.cpp

all: DLPUtilLib2.a cpy

clean:
	rm -v *.o

cpy:
	mv -v *.a ../libs/



