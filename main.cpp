#include <iostream>
#include <random>
#include <chrono>
#include <windows.h>
#include <conio.h>
#include <string>
#include <fstream>
using namespace std;

class Snake_Game
{
private:
    bool gameOver;
    const int width;
    const int height;
    int x;
    int y;
    int tailX[100];
    int tailY[100];
    int nTail;
    int fruitX;
    int fruitY;
    int score;
    int high_score;
    int speed;
    bool reset;
    enum eDirection {STOP, LEFT, RIGHT, UP, DOWN};
    eDirection dir;
    bool includes(int, int);
    void save(string, int);
    int get_high_score(string);
    void reset_game();
public:
    Snake_Game();
    void Setup();
    void Draw();
    void Input();
    void Logic();
};

int main()
{
    Snake_Game sss;
    return 0;
}

Snake_Game::Snake_Game() : width(20), height(20)
{
    std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> disX(0,width - 1);
    std::uniform_real_distribution<> disY(0,height - 1);
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    nTail = 1;
    tailX[0] = x;
    tailY[0] = y;
    fruitX = disX(gen);
    fruitY = disY(gen);
    speed = 200;
    score = 0;
    reset = true;
    high_score = get_high_score("High-Score");
    while(!gameOver && reset) {
        Draw();
        Input();
        Logic();
        Sleep(speed);
    }
}

void Snake_Game::Draw()
{
    system("cls");
    cout << "Score: " << score << ((score>high_score)?"!":"") << "\t" << "High Score: " << high_score << endl;
    for (int i = 0; i <= width; i++) {
        cout << "#";
    }
    cout << endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0 || j == width - 1) {
                cout << "#";
            }

            if (i == fruitY && j == fruitX) {
                cout << "@";
            } else if (i == y && j == x) {
                cout << "O";
            }
            else {
                    bool tail = false;
                for (int k = 0; k < nTail; k++) {
                    if (i == tailY[k] && j == tailX[k]) {
                        cout << "o";
                        tail = true;
                    }
                }
                if(!tail)
                    cout << " ";
            }

        }
        cout << endl;
    }
    for (int i = 0; i <= width; i++) {
        cout << "#";
    }
}

void Snake_Game::Input()
{
    if(_kbhit()) {
        switch(_getch())
        {
        case 'w':
            if(dir != DOWN)
                dir = UP;
                break;
        case 'a':
            if(dir != RIGHT)
                dir = LEFT;
                break;
        case 's':
            if(dir != UP)
                dir = DOWN;
                break;
        case 'd':
            if(dir != LEFT)
                dir = RIGHT;
                break;
        case 'x':
            gameOver = true;
            break;
        default:
            break;
        }
    }
}

void Snake_Game::Logic()
{
    switch(dir)
    {
    case UP:
        y--;
        break;
    case RIGHT:
        x++;
        break;
    case DOWN:
        y++;
        break;
    case LEFT:
        x--;
        break;
    default:
        break;
    }

    for (int i = nTail; i > 0; i--) {
        tailX[i] = tailX[i-1];
        tailY[i] = tailY[i-1];
    }
    tailX[0] = x;
    tailY[0] = y;
    if (x < 0 || x > width - 2 || y < 0 || y > height - 1){
        gameOver = true;
    }
    for (int i = 1; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }
    if(gameOver) {
        if (score > high_score) {
            save("High-Score", score);
        }
        system("cls");
        cout << "GAME OVER" << endl;
        cout << "Your score " << score << endl;
        cout << "High Score: " << get_high_score("High-Score") << endl;
        int option;
        cout << "try again?" << endl;
        cout << "1.Yes\t2.No" << endl;
        cin >> option;
        bool d = true;
        while(d) {
            d = false;
            switch(option)
            {
            case 1:
                reset = true;
                reset_game();
                break;
            case 2:
                reset = false;
                break;
            default:
                cout << "Invalid input" << endl;
                d = true;
                break;
            }
        }
    }
    if (x == fruitX && y == fruitY){
        nTail++;
        score += 10;
        std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<> disX(0,width - 1);
        std::uniform_real_distribution<> disY(0,height - 1);
        do{
            fruitX = disX(gen);
            fruitY = disY(gen);
        } while(includes(fruitX, fruitY));
        if (speed >= 5) {
            speed -= 5;
        }
    }
}

bool Snake_Game::includes(int fX, int fY)
{
    for(int i = 0; i < nTail; i++) {
        if (tailX[i] == fX && tailY[i] == fY)
            return true;
    }
    return false;
}

void Snake_Game::save(string file_name, int high_score)
{
    fstream myfile;
    myfile.open(file_name, ios::out);
    if (myfile.is_open()) {
        myfile << high_score << endl;
        myfile.close();
    }
}

int Snake_Game::get_high_score(string file_name)
{
    fstream myfile;
    myfile.open(file_name, ios::in);
    if (myfile.is_open()) {
        string line;
        getline(myfile, line);
        int hScore = stoi(line);
        myfile.close();
        return hScore;
    }
    else {
        save("High-Score", 0);
        return 0;
    }
}

void Snake_Game::reset_game()
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    nTail = 1;
    tailX[0] = x;
    tailY[0] = y;
    std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> disX(0, width - 1);
    std::uniform_real_distribution<> disY(0, height - 1);
    fruitX = disX(gen);
    fruitY = disY(gen);
    speed = 200;
    score = 0;
    high_score = get_high_score("High-Score");
}
