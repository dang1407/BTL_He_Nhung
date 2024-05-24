#include <SSD1306.h>
#include <Preferences.h>
#include "asset.h"
#define BLUE_BUTTON_PIN 23
#define WHITE_BUTTON_PIN 33
#define BOOT_BUTTON_PIN 0
#define BUZZER_PIN 18

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define MENU_ITEM_COUNT 4
#define MENU_ITEM_DISPLAY_COUNT 4
#define MENU_PAGE 0
#define PLAY_GAME_PAGE 1
#define END_GAME_PAGE 2
#define RACING_CAR_ROW_COUNT 3
#define FRUIT_COLUMN_COUNT 4
#define RACING_CAR_DISTANCE 32
#define TUBE_DISTANCE   32
#define TUBE_WIDTH      6
#define PATH_WIDTH      30
// IIC default address is 0x3c
SSD1306 display(0x3c, 21, 22);

// Preferences to read/write flash memory
Preferences preferences;
const unsigned long DEBOUNCE_DELAY = 250; // Thời gian debounce (ms)
unsigned long lastBlueButtonPressTime = 0;
unsigned long lastWhiteButtonPressTime = 0;
unsigned long lastCreateBulletTime = 0;
unsigned long keyPressTime = 0;
unsigned long millis_value = 0;
char menuItems[MENU_ITEM_COUNT][20] = {
    "Flappy Bird", "Racing Car", "Catch The Fruits", "Dinosaur"};
char *gameName;
int choosenItemIndex = 0, firstItemIndex = 0, lastItemIndex = MENU_ITEM_COUNT;
int page = 0;
int highScore = 0;
int gameState = 0;
float speed = 0.03;
int score = 0;
float playerX, playerY;
bool hasScored[10] = {false};
bool isBuzzerOn = false;
bool increaseSpeed = false;

// Flappy Bird
// At each time, there are only a maximum of 4 tubes on the screen
float tubeX[4];
int bottomTubeHeight[4];
bool isFlyingUp = false;


// Racing car
float obstacleCarsX[RACING_CAR_ROW_COUNT], obstacleCarsY[RACING_CAR_ROW_COUNT], bulletX = 128.0, bulletY = - 10.00;
int carRow = 1;
bool isExistBullet = false;

// Catch the fruits
float fruitColumnX[FRUIT_COLUMN_COUNT], fruitColumnY[FRUIT_COLUMN_COUNT];
int basketColumn = 1;
bool isRockColumn[FRUIT_COLUMN_COUNT] = {false};
int fruitType[FRUIT_COLUMN_COUNT] = {-1};

