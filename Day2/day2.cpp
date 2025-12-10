#include <iostream>
#include <fstream>
#include <sstream> // Required for stringstream
#include <vector>
#include <cmath>
#include <algorithm> // Required for std::sort
#include <cstdint>
 
using namespace std;
 
uint64_t day2(vector<vector<uint64_t>> v1); // declaration
uint64_t day2Part2(vector<vector<uint64_t>> input);
vector<uint64_t> resolveRange(uint64_t start, uint64_t end, int position, int size);
uint64_t resolveLeaf(uint64_t start, uint64_t end, int size);
bool validateId(string id, int interval);
bool incrementId(string id);
 
int main() {
  // Read File
  string myText;
  ifstream MyReadFile("day2Input.txt");
  getline(MyReadFile, myText);
  cout << "Read: " << myText << endl;
 
  // Splitting
  char del = ',';
  stringstream ss(myText);
  string pair;
  vector<vector<uint64_t>> input;

  while (getline(ss, pair, del)) {
    vector<uint64_t> range;
    string lower = pair.substr(0, pair.find('-'));
    string upper = pair.substr(pair.find('-') + 1);
    range.push_back(stoull(lower));
    range.push_back(stoull(upper));
    input.push_back(range);
  }
 
  // Fun logic!
  cout << day2(input) << endl;
  cout << day2Part2(input) << endl;
 
  MyReadFile.close();
}
 
// PART 1
 
uint64_t day2(vector<vector<uint64_t>> input) {
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
 
uint64_t day2Part2(vector<vector<uint64_t>> input) {
  uint64_t sum = 0;
  vector<uint64_t> invalid;
 
  for (vector<uint64_t> range: input) {
    cout << "Processing range: " << range[0] << "-" << range[1] << '\n';
    for (uint64_t i = range[0]; i <= range[1]; i++) {
      string id = to_string(i);
      if (incrementId(id)) {
        invalid.push_back(stoull(id));
        cout << "Invalid ID: " << id << '\n';
        sum += stoull(id);
      }
    }
    cout << "Finished range: " << range[0] << "-" << range[1] << '\n' << '\n';
  }

  return sum;
}
 
bool validateId(string id, int interval) {
  // we only need factors
  if (id.length() % interval != 0) {
    return false;
  }

  string substring = id.substr(0, interval);
  // increment by interval
  for (int k = 0; k < id.length(); k += interval) {
    if (substring != id.substr(k, interval)) {
      return false;
    }
  }
  return true;
}
 
bool incrementId(string id) {
  for (int j = 1; j <= id.length() / 2; j++) {
    if (validateId(id, j)) {
      return true;
    }
  }
  return false;
}