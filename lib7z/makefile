PROG=test
DLIB=lib7z.a
include makefile.p7zip

LOCAL_FLAGS=\
  -DBREAK_HANDLER \
  -DUNICODE -D_UNICODE \
	-g \
	-O0

LDFLAGS=\
	-all_load

#include $(P7ZIP_PATH)/makefile.macosx_64bits

OPTFLAGS=-O

ALLFLAGS=-m64 ${OPTFLAGS}  \
	-DENV_MACOSX \
  -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE \
	-D_REENTRANT -DENV_UNIX \
	$(LOCAL_FLAGS)

CXX=c++ $(ALLFLAGS)
CC=cc $(ALLFLAGS)
LINK_SHARED=-bundle
LIBS=-framework CoreFoundation

OBJS = \
  lib7z.o \
	MLOpenCallback.o \
	MLExtractCallbackWrapper.o \
	MLUpdateCallbackWrapper.o \
	MLCallbackTest.o \

#include makefile.glb

RM=rm -f

CXXFLAGS=-c -I. \
	-I$(P7ZIP_PATH)/CPP/myWindows \
	-I$(P7ZIP_PATH)/CPP/ \
	-I$(P7ZIP_PATH)/CPP/include_windows

all: $(PROG) $(DLIB)

$(DLIB): p7zip/libp7zip.a Main.o $(OBJS)
	libtool -static -o $(DLIB) p7zip/libp7zip.a $(OBJS) $(LIBS) -v

$(PROG): $(DLIB) Main.o
	$(CXX) $(LOCAL_SHARED) -o $(PROG) Main.o $(LDFLAGS) $(DLIB) $(LIBS)

p7zip/libp7zip.a:
	-@make -C p7zip

pre:
	-@make -C p7zip
	-@make -C p7zip clean

clean:
	-@$(RM) $(DLIB) $(PROG) core *.exe *.o *~ .*.swp *.orig *.gch $(PROGS)
	-@$(RM) -fr SunWS_cache/
	-@$(RM) -r .inslog2 pchdir/ tca.map ir.out
	-@$(RM) make.log tags 1 2

clean_all: clean
	-@make -C p7zip clean
	-@$(RM) p7zip/libp7zip.a
