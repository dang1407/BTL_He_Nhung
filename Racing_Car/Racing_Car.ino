#include <SSD1306.h>
#include <Preferences.h>

#include "asset.h"
#include "racingcar.h"

#define BLUE_BUTTON_PIN 23
#define WHITE_BUTTON_PIN 33
#define BOOT_BUTTON_PIN 0
#define BUZZER_PIN 18

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define TUBE_DISTANCE 32
#define TUBE_WIDTH 6
#define PATH_WIDTH 30
#define ENEMY_CAR_WITH 40

// IIC default address is 0x3c
SSD1306 display(0x3c, 21, 22);

// Preferences to read/write flash memory
Preferences preferences;



unsigned int score = 0;
unsigned int highScore;
unsigned int gameState = 0; // 0 - start, 1 - play, 2 - score

bool isFlyingUp = false;
bool isBuzzerOn = false;
bool increaseSpeed = false;
bool hasScored[4];

float playerX = 10.00;
float playerY = 28.00;
float speed = 0.03;
char* gameName = "Car";
unsigned long keyPressTime = 0;

const unsigned long DEBOUNCE_DELAY = 250; // Thời gian debounce (ms)
unsigned long lastUpButtonPressTime = 0;
unsigned long lastDownButtonPressTime = 0;

typedef struct PlayerData
{
  float playerX, playerY;
  unsigned char Player[];
};


void setup()
{
  pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(WHITE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  preferences.begin(gameName, false);
  // Get the high score, if the key does not exist, return a default value of 0
  highScore = preferences.getUInt("highScore", 0);
  // Close the Preferences
  preferences.end();

  display.init();

  // Initialize obstacle cars in the top off screen
  for (int i = 0; i < ROW_COUNT; i++)
  {
    obstacleCarsX[i] = 128 + i * TUBE_DISTANCE;
    obstacleCarsY[i] = i * 21 + 2;
  }

  // Make all asset in the game is right side up
  display.flipScreenVertically();
}

void loop()
{
  display.clear();

  // Display start screen
  if (gameState == 0)
  {
    // Reinitialize in-game status
    playerY = 24.00;
    score = 0;
    speed = 0.01;

    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 4, "Racing Car");

    display.drawXbm(75, 21, Truck_width, Truck_height, Truck);
    display.drawXbm(playerX, playerY, Car_width, Car_height, Car);
    display.setColor(WHITE);
    display.fillRect(0, SCREEN_HEIGHT - 5, SCREEN_WIDTH, 5);

    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 44, "Press blue button to start");

    // Reinitialize all tubes
    for (int i = 0; i < ROW_COUNT; i++)
    {
      obstacleCarsX[i] = 128 + ((i + 1) * TUBE_DISTANCE);
      obstacleCarsY[i] = i*21 + 2;
      hasScored[i] = false;
    }

    if (digitalRead(BLUE_BUTTON_PIN) == LOW)
    {
      gameState = 1;
      delay(50);
    }
  }

  // Display in-game screen
  else if (gameState == 1)
  {
    // Display score
    display.setFont(ArialMT_Plain_10);
    display.drawString(3, 0, String(score));
    unsigned long millis_value = millis();
    // Setup variables and flags if button is pressed
    if (digitalRead(BLUE_BUTTON_PIN) == LOW && millis_value - lastUpButtonPressTime > DEBOUNCE_DELAY)
    {
      keyPressTime = millis_value;
      lastUpButtonPressTime = millis_value;
      isBuzzerOn = true;
      if(carRow > 0) {
        carRow -= 1;
      }
      playerY = carRow * 21 + 3;
    }

    if(digitalRead(WHITE_BUTTON_PIN) == LOW && millis_value - lastDownButtonPressTime > DEBOUNCE_DELAY) {
      keyPressTime = millis_value;
      lastDownButtonPressTime = millis_value;
      if(carRow < ROW_COUNT - 1) {
        carRow += 1;
      }
      playerY = carRow * 21 + 3;
    }

    // Display player car and obstacle cars
    display.setColor(WHITE);
    display.drawXbm(playerX, playerY, Car_width, Car_height, Car);

    // for (int i = 0; i < 3; i++)
    // {
    //   display.drawXbm(obstacleCarsX[i], obstacleCarsY[i], Truck_width, Truck_height, Truck);
    // }

    // for (int i = 0; i < ROW_COUNT; i++)
    // {
    //   // Move all tube to the left
    //   obstacleCarsX[i] -= obstacleCarsSpeed[vehicleType[i]];

    //   // If a tube pass the bird, add a point
    //   if (obstacleCarsX[i] < playerX && !hasScored[i])
    //   {
    //     score++;
    //     hasScored[i] = true;

    //     // Increase speed every 10 tubes
    //     if (score % 10 == 0)
    //     {
    //       speed += 0.01;
    //     }
    //   }

    //   if(obstacleCarsX[i] + Truck_width < 0) {

    //   }
    // }

    // The buzzer will make sound for 10 milliseconds
    if ((keyPressTime + 10) < millis())
    {
      isBuzzerOn = false;
    }

    // Sound when click
    if (isBuzzerOn)
    {
      digitalWrite(BUZZER_PIN, 1);
    }
    else
    {
      digitalWrite(BUZZER_PIN, 0);
    }

    // Display boundary
    display.drawRect(0, 0, 128, 64);
  }

  // Display ending (score) screen
  else
  {
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "Your score: " + String(score));
    display.drawString(0, 20, "High score: " + String(highScore));

    display.setFont(ArialMT_Plain_10);
    display.drawString(32, 44, "Click to restart");

    display.setFont(ArialMT_Plain_10);
    display.drawString(5, 54, "Click BOOT to reset score");

    if (digitalRead(BLUE_BUTTON_PIN) == LOW)
    {
      gameState = 0;
      delay(200);
    }

    // If BOOT button is pressed, reset high score in game and in the flash memory
    if (digitalRead(BOOT_BUTTON_PIN) == LOW)
    {
      score = 0;
      highScore = 0;

      // Write high score (= 0) to flash memory
      preferences.begin(gameName, false);
      preferences.putUInt("highScore", highScore);
      preferences.end();
    }
  }

  display.display();
}

