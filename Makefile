CXX=g++
FLAGS=

# Executable Name
EXEC=ticTacToe

# Target OS Detection
ifeq ($(OS), Windows_NT)

PATH=$(shell cd)
SRC=$(PATH)\src\main.cpp

ifeq ($(win32), 1)
EXEC:=$(EXEC)_x86.exe
FLAGS+= -m32
else
EXEC:=$(EXEC)_x64.exe
FLAGS+= -m64
endif

BUILD=$(PATH)\$(EXEC)

else

UNAME=$(shell uname -s)
ifeq($(UNAME), Linux)
PATH=$(shell pwd)
SRC=$(PATH)/src/main.cpp

FLAGS+= -lpthread
BUILD=$(PATH)/$(EXEC)
endif
endif

# Compilation
build:
	$(shell $(CXX) $(SRC) -o $(BUILD) $(FLAGS))
	@echo "Project Compiled"

clean:
	$(shell rm $(EXEC))
	@echo "Project Cleaned"
