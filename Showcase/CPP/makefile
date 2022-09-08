CC = g++
CFLAGS = -std=c++17 -Wall -c -lgdi32 -g  -lkernel32  -luser32 -m64
CFLAGS_DEBUG = -std=c++17 -Wall  -g -lgdi32  -lkernel32 -m64
CFLAGS_DLL = -std=c++17 -Wall -O2 -shared -lgdi32  -lkernel32  -luser32 -m64
automationPath = src\automation
UtilsPath = src\utils
buildPath = src\build
managerPath = src\manager
cppWrapperPath = ..\Python\cppWrapper

$(buildPath)\manager.o: $(buildPath)\puzzlePirateAutomation.o $(managerPath)\manager.cpp
	$(CC) $(managerPath)\manager.cpp $< -o $@ $(CFLAGS) 

$(buildPath)\puzzlePirateAutomation.o: $(buildPath)\communication.o $(automationPath)\puzzlePirateAutomation.cpp
	$(CC) $< $(automationPath)\puzzlePirateAutomation.cpp -o $@ $(CFLAGS) 

$(buildPath)\communication.o: $(buildPath)\utils.o $(automationPath)\communication.cpp
	$(CC) $< $(automationPath)\communication.cpp -o $@ $(CFLAGS)

$(buildPath)\utils.o: $(UtilsPath)\utils.cpp
	$(CC) $< -o $@ $(CFLAGS) 

$(buildPath)\test.exe: $(buildPath)\manager.o src\test.cpp
	$(CC) $(buildPath)\manager.o $(buildPath)\utils.o $(buildPath)\communication.o $(buildPath)\puzzlePirateAutomation.o src\test.cpp -o $(buildPath)\test.exe $(CFLAGS_DEBUG)

$(buildPath)\manager.dll: $(buildPath)\puzzlePirateAutomation.o
	$(CC) $(buildPath)\utils.o $(buildPath)\communication.o $(buildPath)\puzzlePirateAutomation.o $(managerPath)\manager.cpp -o $(cppWrapperPath)\manager.dll $(CFLAGS_DLL)

dll: $(buildPath)\manager.dll
	echo finish build

debug: $(buildPath)\test.exe
	echo 1234

clean:
	cd $(buildPath) && DEL /F/Q/S *.* > NUL




# "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib"