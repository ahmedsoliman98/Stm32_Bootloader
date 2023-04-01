from PySide2.QtCore import *
from PySide2.QtGui import *
from PySide2.QtWidgets import *
from intelhex import IntelHex
from serial.tools import list_ports
import time
import serial
import re 
import time
import re
import sys

fileName = []


Path=[]
Com=[]
bin_data=0
Port = 0 
Path.append(0)
Com.append(0)
Entery_piont = 0
First_Adress = 0
Reserved_Byte = []
MassageID = []

def Flash_ParceHexFile():
  global bin_data
  global Entery_piont

  try: 
    hex_obj = IntelHex()                     # create empty object
    hex_obj.fromfile(Path[0],format='hex') # load from hex
    bin_data = hex_obj.tobinarray()     #Parse hex file to binary array
    x = len(bin_data)
    #print(str(bin_data))  # if want to print array of bytes 
    print("#define APP_SIZE       "+str(x))
    f = open(Path[0],'r')
    EntryPoint=re.findall(r':04000005(........)..',f.read()) #read line by line and search with regex and return string
    num = int(EntryPoint[0],16)
    bytes=num.to_bytes(4, 'little') # big for big indian reprecentation 
    Entery_piont=list(bytes)  #each of the list reprecent a byte in decimal
    print(Entery_piont)
  except:
    return 'Ok',''
  else:
    return 'Ok',''
    
    
    
def Flash_SessionControl(Port):
  Port.write([0x10])
  time.sleep(1)
  data = int.from_bytes(Port.read(1),'big')
  
  if data == 0x50:
    return 'Ok',' '
  else:
    return 'Nok' , 'Error Flash_SessionControl ' + str(data)
    
    
# THE REQUECT OF FLASHING FRAME  SHOULD BE 
# 0X34 | RESERVED BYTE 0 | SIZEOFCODE (2BYTE)| ENTRYPOINT(4BYTE)
def Flash_RequestDowenload(Port):
  global Reserved_Byte
  global MassageID
  code_size = len(bin_data).to_bytes(2,'little')
  code_size = list(code_size)
  Reserved_Byte = [0x00]
  MassageID = [0x34]
  RequestDowenload =  MassageID + Reserved_Byte + code_size +Entery_piont 
  #time.sleep(1)
  for i in RequestDowenload:
    transfer_data = [i]
    Port.write(transfer_data)
    print(transfer_data)
    
  data = int.from_bytes(Port.read(1),'little')
  if data == (0x34+0x40):
    return 'Ok',' '
  else:
    return 'Nok' , 'Error Flash_RequestDowenload ' + str(data)
   
   
   
def Flash_TransfareData(Port):
  global Reserved_Byte
  global MassageID
  global bin_data
  bin_data = list(bin_data)
  DataBlock_size = 4096
  time.sleep(1)
  Iter_TxNum = int(len(bin_data) /DataBlock_size) #get the number of Tx by divide on the max buffer can send at time
  if (len(bin_data) % DataBlock_size) != 0:
    Iter_TxNum = Iter_TxNum+1
  MassageID = [0x36]
  for start in range(0 , len(bin_data),DataBlock_size):
    end = start + DataBlock_size #each iteration the end of the data field will caculated
    if end > len(bin_data): #check the last frame 
      end = len(bin_data)
    transfer_data = MassageID + bin_data[start:end]
    Port.write(transfer_data)
    print("data send "+ str(end))
    print(len(transfer_data))
    data = int.from_bytes(Port.read(1),'little')
    if data == (0x36 + 0x40):
      print('Ok')
  if data == (0x36 + 0x40):
    return 'Ok',' '
  else:
    return 'Nok' , 'Error TransfareData ' + str(data)
   
   
   
   
def Flash_RequestTransfareExit(Port):
  global MassageID 
  MassageID = [0x37]
  Port.write(MassageID)
  data = int.from_bytes(Port.read(1),'little')
  
  if data == (0x37 + 0x40):
    return 'Ok',' '
  else:
    return 'Nok' , 'Error Flash_RequestTransfareExit ' + str(data)
    
    
    
    
def GetPort():
  global Port
  Port = serial.Serial(port = "COM9",  baudrate=9600  , timeout = 5, 
                               parity = serial.PARITY_NONE  , stopbits = serial.STOPBITS_ONE , 
                               bytesize = serial.EIGHTBITS)
  

def Reconnect_withPort():
  global Port
  Port.close()
  Port = serial.Serial(port = Com[0],  baudrate=9600  , timeout = 5, 
                     parity = serial.PARITY_NONE  , stopbits = serial.STOPBITS_ONE , 
                     bytesize = serial.EIGHTBITS)
  
  

