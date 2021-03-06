#include <iostream>
#include <ncurses.h>
#include <vector>
#include "./apple.h"
#include "./snake.h"

////////COLORS////////
#define YELLOW 1
#define RED 2
#define GREEN 3 
////////COLORS////////

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
  refresh();
}

void init_colors()
{
    if (has_colors())
    {
        start_color();
        
        init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
        init_pair(RED, COLOR_RED, COLOR_BLACK);
        init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    }
}

int main()
{
  char res = 'f';
  do
  {
    initscr();
    clear();
    curs_set(0);
    noecho();
    timeout(0);
    getmaxyx(stdscr, height, width);
    keypad(stdscr, true);

    init_colors();
    reset_game();
    
    while (!game_over)
    {
       move_snake();
       napms(150);
       change_direction();
       check_apple_collision(get_snake_head_x(), get_snake_head_y());
    }
    
    draw_end();

    timeout(-1);

    do res = getch(); while (res != 'q' && res != 'f' &&
                             res != 'Q' && res != 'F');

    endwin();
  } while(res == 'f' || res == 'F');
  
  return 0;
}

