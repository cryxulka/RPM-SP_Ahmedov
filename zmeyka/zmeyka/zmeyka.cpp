#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <ctime>

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
    system("cls");

    for (int y = 0; y < HEIGHT; y++) {
        cout << map[y] << endl;
    }

    gotoxy(0, HEIGHT + 1);
    cout << "Score: " << score << "   Length: " << snake_len;
}

void placeFood() {
    bool validPosition = false;

    while (!validPosition) {
        food_x = 1 + (rand() % (WIDTH - 2));
        food_y = 1 + (rand() % (HEIGHT - 2));

        validPosition = true;

        for (int i = 0; i < snake_len; i++) {
            if (food_x == snake_x[i] && food_y == snake_y[i]) {
                validPosition = false;
                break;
            }
        }
    }

    map[food_y][food_x] = food;
}

void updateSnakeOnMap() {
    for (int y = 1; y < HEIGHT - 1; y++) {
        for (int x = 1; x < WIDTH - 1; x++) {
            if (map[y][x] == snakeHead || map[y][x] == snakeBody) {
                map[y][x] = ' ';
            }
        }
    }

    map[snake_y[0]][snake_x[0]] = snakeHead;

    for (int i = 1; i < snake_len; i++) {
        map[snake_y[i]][snake_x[i]] = snakeBody;
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
            gotoxy(0, HEIGHT + 2);
            cout << "PAUSED - Press SPACE to continue";
            while (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                Sleep(50);
            }
            while (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) {
                Sleep(50);
            }
            while (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                Sleep(50);
            }
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

    placeFood();
    updateSnakeOnMap();
    drawMap();
}

void moveSnake() {
    snake_dir = next_dir;

    int prev_x = snake_x[0];
    int prev_y = snake_y[0];
    int temp_x, temp_y;

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
}

void cleanup() {
    for (int i = 0; i < HEIGHT; i++) {
        delete[] map[i];
    }
    delete[] map;
}

int main() {
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
                updateSnakeOnMap();
                drawMap();
            }

            lastTime = currentTime;
        }

        Sleep(10);
    }

    system("cls");
    cout << "\n\n\n";
    cout << "    GAME OVER!\n";
    cout << "    Your score: " << score << "\n";
    cout << "    Snake length: " << snake_len << "\n\n";
    cout << "    Press any key to exit...";

    cleanup();

    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);

    cin.ignore();
    cin.get();
    return 0;
}