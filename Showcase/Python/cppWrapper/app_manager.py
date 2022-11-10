from ctypes import *
from enum import IntEnum
from typing import Any, Tuple

MANAGER_DLL_PATH = "./cppWrapper/manager.dll"

class CreateType(IntEnum):
    CREATE_PROCESS = 0,
    ATTACH = 1,
    BROADCAST = 2


class WindowRect(Structure):
    _fields_ = [
        ("left", c_long),
        ("top", c_long),
        ("right", c_long),
        ("buttom", c_long),

    ]


class WindowHookData(Structure):
    _fields_ = [
        ("keys", c_bool * 256),
        ("leftClick", c_bool),
        ("middleClick", c_bool),
        ("rightClick", c_bool),
        ("xPos", c_int),
        ("yPos", c_int),
        ("blockInput", c_bool),
        ("_target_hwnd", c_void_p),
        ("screenRect", WindowRect),
        ]

    def __init__(self, *args: Any, **kw: Any) -> None:
        super(WindowHookData).__init__(*args, **kw)
        self.xPos: int
        self.yPos: int
        self.leftClick: bool
        self.rightClick: bool
        self.middleClick: bool
        self.keys: list

    def get_mouse_pos(self) -> Tuple[int, int]:
        return self.xPos, self.yPos

    def is_alt(self) -> bool:
        return self.keys[0x12]

    def is_ctrl(self) -> bool:
        return self.keys[0x11]
    
    def is_shift(self) -> bool:
        return self.keys[0x10]
    
    def is_key_press(self, key: int) -> bool:
        '''
        the list of virtual key code can be found in https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
        '''
        return self.keys[key]

    def block_input(self, enable: bool):
        self.blockInput = enable



class AppManager:
    def __init__(self, parent_window_title: str = None, manager_dll_path: str = MANAGER_DLL_PATH):
        self.instances: list[int] = list()
        self.manager_dll = self.load_manager_dll(manager_dll_path)
        self._parent_hwnd = self.find_window(parent_window_title) if parent_window_title else None
        self._wd = POINTER(WindowHookData)()

        self.init()

    def load_manager_dll(self, manager_dll_path: str) -> CDLL:
        manager_dll = cdll.LoadLibrary(manager_dll_path)
        
        manager_dll.init.argtypes = None
        manager_dll.init.restype = None

        manager_dll.cleanUp.argtypes = None
        manager_dll.cleanUp.restype = None

        manager_dll.createInstace.argtypes = c_char_p, POINTER(POINTER(WindowHookData)), c_int
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

        manager_dll.drawText.argtypes = c_char_p, c_int, c_int, c_int, c_int
        manager_dll.drawText.restype = None
        
        manager_dll.drawRect.argtypes = c_int, c_int, c_int, c_int, c_int, c_bool
        manager_dll.drawRect.restype = None

        manager_dll.present.argtypes = None
        manager_dll.present.restype = None

        return manager_dll

    def init(self):
        self.manager_dll.init()

    def clean_up(self):
        self.manager_dll.cleanUp()

    def create_instance(self, puzzle_pirate_path: str, type: CreateType = CreateType.BROADCAST) -> int:
        key = self.manager_dll.createInstace(puzzle_pirate_path.encode("utf-8"), byref(self._wd), type.value)
        self.instances.append(key)
        return key

    def remove_instance(self, key: int):
        self.instances.remove(key)
        self.manager_dll.removeInstace(key)

    def remove_title_bar(self, key: int):
        self.manager_dll.removeTitleBar(key)
    
    def restroe_title_bar(self, key: int):
        self.manager_dll.restoreTitleBar(key)

    def update_rect_window(self, key: int, x: int, y: int, width: int, height: int):
        self.manager_dll.updateRectWindow(key, x, y, width, height)

    def mouse_click(self, key: int, x: int, y: int, right_click: bool = False):
        self.manager_dll.mouseClick(key, x, y, c_bool(right_click))

    def send_key_press(self, key: int, key_stroke: str):
        self.manager_dll.sendKeyPress(key, c_char(key_stroke[0].upper()))

    def send_key_down(self, key: int, key_stroke: str):
        self.manager_dll.sendKeyDown(key, c_char(key_stroke[0].upper()))

    def send_key_up(self, key: int, key_stroke: str):
        self.manager_dll.sendKeyUp(key, c_char(key_stroke[0].upper()))

    def send_char(self, key: int, chr: str):
        self.manager_dll.sendChar(key, c_char(chr[0]))

    def send_string(self, key: int, string: str):
        self.manager_dll.sendString(key, string.encode("utf-8"))

    def set_parent_hwnd(self, key: int):
        self._parent_hwnd = key

    def get_parent_hwnd(self) -> int:
        return self._parent_hwnd

    def find_window(self, window_name: str) -> int:
        return self.manager_dll.findWindow(window_name.encode("utf-8"))

    def login(self, key: int, user_name: str, password: str, pirate_number: int):
        return self.manager_dll.login(key, user_name.encode("utf-8"), password.encode("utf-8"), pirate_number)

    def attachToWindow(self, from_key: int, to_key: int):
        self.manager_dll.attachToWindow(from_key, to_key)

    def hide_all(self):
        for key in self.instances:
            self.hide(key)

    def hide(self, key: int):
        self.manager_dll.hide(key)
    
    def show(self, key: int):
        self.hide_all()
        self.manager_dll.show(key)

    def get_window_data(self) -> WindowHookData:
        return self._wd[0] if self._wd else None
    
    def draw_text(self, text: str, x: int, y: int, color: int, font_size: int = 0):
        '''
        if font_size == 0 the function will auto detect the minimum value needed 
        '''
        self.manager_dll.drawText(text.encode("utf-8"), x, y, color, font_size)

    def draw_rect(self, x: int, y: int, width: int, height: int, color: int, only_frame: bool):
        self.manager_dll.drawRect(x, y, width, height, color, only_frame)

    def present(self):
        '''
        drawing all object that was added before the previus call
        '''
        self.manager_dll.present()