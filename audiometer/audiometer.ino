
const int START_BUTTON_PIN = 2;     
const int START_EAR_SIDE_PIN = 3; 
const int INPUT_BUTTON = 6:
const int LED_START = 4; 
const int LED_STOP = 5;
int startButtonState = 0;  
int earSideButtonState = 0;

void setup() {
  pinMode(START_BUTTON_PIN, INPUT);
  pinMode(START_EAR_SIDE_PIN, INPUT);
  pinMode(INPUT_BUTTON, INPUT);
  pinMode(LED_START, OUTPUT);
  pinMode(LED_STOP, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  tone(2200, 6);
  startButtonState = digitalRead(START_BUTTON_PIN);
  earSideButtonState = digitalRead(START_EAR_SIDE_PIN);
  if (startButtonState == 1) {
    Serial.println("Button ON");
    digitalWrite(LED_START, 1);
    digitalWrite(LED_STOP, 0);
    if (earSideButtonState == 0) Serial.println("Inicio pela orelha direita!");
    else Serial.println("Inicio pela orelha esquerda!");
  } else {
    Serial.println("Button OFF");
    digitalWrite(LED_START, LOW);
    digitalWrite(LED_STOP, HIGH);
  }
  
  delay(2000);
}
