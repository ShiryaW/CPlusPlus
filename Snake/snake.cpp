#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <cmath>

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, tailLength, score, speed;
int tailX[100], tailY[100];
enum direction { STOP, LEFT, RIGHT, UP, DOWN };
direction currentDirection;

void generateFruit() {
    fruitX = 1 + rand() % (width-2);
    fruitY = 1 + rand() % (height-2);

    for (int i = 0; i < tailLength; i++) {
        if (tailX[i] == fruitX && tailY[i] == fruitY) {
            generateFruit();
        }
    }
}

void drawLine(char c, int length) {
    for (int i = 0; i < length; i++) {
        cout << c;
    }
    cout << endl;
}

void setup() {
    srand(time(0));
    gameOver = false;
    speed = 100;
    score = 0;
    currentDirection = STOP;
    x = width / 2;
    y = height / 2;
    tailLength = 0;
    generateFruit();
}

void draw() {
    system("clear"); // shell-dependent
    drawLine('#', width);

    for (int i = 1; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) {
                cout << "#";
            } else if (i == y && j == x) {
                cout << "O";
            } else if (i == fruitY && j == fruitX) {
                cout << "F";
            } else if (j == width-1) {
                cout << "#";
            }
            else {
                bool isPrinted = false;
                for (int k = 0; k < tailLength; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        isPrinted = true;
                    }
                }
                if (!isPrinted) {
                    cout << " ";
                }
            }
        }
        cout << endl;
    }

    drawLine('#', width);
    cout << "\n\n" << "Score: " << score << endl;
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if (currentDirection != RIGHT) {
                currentDirection = LEFT;
            }
            break;
        case 'd':
            if (currentDirection != LEFT) {
                currentDirection = RIGHT;
            }
            break;
        case 'w':
            if (currentDirection != DOWN) {
                currentDirection = UP;
            }
            break;
        case 's':
            if (currentDirection != UP) {
                currentDirection = DOWN;
            }
            break;
        case 'x':
            gameOver = true;
            break;
        default:
            break;
        }
    }
}

void checkForCrash() {
    if (x >= width || y >= height || x < 1 || y < 1) {
        gameOver = true;
    }

    for (int i = 0; i < tailLength; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = true;
        }
    }
}

int updateSpeed(int speed, int score) {
    return speed - floor(speed / 10);
}

void logic() {
    for (int i = tailLength; i > 0; i--) { 
        tailX[i] = tailX[i - 1];
        tailY[i] = tailY[i - 1];
    }
    tailX[0] = x; // assign current head to be the new head of tail
    tailY[0] = y;

    switch (currentDirection) {
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
    
    if (x == fruitX && y == fruitY) {
        generateFruit();
        tailLength++;
        tailX[tailLength] = x;
        tailY[tailLength] = y;
        score++;
        speed = updateSpeed(speed, score);
    }

    checkForCrash();
}

int main() {
    string tryAgain;

    do {
        setup();

        while (!gameOver) {
            draw();
            input();
            logic();
            Sleep(1 * speed);
        }

        cout << "Game over! Want to try again? (y/n)" << endl;
        cin >> tryAgain;
    } while (tryAgain == "y");

    return 0;
}