void setup()
{
  pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(WHITE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  display.init();

  // Make all asset in the game is right side up
  display.flipScreenVertically();

  while (true)
  {
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setColor(WHITE);
    millis_value = millis();
    for (int i = 0; i < MENU_ITEM_COUNT; i++)
    {
      if (i == choosenItemIndex)
      {
        display.setFont(ArialMT_Plain_16);
      }
      display.drawString(0, i * 16 + 1, menuItems[i]);
      display.setFont(ArialMT_Plain_10);
    }
    if (digitalRead(BLUE_BUTTON_PIN) == LOW && millis_value - lastBlueButtonPressTime > DEBOUNCE_DELAY)
    {
      choosenItemIndex = (choosenItemIndex + 1) % MENU_ITEM_COUNT;
      lastBlueButtonPressTime = millis_value;
    }

    if (digitalRead(WHITE_BUTTON_PIN) == LOW && millis_value - lastWhiteButtonPressTime > DEBOUNCE_DELAY)
    {
      lastBlueButtonPressTime = millis_value;
      gameName = menuItems[choosenItemIndex];
      preferences.begin(gameName, false);
      // Get the high score, if the key does not exist, return a default value of 0
      highScore = preferences.getUInt("highScore", 0);
      // Close the Preferences
      preferences.end();
      page = PLAY_GAME_PAGE;
      break;
    }
    display.display();
  }
}

void loop()
{
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setColor(WHITE);
  millis_value = millis();

  // MAIN GAME
  if (page == PLAY_GAME_PAGE)
  {
    // Game Racing Car
    if (choosenItemIndex == 1)
    {
      RacingCar();
    } else if (choosenItemIndex == 0)
    {
      FlappyBird();
    }
    else if (choosenItemIndex == 2)
    {
      CatchTheFruits();
    }
    
    else
    {
      display.drawString(0, 4, gameName);
    }
  }
  else if (page == END_GAME_PAGE)
  {
  }

  display.display();
}

void RacingCar()
{
  // Display start screen
  if (gameState == 0)
  {
    // Reinitialize in-game status
    score = 0;
    speed = 0.03;

    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 4, "Racing Car");

    display.drawXbm(75, 21, Car_width, Car_height, CarReverse);
    playerY = 24.00;
    display.drawXbm(playerX, playerY, Car_width, Car_height, Car);
    display.setColor(WHITE);
    display.fillRect(0, SCREEN_HEIGHT - 5, SCREEN_WIDTH, 5);

    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 44, "Press blue button to start");

    if (digitalRead(BLUE_BUTTON_PIN) == LOW)
    {
      gameState = 1;
      carRow = 1;
      lastBlueButtonPressTime = millis_value;
      playerX = 10.00;
      playerY = carRow * 21 + 3;
      // Init the obstacle cars
      for (int i = 0; i < RACING_CAR_ROW_COUNT; i++)
      {
        obstacleCarsX[i] = 128 + ((i + 1) * RACING_CAR_DISTANCE * random(i + 1, 8));
        obstacleCarsY[i] = i * 21 + 2;
        hasScored[i] = false;
      }
      delay(50);
    }
  }
  else if (gameState == 1)
  {

    // Display score
    display.setFont(ArialMT_Plain_10);
    display.drawString(3, 0, String(score));
    display.fillRect(0, 20, 128, 2);
    display.fillRect(0, 41, 128, 2);
    unsigned long millis_value = millis();
    if(digitalRead(BLUE_BUTTON_PIN) == LOW &&  digitalRead(WHITE_BUTTON_PIN) == LOW && millis_value - lastCreateBulletTime > DEBOUNCE_DELAY) {
          if(score >= 3) {
            lastCreateBulletTime = millis_value;
            lastBlueButtonPressTime = millis_value;
            lastWhiteButtonPressTime = millis_value;
            score -= 3;
            bulletX = playerX + Car_width;
            bulletY = playerY + 5;
            isExistBullet = true;
          }
        }

    // Setup variables and flags if button is pressed
    if (digitalRead(BLUE_BUTTON_PIN) == LOW && millis_value - lastBlueButtonPressTime > DEBOUNCE_DELAY && !isExistBullet)
    {
      keyPressTime = millis_value;
      lastBlueButtonPressTime = millis_value;
      isBuzzerOn = true;
      if (carRow > 0)
      {
        carRow -= 1;
      }
      playerY = carRow * 21 + 3;
    }

    if (digitalRead(WHITE_BUTTON_PIN) == LOW && millis_value - lastWhiteButtonPressTime > DEBOUNCE_DELAY && !isExistBullet)
    {
      keyPressTime = millis_value;
      lastWhiteButtonPressTime = millis_value;
      if (carRow < RACING_CAR_ROW_COUNT - 1)
      {
        carRow += 1;
      }
      playerY = carRow * 21 + 3;
    }

    

    // Display player car and obstacle cars
    display.setColor(WHITE);
    display.drawXbm(playerX, playerY, Car_width, Car_height, Car);
    display.drawXbm(bulletX, bulletY, Bullet_width, Bullet_height, Bullet);

    

    for (int i = 0; i < RACING_CAR_ROW_COUNT; i++)
    {
        display.drawXbm(obstacleCarsX[i], obstacleCarsY[i], Car_width, Car_height, CarReverse);
    }

    for (int i = 0; i < RACING_CAR_ROW_COUNT; i++)
    {
      // Move all tube to the left
      obstacleCarsX[i] -= speed;
      if(isExistBullet) {
      bulletX += speed; 
      }
      // If a tube pass the bird, add a point
      if (obstacleCarsX[i] < playerX && !hasScored[i])
      {
        score++;
        hasScored[i] = true;

        // Increase speed every 10 tubes
        if (score % 10 == 0)
        {
          speed += 0.01;
        }
      }
      // Xe chướng ngại vật ra khỏi màn chơi sẽ dịch chuyển về phải
      if (obstacleCarsX[i] + Car_width < 0)
      {
        obstacleCarsX[i] = 128 + ((i + 1) * RACING_CAR_DISTANCE * random(0, 6));
        hasScored[i] = false;
      }

      if(-10 <= obstacleCarsY[i] - bulletY && obstacleCarsY[i] - bulletY <= 10 && ((obstacleCarsX[i] <= bulletX && bulletX <= obstacleCarsX[i] + Car_width) || (obstacleCarsX[i] <= bulletX + Bullet_width && bulletX + Bullet_height <= obstacleCarsX[i] + Car_width))) {
        obstacleCarsX[i] = 128;
        bulletX = 128;
        bulletY = -10.0;
        isExistBullet = false;
      }
    }

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

    // Check collision player with other car
    for (int i = 0; i < RACING_CAR_ROW_COUNT; i++)
    {

      if (-3 <= obstacleCarsY[i] - playerY && obstacleCarsY[i] - playerY <= 3 && ((obstacleCarsX[i] <= playerX && playerX <= obstacleCarsX[i] + Car_width) || (obstacleCarsX[i] <= playerX + Car_width && playerX + Car_width <= obstacleCarsX[i] + Car_width)))
      {
        // Ending sound
        digitalWrite(BUZZER_PIN, 1);
        delay(200);
        digitalWrite(BUZZER_PIN, 0);
        delay(50);
        digitalWrite(BUZZER_PIN, 1);
        delay(50);
        digitalWrite(BUZZER_PIN, 0);
        delay(50);
        digitalWrite(BUZZER_PIN, 1);
        delay(50);
        digitalWrite(BUZZER_PIN, 0);

        if (score > highScore)
        {
          highScore = score;

          // Write new high score to flash memory
          preferences.begin(gameName, false);
          preferences.putUInt("highScore", highScore);
          preferences.end();
        }

        gameState = 2;

        delay(50);
      }

      if(-10 <= obstacleCarsY[i] - bulletY && obstacleCarsY[i] - bulletY <= 10 && ((obstacleCarsX[i] <= bulletX && bulletX <= obstacleCarsX[i] + Car_width) || (obstacleCarsX[i] <= bulletX + Bullet_width && bulletX + Bullet_height <= obstacleCarsX[i] + Car_width))) {
        obstacleCarsX[i] = 128;
        bulletX = 128;
        bulletY = -10.0;
        isExistBullet = false;
      }
    }

    // Display boundary
    display.drawRect(0, 0, 128, 64);
  }
  else if (gameState == 2)
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
      carRow = 1;
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
}

