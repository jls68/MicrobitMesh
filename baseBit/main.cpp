#include "MicroBit.h"

// Base station
// James Sheaf-Morrison   ID: 1314151
// Tamahau Brown	  ID: 1314934

MicroBit uBit;
 
uint8_t radioGroup = 29;
uint8_t buffer[15];
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
    uint8_t rxdata [15]; //uBit.radio.datagram.recv();
    int dat = uBit.radio.datagram.recv(rxdata, 12);
    if(rxdata[8] == 0x11 && rxdata[9] == 0x11 &&
      rxdata[10] == 0x11 && rxdata[11] == 0x11) {
    	// Receives data and sends to serial
    		uBit.display.scroll("X");
    		uBit.serial.send(rxdata, 12);
    	}
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

void onButtonBhold(MicroBitEvent)
{	
	//uint8_t buff[15];
		//Sends the location from the buffer
		uBit.display.scroll("who");
		int groupNum = (radioGroup+1) * 256 + (radioGroup+1);
		*((int *)buffer) = 0x54 * 256 + 0x01;
		*((int *)(buffer+2)) = groupNum;
		*((int *)(buffer+8)) = 0x11 * 256 + 0x11;
		*((int *)(buffer+10)) = 0x11 * 256 + 0x11;
		uBit.radio.datagram.send(buffer, 12);
     		allowConnection = true;
		//To allow it to send on 29
		uBit.radio.setGroup(radioGroup+1);
		return;
	//}
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
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_HOLD, onButtonBhold);

    // Set location of microbit
    while (setLocation){
      uBit.display.print(clear);
      uBit.display.print(i, locX, locY);
      uBit.sleep(100);
    }

// Switch control of buttons A and B from location setting to radio group setting
    //uBit.messageBus.ignore(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, setXonButtonA);
    //uBit.messageBus.ignore(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, setYonButtonB);

    //uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonARadioUp);
    //uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonBRadioDown);
    uBit.display.print("+");

    // Start trasmitting
    for (;;) {
	
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
      uBit.serial.send(buffer,12);
      //uBit.radio.datagram.send(buffer, 12);
      
      // Wait 0.5 seconds
      uBit.sleep(500);
      //uBit.display.scroll("Y");
    }
    
    release_fiber();
}
