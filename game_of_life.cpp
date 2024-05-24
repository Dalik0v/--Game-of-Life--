#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <vector>
#include <set>
#include <cstring>
#include <opencv2/opencv.hpp>

using namespace std;

const int HEIGHT = 20;
const int WIDTH = 40;

set<int> stayAlive = {2, 3, 5}; // Изменено: Добавлено новое правило
set<int> becomeAlive = {3, 6}; // Изменено: Изменены правила

void initialize(char universe[HEIGHT][WIDTH]) {
    srand(time(0));
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            universe[i][j] = (rand() % 2 == 0) ? '*' : ' ';
        }
    }
}

void display(char universe[HEIGHT][WIDTH]) {
    system("clear");
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            cout << (universe[i][j] == '*' ? "*" : " "); // Изменено: Используются звездочки для отображения состояния
        }
        cout << endl;
    }
}

void update(char universe[HEIGHT][WIDTH]) {
    char newUniverse[HEIGHT][WIDTH] = {};
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            int liveNeighbors = 0;
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (di == 0 && dj == 0) continue;
                    int ni = (i + di + HEIGHT) % HEIGHT;
                    int nj = (j + dj + WIDTH) % WIDTH;
                    if (universe[ni][nj] == '*') {
                        liveNeighbors++;
                    }
                }
            }
            if (universe[i][j] == '*' && stayAlive.count(liveNeighbors)) {
                newUniverse[i][j] = '*';
            } else if (universe[i][j] == ' ' && becomeAlive.count(liveNeighbors)) {
                newUniverse[i][j] = '*';
            } else {
                newUniverse[i][j] = ' ';
            }
        }
    }
    memcpy(universe, newUniverse, sizeof(newUniverse));
}

// Функция для проверки, была ли нажата клавиша 'Q'
bool isQKeyPressed() {
    int character = getchar();
    if (character == 'q' || character == 'Q') {
        return true;
    }
    return false;
}

int main() {
    char universe[HEIGHT][WIDTH] = {};
    initialize(universe);

    bool running = true;
    while (running) {
        display(universe);
        update(universe);
        usleep(2000000);

        // Проверяем, была ли нажата клавиша 'Q'
        if (isQKeyPressed()) {
            running = false;
        }
    }

    return 0;
}
