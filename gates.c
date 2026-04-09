#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


typedef float sample[3];
sample or_train[] = {
    {0, 0, 0},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 1},
};

sample and_train[] = {
    {0, 0, 0},
    {1, 0, 0},
    {0, 1, 0},
    {1, 1, 1},
};

sample *train = or_train;

// #define train_count (sizeof(train) / sizeof(train[0]))
#define train_count 4 

float rand_float() {
    return (float ) rand() / (float) RAND_MAX;
}

float sigmoidf(float x) {
    return 1.f / (1.f + expf(-x));
}

float cost(float w1, float w2, float b) {
    float result = 0;
    for (size_t i = 0; i < train_count; i++) {
        float x1 = train[i][0];
        float x2 = train[i][1];
        float y = sigmoidf( x1 * w1 + x2 * w2 + b);
        float d = y - train[i][2];
        result += d * d;
    }
    result /= train_count;
    return result;
}


// XOR
// (x|y) & ~(x&y)

int main() {
    for (size_t x = 0; x < 2; x++) {
        for (size_t y = 0; y < 2; y++) {
            printf("%zu ~ %zu = %zu\n", x, y, ((x|y) & ~(x&y)));
        }
    }
    srand(69);
    float w1 = rand_float();
    float w2 = rand_float();
    float b  = rand_float();
    float eps = 1e-2f;
    float rate = 1e-1f;
    for (size_t i = 0; i < 20000; i++){
        float c = cost(w1, w2, b);
        // printf("%f\n",c);
        float deps1 = (cost((w1 + eps), w2, b) - c) / eps;
        float deps2 = (cost(w1, (w2 + eps), b) - c) / eps;
        float db = (cost(w1, w2, (b + eps)) - c) / eps;
        w1 -= rate * deps1;
        w2 -= rate * deps2;
        b  -= rate * db;
        if (i % 100 == 0) {
            printf("w1 = %f, w2 = %f, b = %f, result = %f\n", w1, w2, b, cost(w1, w2, b));
        }
    }

    return 0;
}
