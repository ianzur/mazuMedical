
from PySide6.QtNetwork import QSctpServer


class MySctpServer(QTcpServer):
    
    
    def __init__(self, parent) -> None:
        super().__init__(parent=parent)

    def incomingConnection(self, handle: int) -> None:
        print("incoming connection")
        
        sctpSocket = QAbstractSocket(QAbstractSocket.SctpSocket)
        
        sctpSocket.set
        
        
        return super().incomingConnection(handle)