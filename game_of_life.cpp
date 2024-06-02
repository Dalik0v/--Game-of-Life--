#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h> // Для функции usleep
#include <string>
#include <sstream>
#include <set>
#include <cstring>
#include <termios.h> // Для работы с терминалом
#include <fcntl.h> // Для работы с файловыми дескрипторами

using namespace std;

const int HEIGHT = 20; 
const int WIDTH = 40; 

set<int> stayAlive; // Множество для хранения количества соседей, при которых клетка остается живой
set<int> becomeAlive; // Множество для хранения количества соседей, при которых мертвая клетка оживает

void readRules(const string& filename) {
    ifstream rulesFile(filename);
    if (!rulesFile) {
        cerr << "Cannot open rules file" << endl;
        exit(1);
    }
    string line;
    while (getline(rulesFile, line)) {
        if (line[0] == '#' || line.empty()) continue; // Пропускаем строки-комментарии и пустые строки
        istringstream iss(line);
        string type;
        int num;
        iss >> type;
        while (iss >> num) {
            if (type == "alive") {
                stayAlive.insert(num); // Добавляем количество соседей, при которых клетка остается живой
            } else if (type == "dead") {
                becomeAlive.insert(num); // Добавляем количество соседей, при которых мертвая клетка оживает
            }
        }
    }
}

void display(char universe[HEIGHT][WIDTH]) {
    system("clear"); // Очищаем экран терминала
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            cout << (universe[i][j] == '*' ? "\u25A0" : " "); // Выводим символ '*' или пробел, чтобы отобразить состояние клетки
        }
        cout << endl;
    }
}

void update(char universe[HEIGHT][WIDTH]) {
    char newUniverse[HEIGHT][WIDTH] = {}; // Создаем новое игровое поле
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            int liveNeighbors = 0; // Количество живых соседей
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (di == 0 && dj == 0) continue; // Пропускаем текущую клетку
                    int ni = (i + di + HEIGHT) % HEIGHT;
                    int nj = (j + dj + WIDTH) % WIDTH;
                    if (universe[ni][nj] == '*') {
                        liveNeighbors++; // Увеличиваем количество живых соседей
                    }
                }
            }
            if (universe[i][j] == '*' && stayAlive.count(liveNeighbors)) {
                newUniverse[i][j] = '*'; // Клетка остается живой
            } else if (universe[i][j] == ' ' && becomeAlive.count(liveNeighbors)) {
                newUniverse[i][j] = '*'; // Мертвая клетка оживает
            } else {
                newUniverse[i][j] = ' '; // Клетка умирает
            }
        }
    }
    memcpy(universe, newUniverse, sizeof(newUniverse)); // Копируем новое игровое поле в старое
}

void readBMP(const string& fileName, char universe[HEIGHT][WIDTH]) {
    ifstream bmpFile(fileName, ios::binary); // Открываем файл в бинарном режиме
    if (!bmpFile) {
        cerr << "Cannot open file" << endl;
        return;
    }

    bmpFile.seekg(10);
    int dataOffset;
    bmpFile.read(reinterpret_cast<char*>(&dataOffset), sizeof(int)); // Считываем смещение данных из BMP заголовка

    bmpFile.seekg(18);
    int imgWidth, imgHeight;
    bmpFile.read(reinterpret_cast<char*>(&imgWidth), sizeof(int));
    bmpFile.read(reinterpret_cast<char*>(&imgHeight), sizeof(int)); // Считываем ширину и высоту изображения из BMP заголовка

    const int rowPadding = (4 - (imgWidth * 3) % 4) % 4; // Вычисляем количество байтов, добавляемых для выравнивания строк BMP файла
    vector<unsigned char> rowBuffer(imgWidth * 3 + rowPadding);

    bmpFile.seekg(dataOffset);
    for (int i = 0; i < imgHeight; ++i) {
        bmpFile.read(reinterpret_cast<char*>(rowBuffer.data()), rowBuffer.size());
        for (int j = 0; j < imgWidth; ++j) {
            int index = j * 3;
            universe[imgHeight - 1 - i][j] = (rowBuffer[index] > 128 && rowBuffer[index + 1] > 128 && rowBuffer[index + 2] > 128) ? '*' : ' '; // Преобразуем цвета изображения в символы '*' и ' '
        }
    }

    bmpFile.close(); // Закрываем файл
}

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

    ch = getchar(); // Считываем символ с клавиатуры

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin); // Возвращаем символ обратно во входной поток
        return 1; // Возвращаем 1, если была нажата клавиша
    }

    return 0; // Возвращаем
