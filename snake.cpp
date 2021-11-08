#include <iostream>
#include <ncurses.h>
#include <vector>
#include "./apple.h"

using std::vector;

struct Snake
{
  vector <vector<int>> body; // (x;y)
  char way = 'w';
  char skin = 'o';
  int color = 1;
};

Snake snake;

extern int height, width;
extern bool game_over;

void grow_snake()
{
  /*
    Добавляет +1 к размеру змеи. Начальные координаты за игровым полем
  */
  vector <int> new_body_coord = {height+1, width+1};
  snake.body.push_back(new_body_coord);
}

void init_snake()
{
  snake.color = rand() % 3 + 1; // YELLOW, RED, GREEN
  snake.way = 'w'; // сбрасываем направление
  for (int i = 0; i < 4; i ++) // 4 - начальный размер змейки
  {
    vector <int> cell = {width / 2, height / 2 + i};
    snake.body.push_back(cell);

    attron(COLOR_PAIR(snake.color));
    mvaddch(snake.body[i][1], snake.body[i][0], snake.skin);
    attroff(COLOR_PAIR(snake.color));
  }
}

void change_direction()
{
   /*
     Изменить направление движения ГОЛОВЫ змейки + 
     запрет на движение головы в противоположную сторону:
     например змейка ползет 'w', при нажатии 's' ничего не произойдет,
     поскольку, змея не может ползти сама в себя.
   */
  chtype new_way = getch();

  if ((new_way == 'w' ||
       new_way == 'W' ||
       new_way == KEY_UP) &&
       snake.way != 's') snake.way = 'w';

  else if ((new_way == 'a' ||
            new_way == 'A' ||
            new_way == KEY_LEFT) &&
            snake.way != 'd') snake.way = 'a';

  else if ((new_way == 's' ||
            new_way == 'S' ||
            new_way == KEY_DOWN) &&
            snake.way != 'w') snake.way = 's';

  else if ((new_way == 'd' ||
            new_way == 'D' ||
            new_way == KEY_RIGHT) &&
            snake.way != 'a') snake.way = 'd';
}

bool check_snake_collison(int head_x, int head_y)
{
  /*
    если голова вышла за границы поля либо врезалась в тело
  */
  return ((mvinch(head_y, head_x) & A_CHARTEXT) == snake.skin ||
          head_x < 0 || head_x > width-1 ||
          head_y < 0 || head_y > height-1);
}

void move_snake()
{
    int snake_size = snake.body.size();

    // меняем координаты головы    
    int new_head_x = snake.body[0][0];
    int new_head_y = snake.body[0][1];

    switch(snake.way)
    {
      case 'w': 
        new_head_y--;
        break;
      case 'a': 
        new_head_x--;
        break;
      case 's':
        new_head_y++;
        break;
      case 'd':
        new_head_x++;
        break;
    }
    
    if (check_snake_collison(new_head_x, new_head_y))
    {
      game_over = true;

      attron(COLOR_PAIR(2));
      mvaddch(snake.body[0][1], snake.body[0][0], 'X');
      attroff(COLOR_PAIR(2));
    }
    else
    {
      // стираем последнюю "ячейку" змеи (зад)
      mvaddch(snake.body[snake_size-1][1], snake.body[snake_size-1][0], ' ');

      // координаты n ячейки = координатам n-1 и сразу же их отрисовываем 
      for(int i = snake_size - 1; i > 0; i--)
        {
          snake.body[i][0] = snake.body[i-1][0];
          snake.body[i][1] = snake.body[i-1][1];

          attron(COLOR_PAIR(snake.color));
          mvaddch(snake.body[i][1], snake.body[i][0], snake.skin);
          attroff(COLOR_PAIR(snake.color));
        }

      // по настоящему изменяем координаты головы
      snake.body[0][0] = new_head_x;
      snake.body[0][1] = new_head_y;

      // отрисовываем голову
      attron(COLOR_PAIR(snake.color));
      mvaddch(snake.body[0][1], snake.body[0][0], snake.skin);
      attroff(COLOR_PAIR(snake.color));
  }
}

void clear_snake()
{
  snake.body.clear();
}

int get_snake_head_x() {return snake.body[0][0];}

int get_snake_head_y() {return snake.body[0][1];}
