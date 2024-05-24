#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <string>
#include <sstream>
#include <set>
#include <cstring>

#ifdef _WIN32
#include <conio.h> // For _kbhit and _getch on Windows
#else
#include <stdio.h> // For getchar on Linux
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

using namespace std;

const int HEIGHT = 20;
const int WIDTH = 40;

set<int> stayAlive;
set<int> becomeAlive;

void readRules(const string& filename) {
    ifstream rulesFile(filename);
    if (!rulesFile) {
        cerr << "Cannot open rules file" << endl;
        exit(1);
    }
    string line;
    while (getline(rulesFile, line)) {
        if (line[0] == '#' || line.empty()) continue;
        istringstream iss(line);
        string type;
        int num;
        iss >> type;
        while (iss >> num) {
            if (type == "alive") {
                stayAlive.insert(num);
            } else if (type == "dead") {
                becomeAlive.insert(num);
            }
        }
    }
}

void display(char universe[HEIGHT][WIDTH]) {
    system("clear");
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            cout << (universe[i][j] == '*' ? "\u25A0" : " ");
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

void readBMP(const string& fileName, char universe[HEIGHT][WIDTH]) {
    ifstream bmpFile(fileName, ios::binary);
    if (!bmpFile) {
        cerr << "Cannot open file" << endl;
        return;
    }

    bmpFile.seekg(10);
    int dataOffset;
    bmpFile.read(reinterpret_cast<char*>(&dataOffset), sizeof(int));

    bmpFile.seekg(18);
    int imgWidth, imgHeight;
    bmpFile.read(reinterpret_cast<char*>(&imgWidth), sizeof(int));
    bmpFile.read(reinterpret_cast<char*>(&imgHeight), sizeof(int));

    const int rowPadding = (4 - (imgWidth * 3) % 4) % 4;
    vector<unsigned char> rowBuffer(imgWidth * 3 + rowPadding);

    bmpFile.seekg(dataOffset);
    for (int i = 0; i < imgHeight; ++i) {
        bmpFile.read(reinterpret_cast<char*>(rowBuffer.data()), rowBuffer.size());
        for (int j = 0; j < imgWidth; ++j) {
            int index = j * 3;
            universe[imgHeight - 1 - i][j] = (rowBuffer[index] > 128 && rowBuffer[index + 1] > 128 && rowBuffer[index + 2] > 128) ? '*' : ' ';
        }
    }

    bmpFile.close();
}

#ifdef _WIN32
// Windows version for kbhit and getch
#else
// Linux version for kbhit
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif

int main() {
    char universe[HEIGHT][WIDTH] = {};
    readBMP("game2.bmp", universe);
    readRules("rules.txt");

    cout << "Press Enter to start the game." << endl;
    while (cin.get() != '\n') {}

    while (true) {
        display(universe);
        update(universe);
        usleep(200000);

        // Check for key press
        if (kbhit()) {
#ifdef _WIN32
            char key = _getch();
#else
            char key = getchar();
#endif
            if (key == 'q' || key == 'Q') {
                break;
            }
        }
    }

    return 0;
}
