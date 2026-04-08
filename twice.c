#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// y = x * w;
float train[][2] = {
    {0, 0},
    {1, 2},
    {2, 4},
    {3, 6},
    {4, 8},
};

#define train_count (sizeof(train) / sizeof(train[0]))

float rand_float() {
    return (float ) rand() / (float) RAND_MAX;
}
float cost(float w, float b) {
    float result = 0;
    for (size_t i = 0; i < train_count; i++) {
        float x = train[i][0];
        float y = x * w + b;
        float d = y - train[i][1];
        result += d * d;
    }
    result /= train_count;
    return result;
}

int main() {
    srand(time(0));
    // srand(69);
    float w = rand_float() * 10.0f;
    float b = rand_float() * 5.0f;
    float eps = 1e-3f;
    float rate = 1e-3f;
    for (size_t i = 0; i < 50000; i++){
        float c = cost(w, b);
        float deps = (cost((w + eps), b) - c) / eps;
        float db = (cost(w, (b + eps)) - c) / eps;
        w -= rate * deps;
        b -= rate * db;
        printf("w = %f, b = %f result = %f\n", w, b, cost(w, b));
    }
    return 0;
}
