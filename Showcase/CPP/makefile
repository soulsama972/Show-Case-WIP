CC := g++
CFLAGS := -std=c++17 -Wall -Wl,--add-stdcall-alias -lgdi32 -lkernel32  -luser32
automationPath := src/automation
UtilsPath := src/utils
buildPath := ../build
objPath := ../OBJ
testPath := src
cppWrapperPath := ../Python/cppWrapper
OBJ := utils.o communication.o puzzlePirateAutomation.o windowHook.o manager.o
OBJ := $(addprefix $(objPath)/, $(OBJ))

vpath %.cpp $(UtilsPath):$(automationPath):$(testPath)

.PHONY: clean build dll redll rebuild run build32 rebuild32

$(objPath)/%.o: %.cpp
	$(CC) -o $(objPath)/$@ $< -c $(CFLAGS)

$(buildPath)/test.exe: $(OBJ) test.cpp
	$(CC) -o $@ $^ $(CFLAGS) 

$(cppWrapperPath)/manager.dll: $(OBJ)
	$(CC) -o $@ $^ -shared $(CFLAGS) 

$(cppWrapperPath)/windowHook.dll: $(objPath)/windowHookDLL.o
	$(CC) -o $@ $^ -shared $(CFLAGS)

build: CFLAGS+= -g
build: $(buildPath)/test.exe $(cppWrapperPath)/windowHook.dll

build32: CFLAGS+= -g
build32: CC := g++32
build32: $(buildPath)/test.exe $(cppWrapperPath)/windowHook.dll

run: $(buildPath)/test.exe
	$<

dll: CC:= g++32
dll: CFLAGS+= -O2
dll: $(cppWrapperPath)/manager.dll $(cppWrapperPath)/windowHook.dll

clean:
	cd $(buildPath) && DEL /F/Q/S *.* > NUL
	cd $(objPath) && DEL /F/Q/S *.* > NUL

redll:
	$(MAKE) clean 
	$(MAKE) dll

rebuild:
	$(MAKE) clean 
	$(MAKE) build

rebuild32:
	$(MAKE) clean 
	$(MAKE) build32