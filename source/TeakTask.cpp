#include <MicroBit.h>
#include <ctype.h>
#include "TeakTask.h"
#include "TBCDriver.h"
extern MicroBit uBit;

short corrections[101] = {};
short motor_correction;
//short counter = 0;
extern short versionNumber;
extern bool connected;

SPI spi(MOSI, MISO, SCK);

int hexCharToInt(char c) {
  if ((c >= '0') && (c <= '9')) {
    return c - '0';
  } else if ((c >= 'a') && (c <= 'f')) {
    return c - 'a' + 10;
  }  else if ((c >= 'A') && (c <= 'F')) {
    return c - 'A' + 10;
  }
  return 0;
}

// void TeakTaskManager::MicrobitDalEvent(MicroBitEvent event)
// {
//       if (event.source == MICROBIT_ID_BLE) {
//           if (event.value == MICROBIT_BLE_EVT_CONNECTED) {
//               m_btConnected = true;
//         			connected = true;
//               uBit.display.print('C');
//         			versionNumber = 10;
//           } else if (event.value == MICROBIT_BLE_EVT_DISCONNECTED) {
//               m_btConnected = false;
//         			connected = false;
//               uBit.display.print('D');
//         			versionNumber = -10;
//           }
//       //} else if (event.value == MICROBIT_BUTTON_EVT_HOLD) {
//           //if (event.source == MICROBIT_ID_BUTTON_B) {
//             // uBit.display.print('G');
//           //}
//       } else if (event.value == MICROBIT_BUTTON_EVT_CLICK || event.value == MICROBIT_BUTTON_EVT_HOLD) {
//            if (event.source == MICROBIT_ID_BUTTON_A) {
//             uart->send(FLASH_STRU(gStrA), FLASH_STR_LEN(gStrA));
//            } else if (event.source == MICROBIT_ID_BUTTON_B) {
//              uart->send(FLASH_STRU(gStrB), FLASH_STR_LEN(gStrB));
//            } else if (event.source == MICROBIT_ID_BUTTON_AB) {
//              uart->send(FLASH_STRU(gStrAB), FLASH_STR_LEN(gStrAB));
//            }
//       } else if (event.source == MICROBIT_ID_DISPLAY) {
//         if (event.value == MICROBIT_DISPLAY_EVT_ANIMATION_COMPLETE) {
//           m_animating = false;
//           m_currentImage = 0;
//         }
//       }
//       else if (event.source == MICROBIT_ID_TIMER) {
//           // uBit.serial.send('\r');
//           // uBit.serial.send('\n');
//           // uBit.serial.send(revolutions);
//           // uBit.serial.send(' ');
//           // uBit.serial.send(counter);
//           // uBit.serial.send(' ');
//           // uBit.serial.send(THRESHOLD);
//         // if (counter < 100)
//         // {
//         //   counter++;
//         // }
//         // if (counter == 100)
//         // {
//         //   calibrate();
//         //   counter++;
//         // }
//       }
//
//
//       if (m_currentTask != NULL) {
//           if (event.source == MICROBIT_ID_BUTTON_B && event.value == MICROBIT_BUTTON_EVT_HOLD) {
//               // The Task swap key is treated as a special event
//               event.source = MICROBIT_ID_TASK_SWAP;
//               m_currentTask->Event(event);
//               event.source = MICROBIT_ID_BUTTON_B;
//             }
//
//           m_currentTask->Event(event);
//       }
//
//       if (!m_currentTask->AsyncImage()) {
//           int newImage = m_currentTask->PackedImage();
//           if ((newImage != m_currentImage) && (!m_animating)) {
//               // If task image has changed the push it to the LEDs
//             PBmapUnpack(newImage, uBit.display.image.getBitmap(), uBit.display.image.getWidth());
//             m_currentImage = newImage;
//         }
//     }
// }


MicroBitImage image(5,5);
int j = 0;
int pixVal = 1;

