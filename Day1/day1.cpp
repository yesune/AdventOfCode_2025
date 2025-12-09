#include <iostream>
#include <fstream>
#include <sstream> // Required for stringstream
#include <vector>
#include <cmath>
#include <algorithm> // Required for std::sort

using namespace std;

int day1(vector<string> v1); // declaration
int day1Part2(vector<string> v1);

int main() {
  string myText;

  // Read from the text file
  ifstream MyReadFile("day1Input.txt");

  vector<string> v1;

  while (getline(MyReadFile, myText)) {
    v1.push_back(myText);
  }

  cout << day1(v1);

  cout << '\n';

  cout << day1Part2(v1);

  MyReadFile.close();
}

int day1(vector<string> v1) {
  int start = 50;
  int count = 0;

  for (string s : v1) {
    int dirRight = s.substr(0, 1) == "R";
    int delta = stoi(s.substr(1));
    
    if (dirRight) {
      start += delta;
    } else {
      start -= delta;
    }

    start = start % 100;

    if (start == 0) {
      count += 1;
    }
  }

  return count;

}

int day1Part2(vector<string> v1) {
  int start = 50;
  int count = 0;

  for (string s : v1) {
    int dirRight = s.substr(0, 1) == "R";
    int delta = stoi(s.substr(1));
    int prev = start;
    
    if (dirRight) {
      start += delta;
    } else {
      start -= delta;
    }

    if ((prev > 0 && start < 0) || (prev < 0 && start > 0)) {
      count += 1;
    }
    
    if (start % 100 == 0) {
      if (abs(start / 100) >= 1) {
        count += abs(start / 100) - 1;
      }
    } else {
      count += abs(start / 100);
    }

    start = start % 100;

    if (start == 0) {
      count += 1;
    } else {
    }

  }

  return count;

}
