#include "MicroBit.h"

// Base station
// James Sheaf-Morrison   ID: 1314151
// Tamahau Brown	  ID: 1314934

MicroBit uBit;
 
uint8_t radioGroup = 24;
uint8_t buffer[10];
int locX = 0;
int locY = 0;
int accX = 0;
int accY = 0;
int accZ = 0;
int ave = 0;
bool setLocation= true;
bool allowConnection = false;

ManagedString rxdata;

MicroBitImage i("1,0,0,0,0\n"); 
MicroBitImage clear("0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n"); 

void onData(MicroBitEvent)
{
    // Receive data into string
    PacketBuffer rxdata = uBit.radio.datagram.recv();
    uBit.display.scroll("X");
    uBit.serial.send(rxdata);
    
    // Receive data into byte array buffer and display length
    //int rxlength = uBit.radio.datagram.recv(buffer, 30);
    //uBit.display.scroll(rxlength);

    // Get receive signal strength
    //uint8_t radioRSSI = uBit.radio.getRSSI();
    //uBit.display.scroll(radioRSSI);
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

// Switched radio changing to long button clicks

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
}
void onButtonAlong(MicroBitEvent)
{
    setLocation = false;
}

void onButtonBdouble(MicroBitEvent)
{
	if(allowConnection == false) {
     		uBit.radio.datagram.send("who");
		radioGroup = radioGroup+1;
     		uBit.radio.setGroup(radioGroup);
		uBit.display.scroll("who");
     		allowConnection = true;
	}
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    uBit.radio.enable();
    uBit.radio.setGroup(radioGroup);
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
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_HOLD, onButtonBdouble);

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
    uBit.display.print("+");

    // Start trasmitting
    for (;;) {
	if(allowConnection == true) {
		//Setup a handler to run when data is received.
    		uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);
        }

      	// Get accerlerometer data
	accX = uBit.accelerometer.getX();
	accY = uBit.accelerometer.getY();
	accZ = uBit.accelerometer.getZ();
	ave = sqrt(accX*accX + accY*accY + accZ*accZ);
	

	//TODO: Get location (from base station)
	*((int *)buffer) = 0x80 * 256 + 0x80;
	*((int *)(buffer+2)) = ave;
	*((int *)(buffer+4)) = locY * 256 + locX;

      // Send message
      //uBit.serial.send(buffer,6);
      //uBit.radio.datagram.send(buffer);
      
      // Wait 0.1 seconds
      uBit.sleep(100);
    }
    
    release_fiber();
}
