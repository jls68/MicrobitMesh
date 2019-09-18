import serial
import tkinter # The python tcl/tk ui construction api
top = tkinter.Tk() # The tcl/tk object
canvas = tkinter.Canvas(top,bg="bisque",height=800,width=800)
canvas.pack()
top.update()
ser = serial.Serial()
ser.baudrate = 115200
ser.bytesize = 8
ser.stopbits = 1
ser.parity = 'N'
ser.timeout = 5
ser.port = "/dev/ttyACM0"
ser.xonxoff = 0
ser.rtscts = 0
ser.open()
sx = 0
sy = 0
sz = 0
ay = 0
pos = 0
startPos = 0
posy = 0
#zeroXeightys = [10, 10]
posFail = 0
lastRead = 0

'''
ab = bytearray()
ab.append(0x54)
ab.append(0x41)
ab.append(0x4D)
ab.append(0x41)
ab.append(0x48)
ab.append(0x41)
ab.append(0x55)
ab.append(0x80)
ab.append(0x80)
ab.append(0x80)
'''
while (True):
  xy = ser.read(12)
  print(xy);
  if (xy.__len__() == 12):
   #While it has not found an empty value
   while(posFail != 1):
     #Keeps iterating if there is no empty value
     if(xy[pos] == 0xff):
       pos = (pos + 1) % 12
       if(lastRead == 1):
         posFail = 1
       lastRead = 1
     
     #Otherwise is not 0x80
     else:
       lastRead = 0
       pos = (pos + 1) % 12

   pos = pos % 12
   posy = (pos + 1) % 12
   if (xy[posy] >= 128):
     newsx = -(32768 * 2 - (xy[posy] * 256 + xy[pos])) / 3
   else:
     newsx = (xy[posy] * 256 + xy[pos]) / 3

   if (xy[(posy+2)%12] >= 128):
     newsy = -(32768 * 2 - (xy[(posy+2)%12] * 256 + xy[(pos+2)%12])) / 3
   else:
     newsy = (xy[(posy+2)%12] * 256 + xy[(pos+2)%12]) / 3

   if (xy[(posy+4)%12] >= 128):
     newsz = -(32768 * 2 - (xy[(posy+4)%12] * 256 + xy[(pos+4)%12])) / 3
   else:
     newsz = (xy[(posy+4)%12] * 256 + xy[(pos+4)%12]) / 3

   #print(newsy, end=" ", flush=True)
   #print(newsx, end=" ", flush=True)
   #print(newsz, end=" ", flush=True)

   print("\nX position: " + str(newsx))
   print("Y position " + str(newsy))
   print("Z position: " + str(newsz))

   #TODO: Get magnitude of the accelerometers and display the data on the graph as one. To discuss
   mag = (x * x + y * y + z * z)

   sx = sx + 1
   sz = newsx
   sy = newsy
   ay = newsz
   posFail = 0
   top.update()
   if (sx > 800):
     sx = 0
     canvas.delete("all")
