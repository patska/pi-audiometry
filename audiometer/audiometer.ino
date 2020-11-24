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
const long WAIT_BLINK_INTERVAL = 100000;
const long FRENQUENCY_BUZZER_INTERVAL = 200000;

#define COMMON_ANODE

int frequencyArray[6] = {2000, 4000, 6000, 8000, 500, 250};
int leftSideArray[6] = {0, 0, 0, 0, 0, 0};
int rightSideArray[6] = {0, 0, 0, 0, 0, 0};
int stopButtonState = 0;
int startButtonState = 0;
int rightButtonState = 0;
int leftButtonState = 0;
int exitCode = 0;
int returnCode = -1;
int ledState = LOW;
bool buzzerOn = false;
unsigned long waitPreviousMillis = 0;
unsigned long previousMillis = 0;
unsigned long buzzerPreviousMillis = 0;

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
  setColor(255, 255, 0);
  ledState = HIGH;
}

void turnOffLed()
{
  setColor(0, 0, 0);
  ledState = LOW;
}
void startAudiometerTest()
{
  while (exitCode == 0)
  {
    turnOnBlueLight();
    int earSide = chooseMainEarSide();
    if (earSide == 0 || earSide == 1)
    {
      doWait();
      doTest(earSide);
    }

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
  while (exitCode == 0)
  {
    if (millis() - currentMillis <= interval)
    {
      tone(LEFT_BUZZER, 1000);
    }
    else
    {
      noTone(LEFT_BUZZER);
      if (millis() - currentMillis < 200000)
        tone(RIGHT_BUZZER, 1000);
      else
      {
        noTone(RIGHT_BUZZER);
        leftButtonState = digitalRead(LEFT_BUTTON);
        rightButtonState = digitalRead(RIGHT_BUTTON);
        if (leftButtonState == 1)
        {
          returnCode = 0;
        }
        if (rightButtonState == 1)
        {
          returnCode = 1;
        }
      };
    }
    if (returnCode != -1)
      break;
    exitChecker();
  }

  return returnCode;
}

void doWait()
{
  int counter = 0;
  while (exitCode == 0 && counter != 4)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= WAIT_BLINK_INTERVAL)
    {
      if (ledState == LOW)
      {
        turnOnOrangeLight();
        counter++;
      }
      else
        turnOffLed();
      previousMillis = currentMillis;
    }
    exitChecker();
  }
}
void doTest(int earSide)
{
  while (exitCode == 0)
  {
    turnOnGreenLight();
    int earSideBuzzer[2] = {LEFT_BUZZER, RIGHT_BUZZER};
    int earSideButton[2] = {LEFT_BUTTON, RIGHT_BUTTON};
    int optionSelected = 0;
    int earTestedCounter = 0;
    int tempArray[6];

    while (earTestedCounter < 2 && exitCode == 0)
    {
      int counter = 0;
      buzzerOn = false;
      int buttonState;
      if (earTestedCounter == 1)
      {
        if (earSide == 1)
          earSide = 0;
        else if (earSide == 0)
          earSide = 1;
      
        delay(2000);
      }
  
      while (counter != 7 && exitCode == 0)
      {
        unsigned long currentMillis = millis();
        if (!buzzerOn)
        {
          buttonState = digitalRead(earSideButton[earSide]);
          if (buttonState == 1)
          {
            if (earSide == 0)
              leftSideArray[counter - 1] = 1;
            else if (earSide == 1){
              rightSideArray[counter - 1] = 1;
            }
           
          }
        }
        if (currentMillis - buzzerPreviousMillis >= FRENQUENCY_BUZZER_INTERVAL)
        {
          if (buzzerOn)
          {
            noTone(earSideBuzzer[earSide]);
            buzzerOn = false;
          }
          else
          {
            if (counter < 6)
            {
              tone(earSideBuzzer[earSide], frequencyArray[counter]);
              buzzerOn = true;
            }
            counter++;
          }
          buzzerPreviousMillis = currentMillis;
        }
        exitChecker();
      }
      exitChecker();
      earTestedCounter++;
    }
    printResult();
    exitChecker();
  }
}

void printResult()
{
  while (exitCode == 0)
  {
    for (int i = 0; i < 6; i++)
    {
      String frequency = String(frequencyArray[i]);
      String leftEar = String(leftSideArray[i]);
      String rightEar = String(rightSideArray[i]);
      Serial.println(frequency + " - " + leftEar + " - " + rightEar);
    }
    exitCode = 1;
  }
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
  returnCode = -1;
  turnOnRedLight();
  startButtonState = digitalRead(START_BUTTON);
  if (startButtonState == 1)
    startAudiometerTest();
}