void FlappyBird(){
  // Display start screen
  if(gameState == 0) {  
    // Reinitialize in-game status
    playerY = 28.00;
    playerX = 28.88;
    score = 0;
    speed = 0.01;

    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 4, "Flappy ");

    display.drawXbm(64, 0, Building_width, Building_height, Building);
    display.drawXbm(playerX, playerY, Flappy_width, Flappy_height, Flappy);
    display.setColor(WHITE);
    display.fillRect(0, SCREEN_HEIGHT - 5, SCREEN_WIDTH, 5);

    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 44, "Press to start");

    // Reinitialize all tubes
    for(int i = 0; i < 4; i++) {
      tubeX[i] = 128 + ((i+1) * TUBE_DISTANCE);
      bottomTubeHeight[i] = random(8, 32);
      hasScored[i] = false;
    }

    if(digitalRead(BLUE_BUTTON_PIN) == LOW) {
      gameState = 1;
      delay(50);
    }
  }

  //Display in-game screen
  else if (gameState == 1) {  
    //Display score
    display.setFont(ArialMT_Plain_10);
    display.drawString(3, 0, String(score));
   
    // Setup variables and flags if button is pressed
    if(digitalRead(BLUE_BUTTON_PIN) == LOW) {
    keyPressTime = millis();
      isFlyingUp = true;
      isBuzzerOn = true;
    } 

    // Display bird and tubes
    display.setColor(WHITE);
    display.drawXbm(playerX, playerY, Flappy_width, Flappy_height, Flappy);

    for(int i = 0; i < 4; i++) {
      display.fillRect(tubeX[i], 0, TUBE_WIDTH, bottomTubeHeight[i]);
      display.fillRect(tubeX[i], bottomTubeHeight[i] + PATH_WIDTH, TUBE_WIDTH, SCREEN_HEIGHT - bottomTubeHeight[i] - PATH_WIDTH);
    }
  
    for(int i = 0; i < 4; i++) {
      //Move all tube to the left
      tubeX[i] -= speed;

      // If a tube pass the bird, add a point
      if(tubeX[i] < playerX && !hasScored[i]){
        score++;
        hasScored[i] = true;

        // Increase speed every 10 tubes
        if(score % 10 == 0){
          speed += 0.01;
        }
      }

      // If a tube pass the screen, reinitialize that tube on the right of the screen
      if(tubeX[i] + TUBE_WIDTH < 0){
        bottomTubeHeight[i] = random(8, 32);
        tubeX[i] = 128;
        hasScored[i] = false;
      }
    }

    // The bird will fly up for 80 milliseconds
    if((keyPressTime + 80) < millis()) {
      isFlyingUp = false;
    }

    // The buzzer will make sound for 10 milliseconds
    if((keyPressTime + 10) < millis()) {
      isBuzzerOn = false;
    }
  
    // Bird fly up (y-axis is reverted)
    if(isFlyingUp) {
      playerY -= 0.025;
    } 
    else {
      playerY += 0.015;
    }

    // Sound when click
    if(isBuzzerOn){
      digitalWrite(BUZZER_PIN,1);
    }
    else{
      digitalWrite(BUZZER_PIN,0);
    }

    if(playerY > 63 || playerY < 0) { // Check if out of bound on vertical axis
      // Ending sound
      digitalWrite(BUZZER_PIN,1);
      delay(200);
      digitalWrite(BUZZER_PIN,0); 
      delay(50);
      digitalWrite(BUZZER_PIN,1);
      delay(50);
      digitalWrite(BUZZER_PIN,0); 
      delay(50);
      digitalWrite(BUZZER_PIN,1);
      delay(50);
      digitalWrite(BUZZER_PIN,0); 

      if(score > highScore){
        highScore = score;

        // Write new high score to flash memory
        preferences.begin("Flappy", false);
        preferences.putUInt("highScore", highScore);
        preferences.end();
      }

      gameState = 2;
    }

    //Check for collision with tube
    for(int i = 0; i < 4; i++){
      if(tubeX[i] <= playerX + 7 && playerX + 7 <= tubeX[i] + 6) {
        if(playerY < bottomTubeHeight[i] || playerY + 8 > bottomTubeHeight[i] + PATH_WIDTH){
          // Ending sound
          digitalWrite(BUZZER_PIN,1);
          delay(200);
          digitalWrite(BUZZER_PIN,0); 
          delay(50);
          digitalWrite(BUZZER_PIN,1);
          delay(50);
          digitalWrite(BUZZER_PIN,0); 
          delay(50);
          digitalWrite(BUZZER_PIN,1);
          delay(50);
          digitalWrite(BUZZER_PIN,0); 

          if(score > highScore){
            highScore = score;

            // Write new high score to flash memory
            preferences.begin("Flappy", false);
            preferences.putUInt("highScore", highScore);
            preferences.end();
          }

          gameState = 2;

          delay(50);
        }
      }
    }

    // Display boundary
    display.drawRect(0, 0, 128, 64);
  }

  // Display ending (score) screen
  else{ 
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "Your score: " + String(score));
    display.drawString(0, 20, "High score: " + String(highScore));

    display.setFont(ArialMT_Plain_10);
    display.drawString(32, 44, "Click to restart");

    display.setFont(ArialMT_Plain_10);
    display.drawString(5, 54, "Click BOOT to reset score");

    if(digitalRead(BLUE_BUTTON_PIN) == LOW){
      gameState = 0;
      delay(200);
    }

    // If BOOT button is pressed, reset high score in game and in the flash memory
    if(digitalRead(BOOT_BUTTON_PIN) == LOW){
      score = 0;
      highScore = 0;

      // Write high score (= 0) to flash memory
      preferences.begin("Flappy", false);
      preferences.putUInt("highScore", highScore);
      preferences.end();
    }
  }
}

