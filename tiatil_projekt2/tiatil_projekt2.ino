#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int leds[4] = {8, 9, 10, 11};
int buttons[4] = {2, 3, 4, 5};

unsigned long startTime;
unsigned long reactionTime;

unsigned long totalTime = 0;
unsigned long highscore = 999999;

int roundCounter = 0;
int activeLED = -1;


// Släck alla lampor
void turnOffAllLEDs() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], LOW);
  }
}


// Vänta tills valfri knapp trycks
void waitForAnyButton() {
  while (true) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttons[i]) == LOW) {
        while (digitalRead(buttons[i]) == LOW); // vänta tills släppt
        return;
      }
    }
  }
}


void setup() {

  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(buttons[i], INPUT_PULLUP);
  }

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);

  randomSeed(analogRead(A0));

  // Startskärm
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.println("Tryck");
  display.setCursor(10, 45);
  display.println("Start");
  display.display();

  waitForAnyButton();
}


void loop() {

  turnOffAllLEDs();   // Säkerställer att ingen LED lyser

  delay(random(1000, 3000));

  // Välj EN slumpmässig LED
  activeLED = random(0, 4);

  turnOffAllLEDs();               // Extra säkerhet
  digitalWrite(leds[activeLED], HIGH);   // Tänd endast den

  startTime = millis();

  // Vänta på rätt knapp
  while (true) {

    // Rätt knapp
    if (digitalRead(buttons[activeLED]) == LOW) {
      reactionTime = millis() - startTime;
      break;
    }

    // Fel knapp (ignoreras)
    for (int i = 0; i < 4; i++) {
      if (i != activeLED && digitalRead(buttons[i]) == LOW) {
        while (digitalRead(buttons[i]) == LOW);
      }
    }
  }

  turnOffAllLEDs();

  totalTime += reactionTime;
  roundCounter++;

  // Visa reaktionstid
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Tid:");
  display.setCursor(0, 30);
  display.print(reactionTime);
  display.print(" ms");
  display.display();

  delay(1500);


  // Efter 10 rundor
  if (roundCounter == 10) {

    unsigned long average = totalTime / 10;

    if (average < highscore) {
      highscore = average;
    }

    display.clearDisplay();
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.print("Medel (10): ");
    display.print(average);
    display.print(" ms");

    display.setCursor(0, 20);
    display.print("Highscore: ");
    display.print(highscore);
    display.print(" ms");

    display.setCursor(0, 40);
    display.print("Tryck for ny omgang");

    display.display();

    totalTime = 0;
    roundCounter = 0;

    waitForAnyButton();
  }
}