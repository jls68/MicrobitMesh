#include "MicroBit.h"

MicroBit uBit;
 
uint8_t radioGroup = 22;
uint8_t buffer[30];
int yPos = 0;
int xPos = 0;
int x = 0;
int y = 0;
MicroBitImage i("0,1,0,0,0\n");
int xReset = 0;
int yReset = 0;

ManagedString rxdata;

void onData(MicroBitEvent)
{
    // Receive data into string
    //ManagedString rxdata = uBit.radio.datagram.recv();
    //uBit.display.scroll(rxdata);

    // Receive data into byte array buffer and display length
    //int rxlength = uBit.radio.datagram.recv(buffer, 30);
    //uBit.display.scroll(rxlength);

    // Receive data into byte array buffer and display signal strength
    //int signalStrength =  uBit.radio.getRSSI();
    //uBit.display.scroll(uBit.radio.getRSSI());

    // Get receive signal strength
    //uint8_t radioRSSI = uBit.radio.getRSSI();
    //uBit.display.scroll(radioRSSI);
}

//////////////////////////////////////////////////////////////////////////////////////////

/*
	If the user taps the 'B' button then it clears the MicroBit, moves the LED light right by 		1 position,
	if it is at the rightmost position, it resets back to the leftmost position.
*/
void onButtonA(MicroBitEvent)
{
    MicroBitImage i("0,0,0,0,0\n");
    uBit.display.print(i, x, y);
    /*
	xReset checks to see if it is a modulo of 5, if it is then it puts it at position 0
	then it displays at position 0.
    */
    //if(xReset == 0) { x = x + 1; } else { xReset = 0; }
    y = y + 1;
    //Checks to see if the integer is within the range of lights
    if(x <= 4)
    {
    	MicroBitImage i("1,0,0,0,0\n");
    	uBit.display.print(i, x, y);
    }

    /*
	Otherwise sets it to 0 (reset) and ensure a reset variable is there to access the 		start light
    */
    else { x = 0; xReset = 1; }
}

//////////////////////////////////////////////////////////////////////////////////////////

/*
	If the user taps the 'B' button then it clears the MicroBit, moves the LED light down by 		1 position
	if it is at the bottom, it resets back to the top.
*/
void onButtonB(MicroBitEvent)
{
    MicroBitImage i("0,0,0,0,0\n");
    uBit.display.print(i, x, y);
    /*
	yReset checks to see if it is a modulo of 5, if it is then it puts it at position 0
	then it displays at position 0.
    */
    //if(yReset == 0) { y = y + 1;  yReset = 0;} else { yReset = 0; }
      y = y + 1;

    //Checks to see if the integer is within the range of lights
    if(y <= 4)
    {
    	MicroBitImage i("1,0,0,0,0\n");
    	uBit.display.print(i, x, y);
    }
    /*
	Otherwise sets it to 0 (reset) and ensure a reset variable is there to access the 		start light
    */
    else { y = 0; yReset = 1;}
}

//////////////////////////////////////////////////////////////////////////////////////////

void onButtonAlong(MicroBitEvent)
{
    radioGroup++;
   	uBit.radio.setGroup(radioGroup);
   	uBit.display.scroll(radioGroup);
}

//////////////////////////////////////////////////////////////////////////////////////////


void onButtonBlong(MicroBitEvent)
{
    radioGroup--;
        uBit.radio.setGroup(radioGroup);
   	uBit.display.scroll(radioGroup);
}

//////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
	uBit.radio.enable();
    uBit.radio.setGroup(radioGroup);
    uBit.radio.setTransmitPower(1);

    //Setup a handler to run when data is received.
    //uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);
    
    // Setup some button handlers to allow extra control with buttons.
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    
    // Some other button handler options that you may find useful
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_LONG_CLICK, onButtonAlong);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_LONG_CLICK, onButtonBlong);
    //uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_HOLD, onButtonAhold);
    //uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_DOUBLE_CLICK, onButtonAdouble);
 	
    uBit.display.print("A");
    
	for (;;) {
        uBit.sleep(1000);
        uBit.radio.datagram.send("T");
        //Setup a handler to run when data is received.
        uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);
	}
    
    release_fiber();
}
