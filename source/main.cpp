/*
Copyright (c) 2019 Trashbots, Inc. - SDG

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

#include <string>
#include <stdio.h>
#include <MicroBit.h>
#include "TeakTask.h"
#include "TBCDriver.h"
#include "BLETest.h"

MicroBit uBit;
// MicroBitI2C i2c = MicroBitI2C(I2C_SDA0, I2C_SCL0);

// MicroBitStorage storage;
// MicroBitThermometer thermometer(storage);

char buffer [20];
short versionNumber;
bool connected;

void task_tests();

void onButtonA(MicroBitEvent)
{
    uBit.display.scroll("BUTTON A");
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    TBCInit();
    spi.format(8, 3);
    spi.frequency(1000000);
    // gTaskManager.Setup();
  	versionNumber = -10;
  	connected = false;


    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    // uBit.messageBus.listen(MICROBIT_ID_BLE_UART, MICROBIT_UART_S_EVT_DELIM_MATCH, onDelim);


    uBit.display.scroll('A');

    // fiber_sleep(1000);

    // audio_sound_expression_test();
    // audio_virtual_pin_melody();

    // task_tests();

    ble_test();

    // uBit.display.scroll("GOODBYE!");

    // while(1) {
    //   uBit.sleep(100);
    // }
}

void task_tests() {

    // audio test
    MicrobitBtEvent("(nt:1)");
    fiber_sleep(100);
    MicrobitBtEvent("(nt:2)");
    fiber_sleep(100);
    MicrobitBtEvent("(nt:3)");
    fiber_sleep(100);
    MicrobitBtEvent("(nt:4)");
    fiber_sleep(100);
    MicrobitBtEvent("(nt:5)");
    fiber_sleep(100);
    MicrobitBtEvent("(nt:6)");
    fiber_sleep(100);
    MicrobitBtEvent("(nt:7)");
    fiber_sleep(100);
    MicrobitBtEvent("(nt:8)");

    // motor test
    MicrobitBtEvent("(m:(1 2) d:100)");
    fiber_sleep(1000);
    MicrobitBtEvent("(m:(1 2) d:-100)");
    fiber_sleep(1000);
    MicrobitBtEvent("(m:(1 2) d:0)");

    // pixel image test
    for (int i = 0; i < 5; i++) {
      MicrobitBtEvent("(px:000a00110e:1)");
      fiber_sleep(1000);
      MicrobitBtEvent("(px:1f1f1f1f1f:1)");
      fiber_sleep(1000);
    }

    // print test
    MicrobitBtEvent("(pr:101)");
    MicrobitBtEvent("(pr:202)");
    MicrobitBtEvent("(pr:303)");
}
