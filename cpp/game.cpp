/*** includes ***/
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

/*** defines ***/
#define CTRL_KEY(k) ((k) & 0x1f)

/*** variables ***/
int Width;
int Height;

int playerX;
int playerY;
int playerHealth;
int playerHunger;
int playerThirst;
int playerEnergy;

/*** map ***/
std::vector<std::vector<std::string>> map;

void initMap(int height, int width) {
  Height = height;
  Width = width;

  map.resize(Height);
  for (int i = 0; i < Height; i++) {
    map[i].resize(Width);
  }

  for (int i = 0; i < Height; i++) {
    for (int j = 0; j < Width; j++) {
      map[i][j] = "[#]";
    }
  }
}

void printMap() {
  for (int i = 0; i < Height; i++) {
    for (int j = 0; j < Width; j++) {
      if (j == Width - 1) {
        if (j == playerX && i == playerY) {
          std::cout << "[@]\r\n";
        } else
          std::cout << map[i][j] << "\r\n";
      } else if (j == playerX && i == playerY) {
        std::cout << "[@]";
      } else
        std::cout << map[i][j];
    }
  }
  std::cout << playerX << "," << playerY << "\r\n";
}

/*** player ***/
void initPlayer(int x, int y, int health, int hunger, int thirst, int energy) {
  playerX = x;
  playerY = y;
  playerHealth = health;
  playerHunger = hunger;
  playerThirst = thirst;
  playerEnergy = energy;
}

void playerMove(char dir) {
  switch (dir) {
  case 'u':
    if (playerY >= 1) {
      playerY--;
    }
    break;

  case 'd':
    if (playerY <= Height - 2) {
      playerY++;
    }
    break;

  case 'l':
    if (playerX >= 1) {
      playerX--;
    }
    break;

  case 'r':
    if (playerX <= Width - 2) {
      playerX++;
    }
    break;
  }
}

/*** input ***/
struct editorConfig {
  int screenrows;
  int screencols;
  struct termios orig_termios;
};

struct editorConfig E;

void die(const char *s) {
  write(STDIN_FILENO, "\x1b[2J", 4);
  write(STDIN_FILENO, "\x1b[H", 3);

  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
    die("tcsetattr");
}

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1)
    die("tcgetattr");
  atexit(disableRawMode);

  struct termios raw = E.orig_termios;

  tcgetattr(STDIN_FILENO, &raw);

  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tcsetattr");
}

char editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN)
      die("read");
  }
  return c;
}

void editorProcessKeypress() {
  char c = editorReadKey();

  switch (c) {
  case 'w':
    playerMove('u');
    break;

  case 'a':
    playerMove('l');
    break;

  case 's':
    playerMove('d');
    break;

  case 'd':
    playerMove('r');
    break;

  case CTRL_KEY('q'):
    write(STDIN_FILENO, "\x1b[2J", 4);
    write(STDIN_FILENO, "\x1b[H", 3);

    exit(0);
    break;
  }
}

/*** update ***/
void update() {
  write(STDIN_FILENO, "\x1b[2J", 4);
  write(STDIN_FILENO, "\x1b[H", 3);

  printMap();

  editorProcessKeypress();
}

/*** init ***/
int main() {
  enableRawMode();
  initPlayer(5, 5, 10, 10, 10, 100);
  initMap(11, 11);

  while (1) {
    update();
  }

  return 0;
}
