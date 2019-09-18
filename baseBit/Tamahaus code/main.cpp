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
bool setLocation= true;

ManagedString rxdata;

MicroBitImage i("1,0,0,0,0\n"); 
MicroBitImage clear("0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n"); 

void onData(MicroBitEvent)
{
    // Receive data into string
    ManagedString rxdata = uBit.radio.datagram.recv();
    uBit.display.scroll(rxdata);
    
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

    // Set location of microbit
    while (setLocation){
      uBit.display.print(clear);
      uBit.display.print(i, locX, locY);
      uBit.sleep(1000);
    }

    
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonARadioUp);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonBRadioDown);
    uBit.display.print("+");

    // Start trasmitting every 5 seconds
    for (;;) {
      // Get accerlerometer data
	accX = uBit.accelerometer.getX();
	accY = uBit.accelerometer.getY();
	accZ = uBit.accelerometer.getZ();

	//TODO: Get location (from base station)
	*((int *)buffer) = accX;
	*((int *)(buffer+2)) = accY;
	*((int *)(buffer+4)) = accZ;
	*((int *)buffer+6) = locX;
        *((int *)buffer+8) = locY;
	//uBit.serial.send(buffer, 10);

      // Send message
      uBit.serial.send(buffer,12);
      //uBit.radio.datagram.send(buffer);
      
      // Wait 5 seconds
      uBit.sleep(1000);
    }
    
    release_fiber();
}
