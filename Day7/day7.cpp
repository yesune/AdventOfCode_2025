#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>

using namespace std;

int day7(vector<string> input);
uint64_t day7Part2(vector<string> input);
void printBeam(vector<uint64_t> beam);
void printSplitter(string row);

int main() {
  string myText;
  ifstream MyReadFile("day7Input.txt");
  vector<string> input;

  while (getline(MyReadFile, myText)) {
    input.push_back(myText);
  }

  cout << day7(input) << endl;
  cout << day7Part2(input) << endl;

  MyReadFile.close();
}


int day7(vector<string> input) {
  int count = 0;
  // Initialize beam vector
  bool beam[input[0].length()];

  // Find Location of initial beam
  int S = input[0].find('S');
  beam[S] = true;

  // Iterate down the rows
  for (int i = 1; i < input.size(); i++) {
    for (int j = 0; j < input[i].length(); j++) {
      if (input[i][j] == '^' && beam[j]) {
        // Fortunately, splitters cannot be next to each other
        // We don't have to consider the case of a space under a splitter having a beam from an adjacent splitter
        // We also don't have to create a new beam array too because of this!
        beam[j] = false;
        if (j > 0) beam[j-1] = true;
        if (j < input[i].length() - 1) beam[j+1] = true;
        count += 1;
      }
    }
  }

  return count;
}

uint64_t day7Part2(vector<string> input) {
  // We use a vector instead to track how many possible paths go through a point
  vector<uint64_t> beam(input[0].length(), 0);

  // Find Location of initial beam
  int S = input[0].find('S');
  beam[S] = 1;

  // Iterate down the rows
  for (int i = 1; i < input.size(); i++) {
    // create a new beam array since we will add adjacent beams
    // Ex: .^.^.
    // The middle one will need the sum of # paths from the left and right
    vector<uint64_t> newBeam(input[i].length(), 0);
    for (int j = 0; j < input[i].length(); j++) {
      // New logic here since we recreate the beam array
      if (beam[j] > 0) {
        if (input[i][j] == '^' ) {
          if (j > 0) newBeam[j-1] += beam[j];
          if (j < input[i].length() - 1) newBeam[j+1] += beam[j];
        } else {
          // Make sure to carry down a beam if there's no splitter
          // Had a bug where this was hard-set, but have to add instead for cases like
          // 1 3 0
          // ^ . ^
          // 0 4 0
          newBeam[j] += beam[j]; 
        }
      }
    }
    /* Debugging
    printSplitter(input[i]);
    printBeam(newBeam);
    cout << endl;
    */
    beam = newBeam;
  }

  // Sum it at the end
  uint64_t sum = 0;
  for (int i = 0; i < beam.size(); i++) {
    sum += beam[i];
  }

  return sum;
}

// For Debugging purposes
void printBeam(vector<uint64_t> beam) {
  for (int i = 0; i < beam.size(); i++) {
    cout << beam[i] << " ";
  }
  cout << endl;
}

void printSplitter(string row) {
  for (int i = 0; i < row.length(); i++) {
    cout << row[i] << " ";
  }
  cout << endl;
}