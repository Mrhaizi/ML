#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
 

typedef struct {
    float or_w1;
    float or_w2;
    float or_b;

    float nand_w1;
    float nand_w2;
    float nand_b;

    float and_w1;
    float and_w2;
    float and_b;

} Xor;

float sigmoidf(float x) {
    return 1.f / (1.f + expf(-x));
}


float forward(Xor m, float x1, float x2) {
    float a = sigmoidf(m.or_w1 * x1 + m.or_w2 * x2 + m.or_b);
    float b = sigmoidf(m.nand_w1 * x1 + m.nand_w2 * x2 + m.nand_b);
    return sigmoidf(m.and_w1 * a + m.and_w2 * b + m.and_b);
}

typedef float sample[3];

sample xor_train[] =  {
    {0, 0, 0},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 0},
};

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

sample nor_train[] = {
    {0, 0, 1},
    {1, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
};

sample xnor_train[] = {
    {0, 0, 1},
    {1, 0, 0},
    {0, 1, 0},
    {1, 1, 1},
};

sample *train = xor_train;
size_t train_count = 4;

float cost(Xor m) {
    float result = 0;
    for (size_t i = 0; i < train_count; i++) {
        float x1 = train[i][0];
        float x2 = train[i][1];
        float y = forward(m, x1, x2); // 这样写 成本函数对模型架构一无所知，解耦
        float d = y - train[i][2];
        result += d * d;
    }
    result /= train_count;
    return result;
}

float rand_float() {
    return (float ) rand() / (float) RAND_MAX;
}


Xor randXor() {
    Xor m;
    m.or_w1 = rand_float();
    m.or_w2 = rand_float();
    m.or_b = rand_float();

    m.nand_w1 = rand_float();
    m.nand_w2 = rand_float();
    m.nand_b = rand_float();

    m.and_w1 = rand_float();
    m.and_w2 = rand_float();
    m.and_b = rand_float();
    return m;
}
void printXor(Xor m) {
    printf("or_w1 = %f\n", m.or_w1);
    printf("or_w2 = %f\n", m.or_w2);
    printf("or_b = %f\n", m.or_b);
    printf("and_w1 = %f\n", m.and_w1);
    printf("and_w2 = %f\n", m.and_w2);
    printf("and_b = %f\n", m.and_b);
    printf("nand_w1 = %f\n", m.nand_w1);
    printf("nand_w2 = %f\n", m.nand_w2);
    printf("nand_b = %f\n", m.nand_b);
}
#define epoch 1000000
#define learn_rate 1e-1f

// 计算梯度
Xor finite_diff(Xor m, float eps) {
    Xor g;
    float c = cost(m);
    float saved = m.or_w1;
    m.or_w1 += eps;
    g.or_w1 = (cost(m) - c) / eps;
    m.or_w1 = saved;

    saved = m.or_w2;
    m.or_w2 += eps;
    g.or_w2 = (cost(m) - c) / eps;
    m.or_w2 = saved;

    saved = m.or_b;
    m.or_b += eps;
    g.or_b = (cost(m) - c) / eps;
    m.or_b = saved;

    saved = m.and_w1;
    m.and_w1 += eps;
    g.and_w1 = (cost(m) - c) / eps;
    m.and_w1 = saved;

    saved = m.and_w2;
    m.and_w2 += eps;
    g.and_w2 = (cost(m) - c) / eps;
    m.and_w2 = saved;

    saved = m.and_b;
    m.and_b += eps;
    g.and_b = (cost(m) - c) / eps;
    m.and_b = saved;

    saved = m.nand_w1;
    m.nand_w1 += eps;
    g.nand_w1 = (cost(m) - c) / eps;
    m.nand_w1 = saved;

    saved = m.nand_w2;
    m.nand_w2 += eps;
    g.nand_w2 = (cost(m) - c) / eps;
    m.nand_w2 = saved;

    saved = m.nand_b;
    m.nand_b += eps;
    g.nand_b = (cost(m) - c) / eps;
    m.nand_b = saved;

    return g;
}
Xor learn(Xor m, Xor g) {

     m.or_w1 -= learn_rate * g.or_w1;
     m.or_w2 -= learn_rate * g.or_w2;
     m.or_b -= learn_rate * g.or_b;

     m.nand_w1 -= learn_rate * g.nand_w1;
     m.nand_w2 -= learn_rate * g.nand_w2;
     m.nand_b -= learn_rate * g.nand_b;

     m.and_w1 -= learn_rate * g.and_w1;
     m.and_w2 -= learn_rate * g.and_w2;
     m.and_b -= learn_rate * g.and_b;
    return m;
}
int main(void) {
    srand(time(0));
    float eps = 1e-1;
    Xor m = randXor();
    for (size_t i = 0; i < epoch; i++) {
        Xor g = finite_diff(m, eps);
        m = learn(m, g);
    }
    printf("final:\n");
    printXor(m);
    for (size_t i = 0; i < train_count; i++) {
        float  x1 = train[i][0];
        float  x2 = train[i][1];
        float y = forward(m, x1, x2);
        printf("y = %f, actually = %f \n", y, train[i][2]);
    }

    printf("or\n");
    for (size_t i = 0; i < train_count; i++) {
        float  x1 = train[i][0];
        float  x2 = train[i][1];
        // float y = forward(m, x1, x2);
        printf("%f ~ %f = %f\n", x1, x2, sigmoidf(x1 * m.or_w1 + x2 * m.or_w2 + m.or_b));
    }

    printf("nand\n");
    for (size_t i = 0; i < train_count; i++) {
        float  x1 = train[i][0];
        float  x2 = train[i][1];
        // float y = forward(m, x1, x2);
        printf("%f ~ %f = %f\n", x1, x2, sigmoidf(x1 * m.nand_w1 + x2 * m.nand_w2 + m.nand_b));
    }

    printf("and\n");
    for (size_t i = 0; i < train_count; i++) {
        float  x1 = train[i][0];
        float  x2 = train[i][1];
        // float y = forward(m, x1, x2);
        printf("%f ~ %f = %f\n", x1, x2, sigmoidf(x1 * m.and_w1 + x2 * m.and_w2 + m.and_b));
    }
    return 0;
}
