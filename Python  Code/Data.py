import serial
import math
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
magVal = 0
'''
pos1 = 0x54
pos2 = 0x41
pos3 = 0x4D
pos4 = 0x41
pos5 = 0x48
pos6 = 0x41
pos7 = 0x55
pos8 = 0x80
pos9 = 0x80
pos10 = 0x00
pos11 = 0xff
pos12 = 0xff

ab = bytearray()

ab.append(pos1)
ab.append(pos2)
ab.append(pos3)
ab.append(pos4)

ab.append(pos5)
ab.append(pos6)
ab.append(pos7)
ab.append(pos8)

ab.append(pos9)
ab.append(pos10)
ab.append(pos11)
ab.append(pos12)
'''
while (True):
  xy = ser.read(12)
  #xy = ab
  print(xy)
  if (xy.__len__() == 12):
   #While it has not found an empty value
   while(posFail != 1):
     #Keeps iterating if there is no empty value
     if(xy[pos] == 0x00):
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

   print(newsy, end=" ", flush=True)
   print(newsx, end=" ", flush=True)
   print(newsz, end=" ", flush=True)

   print("\nX position: " + str(newsx))
   print("Y position " + str(newsy))
   print("Z position: " + str(newsz))

   #TODO: Get magnitude of the accelerometers and display the data on the graph as one. To discuss
   mag = math.sqrt(newsx * newsx + newsy * newsy + newsz * newsz)
   #print(mag)

   canvas.create_line(sx, 400+mag/100, sx+1, 400+magVal/100, fill="blue")

   sx = sx + 1
   magVal = mag
   sz = newsx
   sy = newsy
   ay = newsz
   posFail = 0
   top.update()
   if (sx > 800):
     sx = 0
     canvas.delete("all")
