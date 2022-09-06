from ctypes import *
from ctypes import wintypes
from time import sleep

class Manager:
    def __init__(self) -> None:
        self.mydll = cdll.LoadLibrary("./cppWrapper/manager.dll")
        
        self.mydll.createInstace.argtypes = c_char_p, c_char_p, c_char_p, c_int, c_bool
        self.mydll.createInstace.restype = c_int
        
        self.mydll.removeInstace.argtypes = c_int,
        self.mydll.removeInstace.restype = None

        self.mydll.mouseClick.argtypes = c_int, c_int, c_int, c_bool
        self.mydll.mouseClick.restype = None

        self.mydll.sendKeyPress.argtypes = c_int, c_char
        self.mydll.sendKeyPress.restype = None

        self.mydll.sendKeyDown.argtypes = c_int, c_char
        self.mydll.sendKeyDown.restype = None

        self.mydll.sendKeyUp.argtypes = c_int, c_char
        self.mydll.sendKeyUp.restype = None

        self.mydll.sendChar.argtypes = c_int, c_char
        self.mydll.sendChar.restype = None

        self.mydll.sendString.argtypes = c_int, c_char_p
        self.mydll.sendString.restype = None

        self.mydll.hide.argtypes = c_int,
        self.mydll.hide.restype = None

        self.mydll.show.argtypes = c_int,
        self.mydll.show.restype = None

        self.mydll.killGame.argtypes = c_int,
        self.mydll.killGame.restype = None

        self.mydll.removeTitleBar.argtypes = c_int,
        self.mydll.removeTitleBar.restype = None

        self.mydll.restoreTitleBar.argtypes = c_int,
        self.mydll.restoreTitleBar.restype = None

        self.mydll.isFullScreen.argtypes = c_int,
        self.mydll.isFullScreen.restype = c_bool

        self.mydll.attachToWindow.argtypes = c_int, c_int
        self.mydll.attachToWindow.restype = None


    def create_instance(self, puzzle_pirate_path: str, user_name: str, password: str, pirate_number: int) -> int:
        return self.mydll.addInstace(puzzle_pirate_path.encode("utf-8"),user_name.encode("utf-8"), password.encode("utf-8"), c_int(pirate_number) , c_bool(True if len(user_name) > 0 else False))

    def remove_instance(self, key: int):
        self.mydll.removeInstace(c_int(key))
    
    def mouse_click(self, key: int, x: int, y: int, right_click: bool = False):
        self.mydll.mouseClick(c_int(key), c_int(x), c_int(y), c_bool(right_click))

    def send_key_press(self, key: int, key_stroke: str):
        self.mydll.sendKeyPress(c_int(key), c_char(key_stroke[0].upper()))

    def send_key_down(self, key: int, key_stroke: str):
        self.mydll.sendKeyDown(c_int(key), c_char(key_stroke[0].upper()))

    def send_key_up(self, key: int, key_stroke: str):
        self.mydll.sendKeyUp(c_int(key), c_char(key_stroke[0].upper()))

    def send_char(self, key: int, chr: str):
        self.mydll.sendChar(c_int(key), c_char(chr[0]))

    def send_string(self, key: int, string: str):
        self.mydll.sendString(c_int(key), string.encode("utf-8"))

    def unload_DLL(self): 
        windll.kernel32.FreeLibrary.argtypes = [wintypes.HMODULE]
        libHandle = self.mydll._handle
        del self.mydll
        windll.kernel32.FreeLibrary(libHandle)
      

m = Manager()
#m.create_instance("C:\\games\\Puzzle Pirates", "danielkun2", "Aa123456!", 1)

sleep(3)
m.send_string(0, "test")
x = 1
