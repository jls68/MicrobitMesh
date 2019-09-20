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
ser.port = "/dev/ttyACM1"
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
modu = 12
nodes = [[0,1,2,3,4], [0,1,2,3,4], [0,1,2,3,4], [0,1,2,3,4], [0,1,2,3,4]]
for i in range(0,5):
   for j in range (0,5):
      nodes[i][j] = [0];

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
  xy = ser.read(modu)
  #xy = ab
  print(xy)
  if (xy.__len__() == modu):
   #While it has not found an empty value
   while(posFail != 1):
     #Keeps iterating if there is no empty value
     if(xy[pos] == 0x80):
       pos = (pos + 1) % modu
       if(lastRead == 1):
         posFail = 1
       lastRead = 1
     
     #Otherwise is not 0x80
     else:
       lastRead = 0
       pos = (pos + 1) % modu

   pos = pos % modu
   posy = (pos + 1) % modu
   locX = xy[(pos + 2)%modu] 
   locY = xy[(pos + 3)%modu]

   if (xy[posy] >= 128):
     newsx = -(32768 * 2 - (xy[posy] * 256 + xy[pos])) / 3
   else:
     newsx = (xy[posy] * 256 + xy[pos]) / 3

   #print(newsx, end=" ", flush=True)

   print("\n Mag: " + str(newsx))
   print("\n Location: " + str(locX) + "," + str(locY))
   #store magnitude for this location
   if(nodes[locX][locY][0] == 0):
     nodes[locX][locY][0] = newsx
   nodes[locX][locY].append(newsx)

   #TODO: Get magnitude of the accelerometers and display the data on the graph as one. To discuss
   #mag = math.sqrt(newsx * newsx + newsy * newsy + newsz * newsz)
   #print(mag)

   
   for i in range(0,5):
      for j in range (0,5):
         if(len(nodes[i][j]) > 1):
           print("For location {} , {}, the avg movements were {}".format(i, j, nodes[i][j]))
           canvas.create_line(sx, nodes[i][j][0]-400+i*200, sx+1, nodes[i][j][-1]-400+i*200, fill="blue")

   sx = sx + 1
   nodes[locX][locY][0] = newsx
   posFail = 0
   top.update()
   if (sx > 800):
     sx = 0
     canvas.delete("all")
