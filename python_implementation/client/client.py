"""
Qt window which will show the current frame and other information
"""


import sys 


from PyQt6.QtWidgets import QApplication, QLabel, QMainWindow, QWidget, QFrame
from PyQt6 import QtCore

class MainWindow(QMainWindow):
    
    def __init__(self) -> None:
        super().__init__()
                
        self.setWindowTitle("heart rate estimation")
        
        canvas = QLabel()
        canvas.setFixedSize(480 + canvas.frameWidth() * 2, 360 + canvas.frameWidth() * 2)
        canvas.setAlignment(QtCore.Qt.AlignmentFlag.AlignCenter)
        
        self.setCentralWidget(canvas)


if __name__ == "__main__":
    
    app = QApplication(sys.argv)
    
    window = MainWindow()
    window.show()

    app.exec()
