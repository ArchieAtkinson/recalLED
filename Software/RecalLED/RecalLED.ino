//By ProjectAPA
//Clock = 1MHz
//Has been flashed setting EEPROM byte 0 to 0
//Fuse High bits 3 has change to 1 to enable EEPROM

#include <EEPROM.h> //Allow EEPROM Libary
#define adc_disable() (ADCSRA &= ~(1<<ADEN)) //Disables ADC to save power

//Sets usable pins and number of pins
const int pins[] = {3, 4, 0, 1};
const int pinsLens = 3;

//Unused pin used to generate random number
const int RandPin = 2;

int ran;
int ans[128];
int in[128];

bool wait;

int buttonState[pinsLens + 1];
int lastButtonState[pinsLens + 1];
int reading[pinsLens + 1];
unsigned long lastDebounceTime[pinsLens + 1];

int i;
int j;
bool fail;

unsigned long debounceDelay = 15;
int level = 3;
byte highscore;

void setup()
{
  randomSeed(analogRead(RandPin)); //Gain random seed using unused pin

  //Turns all LED pins to Input with Pull-up
  MCUCR &= ~(1 << PUD);
  DDRB &= ~((1 << DDB0) | (1 << DDB1) | (1 << DDB3) | (1 << DDB4));
  PORTB |= ((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB3) | (1 << PORTB4));
  
  delay(1000);
  
  adc_disable();

  //Starts up some cool flashing lights
  startup();

  delay(750);
  
  //reads high score to be displayed on start up
  highscore = EEPROM.read(0);
  for (i = 0; i < highscore; i++) {
    DDRB |= ((1 << DDB0) | (1 << DDB4));
    PORTB &= ~((1 << PORTB0) | (1 << PORTB4));
    delay(300);
    MCUCR &= ~(1 << PUD);
    DDRB &= ~((1 << DDB0) | (1 << DDB4));
    PORTB |= ((1 << PORTB0) | (1 << PORTB4));
    delay(300);
  }

}

void loop()
{
  delay(500);

  startround();

  delay(750);

  //Randomly flashes LED and store value
  for (i = 0; i <= level; i++) {
    ran = random(0, (pinsLens + 1));
    ans[i] = ran;
    digitalWrite(pins[ran], LOW);
    pinMode(pins[ran], OUTPUT);
    delay(250);
    pinMode(pins[ran], INPUT_PULLUP);
    delay(250);
  }
  
  MCUCR &= ~(1 << PUD);
  DDRB &= ~((1 << DDB0) | (1 << DDB1) | (1 << DDB3) | (1 << DDB4));
  PORTB |= ((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB3) | (1 << PORTB4));


  //Sets debounce vars 
  for (int i = 0; i <= pinsLens; i++) {
    buttonState[i] = HIGH;
    lastButtonState[i] = HIGH;
    lastDebounceTime[i] = 0;
  }
  
  //Goes though each blink of LED and debounces inputs
  for (j = 0; j <= level; j++) {
    wait = true;
    while (wait == true) {
      for (i = 0; i <= pinsLens; i++) {
        reading[i] = digitalRead(pins[i]);

        if (reading[i] != lastButtonState[i]) {
          lastDebounceTime[i] = millis();
        }

        if ((millis() - lastDebounceTime[i]) > debounceDelay) {

          if (reading[i] != buttonState[i]) {
            buttonState[i] = reading[i];

            if (buttonState[i] == LOW) {
              in[j] = i;
              wait = false;
            }
          }
        }
        lastButtonState[i] = reading[i];
      }
    }
  }

  //Comparing all inputs
  fail = false;
  for (i = 0; 127 < ; i++) {
    if (in[i] != ans[i]) {
      fail = true;
      break;
    }
  }

  delay(600);
  //If they got it right advances the level
  if (fail == false) {
    for (i = 0; i < 2; i++) {
      DDRB |= (1 << DDB3);
      PORTB &= ~(1 << PORTB3);
      delay(200);
      MCUCR &= ~(1 << PUD);
      DDRB &= ~(1 << DDB3);
      PORTB |= (1 << PORTB3);
      delay(200);
    }
    level++;
    }
  //If wrong reset, if highscore beaten inform player
  else if (fail == true) {
    for (i = 0; i < 2; i++) {
      DDRB |= (1 << DDB1);
      PORTB &= ~(1 << PORTB1);
      delay(200);
      MCUCR &= ~(1 << PUD);
      DDRB &= ~(1 << DDB1);
      PORTB |= (1 << PORTB1);
      delay(200);
    }

    delay(500);
    
    highscore = EEPROM.read(0);
    if (highscore < (level - 3)) {
      highscore = (level - 3);
      EEPROM.update(0, highscore);
      DDRB |= ((1 << DDB1) | (1 << DDB3));
      PORTB &= ~((1 << PORTB1) | (1 << PORTB3));
      delay(200);
      MCUCR &= ~(1 << PUD);
      DDRB &= ~((1 << DDB1) | (1 << DDB3));
      PORTB |= ((1 << PORTB1) | (1 << PORTB3));
      delay(200);
      DDRB |= ((1 << DDB1) | (1 << DDB3));
      PORTB &= ~((1 << PORTB1) | (1 << PORTB3));
      delay(200);
      MCUCR &= ~(1 << PUD);
      DDRB &= ~((1 << DDB1) | (1 << DDB3));
      PORTB |= ((1 << PORTB1) | (1 << PORTB3));
      
      delay(500);
    }
   
    for (i = 0; i < (level - 3); i++) {
      DDRB |= ((1 << DDB0) | (1 << DDB4));
      PORTB &= ~((1 << PORTB0) | (1 << PORTB4));
      delay(300);
      MCUCR &= ~(1 << PUD);
      DDRB &= ~((1 << DDB0) | (1 << DDB4));
      PORTB |= ((1 << PORTB0) | (1 << PORTB4));
      delay(300);
    }
    delay(500);
    level = 3;
  }

  MCUCR &= ~(1 << PUD);
  DDRB &= ~((1 << DDB0) | (1 << DDB1) | (1 << DDB3) | (1 << DDB4));
  PORTB |= ((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB3) | (1 << PORTB4));
}


void startup()
{

  for (i = 0; i <= pinsLens; i++) {
    digitalWrite(pins[i], LOW);
    pinMode(pins[i], OUTPUT);
    delay(150);
    pinMode(pins[i], INPUT_PULLUP);
    delay(150);
  }
  for (i = (pinsLens-1); i >= 0; i--) {
    digitalWrite(pins[i], LOW);
    pinMode(pins[i], OUTPUT);
    delay(150);
    pinMode(pins[i], INPUT_PULLUP);
    delay(150);
  }

}

void startround()
{
  DDRB |= ((1 << DDB0) | (1 << DDB1) | (1 << DDB3) | (1 << DDB4));
  PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB3) | (1 << PORTB4));

  delay(750);

  MCUCR &= ~(1 << PUD);
  DDRB &= ~((1 << DDB0) | (1 << DDB1) | (1 << DDB3) | (1 << DDB4));
  PORTB |= ((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB3) | (1 << PORTB4));

  for (i = 0; i < 127; i++) {
    ans[i] = 0;
    in[i] = 0;
  }
  fail = false;
}
