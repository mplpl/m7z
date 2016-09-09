DLIB=lib7z.a

P7ZIP_PATH=p7zip/p7zip_src

CXX=c++
CC=cc

CXXFLAGS= \
	-m64 \
	-c \
	-I. \
	-I$(P7ZIP_PATH)/CPP/ \
	-I$(P7ZIP_PATH)/CPP/myWindows \
	-I$(P7ZIP_PATH)/CPP/include_windows \
	-D_FILE_OFFSET_BITS=64 \
	-D_LARGEFILE_SOURCE \
	-D_REENTRANT \
	-DENV_MACOSX \
	-DBREAK_HANDLER \
	-DUNICODE \
	-D_UNICODE \
	-DUNIX_USE_WIN_FILE \
	-Wno-logical-op-parentheses \
	-Wno-c++11-extensions

CXXFLAGS_REL= \
	-O

CXXFLAGS_DBG= \
	-g \
	-O0

LDFLAGS=\
	-all_load

OBJS = \
	lib7z.o \
	MLOpenCallback.o \
	MLExtractCallbackWrapper.o \
	MLUpdateCallbackWrapper.o \

RM=rm -f

LIBS=-framework CoreFoundation

.PHONY:all
all: release sample unittest

.PHONY:alldebug
alldebug: debug sample unittest

.PHONY: release
release: CXXFLAGS += $(CXXFLAGS_REL)
release: TARGET = release
release: $(DLIB)

.PHONY: debug
debug: CXXFLAGS += $(CXXFLAGS_DBG)
debug: TARGET = debug
debug: $(DLIB)

$(DLIB): $(OBJS)
	-@make -C p7zip $(TARGET)
	libtool -static -o $(DLIB) p7zip/libp7zip.a $(OBJS) $(LIBS) -v

p7zip/libp7zip.a:
	-@make -C p7zip

.PHONY: sample
sample: $(DLIB)
	-@make -C sample

.PHONY: unittest
unittest: $(DLIB)
	-@make -C unittest

.PHONY: runtests
runtests: unittest
	unittest/lib7ztest

.PHONY: clean
clean:
	-@make -C sample clean
	-@make -C unittest clean
	-@$(RM) $(DLIB) core *.exe *.o *~ .*.swp *.orig *.gch $(PROGS)

.PHONY: clean_all
clean_all: clean
	-@make -C p7zip clean
	-@$(RM) p7zip/libp7zip.a
