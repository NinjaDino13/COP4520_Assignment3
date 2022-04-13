#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>
#include <vector>
#include <random>
#include <algorithm>

int highs[5], lows[5], readings[480], temp_readings[480];
int largest_interval, largest_diff, interval[80];
std::default_random_engine generator(time(NULL));
std::uniform_int_distribution<int> distribution(-100, 70);

int getReading() {
    return distribution(generator);
}

void core(int id) {
    for (int i = 0; i < 60; i++) {
        readings[(id*60)+i] = getReading();
        temp_readings[(id*60)+i] = readings[(id*60)+i]; 
    }
}

void findLargestFive() {
    int maxs[5];

}

void generateReport() {
    std::vector<std::thread> cores;
    
    for (int i = 0; i < 8; i++) {
        cores.push_back(std::thread(core, i));
    }
    for (std::thread &cor : cores) cor.join();
    
    int n = sizeof(temp_readings) / sizeof(temp_readings[0]);
    std::sort(temp_readings, temp_readings + n);

    for (int i = 0; i < 60; i++) {
        printf("core 1: %d\t core 2: %d\t core 3: %d\t core 4: %d\t core 5: %d\t core 6: %d\t core 7: %d\t core 8: %d\n", 
            readings[i], readings[60+i], readings[120+i], readings[180+i], readings[240+i], readings[300+i], readings[360+i], readings[420+i]);
    }

    for (int i = 0; i < 480; i++) {
        std::cout << temp_readings[i] << std::endl;
    }
}

int main() {
    generateReport();
    return 0;
}