CC := g++
CFLAGS := -std=c++17 -Wall -Wl,--add-stdcall-alias -lgdi32 -lkernel32  -luser32
EXTRAFLAGS = -g -DDEBUG

automationPath := src/automation
hooksPath := src/hooks
utilsPath := src/utils
overlayPath := src/overlay
buildPath := ../build
objPath := ../OBJ
testPath := src/Test
cppWrapperPath := ../Python/cppWrapper
examplePath := src/example


OBJ := utils.o communication.o puzzlePirateAutomation.o manager.o overlay.o windowHook.o
OBJ := $(addprefix $(objPath)/, $(OBJ))

vpath %.cpp $(utilsPath):$(automationPath):$(testPath):$(overlayPath):$(examplePath):$(hooksPath)
vpath %.h   $(utilsPath):$(automationPath):$(testPath):$(overlayPath):$(examplePath):$(hooksPath)

.PHONY: clean build rebuild build32 rebuild32 dll redll dll32 redll32 dbgdll32 run

$(objPath)/%.o: %.cpp %.h
	$(CC) -o $(objPath)/$@ $< -c $(CFLAGS) $(EXTRAFLAGS)

$(buildPath)/test.exe: $(OBJ) test.cpp
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRAFLAGS)

$(cppWrapperPath)/manager.dll: $(OBJ)
	$(CC) -o $@ $^ -shared $(CFLAGS) $(EXTRAFLAGS)


run: $(buildPath)/test.exe
	$<

build: $(buildPath)/test.exe

dll: $(cppWrapperPath)/manager.dll

build32: CC := g++32
build32: build

dll32: CC:= g++32
dll32: dll

clean:
	cd $(buildPath) && DEL /F/Q/S *.* > NUL
	cd $(objPath) && DEL /F/Q/S *.* > NUL

redll:
	$(MAKE) clean 
	$(MAKE) dll

redll32:
	$(MAKE) clean 
	$(MAKE) dll32

rebuild:
	$(MAKE) clean 
	$(MAKE) build