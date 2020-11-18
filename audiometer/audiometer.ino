#include <Volume.h>

const int STOP_BUTTON = 2;
const int START_BUTTON = 3;
const int RIGHT_BUTTON = 4;
const int LEFT_BUTTON = 7;
const int BLUE_LED = 9;
const int GREEN_LED = 10;
const int RED_LED = 11;
const int LEFT_BUZZER = 5;
const int RIGHT_BUZZER = 6;

#define COMMON_ANODE

int testMatrix[2][6] = {2000, 4000, 6000, 8000, 500, 250, 0, 0, 0, 0, 0, 0};
int stopButtonState = 0;
int startButtonState = 0;
int rightButtonState = 0;
int leftButtonState = 0;
int exitCode = 0;
int returnCode = -1;

unsigned long previousMillis = 0;
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
    if (earSide == 1 || earSide == 2) doTest(earSide);
    else
      Serial.println("Ear side doesn't chosen");

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
    noTone(LEFT_BUZZER);
    noTone(RIGHT_BUZZER);
  };
}

int chooseMainEarSide()
{
  const long interval = 100000;
  unsigned long currentMillis = millis();
  while (exitCode == 0 && returnCode == -1)
  {
    if (millis() - currentMillis <= interval)
    {
      tone(LEFT_BUZZER, 1000);
    }
    else
    {
      noTone(LEFT_BUZZER);
      if (millis() - currentMillis < 300000)
        tone(RIGHT_BUZZER, 1000);
      else
      {
        noTone(RIGHT_BUZZER);
        leftButtonState = digitalRead(LEFT_BUTTON);
        rightButtonState = digitalRead(RIGHT_BUTTON);
        if (leftButtonState == 1)
        {
          returnCode =  0;
        }
        if (rightButtonState == 1)
        {
          returnCode =  1;
        }
      };
    }
    Serial.println(returnCode);
    if (returnCode != -1) break;
    exitChecker();
  }

  return returnCode;
}

// void testLeftEar()
// {
//   vol.alternatePin(false);
//   vol.tone(1000, 255);
//   vol.delay(1000);
//   vol.end();
// }

// void testRightEar()
// {
//   vol.alternatePin(true);
//   vol.tone(1000, 255);
//   vol.delay(1000);
//   vol.end();
// }

void doTest(int earSide){
  if (earSide == 1) Serial.println("Test will start in left ear");
  else Serial.println("Test will start in right ear");

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
  Serial.println("Audiometer started");
}

void loop()
{
  exitCode = 0;
  returnCode = -1;
  turnOnRedLight();
  startButtonState = digitalRead(START_BUTTON);
  if (startButtonState == 1)
    startAudiometerTest();
}
