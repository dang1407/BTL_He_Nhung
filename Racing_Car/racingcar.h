#define RACING_CAR_ROW_COUNT 3

// At each time, there are only a maximum of RACING_CAR_ROW_COUNT tubes on the screen
float obstacleCarsX[RACING_CAR_ROW_COUNT];
int obstacleCarsY[RACING_CAR_ROW_COUNT];

float obstacleCarsSpeed[3] = {0.02, 0.05, 0.03};
int vehicleType[3] = {0, 1, 2};
int carRow = 1;

void move_racing_car(float *playerX, float *playerY, float newPlayerX, float newPlayerY){
  *playerX = newPlayerX;
  *playerY = newPlayerY;
}

bool checkCollision(float playerX, float playerY, float obstacleX, float obstacleY, float Car_width, float enemy_width) {
  if(-3 <= playerY - obstacleY && playerY - obstacleY <= 3) {
    if((playerX <= obstacleX && obstacleX <= playerX + Car_width) || (obstacleX <= playerX && playerX <= obstacleX + enemy_width)) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

void storeRacingCarHighScore(Preference preferences, float highScore){
  preferences.begin("RacingCar", false);
  preferences.putUInt("highScore", highScore);
  preferences.end();
}