class Ui_Form(object):
    def setupUi(self, Form):
        if not Form.objectName():
            Form.setObjectName(u"Form")
        Form.setFixedSize(882, 292)
        Form.setAutoFillBackground(False)
        Form.setStyleSheet(u"")
        self.progressBar = QProgressBar(Form)
        self.progressBar.setObjectName(u"progressBar")
        self.progressBar.setGeometry(QRect(40, 250, 591, 31))
        self.progressBar.setValue(0)
        self.detect = QPushButton(Form)
        self.detect.setObjectName(u"detect")
        self.detect.setGeometry(QRect(440, 210, 93, 31))
        self.Com_Text = QTextEdit(Form)
        self.Com_Text.setObjectName(u"Com_Text")
        self.Com_Text.setGeometry(QRect(160, 210, 251, 31))
        self.label = QLabel(Form)
        self.label.setObjectName(u"label")
        self.label.setGeometry(QRect(40, 210, 101, 31))
        self.label.setStyleSheet(u"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(85, 170, 255, 255), stop:1 rgba(255, 255, 255, 255));")
        self.FlashEcu = QPushButton(Form)
        self.FlashEcu.setObjectName(u"FlashEcu")
        self.FlashEcu.setGeometry(QRect(640, 250, 93, 28))
        self.Load = QPushButton(Form)
        self.Load.setObjectName(u"Load")
        self.Load.setGeometry(QRect(440, 20, 91, 31))
        self.Path_lable = QTextEdit(Form)
        self.Path_lable.setObjectName(u"Path_lable")
        self.Path_lable.setGeometry(QRect(30, 20, 400, 40))
        self.Info_lable = QLabel(Form)
        self.Info_lable.setObjectName(u"Info_lable")
        self.Info_lable.setGeometry(QRect(600, 180, 201, 20))
        self.Info_lable.setStyleSheet(u"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(85, 170, 255, 255), stop:1 rgba(255, 255, 255, 255));")
        self.img = QLabel(Form)
        self.img.setObjectName(u"img")
        self.img.setGeometry(QRect(0, 0, 881, 291))
        self.img.setAutoFillBackground(True)
        self.img.setPixmap(QPixmap("Iron2.jpg"))
        self.img.setScaledContents(True)
        self.img.setWordWrap(False)
        self.img.raise_()
        self.progressBar.raise_()
        self.detect.raise_()
        self.Com_Text.raise_()
        self.label.raise_()
        self.FlashEcu.raise_()
        self.Load.raise_()
        self.Path_lable.raise_()
        self.Info_lable.raise_()

        self.retranslateUi(Form)

        QMetaObject.connectSlotsByName(Form)
        QMetaObject.connectSlotsByName(Form)
        
        self.detect.clicked.connect(self.DetctCbf)
        self.Load.clicked.connect(self.LoadCbf)
        self.FlashEcu.clicked.connect(self.FlashEcuCbf)
        
    # setupUi
    def LoadCbf(self):
      fileName = QFileDialog.getOpenFileName(filter ="hex Files (*.hex)" , directory = ".")
      self.Path_lable.setText(fileName[0])
      Path[0]=fileName[0]

    def DetctCbf(self):
      global Port
      comPorts = list(serial.tools.list_ports.comports())    # get list of all devices connected through serial port
      for port in comPorts:
        if('Serial' in str(port) or 'SERIAL' in str(port)):
          self.Com_Text.setText(port.device)
          Com[0]=port.device
          Port = serial.Serial(port = Com[0],  baudrate=9600  , timeout = 5, 
                               parity = serial.PARITY_NONE  , stopbits = serial.STOPBITS_ONE , 
                               bytesize = serial.EIGHTBITS)
          self.Info_lable.setText("Connected")
          print(Com)
          return
      self.Info_lable.setText("com port can't be detected")
      self.Com_Text.setText("")
      
    def FlashEcuCbf(self):
      global Port
      if(Path[0] != 0 and  Com[0] != 0):
        state , respons = Flash_ParceHexFile()
        if state == 'Ok':
          self.progressBar.setValue(20)
          self.Info_lable.setText("Flashing !")
        else:
          self.Info_lable.setText(respons)
          Reconnect_withPort()
          return
          
        state , respons = Flash_SessionControl(Port)
        if state == 'Ok':
          self.progressBar.setValue(40)
        else:
          self.Info_lable.setText(respons)
          Reconnect_withPort()
          return
          
        state , respons = Flash_RequestDowenload(Port)
        if state == 'Ok':
          self.progressBar.setValue(60)
        else:
          self.Info_lable.setText(respons)
          Reconnect_withPort()
          return
          
        state , respons = Flash_TransfareData(Port)
        if state == 'Ok':
          self.progressBar.setValue(80)
        else:
          self.Info_lable.setText(respons)
          Reconnect_withPort()
          return
          
        state , respons = Flash_RequestTransfareExit(Port)
        if state == 'Ok':
          self.progressBar.setValue(100)
          self.Info_lable.setText("Finshed Flashing !!")
        else:
          self.Info_lable.setText(respons)
          Reconnect_withPort()
          return
        
        
        
        return
    def retranslateUi(self, Form):
        Form.setWindowTitle(QCoreApplication.translate("Form", u"Boot Loader Tool", None))
        Form.setWindowIcon(QIcon('icon.png'))
        self.detect.setText(QCoreApplication.translate("Form", u"detect", None))
        self.label.setText(QCoreApplication.translate("Form", u"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600;\">COM Port</span></p></body></html>", None))
        
        self.FlashEcu.setText(QCoreApplication.translate("Form", u"Flash ECU", None))
        self.Load.setText(QCoreApplication.translate("Form", u"Load", None))
        self.Info_lable.setText("")
        self.img.setText("")
    # retranslateUi
if __name__=='__main__':
  app = QApplication(sys.argv)
  Widget = QWidget()
  Form = Ui_Form()
  Form.setupUi(Widget)
  Widget.show()
  sys.exit(app.exec_())