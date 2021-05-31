/*
The MIT License (MIT)

Copyright (c) 2021 Lancaster University.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

/*
To switch to open link, the simplest method is to replace in codal.json
        "MICROBIT_BLE_OPEN": 0,
        "MICROBIT_BLE_SECURITY_LEVEL": "SECURITY_MODE_ENCRYPTION_NO_MITM"
with
        "MICROBIT_BLE_OPEN": 1,
        "MICROBIT_BLE_SECURITY_LEVEL": "SECURITY_MODE_ENCRYPTION_OPEN_LINK"

and in config.json
        "security_level": "SECURITY_MODE_ENCRYPTION_NO_MITM"
with
        "open": 1

The need to specify MICROBIT_BLE_SECURITY_LEVEL with MICROBIT_BLE_OPEN in codal.json
is a workaround for a problem with the way the macros get defined.

MICROBIT_BLE_WHITELIST stops micro:bit advertising to unbonded devices.

Setting MICROBIT_BLE_OPEN sets some other macros, including clearing MICROBIT_BLE_WHITELIST.
It's also possible to set the individual macros instead of MICROBIT_BLE_OPEN.

See
https://github.com/lancaster-university/microbit-dal/blob/master/inc/core/MicroBitConfig.h#L221
https://github.com/lancaster-university/codal-microbit-v2/blob/master/inc/MicroBitConfig.h#L167
*/

#include "MicroBit.h"
#include "MicroBitUARTService.h"

extern MicroBit uBit;

MicroBitUARTService *uart;
ManagedString eom(";");

// we use events abd the 'connected' variable to keep track of the status of the Bluetooth connection
void onConnected(MicroBitEvent)
{
    uBit.display.print("C");
}

void onDisconnected(MicroBitEvent)
{
    uBit.display.print("D");
}

void onDelim(MicroBitEvent)
{
    ManagedString r = uart->readUntil(eom, ASYNC);
    uart->send(r);
}

void ble_test()
{
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);

    uBit.messageBus.listen(MICROBIT_ID_BLE_UART, MICROBIT_UART_S_EVT_DELIM_MATCH, onDelim);

    new MicroBitAccelerometerService(*uBit.ble, uBit.accelerometer);
    new MicroBitButtonService(*uBit.ble);
    new MicroBitIOPinService(*uBit.ble, uBit.io);
    new MicroBitLEDService(*uBit.ble, uBit.display);
    //new MicroBitMagnetometerService(*uBit.ble, uBit.compass);
    new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);

    uart = new MicroBitUARTService(*uBit.ble, 32, 32);
    uart->eventOn("\r\n");

    uBit.display.scroll("OPEN");

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
