#include <cstdio>

const int SIZE = 20;

const char* GREEN = "\033[32m"; // Зеленый цвет текста
const char* RESET = "\033[0m";  // Сброс цвета к стандартному

// Структура для хранения информации о пути
struct Path {
    int sum;    // Сумма чисел
    int steps;  // Количество шагов
    int path[SIZE * 2][2]; // Массив координат пути
};

// Функция чтения матрицы из файла
void readMatrix(int matrix[SIZE][SIZE], const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: File '%s' not found!\n", filename);
        return;
    }

    // Чтение матрицы построчно
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            // Читаем число и проверяем успешность чтения
            if (fscanf(file, "%d", &matrix[i][j]) != 1) {
                printf("Ошибка чтения элемента в [%d][%d]\n", i, j);
                fclose(file);
                return;
            }
        }
    }
    fclose(file);
}

// Функция восстановления пути
void restorePath(int dp[SIZE][SIZE], int matrix[SIZE][SIZE], Path& result) {
    // Начинаем с конечной точки (правый нижний угол)
    int x = SIZE - 1, y = SIZE - 1;
    result.sum = dp[x][y]; // Сохраняем максимальную сумму
    result.steps = 0;      // Счетчик шагов

    // Двигаемся от конца к началу
    while (x > 0 || y > 0) {
        // Записываем текущую координату в путь
        result.path[result.steps][0] = x;
        result.path[result.steps][1] = y;
        result.steps++;

        // Определяем направление движения (вверх или влево)
        if (x == 0) {
            y--; // Двигаемся влево, если нельзя вверх
        } else if (y == 0) {
            x--; // Двигаемся вверх, если нельзя влево
        } else {
            // Выбираем направление с максимальной суммой
            if (dp[x-1][y] > dp[x][y-1]) {
                x--; // Двигаемся вверх
            } else {
                y--; // Двигаемся влево
            }
        }
    }

    // Добавляем стартовую точку [0,0]
    result.path[result.steps][0] = 0;
    result.path[result.steps][1] = 0;
    result.steps++;

    // Разворачиваем путь, чтобы он шел от начала к концу
    for (int i = 0; i < result.steps / 2; ++i) {
        // Меняем местами начало и конец массива
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
    int dp[SIZE][SIZE] = {0};

    // Начальная точка
    dp[0][0] = matrix[0][0];

    // Заполнение первой строки (движение только вправо)
    for (int j = 1; j < SIZE; ++j) {
        dp[0][j] = dp[0][j-1] + matrix[0][j];
    }

    // Заполнение первого столбца (движение только вниз)
    for (int i = 1; i < SIZE; ++i) {
        dp[i][0] = dp[i-1][0] + matrix[i][0];
    }

    // Заполнение остальной части матрицы
    for (int i = 1; i < SIZE; ++i) {
        for (int j = 1; j < SIZE; ++j) {
            // Выбираем максимальное значение из верхней или левой ячейки
            dp[i][j] = matrix[i][j] + ((dp[i-1][j] > dp[i][j-1]) ? dp[i-1][j] : dp[i][j-1]);
        }
    }

    // Восстанавливаем путь по заполненной матрице
    restorePath(dp, matrix, result);
}
// Проверка, принадлежит ли точка (x,y) найденному пути
bool isInPath(int x, int y, const Path& path) {
    for (int i = 0; i < path.steps; ++i) {
        if (path.path[i][0] == x && path.path[i][1] == y) {
            return true;
        }
    }
    return false;
}

// Вывод матрицы с подсветкой пути зеленым цветом
void printMatrixWithPath(int matrix[SIZE][SIZE], const Path& path) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (isInPath(i, j, path)) {
                // Вывод зеленым цветом
                printf("%s%3d %s", GREEN, matrix[i][j], RESET);
            } else {
                // Обычный вывод
                printf("%3d ", matrix[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    int matrix[SIZE][SIZE]; 
    const char* filename = "matrix.txt";
    // Чтение матрицы из файла
    readMatrix(matrix, filename);

    Path result; // Структура для хранения результата
    findMaxPathDP(matrix, result); // Поиск оптимального пути

    // Вывод результатов
    printf("\nМаксимальная сумма: %d\n", result.sum);
    printf("Координаты пути:\n");
    for (int i = 0; i < result.steps; ++i) {
        printf("[%d,%d] ", result.path[i][0], result.path[i][1]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n\n");

    // Вывод матрицы с подсвеченным путем
    printf("Матрица с выделенным путем\n");
    printMatrixWithPath(matrix, result);

    return 0;
}