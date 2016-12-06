#include "MicroBit.h"
#include "MicroBitUARTService.h"
#include <MicroBitAccelerometer.h>
#include "Math.h"

MicroBit uBit;
MicroBitUARTService *uart;

MicroBitI2C i2c = MicroBitI2C(I2C_SDA0, I2C_SCL0); 
MicroBitAccelerometer accelerometer=MicroBitAccelerometer(i2c);
int connected = 0;

int steps;
double calculateAcceleration();
void onConnected(MicroBitEvent)
{

    uBit.display.scroll("C");

    connected = 1;

    // mobile app will send ASCII strings terminated with the colon character
    ManagedString eom(":");

    while(connected == 1) {
        ManagedString msg = uart->readUntil(eom);
        uBit.display.scroll(msg);
    }

}

void onDisconnected(MicroBitEvent)
{
    uBit.display.scroll("D");
    connected = 0;
}

void onButtonA(MicroBitEvent)
{
    if (connected == 0) {
        return;
    }
    uart->send(ManagedString("YES"));
    uBit.display.scroll("Y");
}

void onButtonB(MicroBitEvent)
{
    if (connected == 0) {
        return;
    }
	
	//uart->send(ManagedString(steps));
    uBit.display.scroll("N");
}

void onButtonAB(MicroBitEvent)
{
    if (connected == 0) {
        return;
    }
    uart->send(ManagedString("GOT IT!!"));
    uBit.display.scroll("!");
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

	uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
	uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, onButtonAB);


    accelerometer.configure();
    // Note GATT table size increased from default in MicroBitConfig.h
    // #define MICROBIT_SD_GATT_TABLE_SIZE             0x500
    uart = new MicroBitUARTService(*uBit.ble, 32, 32);
    uBit.display.scroll("UART ready");
    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
	double avgReading=0.0;
	double lastAvgReading=0.0;
	double currentReading=0.0;
	int flag=0;
	while(true){
		currentReading=calculateAcceleration();
		avgReading = (lastAvgReading + currentReading) / 2;
		lastAvgReading = avgReading;
		if (( avgReading > 1000 ) && ( flag==0 )) {
			steps++;
			flag=1;
		}
		
		else if (( avgReading > 1000 ) && ( flag==0 )) {	
			
		}
		
		if (( avgReading < 1000 ) && ( flag ==1 )) {
			
			flag =false;
			
		}
		int x=currentReading;
		int y=avgReading;
		
		ManagedString data1(x);
		ManagedString data2(y);
		ManagedString data3(steps);
		uart->send(ManagedString(data1+data2+data3+100));
		//uBit.display.scroll(steps);	
		wait(2);
	}
	release_fiber();
}
double calculateAcceleration(){
		int xa=accelerometer.getX();
		int ya=accelerometer.getY();
		int za=accelerometer.getZ();
		
		
		return sqrt(pow(xa,2)+pow(ya,2)+pow(za,2));

}
}