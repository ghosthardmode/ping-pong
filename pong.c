#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct {
  Vector2 top;
  Vector2 bottom;
  Vector2 left;
  Vector2 right;
}Bounds;

typedef struct {
  Vector2 position;
  float length;
  float width;
  Rectangle rect;
}Paddle;

Paddle initPaddle(Vector2 pos, float length, float width) {
  Paddle p;
  p.position = pos;
  p.length = length;
  p.width = width;
  p.rect.x = p.position.x;
  p.rect.y = p.position.y;
  p.rect.width = p.width;
  p.rect.height = p.length;
  return p;
}

Rectangle updatePaddle(Paddle p){
  p.rect.x = p.position.x;
  p.rect.y = p.position.y;
  p.rect.width = p.width;
  p.rect.height = p.length;
  return p.rect;
}

Vector2 angleToVector2(float angle) {
  Vector2 vector = {cos(angle), sin(angle)};
  return vector;
}

int main() {
  Image icon = LoadImage("icon.png");
  InitWindow(600, 350, "Pong");
  SetWindowIcon(icon);
  Bounds plBounds = {100, 0, 100, 350};
  Vector2 pos = {100, 150};
  Vector2 botPos = {490, 150};
  Vector2 ballPos = {300, 175};
  Paddle pl = initPaddle(pos, 50, 2);
  Paddle bot = initPaddle(botPos, 50, 2);

  float angle = (rand() % 360) + 1.f;
  const float radius = 5;
  const float ballSpeed = 125.f;
  int plScore = 0;
  char plString[125];
  int botScore = 0;
  char botString[125];
  angle *= M_PI / 180.f;

  SetTargetFPS(1000);

  while (!WindowShouldClose()) {
    if (angle == 90.f) {
      angle = (rand() % 360) + 1.f;
      angle *= M_PI / 180.f;
    }
    sprintf(plString, "score: %d", plScore);
    sprintf(botString, "score: %d", botScore);
    Bounds ballBounds = {ballPos.x, 0, ballPos.x, 350, 0, ballPos.y, 600, ballPos.y};
    float delta = GetFrameTime();
    Vector2 ballTrajectory = angleToVector2(angle);
    ballPos.x += ballTrajectory.x * ballSpeed * delta;
    ballPos.y += ballTrajectory.y * ballSpeed * delta;
    pl.rect = updatePaddle(pl);
    bot.rect = updatePaddle(bot);
    if (ballPos.y > 25 && ballPos.y < 325)
      bot.position.y = ballPos.y - 25;
    if (IsKeyDown(KEY_UP)) {
      if (pl.position.x > 0)
        pl.position.y -= 125 * delta;
    }
    if (IsKeyDown(KEY_DOWN)) {
      if (pl.position.y < 600)
        pl.position.y += 125 * delta;
    }

    if (IsKeyPressed(KEY_R)) {
      angle = (rand() % 360) + 1.f;
      angle *= M_PI / 180.f;
      pl.position.y = 150;
      bot.position.y = 150;
      ballPos.x = 300;
      ballPos.y = 175;
      plScore = 0;
      botScore = 0;
    }
    if (ballPos.y >= pl.position.y && ballPos.y <= pl.position.y + pl.length) {
      if (ballPos.x < pl.position.x) {
        angle = -angle + -90.f;
        ballPos.x += 5;
      }
    }
    if (ballPos.x > bot.position.x) {
        angle = -angle + 90.f;
        ballPos.x -= 5;
    }
    if (ballPos.y <= 0){
      angle = -angle;
    }else if (ballPos.y >= 350) {
      angle = -angle;
    }else if (ballPos.x <= 0) {
      ballPos.x = 300;
      ballPos.y = 175;
      angle = (rand() % 360) + 1.f;
      angle *= M_PI / 180.f;
      botScore ++;
    }else if (ballPos.x >= 600) {
      ballPos.x = 300;
      ballPos.y = 175;
      angle = (rand() % 360) + 1.f;
      angle *= M_PI / 180.f;
      plScore ++;
    }
    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangleRec(pl.rect, WHITE);
    DrawRectangleRec(bot.rect, WHITE);
    DrawCircleV(ballPos, radius, WHITE);
    DrawText(plString, 110, 15, 24, WHITE);
    DrawText(botString, 390, 15, 24, WHITE);
    DrawLine(300, 0, 300, 350, WHITE);
    DrawText("press R to restart, press UP to move up, press DOWN to move down", 15, 330, 16, WHITE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}