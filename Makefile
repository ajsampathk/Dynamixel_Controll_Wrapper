CC= g++
STD = c++11
CCFLAGS     = $(INCLUDES) $(FORMAT) -g
CXFLAGS     =  $(INCLUDES) $(FORMAT) -g
FORMAT      = -m64
LNKCC       = $(CC)
LNKFLAGS    = $(CXFLAGS)
IDIR = ./include
INCLUDES   += -I$(IDIR)
LIBRARIES  += -ldxl_x64_cpp
LIBRARIES  += -lrt
SRC = ./src/
BIN = ./bin/



main: $(SRC)main.cpp
	$(info [INFO]Compiling main...)
	@$(LNKCC) -std=$(STD) $(LNKFLAGS) $(SRC)main.cpp -o $(BIN)main $(LIBRARIES)
	
test: $(SRC)test.cpp
	$(info [INFO]Compiling test..)
	@$(LNKCC) -std=$(STD) $(LNKFLAGS) $(SRC)test.cpp -o $(BIN)test $(LIBRARIES)