void CatchTheFruits(){
  if(gameState == 0) {
    // Reinitialize in-game status
    score = 0;
    speed = 0.015;
    playerX = 10.0;
    playerY = 25.0;
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 4, "Catch the Fruits");

    display.drawXbm(75, 21, Fruit_width, Fruit_height, Banana);
    // display.drawXbm(75, 40, Rock_width, Rock_height, Rock);
    display.drawXbm(94, 21, Fruit_width, Fruit_height, Apple);
    display.drawXbm(playerX, playerY, Basket_width, Basket_height, Basket);
    display.setColor(WHITE);
    display.fillRect(0, SCREEN_HEIGHT - 5, SCREEN_WIDTH, 5);

    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 44, "Press blue button to start");

    if (digitalRead(BLUE_BUTTON_PIN) == LOW)
    {
      gameState = 1;
      basketColumn = 1;
      lastBlueButtonPressTime = millis();
      lastWhiteButtonPressTime = millis();
      playerY = 45.00;
      playerX = basketColumn * 32 + 1;
      // Init the fruits
      for(int i = 0; i < FRUIT_COLUMN_COUNT; i++) {
        fruitColumnX[i] = i * 32 + 12;
        fruitColumnY[i] = 0;
        fruitType[i] = random(1,5);
      }
      delay(50);
    }
  } else if(gameState == 1) {

    // Display score
    display.setFont(ArialMT_Plain_10);
    display.drawString(3, 0, String(score));
    // Setup variables and flags if button is pressed
    if (digitalRead(BLUE_BUTTON_PIN) == LOW && millis_value - lastBlueButtonPressTime > DEBOUNCE_DELAY )
    {
      keyPressTime = millis_value;
      lastBlueButtonPressTime = millis_value;
      isBuzzerOn = true;
      if (basketColumn > 0)
      {
        basketColumn -= 1;
      }
      playerX = basketColumn * 32 + 3;
    }

    if (digitalRead(WHITE_BUTTON_PIN) == LOW && millis_value - lastWhiteButtonPressTime > DEBOUNCE_DELAY )
    {
      keyPressTime = millis_value;
      lastWhiteButtonPressTime = millis_value;
      isBuzzerOn = true;
      if (basketColumn < FRUIT_COLUMN_COUNT - 1)
      {
        basketColumn += 1;
      }
      playerX = basketColumn * 32 + 3;
    }

    // Display player car and obstacle cars
    display.setColor(WHITE);
    display.drawXbm(playerX, playerY, Basket_width, Basket_height, Basket);

    
    // fruitDataPointer = &Apple;
    for (int i = 0; i < FRUIT_COLUMN_COUNT; i++)
    {
      if(fruitType[i] == 1 ) {
        display.drawXbm(fruitColumnX[i], fruitColumnY[i], Fruit_width, Fruit_height, Apple);
        isRockColumn[i] = false;
      } else if(fruitType[i] == 2) {
        display.drawXbm(fruitColumnX[i], fruitColumnY[i], Fruit_width, Fruit_height, Banana);
        isRockColumn[i] = false;
      } else if(fruitType[i] == 3) {
        display.drawXbm(fruitColumnX[i], fruitColumnY[i], Fruit_width, Fruit_height, PineApple);
        isRockColumn[i] = false;
      } else {
        display.drawXbm(fruitColumnX[i], fruitColumnY[i], Rock_width, Rock_height, Rock);
        isRockColumn[i] = true;
      }
      
    }

    for (int i = 0; i < FRUIT_COLUMN_COUNT; i++)
    {
      // Move all tube to the left
      fruitColumnY[i] += speed;
      // If a tube pass the bird, add a point
      if (!isRockColumn[i] && ( (playerX <= fruitColumnX[i] && fruitColumnX[i] <= playerX + Basket_width) && ((fruitColumnY[i] <= playerY && playerY <= fruitColumnY[i] + Fruit_height) || (playerY <= fruitColumnY[i] && fruitColumnY[i] <= playerY + Basket_height)) ) && !hasScored[i])
      {
        score++;
        hasScored[i] = true;
        fruitColumnY[i] = -16 - random(0, 4) * TUBE_DISTANCE;
        fruitType[i] = random(1,5);
        if(fruitType[i] < 4) {
          isRockColumn[i] = false;
        } else {
          isRockColumn[i] = true;
        }
        hasScored[i] = false;
        // Increase speed every 10 tubes
        if (score % 10 == 0)
        {
          speed += 0.005;
        }
      }
      // Xe chướng ngại vật ra khỏi màn chơi sẽ dịch chuyển về phải
      if (fruitColumnY[i] - Fruit_height > 64)
      {
        fruitColumnY[i] = -16 - (i * RACING_CAR_DISTANCE);
        hasScored[i] = false;
        fruitType[i] = random(1,5);
        if(fruitType[i] < 4) {
          isRockColumn[i] = false;
        } else {
          isRockColumn[i] = true;
        }
      }
    }

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



    // Check collision player with other car
    for (int i = 0; i < FRUIT_COLUMN_COUNT; i++)
    {
      // Kiểm tra va chạm với đá
      if (isRockColumn[i] && ( (playerX <= fruitColumnX[i] && fruitColumnX[i] <= playerX + Basket_width) && ((fruitColumnY[i] <= playerY && playerY <= fruitColumnY[i] + Rock_height) || (playerY <= fruitColumnY[i] && fruitColumnY[i] <= playerY + Basket_height)) ))
      {
        // Ending sound
        digitalWrite(BUZZER_PIN, 1);
        delay(200);
        digitalWrite(BUZZER_PIN, 0);
        delay(50);
        digitalWrite(BUZZER_PIN, 1);
        delay(50);
        digitalWrite(BUZZER_PIN, 0);
        delay(50);
        digitalWrite(BUZZER_PIN, 1);
        delay(50);
        digitalWrite(BUZZER_PIN, 0);

        if (score > highScore)
        {
          highScore = score;

          // Write new high score to flash memory
          preferences.begin(gameName, false);
          preferences.putUInt("highScore", highScore);
          preferences.end();
        }

        gameState = 2;

        delay(50);
      }

      
    }

    // Display boundary
    display.drawRect(0, 0, 128, 64);

  } else if(gameState == 2) {
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "Your score: " + String(score));
    display.drawString(0, 20, "High score: " + String(highScore));

    display.setFont(ArialMT_Plain_10);
    display.drawString(32, 44, "Click blue button to restart");

    display.setFont(ArialMT_Plain_10);
    display.drawString(5, 54, "Click BOOT to reset score");

    if(digitalRead(BLUE_BUTTON_PIN) == LOW){
      gameState = 0;
      delay(200);
    }

    // If BOOT button is pressed, reset high score in game and in the flash memory
    if(digitalRead(BOOT_BUTTON_PIN) == LOW){
      score = 0;
      highScore = 0;

      // Write high score (= 0) to flash memory
      preferences.begin(gameName, false);
      preferences.putUInt("highScore", highScore);
      preferences.end();
    }
  }
}
