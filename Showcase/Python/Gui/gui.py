import sys
from time import sleep
from PyQt5 import QtWidgets, uic

class Ui(QtWidgets.QMainWindow):
    def __init__(self):
        super(Ui, self).__init__()
        self.setWindowTitle("PP")
        print()
        uic.loadUi('.\Gui\qtFrom.ui', self)

        #button = self.findChild(QtWidgets.QPushButton, 'pb_Open_PP') # Find the button
        self.lineEditUserName = self.findChild(QtWidgets.QLineEdit, 'le_user_name')
        self.findChild(QtWidgets.QPushButton, 'pb_open_puzzle_pirate').clicked.connect(self.open_puzzle_pirate) # Remember to pass the definition/method, not the return value!

        self.show()

    def open_puzzle_pirate(self):
        # This is executed when the button is pressed
        
        print(self.lineEditUserName.text())




if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = Ui()
    app.exec_()

