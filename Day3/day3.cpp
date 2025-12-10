#include <iostream>
#include <fstream>
#include <sstream> // Required for stringstream
#include <vector>
#include <cmath>
#include <algorithm> // Required for std::sort
#include <cstdint>
#include <set>
#include <numeric>
#include <utility> 

using namespace std;

int day3(vector<vector<int>>);
uint64_t day3Part2(vector<vector<int>> input);

int main() {
  // Read File
  string myText;
  vector<vector<int>> input;
  ifstream MyReadFile("day3Input.txt");
  while(getline(MyReadFile, myText)) {
    vector<int> row;
    for (char& c : myText) {
        row.push_back(c - '0'); // this converts char to int??
    }
    input.push_back(row);
  }
 
  // Fun logic!
  cout << day3(input) << endl;
  cout << day3Part2(input) << endl;
 
  MyReadFile.close();
}

int day3(vector<vector<int>> input) {
    int sum = 0;

    for (vector<int> row: input) {
        int biggest = 0;
        int secondBiggest = 0;
        for (int i = 0; i < row.size(); i++) {
            if (row[i] > biggest) {
                if (i == row.size() - 1) {
                    secondBiggest = row[i];
                } else {
                    biggest = row[i];
                    secondBiggest = 0;
                }
            } else if (row[i] > secondBiggest) {
                secondBiggest = row[i];
            }
        }
        sum += biggest * 10 + secondBiggest;
    }

    return sum;
}

void clearSmallerPairs(multiset<pair<int, int>>* biggestVoltage, int v, int rowSize, int index, int topN) {
    while (!biggestVoltage->empty()) {
        if (biggestVoltage->begin()->first >= v || biggestVoltage->size() + (rowSize - (index)) <= topN) {
            break;
        } else {
            biggestVoltage->erase(biggestVoltage->begin()); 
        }
    }
}

uint64_t day3Part2(vector<vector<int>> input) {
    static int topN = 12;
    uint64_t sum = 0;

    for (vector<int> row: input) {
        uint64_t endVoltage = 0;
        multiset<pair<int, int>> biggestVoltage; 
        for (int i = 0; i < row.size(); i++) {
            int v = row[i];
            int reverseIndex = row.size() - 1 - i;
            int smallest = biggestVoltage.begin()->first;
            int biggest = biggestVoltage.rbegin()->first;
            auto it = biggestVoltage.upper_bound({v, i});
            int numGreater = distance(it, biggestVoltage.end());
            
            // If there are less than N elements greater than current
            if (numGreater < topN) {
                // If we are greater and have space before the end, clear anything smaller, excluding equals
                if (v > smallest) {
                    clearSmallerPairs(&biggestVoltage, v, row.size(), i, topN);
                    biggestVoltage.insert({v, i});
                }
                // If we are not full, just add
                else if (biggestVoltage.size() < topN) {
                    biggestVoltage.insert({v, i});
                }
                // If we are equal to the smallest and full, remove the smallest one
                else if (v == smallest) {
                    biggestVoltage.erase(biggestVoltage.begin());
                    biggestVoltage.insert({v, i});
                }
            }
        }

        // construct number
        multiset<pair<int, int>> optimizedVoltage; 
        for (auto &p : biggestVoltage) {
            optimizedVoltage.insert({p.second, p.first});
        }
        int digit = topN - 1;
        for (auto &p : optimizedVoltage) {
            endVoltage += p.second * pow(10, digit);  
            digit -= 1;
        }

        sum += endVoltage;

        cout << endVoltage << endl;
    }
    cout << endl;
    return sum;
}