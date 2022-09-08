from ctypes import *
from ctypes import wintypes
from enum import IntEnum
from typing import Tuple



class CreateType(IntEnum):
    CREATE_PROCESS = 0,
    ATTACH = 1,
    BROADCAST = 2


class Manager:
    def __init__(self) -> None:
        self.mydll = cdll.LoadLibrary("./cppWrapper/manager.dll")
        
        self.mydll.createInstace.argtypes = c_char_p, c_int
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

        self.mydll.login.argtypes = c_int, c_char_p, c_char_p, c_int
        self.mydll.login.restype = None

        self.mydll.getMousePos.argtypes = c_int, POINTER(c_int), POINTER(c_int)
        self.mydll.getMousePos.restype = c_bool

        self.mydll.attachToWindow.argtypes = c_int, c_int
        self.mydll.attachToWindow.restype = None

        self.mydll.findWindow.argtypes = c_char_p,
        self.mydll.findWindow.restype = c_int

        self.mydll.updateRectWindow.argtypes = c_int, c_int, c_int
        self.mydll.updateRectWindow.restype = None

        self.mydll.hide.argtypes = c_int,
        self.mydll.hide.restype = None

        self.mydll.show.argtypes = c_int,
        self.mydll.show.restype = None

        self.mydll.updateImage.argtypes = c_int, c_int, c_int, c_int
        self.mydll.updateImage.restype = None

    def unload_DLL(self):
        windll.kernel32.FreeLibrary.argtypes = [wintypes.HMODULE]
        libHandle = self.mydll._handle
        del self.mydll
        windll.kernel32.FreeLibrary(libHandle)

    def create_instance(self, puzzle_pirate_path: str = "", type: CreateType = CreateType.BROADCAST) -> int:
        return self.mydll.createInstace(puzzle_pirate_path.encode("utf-8"), c_int(type.value))

    def remove_instance(self, key: int):
        self.mydll.removeInstace(c_int(key))

    def remove_title_bar(self, key: int):
        self.mydll.removeTitleBar(c_int(key))
    
    def restroe_title_bar(self, key: int):
        self.mydll.restoreTitleBar(c_int(key))

    def update_rect_window(self, key: int, x: int, y: int, width: int, height: int):
        self.mydll.updateRectWindow(c_int(key), c_int(x), c_int(y), c_int(width), c_int(height))

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

    def get_mouse_pos(self, key) -> Tuple[int, int]:
        x = c_int()
        y = c_int()
        if self.mydll.getMousePos(key, byref(x), byref(y)):
            return x.value, y.value
        return -1, -1

    def FindWindow(self, window_name: str) -> int:
        return self.mydll.findWindow(window_name.encode("utf-8"))

    def login(self, key: int, user_name: str, password: str, pirate_number: int):
        return self.mydll.login(c_int(key), user_name.encode("utf-8"), password.encode("utf-8"), c_int(pirate_number))

    def attachToWindow(self, fromKey: int, toKey: int):
        self.mydll.attachToWindow(fromKey, toKey)

    def hide(self, key: int):
        self.mydll.hide(c_int(key))
    
    def show(self, key: int):
        self.mydll.show(c_int(key))

    def update_image(self, key: int, target_HWND: int, x: int, y: int):
        self.mydll.updateImage(c_int(key), c_int(target_HWND), c_int(x), c_int(y))

# m = Manager()
# a = m.create_instance("C:\\games\\Puzzle Pirates", CreateType.CREATE_PROCESS)
# # sleep(3)
# a = m.create_instance()
# m.get_my_HWND()
# while True:
#     print(m.get_mouse_pos(a))

# m.mouse_click(a, 10, 0, True)
# x = 1
