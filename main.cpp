#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <poll.h>
using namespace std;
bool gameOver;
const int width = 20;
const int height = 20;
int tailX[100], tailY[100];
int x, y, fruitX, fruitY, score, nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

void Setup()
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Draw()
{
    system("clear");
    for (int i = 0; i < width + 2; i++)
        cout << '#';
    cout << endl;


    for (int i = 0; i < height; i++)
    {
        for (int j  = 0; j < width; j++)
        {
            if (j == 0 )
                cout << '#';

            if ( i == y && j == x)
                cout << "0";
            else if (j == fruitX && i == fruitY)
                cout << "F";
            else
            {
                bool print = false;
                for (int k = 0; k < nTail; k++){
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        cout << 'o';
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }     

            if (j == width - 1)
                cout << '#';
        }
        cout << endl;
    }   



    for (int i = 0; i < width + 2; i++)
        cout << '#';
    cout << endl;
    cout << "Score: " << score << endl;
}

void Input()
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    struct pollfd fds[1];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    int timeout = 0; // bez oczekiwania na dane wejściowe
    int ret = poll(fds, 1, timeout);
    if (ret == 1 && (fds[0].revents & POLLIN)) {
        int ch = getchar();
        switch (ch)
        {
            case 'a':
                dir = LEFT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }

    term.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

}

void Logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir)
    {
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

    if (x > width - 1 || x < 0 || y > height - 1 || y < 0)
        gameOver = true;
    for (int i = 0; i < nTail; i++){
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }
    if (x == fruitX && y == fruitY)
    {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main()
{
    Setup();
    while(!gameOver)
    {
        Draw();
        Input();
        Logic();
        usleep(100000);//czekaj 100 milisekund, 0,1s
    }

    return 0;
}