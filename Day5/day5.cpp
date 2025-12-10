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

uint64_t day5(vector<pair<uint64_t, uint64_t>>, vector<uint64_t>);
uint64_t day5Part2(vector<pair<uint64_t, uint64_t>> fresh);

int main() {
  // Read File
  string myText;
  vector<pair<uint64_t, uint64_t>> fresh;
  vector<uint64_t> ingr;
  ifstream MyReadFile("day5Input.txt");
  while(getline(MyReadFile, myText)) {
    if (myText.empty()) {
        break;
    }
    pair<uint64_t, uint64_t> freshPair;
    freshPair.first = (stoull(myText.substr(0, myText.find("-"))));
    freshPair.second = (stoull(myText.substr(myText.find("-") + 1)));
    fresh.push_back(freshPair);
  }

  while (getline(MyReadFile, myText)) {
    ingr.push_back(stoull(myText));
  }
 
  cout << day5(fresh, ingr) << endl;
  cout << day5Part2(fresh) << endl;
 
  MyReadFile.close();
}

uint64_t day5(vector<pair<uint64_t, uint64_t>> fresh, vector<uint64_t> ingr) {
    int count = 0;
    
    sort(fresh.begin(), fresh.end(),  [](auto &left, auto &right) {
        return right.first < left.first;
    });

    sort(ingr.begin(), ingr.end(), greater<>());

    cout << "Freshness sorted" << endl;
    for (pair<uint64_t, uint64_t> p : fresh) {
        cout << p.first << "-" << p.second << endl;
    }

    cout << "Ingredients sorted" << endl;
    for (uint64_t i : ingr) {
        cout << i << endl;
    }

    uint64_t currentIngredient = ingr.back();
    pair<uint64_t, uint64_t> currentFreshness = fresh.back();

    while (!ingr.empty() && !fresh.empty()) {
        if (currentIngredient < currentFreshness.first) {
            ingr.pop_back();
            currentIngredient = ingr.back();
            continue;
        } else {
            if (currentIngredient > currentFreshness.second) {
                fresh.pop_back();
                currentFreshness = fresh.back();
                continue;
            } else {
                ingr.pop_back();
                currentIngredient = ingr.back();
                count += 1;
            }
        }
    }

    return count;

}

uint64_t day5Part2(vector<pair<uint64_t, uint64_t>> fresh) {
    uint64_t total = 0;

    // sort
    sort(fresh.begin(), fresh.end(),  [](auto &left, auto &right) {
        return left.first < right.first;
    });

    uint64_t upper = 0;
    // just keep track of the upper bound since we'll go increasing
    for (pair<uint64_t, uint64_t> p : fresh) {
        if (p.second > upper) {
            if (p.first > upper) {
                total += (p.second - p.first + 1);
                upper = p.second;
            } else {
                total += (p.second - upper);
                upper = p.second;
            }
        }
    }

    return total;
}