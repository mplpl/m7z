PROG=libp7zip.a

P7ZIP_PATH=p7zip_src

COMMON_FLAGS= \
	-I$(P7ZIP_PATH)/CPP/myWindows \
	-I$(P7ZIP_PATH)/CPP/include_windows \
	-I$(P7ZIP_PATH)/CPP/ \
	-DBREAK_HANDLER \
	-DUNICODE \
	-D_UNICODE \
	-DUNIX_USE_WIN_FILE

include $(P7ZIP_PATH)/makefile.crc32
include $(P7ZIP_PATH)/makefile.macosx_llvm_64bits

CFLAGS=-c -I. \
	-I$(P7ZIP_PATH)/C \
	$(COMMON_FLAGS) \
	$(ALLFLAGS) $(ALLFLAGS_C)

CXXFLAGS=-c -I. \
	$(COMMON_FLAGS) \
	$(ALLFLAGS) $(ALLFLAGS_CPP)

REL_FLAGS= \
	-O

DBG_FLAGS= \
	-g \
	-O0

LDFLAGS=\
	-all_load


LIBS=$(LOCAL_LIBS_DLL)

include makefile.list

RM=rm -f

.PHONY: release
release: CXXFLAGS += $(REL_FLAGS)
release: CLAGS += $(REL_FLAGS)
release: $(PROG)

.PHONY: debug
debug: CXXFLAGS += $(DBG_FLAGS)
debug: CFLAGS += $(DBG_FLAGS)
debug: $(PROG)

$(PROG): $(OBJS)
	libtool -static -o $(PROG) $(OBJS) $(LIBS) -v

include makefile.rules

.PHONY: clean
clean:
	-@$(RM) core *.exe *.o *~ .*.swp *.orig *.gch $(PROGS)
	-@$(RM) -fr SunWS_cache/
	-@$(RM) -r .inslog2 pchdir/ tca.map ir.out
	-@$(RM) make.log tags 1 2
