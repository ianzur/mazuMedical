"""
Qt window which will show the current frame and other information
"""


import sys 


from PySide6.QtWidgets import QApplication, QComboBox, QHBoxLayout, QLabel, QMainWindow, QPushButton, QVBoxLayout, QWidget, QFrame
from PySide6.QtNetwork import QAbstractSocket

from PySide6 import QtCore


from enum import Enum

class StpChannels(Enum):
    FRAMES = 1
    FPS = 2
    HR = 3
    INFO = 4

class MainWindow(QMainWindow):
    
    def __init__(self) -> None:
        super().__init__()
                
        self.setWindowTitle("heart rate estimation")
        
        sctpSocket = QAbstractSocket(QAbstractSocket.SctpSocket, self)
        
        canvas = QLabel()
        canvas.setFrameStyle(QFrame.Panel | QFrame.Raised)
        canvas.setFixedSize(480 + canvas.frameWidth() * 2, 360 + canvas.frameWidth() * 2)
        canvas.setAlignment(QtCore.Qt.AlignmentFlag.AlignCenter)
    
        self.ipAddr = QComboBox()
        self.ipAddr.addItems(["127.0.0.1", "192.168.0.69"])
        self.ipAddr.setEditable(True)
        
        self.connectBtn = QPushButton("connect")
        self.connectBtn.s
        
        infoLay = QHBoxLayout()
        infoLay.addWidget(self.ipAddr)
        infoLay.addWidget(self.connectBtn)
        
        mainLay = QVBoxLayout()
        mainLay.addWidget(canvas)
        mainLay.addLayout(infoLay)
        
        # central widget
        widget = QWidget(self)
        widget.setLayout(mainLay)
        self.setCentralWidget(widget)
        
        # connections (UI buttons)
        
        
        
        

if __name__ == "__main__":
    
    app = QApplication(sys.argv)
    
    window = MainWindow()
    window.show()

    app.exec()
