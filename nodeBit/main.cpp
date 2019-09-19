#include "MicroBit.h"

// Node MicroBit
// James Sheaf-Morrison   ID: 1314151
// Tamahau Brown	  ID: 1314934

MicroBit uBit;
 
uint8_t whoRadioGroup = 29;
uint8_t sendToRadioGroup = 0;
uint8_t listenerRadioGrouop = whoRadioGroup;
int size = 12;
uint8_t buffer[16];
int locX = 0;
int locY = 0;
int accX = 0;
int accY = 0;
int accZ = 0;
int ave = 0;
int county = 0;
bool setLocation= true;
bool who= false;

MicroBitImage i("1,0,0,0,0\n"); 
MicroBitImage clear("0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n"); 

void onData(MicroBitEvent)
{
    // Get recieved message into PacketBuffer

  uint8_t rxdata[size+4];
  int num = uBit.radio.datagram.recv(rxdata, size);
  //PacketBuffer rxdata = uBit.radio.datagram.recv();
    
  // Receive data into byte array buffer and display length
  //int rxlength = uBit.radio.datagram.recv(buffer, 30);
  //uBit.display.scroll(rxlength);
  
  // Get receive signal strength
  //uint8_t radioRSSI = uBit.radio.getRSSI();
  //uBit.display.scroll(radioRSSI);

  // Check checksum
  if((rxdata[8] == 0x11 && rxdata[9] == 0x11 /*&&
      rxdata[10] == 0x11 && rxdata[11] == 0x11*/)/* | true*/){
    if(who == false && rxdata[1] == 0x54 && rxdata[0] == 0x1){
      
      who = true;
      uBit.display.scroll("who");

      num = rxdata[2];
      uBit.display.scroll(num);
      sendToRadioGroup = num;
      listenerRadioGrouop = sendToRadioGroup + 1;
      
      //Sends its own who broadcast
      *((int *)rxdata) = 0x54 * 256 + 0x1;
      *((int *)rxdata+2) = listenerRadioGrouop;
      *((int *)rxdata+8) = 0x11 * 256 + 0x11;
      *((int *)rxdata+10) = 0x11 * 256 + 0x11;
      
      // Set radiogroup to match recieved who radio group
      uBit.radio.setGroup(whoRadioGroup);
    
      // Pass on the message
      uBit.radio.datagram.send(rxdata, size);
    
      // Set radiogroup back
      uBit.radio.setGroup(listenerRadioGrouop);
    }
    else {
      uBit.display.scroll("X");

      if (who == true){
	  // Set radiogroup to match recieved who radio group
	  uBit.radio.setGroup(sendToRadioGroup);
    
	  // Pass on the message
	  uBit.radio.datagram.send(rxdata, size);
    
	  // Set radiogroup back
	  uBit.radio.setGroup(listenerRadioGrouop);
	}
    }
  }
  else{
    uBit.display.scroll("?");
  }
  return;
}
void setXonButtonA(MicroBitEvent)
{
    locX++;
    locX %= 5;
  return;
}

void setYonButtonB(MicroBitEvent)
{
    locY++;
    locY %= 5;
  return;
}

void onButtonARadioUp(MicroBitEvent)
{
  //radioGroup++;
  //uBit.radio.setGroup(radioGroup);
    uBit.display.scroll(listenerRadioGrouop);
  return;
}

void onButtonBRadioDown(MicroBitEvent)
{
  //radioGroup--;
  //uBit.radio.setGroup(radioGroup);
    uBit.display.scroll(sendToRadioGroup);
  return;
}
void onButtonAlong(MicroBitEvent)
{
    setLocation = false;
  return;
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    uBit.radio.enable();
    uBit.radio.setGroup(whoRadioGroup);
    uBit.radio.setTransmitPower(1);
    
    // Setup some button handlers to allow extra control with buttons.
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, setXonButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, setYonButtonB);
    
    // Some other button handler options that you may find useful
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_LONG_CLICK, onButtonAlong);
    //uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_LONG_CLICK, onButtonBlong);
    //uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_HOLD, onButtonAhold);
    //uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_DOUBLE_CLICK, onButtonAdouble);

    // Set location of microbit
    while (setLocation){
      uBit.display.print(clear);
      uBit.display.print(i, locX, locY);
      uBit.sleep(100);
    }

	// Switch control of buttons A and B from location setting to radio group setting
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, setXonButtonA);
    uBit.messageBus.ignore(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, setYonButtonB);

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonARadioUp);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonBRadioDown);

    //Setup a handler to run when data is received.
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);
    
    uBit.display.print("+");

    // Start trasmitting
    for (;;) {
      
      // Get accerlerometer data
	accX = uBit.accelerometer.getX();
	accY = uBit.accelerometer.getY();
	accZ = uBit.accelerometer.getZ();
	ave = ave + sqrt(accX*accX + accY*accY + accZ*accZ);
      
      // Wait 0.1 seconds
      uBit.sleep(100);

      // Every 5 seconds (50 0.1 second waits) send average movement
      county = county + 1;
      if (county >= 2){
	ave = ave / county;
	*((int *)buffer) = 0x80 * 256 + 0x80;
	*((int *)(buffer+2)) = ave;
	*((int *)(buffer+4)) = locY * 256 + locX;
	*((int *)(buffer+6)) = listenerRadioGrouop;
	*((int *)buffer+8) = 0x72 * 256 + 0x71;
	*((int *)buffer+10) = 0x70 * 256 + 0x60;

	if (who == true){
	  // Set radiogroup to match recieved who radio group
	  uBit.radio.setGroup(sendToRadioGroup);
  
	  // Pass on the message
	  uBit.radio.datagram.send(buffer, size);

	  // Set radiogroup back
	  uBit.radio.setGroup(listenerRadioGrouop);
	}
	
	county = 0;
	ave = 0;
	}
    }
    
    release_fiber();
}
