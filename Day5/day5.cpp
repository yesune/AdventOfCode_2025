#include <iostream>
#include <fstream>
#include <sstream> // Required for stringstream
#include <vector>
#include <cmath>
#include <algorithm> // Required for std::sort
#include <cstdint>

using namespace std;

uint64_t day5(vector<vector<uint64_t>> fresh, vector<uint64_t> ingr); // declaration
unit64_t day5Part2(vector<vector<uint64_t>> input, vector<uint64_t> ingr);


int main() {
  string myText;

  // Read from the text file
  ifstream MyReadFile("day5Input.txt");
  vector<vector<uint64_t>> fresh;
  vector<uint64_t> ingr;
  
  while (getline(MyReadFile, myText)) {
    // If empty, then quit and start reading again
    if (myText.empty()) {
      break;
    }

    vector<uint64_t> range;
    
    string lower = myText.substr(0, myText.find('-'));
    string upper = myText.substr(myText.find('-') + 1);

    range.push_back(stoull(lower));
    range.push_back(stoull(upper));

    fresh.push_back(range);
  }

  while (getline(MyReadFile, myText)) {
    ingr.push_back(stoull(myText));
  }

  cout << day5(fresh, ingr) << endl;
  cout << day5Part2(fresh, ingr) << endl;

  MyReadFile.close();
}

// PART 1


uint64_t day5(vector<vector<uint64_t>> input) {
  uint64_t sum = 0;

  for (vector<uint64_t> range: input) {
    vector<uint64_t> invalid;

    uint64_t lower = range[0];
    uint64_t upper = range[1];

    int lowerDigits = log10(lower) + 1;
    int upperDigits = log10(upper) + 1;

    if (lowerDigits % 2 != 0) {
      lower = uint64_t(pow(10, lowerDigits));
    }

    if (upperDigits % 2 != 0) {
      upper = uint64_t(pow(10, upperDigits - 1) - 1);
    }

    lowerDigits = log10(lower) + 1;
    upperDigits = log10(upper) + 1;

    // This fancy algorithm is good enough for all cases somehow

    // Raise lower to the next known value of an invalid ID
    uint64_t lowerFirstHalf = lower / uint64_t(pow(10, lowerDigits / 2));
    uint64_t lowerSecondHalf = lower % uint64_t(pow(10, lowerDigits / 2));
    if (lowerSecondHalf < lowerFirstHalf) {
      lower = (lowerFirstHalf) * uint64_t(pow(10, lowerDigits / 2)) + lowerFirstHalf;
    } else if (lowerSecondHalf > lowerFirstHalf) {
      lowerFirstHalf += 1;
      lowerDigits = int(log10(lowerFirstHalf) + 1) * 2;
      lower = (lowerFirstHalf) * uint64_t(pow(10, lowerDigits / 2)) + lowerFirstHalf;
    }

    // Lower upper value to previous known value of an invalid ID
    uint64_t upperFirstHalf = upper/ uint64_t(pow(10, upperDigits / 2));
    uint64_t upperSecondHalf = upper % uint64_t(pow(10, upperDigits / 2));
    if (upperSecondHalf > upperFirstHalf) {
      upper = (upperFirstHalf) * uint64_t(pow(10, upperDigits / 2)) + upperFirstHalf;
    } else if (upperSecondHalf < upperFirstHalf) {
      upperFirstHalf -= 1;
      upperDigits = int(log10(upperFirstHalf) + 1) * 2;
      upper = (upperFirstHalf) * uint64_t(pow(10, upperDigits / 2)) + upperFirstHalf;
    }

    // Calculate the inclusive difference

    if (lower > upper) {
      continue;
    }

    int firstHalf = lowerFirstHalf;

    while (firstHalf <= upperFirstHalf) {
      
      lowerDigits = log10(firstHalf) + 1;
      uint64_t invalidId = (firstHalf) * uint64_t(pow(10, lowerDigits)) + firstHalf;

      invalid.push_back(invalidId);
      sum += invalidId;
      firstHalf += 1;

    }

  }

  return sum;

}


// PART 2

uint64_t day5Part2(vector<vector<uint64_t>> input) {

  return 0;

}
