#include <cstdio>

const int SIZE = 20; // Размер квадратной матрицы

// Структура для хранения информации о пути
struct Path {
    int sum;    // Сумма чисел на пути
    int steps;  // Количество шагов в пути
    int path[SIZE * 2][2]; // Массив координат пути
};

// Функция для чтения матрицы из файла
void readMatrix(int matrix[SIZE][SIZE], const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка: Файл '%s' не найден!\n", filename);
        return;
    }

    // Читаем матрицу построчно
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            // Если не удалось прочитать число - выводим ошибку
            if (fscanf(file, "%d", &matrix[i][j]) != 1) {
                printf("Ошибка чтения элемента в [%d][%d]\n", i, j);
                fclose(file);
                return;
            }
        }
    }
}

// Функция для восстановления пути по заполненной матрице
void restorePath(int dp[SIZE][SIZE], int matrix[SIZE][SIZE], Path& result) {
    // Начинаем с конечной точки
    int x = SIZE - 1, y = SIZE - 1;
    result.sum = dp[x][y]; // Запоминаем максимальную сумму
    result.steps = 0;      // Сбрасываем счетчик шагов

    // Восстанавливаем путь, двигаясь от конца к началу
    while (x > 0 || y > 0) {
        // Записываем текущую точку в путь
        result.path[result.steps][0] = x;
        result.path[result.steps][1] = y;
        result.steps++;

        // Выбираем направление движения (вверх или влево)
        if (x == 0) {
            y--; // Двигаемся влево, если нельзя вверх
        } else if (y == 0) {
            x--; // Двигаемся вверх, если нельзя влево
        } else {
            // Выбираем направление с максимальной суммой
            if (dp[x-1][y] > dp[x][y-1]) {
                x--; // Вверх
            } else {
                y--; // Влево
            }
        }
    }

    // Добавляем стартовую точку [0,0]
    result.path[result.steps][0] = 0;
    result.path[result.steps][1] = 0;
    result.steps++;

    // Разворачиваем путь, чтобы он шел от начала к концу
    for (int i = 0; i < result.steps / 2; ++i) {
        // Меняем местами начало и конец
        int tempX = result.path[i][0];
        int tempY = result.path[i][1];
        result.path[i][0] = result.path[result.steps - 1 - i][0];
        result.path[i][1] = result.path[result.steps - 1 - i][1];
        result.path[result.steps - 1 - i][0] = tempX;
        result.path[result.steps - 1 - i][1] = tempY;
    }
}

// Функция поиска пути с максимальной суммой
void findMaxPathDP(int matrix[SIZE][SIZE], Path& result) {
    int dp[SIZE][SIZE] = {0}; //матрица для хранения максимальных сумм

    // Начальная точка
    dp[0][0] = matrix[0][0];

    // Заполняем первую строку (движение только вправо)
    for (int j = 1; j < SIZE; ++j) {
        dp[0][j] = dp[0][j-1] + matrix[0][j];
    }

    // Заполняем первый столбец (движение только вниз)
    for (int i = 1; i < SIZE; ++i) {
        dp[i][0] = dp[i-1][0] + matrix[i][0];
    }

    // Заполняем остальную часть матрицы
    for (int i = 1; i < SIZE; ++i) {
        for (int j = 1; j < SIZE; ++j) {
            // Выбираем максимальное значение из верхней или левой ячейки
            dp[i][j] = matrix[i][j] + ((dp[i-1][j] > dp[i][j-1]) ? dp[i-1][j] : dp[i][j-1]);
        }
    }

    // Восстанавливаем путь
    restorePath(dp, matrix, result);
}

int main() {
    int matrix[SIZE][SIZE];          // Матрица для хранения данных
    const char* filename = "matrix.txt";

    // Читаем матрицу из файла
    readMatrix(matrix, filename);

    // Проверяем, что матрица загружена корректно
    printf("Первый элемент: %d\n", matrix[0][0]);
    printf("Последний элемент: %d\n", matrix[SIZE-1][SIZE-1]);
    Path result; // Структура для хранения результата
    findMaxPathDP(matrix, result); // Находим оптимальный путь

    // Выводим результаты
    printf("\nМаксимальная сумма: %d\n", result.sum);
    printf("Путь:\n");
    for (int i = 0; i < result.steps; ++i) {
        printf("[%d,%d] ", result.path[i][0], result.path[i][1]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n");

    return 0;
}