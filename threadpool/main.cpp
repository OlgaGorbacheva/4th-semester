#define BOOST_TEST_MODULE stringtest
#include <iostream>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <boost/test/included/unit_test.hpp>

#include "threadpool.h"

using namespace std;

struct matrix {
    vector<vector<int>> data;
    int size;
    matrix(int _size):size(_size) {
        data.resize(size);
        for (int i = 0; i < size; i++) {
            data[i].resize(size);
        }
    }
    void operator = (matrix a) {
        size = a.size;
        data = a.data;
    }
};

void write(matrix const &m) {
    for (int i = 0; i < m.size; i++) {
        for (int j = 0; j < m.size; j++) {
            cout << m.data[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void generate(matrix &m) {
    srand(time(0));
    for (int i = 0; i < m.size; i++)
        for (int j = 0; j < m.size; j++)
            m.data[i][j] = rand() % 10;
}

matrix multiply(matrix &a, matrix &b) {
    if (a.size != b.size)
        throw "плохо";
    matrix c(a.size);
    for (int i = 0; i < a.size; i++) {
        for (int j = 0; j < b.size; j++) {
            for (int k = 0; k < c.size; k++) {
                c.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }
    return c;
}

boost::mutex mut;

void sum(matrix const &a, matrix const &b, shared_ptr<matrix> c, int i, int j) {
    for (int k = 0; k < a.size; k++) {
        c->data[i][j] += a.data[i][k] * b.data[k][j];
    }
}

matrix multithr_muktiply(matrix const &a, matrix const &b) {
    if (a.size != b.size)
        throw "плохо";
    shared_ptr<matrix> pointer (new matrix(a.size));
    my::threadpool pool;
    for (int i = 0; i < a.size; i++) {
        for (int j = 0; j < b.size; j++) {
            pool.add<void>(sum, a, b, pointer, i, j);
        }
    }
    pool.stop();
    cout << endl;
    return *pointer;
}

BOOST_AUTO_TEST_SUITE (stringtest)

BOOST_AUTO_TEST_CASE (test1)
{
    int n = 100;
    matrix a(n), b(n);
    generate(a);
    generate(b);
    clock_t t1 = clock();
    matrix c = multiply(a, b);
    t1 = clock() - t1;
    clock_t t2 = clock();
    matrix d = multithr_muktiply(a, b);
    t2 = clock() - t2;
    for (int i = 0; i < c.size; i++) {
        for (int j = 0; j < c.size; j++) {
            BOOST_CHECK(c.data[i][j] == d.data[i][j]);
        }
    }
    cout << "the diference of time on " << n << " elements is " << (float)(t2 - t1) / CLOCKS_PER_SEC << " seconds" << endl;
}



BOOST_AUTO_TEST_SUITE_END( )
