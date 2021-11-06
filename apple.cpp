#include <iostream>
#include <ncurses.h>
#include "./snake.h"

void grow_snake();
void draw_score();
extern int score;

struct Apple
{
  int x = -1;
  int y = -1;
  char skin = '@';
};

Apple apple;

extern int width, height;

void spawn_apple(int width, int height)
{
  /*
    Генерирует рандомные координаты для яблока
    Если координаты яблока совпадут с координатами
    змейки - перегенерировать координаты
  */
  while(true)
  {
    apple.x = rand() % width;
    apple.y = rand() % height;
    if (mvinch(apple.y, apple.x) == ' ') break;
  }
  mvaddch(apple.y, apple.x, apple.skin); // перенести в логику
}

void check_apple_collision(int head_x, int head_y)
{
  /*
    Принимает на вход координаты яблока и координаты головы змейки
    проверяет их на колизию
  */
  // head_x = snake.body[0][0]
  // head_y = snake.body[0][1]
  if(apple.x == head_x && apple.y == head_y)
  {
    grow_snake();
    score++;
    draw_score();
    spawn_apple(width, height);
  }
}
