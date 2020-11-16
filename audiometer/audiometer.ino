#include <Volume.h>

const int STOP_BUTTON = 2;
const int START_BUTTON = 3;
const int RIGHT_BUTTON = 4;
const int LEFT_BUTTON = 7;
const int BLUE_LED = 9;
const int GREEN_LED = 10;
const int RED_LED = 11;

#define COMMON_ANODE

int stopButtonState = 0;
int startButtonState = 0;
int rightButtonState = 0;
int leftButtonState = 0;
int exitCode = 0;
Volume vol;

void setColor(int red, int green, int blue)
{

#ifdef COMMON_ANODE
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
#endif
  analogWrite(RED_LED, red);
  analogWrite(GREEN_LED, green);
  analogWrite(BLUE_LED, blue);
}

void turnOnRedLight()
{
  setColor(255, 0, 0);
}

void turnOnGreenLight()
{
  setColor(0, 255, 0);
}

void turnOnBlueLight()
{
  setColor(0, 0, 255);
}

void turnOnOrangeLight()
{
  setColor(255, 99, 71);
}

void startAudiometerTest()
{
  while (exitCode == 0)
  {
    turnOnBlueLight();
    int earSide = chooseMainEarSide();
    exitChecker();
  }
}

void exitChecker()
{
  stopButtonState = digitalRead(STOP_BUTTON);
  if (stopButtonState == 1)
  {
    exitCode = 1;
    startButtonState = 0;
    vol.end();
  };
}

int chooseMainEarSide()
{
  while (exitCode == 0)
  {
    //testLeftEar();
    // testRightEar();

    while (exitCode == 0)
    {
      exitChecker();
    }

    exitChecker();
  }

  return 0;
}

void testLeftEar()
{
  vol.alternatePin(false);
  vol.tone(1000, 255);
  vol.delay(1000);
  vol.end();
}

void testRightEar()
{
  vol.alternatePin(true);
  vol.tone(1000, 255);
  vol.delay(1000);
   vol.end();
}

void setup()
{
  Serial.begin(9600);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(START_BUTTON, INPUT);
  pinMode(STOP_BUTTON, INPUT);
  vol.begin();
}

void loop()
{
  exitCode = 0;
  turnOnRedLight();
  startButtonState = digitalRead(START_BUTTON);
  if (startButtonState == 1)
    startAudiometerTest();
}
