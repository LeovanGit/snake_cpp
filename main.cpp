#include <iostream>
#include <ncurses.h>
#include <vector>
#include "./apple.h"
#include "./snake.h"

using std::vector;

int score = 0;
int height = -1, width = -1;
bool game_over = false;

void spawn_apple(int width, int height);
void check_apple_collision(int head_x, int head_y);
void clear_snake();
void init_snake();
void move_snake();
void change_direction();
int get_snake_head_x();
int get_snake_head_y();

void draw_score()
{
  move(0,0);
  printw("SCORE:%d", score);
}

void reset_game()
{
  game_over = false;
  score = 0;
  draw_score();
  clear_snake();
  init_snake();
  spawn_apple(width, height);
}

void draw_end()
{
  mvprintw(height/2-2, width/2-4, "THE END");
  mvprintw(height/2-1, width/2-4, "Score: %d\n", score);
  mvprintw(height/2, width/2-9, "Press F to restart");
}

int main()
{
  char res = 'f';
  do
  {
    initscr();
    clear();
    curs_set(0); // скрыть курсор
    noecho(); // не показывать ввод в терминале
    timeout(0); // getch() не перехватывает управление
    getmaxyx(stdscr, height, width);

    reset_game();
    
    while (!game_over)
    {
       move_snake();
       napms(150);
       change_direction();
       check_apple_collision(get_snake_head_x(), get_snake_head_y());
    }
    
    timeout(-1); // getch() ожидает указания пользователя

    do res = getch(); while (res != 'q' && res != 'f');

    endwin();
  } while(res == 'f');
  
  return 0;
}

