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
  snake.way = 'w'; // сбрасываем направление
  for (int i = 0; i < 4; i ++) // 4 - начальный размер змейки
  {
    vector <int> cell = {width / 2, height / 2 + i};
    snake.body.push_back(cell);
    mvaddch(snake.body[i][1], snake.body[i][0], snake.skin);
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
  char new_way = getch();

  if ((new_way == 'w' && snake.way != 's') || 
      (new_way == 'a' && snake.way != 'd') ||
      (new_way == 's' && snake.way != 'w') ||
      (new_way == 'd' && snake.way != 'a')) 
  snake.way = new_way;
}

bool check_snake_collison(int head_x, int head_y)
{
  /*
    если голова вышла за границы поля либо врезалась в тело
  */
  return (mvinch(head_y, head_x) == snake.skin ||
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
      mvaddch(snake.body[0][1], snake.body[0][0], 'X');
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
          mvaddch(snake.body[i][1], snake.body[i][0], snake.skin);
        }

      // по настоящему изменяем координаты головы
      snake.body[0][0] = new_head_x;
      snake.body[0][1] = new_head_y;

      // отрисовываем голову
      mvaddch(snake.body[0][1], snake.body[0][0], snake.skin);
  }
}

void clear_snake()
{
  snake.body.clear();
}

int get_snake_head_x() {return snake.body[0][0];}

int get_snake_head_y() {return snake.body[0][1];}
