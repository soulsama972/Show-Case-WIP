from os import system
import sys
from time import sleep
from typing import Tuple
from PyQt5 import QtWidgets, uic, QtGui, QtCore

from cppWrapper.manager import CreateType, Manager


class Ui(QtWidgets.QMainWindow):
    def __init__(self):
        super(Ui, self).__init__()
        uic.loadUi(r'.\Gui\qtFrom.ui', self)

        self.centralwidget:             QtWidgets.QWidget
        self.tabWidget:                 QtWidgets.QTabWidget
        self.frame_display:             QtWidgets.QFrame
        self.pb_add_bot:                QtWidgets.QPushButton
        self.pb_slove_puzzle:           QtWidgets.QPushButton
        self.pb_add_bots_from_config:   QtWidgets.QPushButton

        self.cb_navi_and_solve:         QtWidgets.QCheckBox
        self.cb_all_bots:               QtWidgets.QCheckBox
        self.cb_slove_puzzle_name:      QtWidgets.QComboBox
        self.cb_display_bot:            QtWidgets.QComboBox
        self.le_password:               QtWidgets.QLineEdit
        self.le_user_name:              QtWidgets.QLineEdit
        self.label_pos:                 QtWidgets.QLabel
        self.ins = None
        self.lineEditUserName = self.findChild(QtWidgets.QLineEdit, 'le_user_name')
        self.pb_add_bot.clicked.connect(self.add_bot)
        self.pb_add_bots_from_config.clicked.connect(self.add_bots_from_config)
        
        self.installEventFilter(self)
        self.frame_display.installEventFilter(self)

        self.window_pos: Tuple[int, int] = None
        self.num = 0

        self.manager = Manager()

        self.show()

        self.my_HWND = self.manager.FindWindow(self.windowTitle())
        self.ins = self.manager.create_instance("C:\\games\\Puzzle Pirates", CreateType.CREATE_PROCESS)
        self.manager.remove_title_bar(self.ins)
        self.manager.attachToWindow(self.ins, self.my_HWND)

        self._update_pos()

    def _update_pos(self):
        p = self.frame_display.geometry().topLeft()
        self.window_pos = p.x() + 5, p.y() + 25
        if self.ins:
            self.manager.move_window(self.ins, self.window_pos[0], self.window_pos[1], self.frame_display.size().width(), self.frame_display.size().height())
        

    def eventFilter(self, obj: QtCore.QObject, event: QtCore.QEvent) -> bool:

        event_type = event.type()
        if event_type == QtCore.QEvent.Move:
            self._update_pos()
        elif event_type == QtCore.QEvent.MouseButtonRelease:
            pass

        if event_type == QtCore.QEvent.MouseMove:
            self.num += 1
            system("cls")
            print(self.num)
        if event_type == QtCore.QEvent.HoverEnter:
            print(event_type)
        return super().eventFilter(obj, event)

    def add_bot(self):
        self.manager.login(self.ins, "danielkun2", "Aa123456!", 1)
        # This is executed when the button is pressed

    def add_bots_from_config(self):
        sleep(2)
        self.manager.send_string(self.ins, "test1234")


def run_gui():
    app = QtWidgets.QApplication(sys.argv)
    _ = Ui()
    app.exec_()


# run_gui()
