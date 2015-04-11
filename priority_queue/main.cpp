#define BOOST_TEST_MODULE stringtest
#include <iostream>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <boost/test/included/unit_test.hpp>

#include "priority_queue.h"

using namespace std;

BOOST_AUTO_TEST_SUITE (stringtest)

BOOST_AUTO_TEST_CASE (testGet) {
    my::priority_queue<int, int> q;
    const int n = 10;
    for (int t = 0; t < 10000; ++t) {
//        if (t % 100 == 0)
//            std::cout << t << std::endl;
        std::vector<std::shared_ptr<boost::thread>> threads;
        for (int i = 0; i < n; ++i) {
            q.put(i, i);
        }
        std::vector<int> result(n);
        for (int i = 0; i < n; ++i) {
            auto th = std::make_shared<boost::thread> (boost::thread([&q, i, &result]{q.get(result[i]);}));
            threads.push_back(th);
        }
        for (int i = 0; i < n; ++i) {
            threads[i]->join();
        }
    }
}

BOOST_AUTO_TEST_CASE (testPut) {
    const int n = 100;
    const int t = 10000;
    my::priority_queue<int, int> q;
    for (int i = 0; i < t; ++i) {
//        if (i % 100 == 0)
//            std::cout << i << std::endl;
        std::vector<boost::thread> threads;
        for (int j = 0; j < n; ++j) {
            threads.push_back(boost::thread([&q, j]{q.put(j, j);}));
        }
        for (int j = 0; j < n; ++j) {
            threads[j].join();
        }
        std::vector<int> result(n);
        for (int j = 0; j < n; ++j) {
            q.get(result[j]);
        }
        for (int j = 0; j < n - 1; ++j) {
            BOOST_CHECK(result[j] >= result[j + 1]);
        }
    }
}

BOOST_AUTO_TEST_CASE (testPutGet) {
    const int n = 10;
    const int t = 10000;
    for (int i = 0; i < t; ++i) {
        my::priority_queue<int, int> q;
//        if (i % 100 == 0)
//            std::cout << i << std::endl;
        std::vector<boost::thread> threads;
        for (int j = 0; j < n; ++j) {
                threads.push_back(boost::thread([&q, i, j]{q.put(i + 2 * j, i + 2 * j);}));
                threads.push_back(boost::thread([&q, i, j]{q.put(i * j - j, i * j - j);}));
                threads.push_back(boost::thread([&q]{int a; q.get(a);}));
        }
        for (int j = 0; j < n * 3; ++j) {
            threads[j].join();
        }
        std::vector<int> result(n);
        for (int j = 0; j < n; ++j) {
            q.get(result[j]);
        }
        for (int j = 0; j < n - 1; ++j) {
            BOOST_CHECK(result[j] >= result[j + 1]);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END( )
