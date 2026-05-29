SHELL = /bin/sh

CC = g++

#Debug compile
CFLAGS = -g -I./includes -std=c++17 -fPIC

INCLUDE1 = 
INCLUDE = 

OBJS = adler32.o \
       ARACrypt.o \
       BuildConfig.o \
       COMPRESS.o \
       CRC32.o \
       CStr.o \
       DBConPool.o \
       dbquery.o \
       DebugInfo.o \
       DEFLATE.o \
       diskid32.o \
       DLP_Color.o \
       DLPFile.o \
       DLPRect.o \
       dlprnd.o \
       driveInfo.o \
       FileInfo.o \
       INFBLOCK.o \
       INFCODES.o \
       INFFAST.o \
       INFLATE.o \
       INFTREES.o \
       INFUTIL.o \
       KeyAuthObj.o \
       Markup.o \
       NetMutex.o \
       NetTimer.o \
       NewCol.o \
       NToklib.o \
       OCRC32.o \
       PoolObj.o \
       Profiler.o \
       stringpool.o \
       TimerManObj.o \
       treeprint.o \
       TREES.o \
       uidmanager.o \
       uniqueid.o \
       UNCOMPR.o \
       ZUTIL.o

DLPUtilLib2: DLPUtilLib2.a

DLPUtilLib2.a: $(OBJS)
	$(AR) rcs DLPUtilLib2.a $(OBJS)

%.o: src/%.cpp includes/*.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: src/%.C includes/*.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: src/%.c includes/*.h
	$(CC) $(CFLAGS) -c $< -o $@

all: DLPUtilLib2.a

clean:
	rm -v *.o DLPUtilLib2.a
