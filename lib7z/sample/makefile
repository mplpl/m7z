PROG=sample
M7Z_DIR=../


all: $(PROG)

$(PROG): Main.o 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread ../lib7z.a -framework CoreFoundation -all_load $^ -o $@

Main.o :
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(M7Z_DIR) -g -O0 -c Main.cpp

clean:
	-@$(RM) Main.o $(PROG)
