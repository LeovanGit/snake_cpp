#include <iostream>
#include <ncurses.h>
#include <vector>
// #define DEBUG

using std::cout;
using std::vector;

int score = 0;

int height = -1, width = -1;

bool game_over = false;

struct Snake
{
  vector <vector<int>> body; // массив координат (x;y)
  char way = 'w';
  char skin = 'o';
};

Snake snake;

struct Apple
{
  int x = -1;
  int y = -1;
  char skin = '@';
};

Apple apple;

void spawn_apple()
{
  while(true)
  {
    apple.x = rand() % width;
    apple.y = rand() % height;
    if (mvinch(apple.y, apple.x) == ' ') break;
  }
  mvaddch(apple.y, apple.x, apple.skin);
}

void draw_score()
{
  move(0,0);
  printw("SCORE: %d", score);
}

void reset_game()
{
  game_over = false;
  draw_score();
  snake.body.clear();
  vector <int> start_coord = {width / 2, height / 2}; // (x;y)
  snake.body.push_back(start_coord);
  mvaddch(snake.body[0][1], snake.body[0][0], snake.skin); // под вопросом
  spawn_apple();
}

void grow_snake()
{
  int s = snake.body.size();
  int x = snake.body[s-1][0];
  int y = snake.body[s-1][1] + 1;
  vector <int> new_body_coord = {x, y};
  snake.body.push_back(new_body_coord);
//  mvaddch(y, x, snake.skin);
}

void check_apple_collision()
{
  if(apple.x == snake.body[0][0] && apple.y == snake.body[0][1])
  {
    grow_snake();
    score++;
    draw_score();
    spawn_apple();
  }
}

void change_direction()
{
  char new_way = '\0';
  new_way = getch();

  if ((new_way == 'w' && snake.way != 's') || 
      (new_way == 'a' && snake.way != 'd') ||
      (new_way == 's' && snake.way != 'w') ||
      (new_way == 'd' && snake.way != 'a')) 
  snake.way = new_way;
}

void move_snake()
{
  int snake_size = snake.body.size();

  // стираем зад
  mvaddch(snake.body[snake_size-1][1], snake.body[snake_size-1][0], ' ');

  // меняем координаты и сразу отрисовываем
  for(int i = snake_size - 1; i > 0; i--)
  {
      snake.body[i][0] = snake.body[i-1][0];
      snake.body[i][1] = snake.body[i-1][1];
      mvaddch(snake.body[i][1], snake.body[i][0], snake.skin);
  }

  int prev_head_x = snake.body[0][0];
  int prev_head_y = snake.body[0][1];

  // меняем координаты головы
  switch(snake.way)
  {
    case 'w': 
        snake.body[0][1]--;
        break;
    case 'a': 
        snake.body[0][0]--;
        break;
    case 's':
        snake.body[0][1]++;
        break;
    case 'd':
        snake.body[0][0]++;
        break;
  }

  if (mvinch(snake.body[0][1], snake.body[0][0]) == snake.skin ||
      snake.body[0][0] < 0 || snake.body[0][0] > width-1 ||
      snake.body[0][1] < 0 || snake.body[0][1] > height-1)
  {
    game_over = true;
    mvaddch(prev_head_y, prev_head_x, 'X');
  }
  else mvaddch(snake.body[0][1], snake.body[0][0], snake.skin); // рисуем голову
}

int main()
{
    initscr();
    curs_set(0); // скрыть курсор
    noecho(); // не показывать ввод в терминале
    timeout(0); // getch() не перехватывает управление
    getmaxyx(stdscr, height, width);
    reset_game();

    for (int i = 0; i < 10; i++) // 3
    {
      grow_snake();
    }


    while (!game_over)
    {
     check_apple_collision();
     move_snake();
     napms(200);
     change_direction();
    }
    napms(10000);

    endwin();
    return 0;
}

