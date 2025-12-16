#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <io.h>
#include <fcntl.h>

using namespace std;

void gotoxy(int x, int y) {
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

const int WIDTH = 40;
const int HEIGHT = 20;
const int MAX_LEN_SNAKE = (WIDTH - 2) * (HEIGHT - 2);

const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;

int snake_dir = RIGHT;
int next_dir = RIGHT;
bool isRunning = true;

char** map;

char snakeHead = 'O';
char snakeBody = 'o';
int snake_x[MAX_LEN_SNAKE] = { 0 };
int snake_y[MAX_LEN_SNAKE] = { 0 };
int snake_len = 3;
int food_x, food_y;
char food = '*';
int score = 0;

void initializeMap();
void drawMap();
void placeFood();
void initializeGame();
void updateSnakeOnMap();
void moveSnake();
void cleanup();
void handleInput();

void initializeMap() {
    map = new char* [HEIGHT];
    for (int i = 0; i < HEIGHT; i++) {
        map[i] = new char[WIDTH + 1];
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1 || x == 0 || x == WIDTH - 1) {
                map[y][x] = '#';
            }
            else {
                map[y][x] = ' ';
            }
        }
        map[y][WIDTH] = '\0';
    }
}

void drawMap() {
    gotoxy(0, 0);

    for (int y = 0; y < HEIGHT; y++) {
        cout << map[y] << endl;
    }

    gotoxy(0, HEIGHT);
    cout << "Очки: " << score << "   Длина: " << snake_len << "   ";
}

void placeFood() {
    bool validPosition = false;

    while (!validPosition) {
        food_x = 1 + (rand() % (WIDTH - 2));
        food_y = 1 + (rand() % (HEIGHT - 2));

        validPosition = true;

        if (map[food_y][food_x] == '#') {
            validPosition = false;
            continue;
        }

        for (int i = 0; i < snake_len; i++) {
            if (food_x == snake_x[i] && food_y == snake_y[i]) {
                validPosition = false;
                break;
            }
        }
    }

    map[food_y][food_x] = food;
    gotoxy(food_x, food_y);
    cout << food;
}

void updateSnakeOnMap() {
    int head_x = snake_x[0];
    int head_y = snake_y[0];
    int tail_x = snake_x[snake_len - 1];
    int tail_y = snake_y[snake_len - 1];

    map[head_y][head_x] = snakeHead;

    for (int i = 1; i < snake_len; i++) {
        map[snake_y[i]][snake_x[i]] = snakeBody;
    }

    if (snake_len > 1) {
        bool tail_still_exists = false;
        for (int i = 0; i < snake_len; i++) {
            if (snake_x[i] == tail_x && snake_y[i] == tail_y) {
                tail_still_exists = true;
                break;
            }
        }
        if (!tail_still_exists) {
            map[tail_y][tail_x] = ' ';
        }
    }
}

void handleInput() {
    if (GetAsyncKeyState('W') & 0x8000) {
        if (snake_dir != DOWN) {
            next_dir = UP;
        }
    }
    if (GetAsyncKeyState('S') & 0x8000) {
        if (snake_dir != UP) {
            next_dir = DOWN;
        }
    }
    if (GetAsyncKeyState('A') & 0x8000) {
        if (snake_dir != RIGHT) {
            next_dir = LEFT;
        }
    }
    if (GetAsyncKeyState('D') & 0x8000) {
        if (snake_dir != LEFT) {
            next_dir = RIGHT;
        }
    }

    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        isRunning = false;
    }

    static bool spacePressed = false;
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        if (!spacePressed) {
            spacePressed = true;
            gotoxy(0, HEIGHT + 1);
            cout << "ПАУЗА - Нажмите ПРОБЕЛ для продолжения";
            while (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                Sleep(50);
            }
            while (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) {
                Sleep(50);
            }
            while (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                Sleep(50);
            }
            gotoxy(0, HEIGHT + 1);
            cout << "                                     ";
            spacePressed = false;
        }
    }
}

void initializeGame() {
    srand(static_cast<unsigned int>(time(nullptr)));

    initializeMap();

    int start_x = WIDTH / 2;
    int start_y = HEIGHT / 2;

    for (int i = 0; i < snake_len; i++) {
        snake_x[i] = start_x - i;
        snake_y[i] = start_y;
    }

    for (int i = 0; i < snake_len; i++) {
        if (i == 0) {
            map[snake_y[i]][snake_x[i]] = snakeHead;
        }
        else {
            map[snake_y[i]][snake_x[i]] = snakeBody;
        }
    }

    placeFood();

    system("cls");
    drawMap();
}

void moveSnake() {
    snake_dir = next_dir;

    int prev_x = snake_x[0];
    int prev_y = snake_y[0];
    int temp_x, temp_y;

    int old_tail_x = snake_x[snake_len - 1];
    int old_tail_y = snake_y[snake_len - 1];

    if (snake_dir == UP) snake_y[0]--;
    else if (snake_dir == DOWN) snake_y[0]++;
    else if (snake_dir == LEFT) snake_x[0]--;
    else if (snake_dir == RIGHT) snake_x[0]++;

    if (snake_x[0] <= 0 || snake_x[0] >= WIDTH - 1 ||
        snake_y[0] <= 0 || snake_y[0] >= HEIGHT - 1) {
        isRunning = false;
        return;
    }

    for (int i = 1; i < snake_len; i++) {
        if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) {
            isRunning = false;
            return;
        }
    }

    for (int i = 1; i < snake_len; i++) {
        temp_x = snake_x[i];
        temp_y = snake_y[i];
        snake_x[i] = prev_x;
        snake_y[i] = prev_y;
        prev_x = temp_x;
        prev_y = temp_y;
    }

    if (snake_x[0] == food_x && snake_y[0] == food_y) {
        snake_len++;
        snake_x[snake_len - 1] = prev_x;
        snake_y[snake_len - 1] = prev_y;

        score += 10;
        placeFood();
    }
    else {
        map[old_tail_y][old_tail_x] = ' ';
        gotoxy(old_tail_x, old_tail_y);
        cout << ' ';
    }

    updateSnakeOnMap();
    gotoxy(snake_x[0], snake_y[0]);
    cout << snakeHead;

    for (int i = 1; i < snake_len; i++) {
        gotoxy(snake_x[i], snake_y[i]);
        cout << snakeBody;
    }
}

void cleanup() {
    for (int i = 0; i < HEIGHT; i++) {
        delete[] map[i];
    }
    delete[] map;
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);

    initializeGame();
    double lastTime = clock();
    double gameTime = 0;

    while (isRunning) {
        handleInput();

        double currentTime = clock();
        gameTime = (currentTime - lastTime) / CLOCKS_PER_SEC;

        if (gameTime >= 0.10) {
            moveSnake();

            if (isRunning) {
                gotoxy(0, HEIGHT);
                cout << "Очки: " << score << "   Длина: " << snake_len << "   ";
            }

            lastTime = currentTime;
        }

        Sleep(10);
    }

    system("cls");
    cout << "\n\n\n";
    cout << "    ИГРА ОКОНЧЕНА!\n";
    cout << "    Ваши очки: " << score << "\n";
    cout << "    Длина змейки: " << snake_len << "\n\n";
    cout << "    Нажмите любую клавишу для выхода...";

    cleanup();

    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);

    cin.ignore();
    cin.get();
    return 0;
}