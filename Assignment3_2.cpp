#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>
#include <vector>
#include <random>
#include <algorithm>

int highs[5], lows[5], readings[480];
int largest_interval = 0, largest_diff = -1, interval[80];
std::default_random_engine generator(time(NULL));
std::uniform_int_distribution<int> distribution(-100, 70);

int getReading() {
    return distribution(generator);
}

void core(int id) {
    for (int i = 0; i < 60; i++) {
        readings[(id*60)+i] = getReading();
    }
}

void findLargestFive() {
    int maxs[] = {INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN};
    int curr_max = INT_MIN;
    for (int i = 0; i < 480; i++) {
        if (curr_max < readings[i])
            curr_max = readings[i];
    }
    maxs[0] = curr_max;
    curr_max = INT_MIN;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 480; j++) {
            if (curr_max < readings[j] && readings[j] < maxs[i])
                curr_max = readings[j];
        }
        maxs[i+1] = curr_max;
        curr_max = INT_MIN;
    }
    for (int i = 0; i < 5; i++) {
        highs[i] = maxs[i];
    }
}

void findSmallestFive() {
    int mins[] = {INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX};
    int curr_min = INT_MAX;
    for (int i = 0; i < 480; i++) {
        if (curr_min > readings[i])
            curr_min = readings[i];
    }
    mins[0] = curr_min;
    curr_min = INT_MAX;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 480; j++) {
            if (curr_min > readings[j] && readings[j] > mins[i])
                curr_min = readings[j];
        }
        mins[i+1] = curr_min;
        curr_min = INT_MAX;
    }
    for (int i = 0; i < 5; i++) {
        lows[i] = mins[i];
    }
}

void findInterval() {
    int i;

    for (i = 0; i < 6; i++) {
        for (int j = 0; j < 80; j++) {
            interval[j] = readings[(i*80)+j];
        }
        int min = *std::min_element(interval, interval+80);
        int max = *std::max_element(interval, interval+80);
        int diff = max - min;
        if (largest_diff < diff) {
            largest_diff = diff;
            largest_interval = i;
        }
    }
}

void generateReport() {
    std::vector<std::thread> cores;
    
    for (int i = 0; i < 8; i++) {
        cores.push_back(std::thread(core, i));
    }
    for (std::thread &cor : cores) cor.join();
    findLargestFive();
    findSmallestFive();
    findInterval();
    std::cout << "The five highest temperatures recorded were:\n";
    for (int &high : highs) std::cout << high << "F" << std::endl;
    std::cout << "The five lowest temperatures recorded were:\n";
    for (int &low : lows) std::cout << low << "F" << std::endl;
    std::cout << "The interval between ";
    switch (largest_interval)
    {
        case 0:
            std::cout << "0:00 and 0:10";
            break;
        case 1:
            std::cout << "0:10 and 0:20";
            break;
        case 2:
            std::cout << "0:20 and 0:30";
            break;
        case 3:
            std::cout << "0:30 and 0:40";
            break;
        case 4:
            std::cout << "0:40 and 0:50";
            break;
        case 5:
            std::cout << "0:50 and 0:00";
            break;
        
        default:
            std::cerr << "An unexpected error occured, please rerun the program.\n";
            break;
    }
    
    std::cout << " had the largest temperature difference of " << largest_diff << "F.\n";

}

int main() {
    generateReport();
    return 0;
}