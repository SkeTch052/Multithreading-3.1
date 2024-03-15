#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <future>
#include <mutex>

const unsigned int SIZE = 15;
const unsigned int MIN_ELEMENT = 1;
const unsigned int MAX_ELEMENT = 100;
std::mutex mtx;

int findMin(int start, int size, std::vector<int>& v, std::promise<int>& p) {
    std::lock_guard<std::mutex> lock(mtx);
    int min = MAX_ELEMENT + 1;
    int min_elem = start;

    for (int i = start; i < size; i++) {
        if (v[i] < min) {
            min = v[i];
            min_elem = i;
        }
    }
    p.set_value(min_elem);
    return min_elem;
}

int main()
{
    std::vector<int> vec;
    std::mt19937 gen(15);
    std::uniform_int_distribution<int> distribution(MIN_ELEMENT, MAX_ELEMENT);
    for (int i = 0; i < SIZE; ++i) {
        vec.push_back(distribution(gen));
    }

    std::cout << "Before sort:\t";
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < SIZE - 1; ++i) {

        std::promise<int> promise;
        std::future<int> future = promise.get_future();

        std::thread(findMin, i, SIZE, std::ref(vec), std::ref(promise)).join();

        int min_index = future.get();
        std::swap(vec[i], vec[min_index]);
    }

    std::cout << "After sort:\t";
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}