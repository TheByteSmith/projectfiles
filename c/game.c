#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For usleep()

#define WIDTH 20
#define HEIGHT 20

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum Direction dir;

int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

int gameOver;

void setup() {
  initscr();
  clear();
  noecho();
  cbreak();
  curs_set(0);

  gameOver = 0;
  dir = STOP;
  x = WIDTH / 2;
  y = HEIGHT / 2;
  fruitX = rand() % WIDTH;
  fruitY = rand() % HEIGHT;
  score = 0;
}

void draw() {
  clear();
  for (int i = 0; i < WIDTH + 2; i++)
    mvprintw(0, i, "#");
  mvprintw(0, WIDTH + 2, "\n");

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (j == 0)
        mvprintw(i + 1, j, "#");
      if (i == y && j == x)
        mvprintw(i + 1, j + 1, "O");
      else if (i == fruitY && j == fruitX)
        mvprintw(i + 1, j + 1, "F");
      else {
        int print = 0;
        for (int k = 0; k < nTail; k++) {
          if (tailX[k] == j && tailY[k] == i) {
            mvprintw(i + 1, j + 1, "o");
            print = 1;
          }
        }
        if (!print)
          mvprintw(i + 1, j + 1, " ");
      }

      if (j == WIDTH - 1)
        mvprintw(i + 1, j + 1, "#");
    }
    mvprintw(i + 1, WIDTH + 2, "\n");
  }

  for (int i = 0; i < WIDTH + 2; i++)
    mvprintw(HEIGHT + 1, i, "#");
  mvprintw(HEIGHT + 1, WIDTH + 2, "\n");

  mvprintw(HEIGHT + 3, 0, "Score: %d", score);
  refresh();
}

void input() {
  keypad(stdscr, TRUE);
  halfdelay(1);

  int c = getch();
  switch (c) {
  case KEY_LEFT:
    dir = LEFT;
    break;
  case KEY_RIGHT:
    dir = RIGHT;
    break;
  case KEY_UP:
    dir = UP;
    break;
  case KEY_DOWN:
    dir = DOWN;
    break;
  case 'x':
    gameOver = 1;
    break;
  }
}

void logic() {
  int prevX = tailX[0];
  int prevY = tailY[0];
  int prev2X, prev2Y;
  tailX[0] = x;
  tailY[0] = y;

  for (int i = 1; i < nTail; i++) {
    prev2X = tailX[i];
    prev2Y = tailY[i];
    tailX[i] = prevX;
    tailY[i] = prevY;
    prevX = prev2X;
    prevY = prev2Y;
  }

  switch (dir) {
  case LEFT:
    x--;
    break;
  case RIGHT:
    x++;
    break;
  case UP:
    y--;
    break;
  case DOWN:
    y++;
    break;
  default:
    break;
  }

  if (x >= WIDTH)
    x = 0;
  else if (x < 0)
    x = WIDTH - 1;
  if (y >= HEIGHT)
    y = 0;
  else if (y < 0)
    y = HEIGHT - 1;

  for (int i = 0; i < nTail; i++) {
    if (tailX[i] == x && tailY[i] == y)
      gameOver = 1;
  }

  if (x == fruitX && y == fruitY) {
    score += 10;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    nTail++;
  }
}

void cleanup() { endwin(); }

int main() {
  setup();
  while (!gameOver) {
    draw();
    input();
    logic();
    usleep(100000); // Sleep for 100ms
  }
  cleanup();
  return 0;
}
