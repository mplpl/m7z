PROG=sample
M7Z_DIR=../

CXXFLAGS += -std=c++11 -stdlib=libc++

.PHONY: all
all: $(PROG)

$(PROG): Main.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread ../lib7z.a -framework CoreFoundation -all_load $^ -o $@

Main.o : Main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(M7Z_DIR) -g -O0 -c Main.cpp

.PHONY: clean
clean:
	-@$(RM) Main.o $(PROG)
