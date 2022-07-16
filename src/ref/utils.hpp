#pragma once
#include <QtMath>

struct Index {
    int row;
    int column;
};

Index mapFromLinear(const Index &linearIndex);
Index mapToLinear(const Index &quadraticIndex);
