#ifdef TEENSY
#include "WProgram.h"
#else
#include "Arduino.h"
#endif

int main(void)
{
#ifndef TEENSY
	init();
#endif

#if defined(USBCON)
	USBDevice.attach();
#endif

	setup();
    
	for (;;) {
		loop();
#ifndef TEENSY
		if (serialEventRun) serialEventRun();
#endif
	}
        
	return 0;
}

#ifdef USB_SERIAL
usb_serial_class Serial;
#endif
