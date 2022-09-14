from ctypes import *
from enum import IntEnum
from typing import Any

MANAGER_DLL_PATH = "./cppWrapper/manager.dll"
WINDOW_HOOK_DLL_PATH = r"G:\projects\Showcase\Showcase\Python\cppWrapper\windowHook.dll"


class CreateType(IntEnum):
    CREATE_PROCESS = 0,
    ATTACH = 1,
    BROADCAST = 2


class WindowHookData(Structure):
    _fields_ = [
        ("keys", c_bool * 256),
        ("leftClick", c_bool),
        ("middleClick", c_bool),
        ("rightClick", c_bool),
        ("xPos", c_int),
        ("yPos", c_int)]

    def __init__(self, *args: Any, **kw: Any) -> None:
        super(WindowHookData).__init__(*args, **kw)
        self.xPos: int
        self.yPos: int
        self.leftClick: bool
        self.rightClick: bool
        self.middleClick: bool
        self.keys: list

    def is_alt(self) -> bool:
        return self.keys[0x12]

    def is_ctrl(self) -> bool:
        return self.keys[0x11]
    
    def is_shift(self) -> bool:
        return self.keys[0x10]



class OutDoorAppManager:
    def __init__(self, parent_window_title: str = None, manager_dll_path: str = MANAGER_DLL_PATH):
        self.instances: list[int] = list()
        self.manager_dll = self.load_manager_dll(manager_dll_path)
        self._parent_hwnd = self.FindWindow(parent_window_title) if parent_window_title else None

    def load_manager_dll(self, manager_dll_path: str) -> CDLL:
        manager_dll = cdll.LoadLibrary(manager_dll_path)
        
        manager_dll.createInstace.argtypes = c_char_p, c_int
        manager_dll.createInstace.restype = c_int
        
        manager_dll.removeInstace.argtypes = c_int,
        manager_dll.removeInstace.restype = None

        manager_dll.mouseClick.argtypes = c_int, c_int, c_int, c_bool
        manager_dll.mouseClick.restype = None

        manager_dll.sendKeyPress.argtypes = c_int, c_char
        manager_dll.sendKeyPress.restype = None

        manager_dll.sendKeyDown.argtypes = c_int, c_char
        manager_dll.sendKeyDown.restype = None

        manager_dll.sendKeyUp.argtypes = c_int, c_char
        manager_dll.sendKeyUp.restype = None

        manager_dll.sendChar.argtypes = c_int, c_char
        manager_dll.sendChar.restype = None

        manager_dll.sendString.argtypes = c_int, c_char_p
        manager_dll.sendString.restype = None

        manager_dll.hide.argtypes = c_int,
        manager_dll.hide.restype = None

        manager_dll.show.argtypes = c_int,
        manager_dll.show.restype = None

        manager_dll.killGame.argtypes = c_int,
        manager_dll.killGame.restype = None

        manager_dll.removeTitleBar.argtypes = c_int,
        manager_dll.removeTitleBar.restype = None

        manager_dll.restoreTitleBar.argtypes = c_int,
        manager_dll.restoreTitleBar.restype = None

        manager_dll.isFullScreen.argtypes = c_int,
        manager_dll.isFullScreen.restype = c_bool

        manager_dll.attachToWindow.argtypes = c_int, c_int
        manager_dll.attachToWindow.restype = None

        manager_dll.login.argtypes = c_int, c_char_p, c_char_p, c_int
        manager_dll.login.restype = None

        manager_dll.attachToWindow.argtypes = c_int, c_int
        manager_dll.attachToWindow.restype = None

        manager_dll.findWindow.argtypes = c_char_p,
        manager_dll.findWindow.restype = c_int

        manager_dll.updateRectWindow.argtypes = c_int, c_int, c_int
        manager_dll.updateRectWindow.restype = None

        manager_dll.hide.argtypes = c_int,
        manager_dll.hide.restype = None

        manager_dll.show.argtypes = c_int,
        manager_dll.show.restype = None

        manager_dll.setHook.argtypes = c_int, c_char_p, POINTER(POINTER(WindowHookData))
        manager_dll.setHook.restype = c_bool

        return manager_dll

    def create_instance(self, puzzle_pirate_path: str = WINDOW_HOOK_DLL_PATH, type: CreateType = CreateType.BROADCAST) -> int:
        key = self.manager_dll.createInstace(puzzle_pirate_path.encode("utf-8"), c_int(type.value))
        self.instances.append(key)
        return key

    def remove_instance(self, key: int):
        self.instances.remove(key)
        self.manager_dll.removeInstace(c_int(key))

    def remove_title_bar(self, key: int):
        self.manager_dll.removeTitleBar(c_int(key))
    
    def restroe_title_bar(self, key: int):
        self.manager_dll.restoreTitleBar(c_int(key))

    def update_rect_window(self, key: int, x: int, y: int, width: int, height: int):
        self.manager_dll.updateRectWindow(c_int(key), c_int(x), c_int(y), c_int(width), c_int(height))

    def mouse_click(self, key: int, x: int, y: int, right_click: bool = False):
        self.manager_dll.mouseClick(c_int(key), c_int(x), c_int(y), c_bool(right_click))

    def send_key_press(self, key: int, key_stroke: str):
        self.manager_dll.sendKeyPress(c_int(key), c_char(key_stroke[0].upper()))

    def send_key_down(self, key: int, key_stroke: str):
        self.manager_dll.sendKeyDown(c_int(key), c_char(key_stroke[0].upper()))

    def send_key_up(self, key: int, key_stroke: str):
        self.manager_dll.sendKeyUp(c_int(key), c_char(key_stroke[0].upper()))

    def send_char(self, key: int, chr: str):
        self.manager_dll.sendChar(c_int(key), c_char(chr[0]))

    def send_string(self, key: int, string: str):
        self.manager_dll.sendString(c_int(key), string.encode("utf-8"))

    def set_parent_hwnd(self, key: int):
        self._parent_hwnd = key

    def set_window_hook(self, key: int, dll_win_hook_path: str = WINDOW_HOOK_DLL_PATH) -> POINTER(WindowHookData):
        wd = POINTER(WindowHookData)()
        res = self.manager_dll.setHook(key, dll_win_hook_path.encode("utf-8"), byref(wd))
        return wd[0] if res else None

    def get_parent_hwnd(self) -> int:
        return self._parent_hwnd

    def FindWindow(self, window_name: str) -> int:
        return self.manager_dll.findWindow(window_name.encode("utf-8"))

    def login(self, key: int, user_name: str, password: str, pirate_number: int):
        return self.manager_dll.login(c_int(key), user_name.encode("utf-8"), password.encode("utf-8"), c_int(pirate_number))

    def attachToWindow(self, from_key: int, to_key: int):
        self.manager_dll.attachToWindow(from_key, to_key)

    def hide_all(self):
        for key in self.instances:
            self.hide(key)

    def hide(self, key: int):
        self.manager_dll.hide(c_int(key))
    
    def show(self, key: int):
        self.hide_all()
        self.manager_dll.show(c_int(key))