void MicrobitBtEvent(const char* str) {
  int len = strlen(str);
  int value = 0;
  if ((strncmp(str, "(px:", 4) == 0) && len >= 14) {
      str += 4;
      for (int i = 0; i < 5; i++) {
          char c1 = hexCharToInt(*str);
          str++;
          char c2 = hexCharToInt(*str);
          str++;
          image.setPixelValue(0, i, c1 & 0x01);
          image.setPixelValue(1, i, c2 & 0x08);
          image.setPixelValue(2, i, c2 & 0x04);
          image.setPixelValue(3, i, c2 & 0x02);
          image.setPixelValue(4, i, c2 & 0x01);
      }
      uBit.display.print(image);
  } else if ((strncmp(str, "(sr:", 4) == 0) && len >= 5) {
      str += 4;
      value = atoi(str);
      uBit.io.P1.setServoValue(value);
      uBit.display.scroll("S");
  } else if ((strncmp(str, "(m:", 3) == 0) && len >= 4) {
      str += 3;
      if(strncmp(str, "(1 2)", 5) == 0) {
          if(strncmp(str + 6, "d", 1) == 0) {
              value = atoi(str + 8);

                int index = (-value+5)/10*10;
                int correction = corrections[index];
                correction = 1.0 * correction / index * (-value);
                if (correction > 0)
                {
                  correction = 0;
                }
                SetMotorPower(1, value);
                SetMotorPower(2, -value+correction);

          }
      } else if(strncmp(str, "1", 1) == 0) {
          if(strncmp(str + 2, "d", 1) == 0) {
              value = atoi(str + 4);
              SetMotorPower(1, value);
          }
      } else if(strncmp(str, "2", 1) == 0) {
          if(strncmp(str + 2, "d", 1) == 0) {
              value = atoi(str + 4);

                int index = (-value+5)/10*10;
                int correction = corrections[index];
                correction = 1.0 * correction / index * (-value);
                if (correction > 0)
                {
                  correction = 0;
                }
                SetMotorPower(2, -value+correction);

          }
      }
  } else if ((strncmp(str, "(nt:", 4) == 0) && len >= 5) {
      // Notes come in the form nn where n is the
      // piano key number
      value = atoi(str + 4);
      if (value <= 13) {
        // early version pay in register0 ( notes 1-12 ( + next C))
        // bump to C4 (key number 40)
        value += 39;
      }
      PlayNote(value, 64);
  } else if ((strncmp(str, "(pr:", 4) == 0) && len >= 5) {
      value = atoi(str + 4);
      // m_animating = true;
      uBit.display.scroll(value);
	//   versionNumber = -10;
	//   MicroBitEvent tick(MICROBIT_ID_TIMER, 0, CREATE_ONLY);
	//   	  MicrobitDalEvent(tick);
	//   	  	// uBit.display.scroll("1");
	// 	char buffer [20];
	// 	// uBit.display.scroll("2");
	// 	const char* versionMessage = "(vs:%d)";
	// 	// uBit.display.scroll("3");

	// 	snprintf(buffer, sizeof(buffer), versionMessage, versionNumber);
	// 	// uBit.display.scroll("4");

	// 	uBit.serial.send(buffer);
	// 	// uBit.display.scroll("5");

	// 	uart->send((uint8_t *)buffer, strlen(buffer));
		// uBit.display.scroll("6");
  } else if ((strncmp(str, "(st)", 4) == 0)) {
      stopAll();
  } else if((strncmp(str, "(vr)", 4) == 0)) {
    	// uBit.display.scroll("1");
        // char buffer [20];
    	// uBit.display.scroll("2");
    	// const char* versionMessage = "(vs:%d)";
    	// uBit.display.scroll("3");

    	// snprintf(buffer, sizeof(buffer), versionMessage, versionNumber);
    	// uBit.display.scroll("4");

    	// uBit.serial.send(buffer);
    	// uBit.display.scroll("5");

    	// uart->send((uint8_t *)buffer, strlen(buffer));
    	// uBit.display.scroll("6");
    	versionNumber = -10;
  } else if ((strncmp(str, "(cl)", 4) == 0)) {
    /*
    calibrate();
    //uBit.display.print(kEmojiHouse);
    */
  } else {
      // Debug option, if its not understood show the message.
      uBit.display.scroll(str);
  }
}
