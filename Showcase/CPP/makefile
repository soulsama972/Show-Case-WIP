CC := g++
CFLAGS := -std=c++17 -Wall -lgdi32 -lkernel32  -luser32
automationPath := src/automation
UtilsPath := src/utils
buildPath := ../build
objPath := ../OBJ
testPath := src
cppWrapperPath := ../Python/cppWrapper
OBJ := utils.o communication.o puzzlePirateAutomation.o manager.o 
OBJ := $(addprefix $(objPath)/, $(OBJ))

vpath %.cpp $(UtilsPath):$(automationPath):$(testPath)

.PHONY: clean build dll redll rebuild

$(objPath)/%.o: %.cpp
	$(CC) -o $(objPath)/$@ $< -c $(CFLAGS)

$(buildPath)/test.exe: $(OBJ) test.cpp
	$(CC) -o $@ $^ $(CFLAGS) 

$(cppWrapperPath)/manager.dll: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 

build: CFLAGS+= -g
build: $(buildPath)/test.exe

dll: CFLAGS+= -O2 -shared
dll: $(cppWrapperPath)/manager.dll

clean:
	cd $(buildPath) && DEL /F/Q/S *.* > NUL
	cd $(objPath) && DEL /F/Q/S *.* > NUL

redll:
	$(MAKE) clean 
	$(MAKE) dll

rebuild:
	$(MAKE) clean 
	$(MAKE) build