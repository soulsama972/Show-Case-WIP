from ctypes import *
from ctypes import wintypes

class Manager:
    def __init__(self) -> None:
        self.mydll = cdll.LoadLibrary("./cppWrapper/manager.dll")
        self.mydll.addInstace.argtypes = c_char_p, c_char_p, c_char_p, c_int, c_bool
        self.mydll.addInstace.restype = c_int
        self.instaces: list[int] = list()

    def add_instace(self, puzzle_pirate_path: str, user_name: str, password: str, pirate_number: int):
        id = self.mydll.addInstace(puzzle_pirate_path.encode("utf-8"),user_name.encode("utf-8"), password.encode("utf-8"), c_int(pirate_number) , c_bool(True if len(user_name) > 0 else False))
        self.instaces.append(id)
        pass

    def unload_DLL(self): 
        windll.kernel32.FreeLibrary.argtypes = [wintypes.HMODULE]
        libHandle = self.mydll._handle
        del self.mydll
        windll.kernel32.FreeLibrary(libHandle)
      

m = Manager()
m.add_instace("C:\\games\\Puzzle Pirates", "danielkun2", "Aa123456!", 1)

x = 1
