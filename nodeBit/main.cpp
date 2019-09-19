#include "MicroBit.h"

// Node MicroBit
// James Sheaf-Morrison   ID: 1314151
// Tamahau Brown	  ID: 1314934

MicroBit uBit;
 
uint8_t whoRadioGroup = 24;
uint8_t sendToRadioGroup = whoRadioGroup;
uint8_t listenerRadioGrouop = whoRadioGroup + 1;
uint8_t buffer[10];
int locX = 0;
int locY = 0;
int accX = 0;
int accY = 0;
int accZ = 0;
int ave = 0;
int county = 0;
bool setLocation= true;
bool who= false;

ManagedString rxdata;

MicroBitImage i("1,0,0,0,0\n"); 
MicroBitImage clear("0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n"); 

void onData(MicroBitEvent)
{
    // Receive data into string
    PacketBuffer rxdata = uBit.radio.datagram.recv();
    uBit.display.scroll("X");
    
    // Receive data into byte array buffer and display length
    //int rxlength = uBit.radio.datagram.recv(buffer, 30);
    //uBit.display.scroll(rxlength);

    // Get receive signal strength
    uint8_t radioRSSI = uBit.radio.getRSSI();
    uBit.display.scroll(radioRSSI);

    if(who == false){
      sendToRadioGroup = 24;
      who = true;
    }
    else {
      // Pass on recieved message
      // Set radiogroup to match recieved who radio group
      uBit.radio.setGroup(sendToRadioGroup);
    
      // Pass on the message
      uBit.radio.datagram.send(rxdata);
      // Set radiogroup back
      uBit.radio.setGroup(listenerRadioGrouop);
    }
}
void setXonButtonA(MicroBitEvent)
{
    locX++;
    locX %= 5;
}

void setYonButtonB(MicroBitEvent)
{
    locY++;
    locY %= 5;
}
/*
void onButtonARadioUp(MicroBitEvent)
{
    radioGroup++;
    uBit.radio.setGroup(radioGroup);
    uBit.display.scroll(radioGroup);
}

void onButtonBRadioDown(MicroBitEvent)
{
    radioGroup--;
    uBit.radio.setGroup(radioGroup);
    uBit.display.scroll(radioGroup);
}*/
void onButtonAlong(MicroBitEvent)
{
    setLocation = false;
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    uBit.radio.enable();
    uBit.radio.setGroup(whoRadioGroup);
    uBit.radio.setTransmitPower(1);

    //Setup a handler to run when data is received.
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);
    
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

    /*uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonARadioUp);
      uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonBRadioDown);*/
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
	// Send message
        // Set radiogroup to match recieved who radio group
	uBit.radio.setGroup(sendToRadioGroup);
    
	// Pass on the message
	uBit.radio.datagram.send(buffer, 8);
	// Set radiogroup back
	uBit.radio.setGroup(listenerRadioGrouop);
	county = 0;
	ave = 0;
	}
    }
    
    release_fiber();
}
