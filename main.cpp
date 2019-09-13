#include "MicroBit.h"

MicroBit uBit;
 
uint8_t radioGroup = 22;
uint8_t buffer[30];
int x = 0;
int y = 0;
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
    uint8_t radioRSSI = uBit.radio.getRSSI();
    uBit.display.scroll(radioRSSI);
}

void onButtonA(MicroBitEvent)
{
    x++;
    x %= 5;
}

void onButtonB(MicroBitEvent)
{
    y++;
    y %= 5;
}

// Switched radio changing to long button clicks

void onButtonARadio(MicroBitEvent)
{
    radioGroup++;
   	uBit.radio.setGroup(radioGroup);
   	uBit.display.scroll(radioGroup);
}

void onButtonBRadio(MicroBitEvent)
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
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    
    // Some other button handler options that you may find useful
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_LONG_CLICK, onButtonAlong);
    //uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_LONG_CLICK, onButtonBlong);
    //uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_HOLD, onButtonAhold);
    //uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_DOUBLE_CLICK, onButtonAdouble);
    while (setLocation){
	uBit.display.print(clear);
	uBit.display.print(i, x, y);
	uBit.sleep(100);
    }
    
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonARadio);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonBRadio);
    uBit.display.print("+");
    
    for (;;) {
        uBit.sleep(5000);
        uBit.radio.datagram.send("J");
    }
    
    release_fiber();
}
