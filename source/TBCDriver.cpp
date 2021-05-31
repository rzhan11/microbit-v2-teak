#include <MicroBit.h>
#include "TBCDriver.h"
#include "teak/tstring.h"
#include <string>

extern MicroBit uBit;

void TBCInit()
{
  spi.format(8, 0);
  spi.frequency(1000000);
  uBit.io.P16.setDigitalValue(1);
  uBit.display.setDisplayMode(DISPLAY_MODE_BLACK_AND_WHITE);
}


void SetMotorPower(int motor, int power)
{
  if (motor < 1 || motor > 2)
    return;

  if (power > 100) {
      power = 100;
  } else if (power < -100) {
      power = -100;
  }

//   uBit.serial.send('\r');
//   uBit.serial.send('\n');
//   uBit.serial.send(motor);
//   uBit.serial.send(' ');
//   uBit.serial.send(power);

  uBit.io.P16.setDigitalValue(0);
  if (motor == 1) {
      spi.write(kRM_Motor1Power);
  } else {
      spi.write(kRM_Motor2Power);
  }
  spi.write(power);
  uBit.io.P16.setDigitalValue(1);
}

void PlayNote(int solfegeNote, int duration) {
  uBit.io.P16.setDigitalValue(0);
  spi.write(kRM_NoteLength);
  spi.write(duration);
  spi.write(kRM_NoteSolfege);
  spi.write(solfegeNote);
  uBit.io.P16.setDigitalValue(1);
}

void PlayNoteStream(char streamOpCode) {
  // Put a note opcode in the stream.
  uBit.io.P16.setDigitalValue(0);
  spi.write(kRM_NoteStream);
  spi.write(streamOpCode);
  uBit.io.P16.setDigitalValue(1);
}

void PlayNoteStream(const char* streamOpCode, int length) {
  // Put a note in ote stream.
  uBit.io.P16.setDigitalValue(0);
  for ( int i = 0; i < length; i++ ) {
    spi.write(kRM_NoteStream);
    spi.write(streamOpCode[i]);
  }
  uBit.io.P16.setDigitalValue(1);
}

int reply[4] = {0};

int ReadEncoder1() {
  uBit.io.P16.setDigitalValue(0);
  spi.write(-15);  // OR kRM_Motor1Encoder
  fiber_sleep(6); //HW
  spi.write(4); // dummy byte - keep at 4
  reply[0] = spi.write(3); // returns 4
  reply[1] = spi.write(2); // returns 3
  reply[2] = spi.write(1); // returns 2
  reply[3] = spi.write(0); // returns 1


  uBit.io.P16.setDigitalValue(1);
  int combo = (reply[0]) | (reply[1] << 8) | (reply[2] << 16) | (reply[3] << 24);
  return combo;
}


int ReadEncoder2() {
  uBit.io.P16.setDigitalValue(0);
  spi.write(-25);  // OR kRM_Motor1Encoder
  fiber_sleep(6); //HW
  spi.write(4); // dummy byte - keep at 4
  reply[0] = spi.write(3); // returns 4
  reply[1] = spi.write(2); // returns 3
  reply[2] = spi.write(1); // returns 2
  reply[3] = spi.write(0); // returns 1

  uBit.io.P16.setDigitalValue(1);
  int combo = (reply[0]) | (reply[1] << 8) | (reply[2] << 16) | (reply[3] << 24);
  return combo;
}

void stopAll()
{
  SetMotorPower(1, 0);
  SetMotorPower(2, 0);
  uBit.display.clear();
}
