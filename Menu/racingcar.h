

typedef struct Object
{
  float objectX = 0, objectY = 0;
  float obj_width = 0, obj_height = 0;
  unsigned char ObjectData[];
};

void RacingCar(Object &player, int &gameState, int *score)
{
  // Display start screen
  if (gameState == 0)
  {
    // Reinitialize in-game status
    player.objectY = 24.00;
    player.objectX = 10.00;
    player.obj_width = Car_width;
    player.obj_height = Car_height;
    player.ObjectData = Car;
    score = 0;
    speed = 0.01;

    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 4, "Racing Car");

    display.drawXbm(75, 21, Truck_width, Truck_height, Truck);
    display.drawXbm(player.objectX, player.objectY, player.obj_width, player.obj_height, player.ObjectData);
    display.setColor(WHITE);
    display.fillRect(0, SCREEN_HEIGHT - 5, SCREEN_WIDTH, 5);

    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 44, "Press blue button to start");

    if (digitalRead(BLUE_BUTTON_PIN) == LOW)
    {
      gameState = 1;
      delay(50);
    }
  }
}