from pickle import TRUE
import sys
from time import sleep
from PyQt5 import QtWidgets, uic, QtGui

from cppWrapper.app_manager import CreateType, AppManager, WindowHookData
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
        self.cb_pirate_id:              QtWidgets.QComboBox
        self.le_password:               QtWidgets.QLineEdit
        self.le_user_name:              QtWidgets.QLineEdit
        self.label_pos:                 QtWidgets.QLabel

        self.current_key: int = -1
        self.th: Thread = None
        self.winData: WindowHookData = None
        
        self.pb_add_bot.clicked.connect(self._add_bot)
        self.pb_add_bots_from_config.clicked.connect(self._add_bots_from_config)
        self.cb_display_bot.currentTextChanged.connect(self._change_current_instance)

        self.show()  # make sure to call this before manager because we cant find the window id without it showing first
        self.manager = AppManager(self.windowTitle())

        self._add_bot()
        self._add_bots_from_config()

        self.run = True
        self.th = Thread(target=self._side_thread, args=())
        self.th.start()

        
    def _update_child_rect_window(self, key: int):
        p = self.label_display.geometry().topLeft()
        window_pos = p.x() + 1, p.y() + 20
        self.manager.update_rect_window(key, window_pos[0], window_pos[1], self.label_display.size().width(), self.label_display.size().height())

    def moveEvent(self, a0: QtGui.QMoveEvent):
        if self.current_key != -1:
            self._update_child_rect_window(self.current_key)
        return super().moveEvent(a0)

    def resizeEvent(self, a0: QtGui.QResizeEvent):
        if self.current_key != -1:
            self._update_child_rect_window(self.current_key)
        return super().resizeEvent(a0)

    def closeEvent(self, a0: QtGui.QCloseEvent):
        self.run = False
        self.th.join()
        self.manager.clean_up()
        return super().closeEvent(a0)

    def _add_bot(self):
        
        key = self.manager.create_instance("C:\\games\\Puzzle Pirates", type=CreateType.CREATE_PROCESS)
        self.manager.remove_title_bar(key)
        self.manager.attach_to_window(key, self.manager.get_parent_hwnd())
        self._update_child_rect_window(key)
        
        length = len(self.manager.instances)
        
        self.cb_display_bot.addItem(str(length), length - 1)
        self.cb_display_bot.setCurrentIndex(length - 1)
        
        user_name = self.le_user_name.text()
        password = self.le_password.text()

        self.winData = self.manager.get_window_data()

        if len(user_name) > 0 and len(password) > 0:
            self.manager.login(key, user_name, password, int(self.cb_pirate_id.currentText()))

    def _change_current_instance(self):
        index = self.cb_display_bot.currentData()
        self.current_key = self.manager.instances[index]
        self.manager.show(self.current_key)
        
    def _add_bots_from_config(self):
        pass
        # self.winData.block_input(TRUE)
        # self.manager.draw_text("hey", 100, 100, 0xffff00)
        # self.manager.draw_rect(0, 0, 50, 50, 0xffff00, True)
        # self.manager.present()

    def _side_thread(self):
        '''
        used for handling event from target app
        '''
        startX = 0
        startY = 0
        capture = False
        save_rects = list()
       
        while self.run:
            if self.winData:
            #    print(self.winData.xPos, self.winData.yPos, self.winData.leftClick, self.winData.middleClick, self.winData.rightClick, self.winData.is_alt())
            #    print(self.appData.keys[32])
                if self.winData.leftClick and not capture:
                    startX = self.winData.xPos
                    startY = self.winData.yPos
                    capture = True
                
                if not self.winData.leftClick and capture:
                    save_rects.append([startX, startY, self.winData.xPos, self.winData.yPos])
                    capture = False

                if capture:
                    self.manager.draw_rect(startX, startY, self.winData.xPos, self.winData.yPos, 0xffff00, True)
                
                for rect in save_rects:
                    self.manager.draw_rect(rect[0], rect[1], rect[2], rect[3], 0xffff00, True)

                self.label_pos.setText(f"{self.winData.xPos},{self.winData.yPos}")


                self.manager.draw_text("ShowCase", 50, 100, 0xffff00, 30)
                self.manager.present()
            


            if self.winData.is_key_press(ord('Z')):
                save_rects = save_rects[:-1]

            sleep(0.01)



def run_gui():
    app = QtWidgets.QApplication(sys.argv)
    _ = Ui()
    app.exec_()
