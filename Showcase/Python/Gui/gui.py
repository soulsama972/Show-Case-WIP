import sys
from time import sleep
from PyQt5 import QtWidgets, uic, QtCore, QtGui

from cppWrapper.manager import CreateType, Manager
from threading import Thread

class Ui(QtWidgets.QMainWindow):
    def __init__(self):
        super(Ui, self).__init__()
        uic.loadUi(r'.\Gui\qtFrom.ui', self)

        self.centralwidget:             QtWidgets.QWidget
        self.tabWidget:                 QtWidgets.QTabWidget
        self.label_display:             QtWidgets.QLabel
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


        self.manager = Manager()

        self.show()
        self.my_HWND = self.manager.FindWindow(self.windowTitle())
        self.ins = self.manager.create_instance("C:\\games\\Puzzle Pirates", CreateType.CREATE_PROCESS)
        self.manager.remove_title_bar(self.ins)
        self.manager.attachToWindow(self.ins, self.my_HWND)
        
        self._update_child_rect_window()


        self.th = Thread(target=self._side_thread, args=())
        self.th.start()


    def _update_child_rect_window(self):
        p = self.label_display.geometry().topLeft()
        window_pos = p.x() + 1, p.y() + 20
        if self.ins:
            self.manager.update_rect_window(self.ins, window_pos[0], window_pos[1], self.label_display.size().width(), self.label_display.size().height())

    def moveEvent(self, a0: QtGui.QMoveEvent):
        self._update_child_rect_window()
        return super().moveEvent(a0)

    def resizeEvent(self, a0: QtGui.QResizeEvent) -> None:
        self._update_child_rect_window()
        return super().resizeEvent(a0)

    def add_bot(self):
        self.manager.login(self.ins, "danielkun2", "Aa123456!", 1)

    def add_bots_from_config(self):
        sleep(2)
        self.manager.send_string(self.ins, "test1234")

    def _side_thread(self):
        '''
        used for getting information
        '''
        while True:
            xPos, yPos = self.manager.get_mouse_pos(self.ins)
            self.label_pos.setText(f"{xPos}, {yPos}")
            sleep(0.01)


def run_gui():
    app = QtWidgets.QApplication(sys.argv)
    _ = Ui()
    app.exec_()


# run_gui()
