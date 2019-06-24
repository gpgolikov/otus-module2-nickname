#include <iostream>
#include <tuple>

#include "matrix.h"

using namespace std;
using namespace griha;

int main() {

    Matrix<int, 0> matrix;
    for (size_t i = 0u; i <= 9u; ++i) {
        matrix[i][i] = i;
        matrix[i][9u - i] = 9u - i;
    }

    for (size_t i = 1u; i <= 8u; ++i) {
        for (size_t j = 1u; j < 8u; ++j) {
            cout << matrix[i][j] << ' ';
        }
        cout << matrix[i][8u];
        endl(cout);
    }
    cout << matrix.size() << endl;

    for (auto el : matrix) {
        size_t x, y;
        int v;
        tie(x, y, v) = el;
        cout << "matrix[" << x << "][" << y << "]: " << v << endl;
    }

    return 0;
}