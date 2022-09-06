import sys
from time import sleep
from PyQt5 import QtWidgets, uic

class Ui(QtWidgets.QMainWindow):
    def __init__(self):
        super(Ui, self).__init__()
        self.setWindowTitle("PP")
        uic.loadUi('.\Gui\qtFrom.ui', self)
        self.pb_add_bot:                QtWidgets.QPushButton
        self.pb_slove_puzzle:           QtWidgets.QPushButton
        self.pb_add_bots_from_config:   QtWidgets.QPushButton

        self.cb_navi_and_solve:         QtWidgets.QCheckBox
        self.cb_all_bots:               QtWidgets.QCheckBox
        self.cb_slove_puzzle_name:      QtWidgets.QComboBox
        self.cb_display_bot:            QtWidgets.QComboBox
        self.le_password:               QtWidgets.QLineEdit
        self.le_user_name:              QtWidgets.QLineEdit
        
        self.lineEditUserName = self.findChild(QtWidgets.QLineEdit, 'le_user_name')
        self.pb_add_bot.clicked.connect(self.add_bot)

        self.show()

    def add_bot(self):
        # This is executed when the button is pressed
        
        print(self.le_user_name.text() + " " + self.le_password.text())




if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = Ui()
    app.exec_()

