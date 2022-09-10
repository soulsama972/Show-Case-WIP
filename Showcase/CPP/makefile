CC := g++
CFLAGS := -std=c++17 -Wall -g -c -lgdi32 -lkernel32  -luser32
CFLAGS_DEBUG := -std=c++17 -Wall -g -lgdi32  -lkernel32 -luser32
CFLAGS_DLL := -std=c++17 -Wall -O2 -shared -lgdi32  -lkernel32  -luser32

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
	$(CC) -o $(objPath)/$@ $< $(CFLAGS)

$(buildPath)/test.exe: $(OBJ) test.cpp
	$(CC) -o $@ $^ $(CFLAGS_DEBUG) 

$(cppWrapperPath)/manager.dll: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS_DLL) 

build: $(buildPath)/test.exe
	echo done build debug 

dll: $(cppWrapperPath)/manager.dll
	echo finish build

clean:
	cd $(buildPath) && DEL /F/Q/S *.* > NUL
	cd $(objPath) && DEL /F/Q/S *.* > NUL

redll:
	$(MAKE) clean 
	$(MAKE) dll

rebuild:
	$(MAKE) clean 
	$(MAKE) debug