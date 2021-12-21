"""
multiprocess that will,
- capture frames from camera
- perform video filtering
- emit frames along with other info to client
"""

import time 

from PySide6 import QtNetwork

from image_generator import imgGenerator


def main():
    
    imgGen = imgGenerator()
    
    sctp_server = QtNetwork.QSctpServer()
    sctp_server.setMaximumChannelCount(4)        

    print("success")

    print(sctp_server.maximumChannelCount())    
    
    
    # while True:
    #     img = next(imgGen)
        
    #     # send img
        
                
    #     time.sleep(0.5)
        

if __name__ == "__main__":
    
    main()