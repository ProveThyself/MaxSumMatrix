#include <cstdio>

const int SIZE = 20;

// ANSI-коды для цветов
#define GREEN "\033[32m"
#define RESET "\033[0m"

struct Path {
    int sum;
    int steps;
    int path[SIZE * 2][2];
};

void readMatrix(int matrix[SIZE][SIZE], const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка: файл '%s' не найден!\n", filename);
        return;
    }

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (fscanf(file, "%d", &matrix[i][j]) != 1) {
                printf("Ошибка чтения в [%d][%d]\n", i, j);
                fclose(file);
                return;
            }
        }
    }
    fclose(file);
}

void restorePath(int dp[SIZE][SIZE], int matrix[SIZE][SIZE], Path& result) {
    int x = SIZE - 1, y = SIZE - 1;
    result.sum = dp[x][y];
    result.steps = 0;

    while (x > 0 || y > 0) {
        result.path[result.steps][0] = x;
        result.path[result.steps][1] = y;
        result.steps++;

        if (x == 0) {
            y--;
        } else if (y == 0) {
            x--;
        } else {
            if (dp[x-1][y] > dp[x][y-1]) {
                x--;
            } else {
                y--;
            }
        }
    }

    result.path[result.steps][0] = 0;
    result.path[result.steps][1] = 0;
    result.steps++;

    for (int i = 0; i < result.steps / 2; ++i) {
        int tempX = result.path[i][0];
        int tempY = result.path[i][1];
        result.path[i][0] = result.path[result.steps - 1 - i][0];
        result.path[i][1] = result.path[result.steps - 1 - i][1];
        result.path[result.steps - 1 - i][0] = tempX;
        result.path[result.steps - 1 - i][1] = tempY;
    }
}

void findMaxPathDP(int matrix[SIZE][SIZE], Path& result) {
    int dp[SIZE][SIZE] = {0};
    dp[0][0] = matrix[0][0];

    for (int j = 1; j < SIZE; ++j) {
        dp[0][j] = dp[0][j-1] + matrix[0][j];
    }

    for (int i = 1; i < SIZE; ++i) {
        dp[i][0] = dp[i-1][0] + matrix[i][0];
    }

    for (int i = 1; i < SIZE; ++i) {
        for (int j = 1; j < SIZE; ++j) {
            dp[i][j] = matrix[i][j] + ((dp[i-1][j] > dp[i][j-1]) ? dp[i-1][j] : dp[i][j-1]);
        }
    }

    restorePath(dp, matrix, result);
}

// Функция для проверки, является ли текущая клетка частью пути
bool isInPath(int x, int y, const Path& path) {
    for (int i = 0; i < path.steps; ++i) {
        if (path.path[i][0] == x && path.path[i][1] == y) {
            return true;
        }
    }
    return false;
}

// Вывод матрицы с подсветкой пути зелёным цветом
void printMatrixWithPath(int matrix[SIZE][SIZE], const Path& path) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (isInPath(i, j, path)) {
                printf(GREEN "%3d " RESET, matrix[i][j]); // Зелёный цвет
            } else {
                printf("%3d ", matrix[i][j]); // Обычный цвет
            }
        }
        printf("\n");
    }
}

int main() {
    int matrix[SIZE][SIZE];
    const char* filename = "matrix.txt";

    readMatrix(matrix, filename);

    Path result;
    findMaxPathDP(matrix, result);

    printf("\nМаксимальная сумма: %d\n", result.sum);
    printf("Путь:\n");
    for (int i = 0; i < result.steps; ++i) {
        printf("[%d,%d] ", result.path[i][0], result.path[i][1]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n\n");

    printf("Матрица с выделенным путем:\n");
    printMatrixWithPath(matrix, result);

    return 0;
}