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

uint64_t day6(vector<vector<uint64_t>>, vector<char>);

int main() {
  // Read File
  string myText;
  ifstream MyReadFile("day6Input.txt");

  string row;
  vector<vector<uint64_t>> input;
  vector<char> op;

  while(getline(MyReadFile, myText)) {
    row = myText;
    vector<uint64_t> nums;

    // Trim the beginning whitespace
    row.erase(0, row.find_first_not_of(' '));

    if (!(row[0] == '+' || row[0] == '*')) {
      while (row.size() != 0) {
        nums.push_back(stoull(row.substr(0, row.find(' '))));
        if (row.find(' ') == string::npos) {
          nums.push_back(stoull(row.substr(0, row.find(' '))));
          break;
        }
        row.erase(0, row.find(' ') + 1);
        row.erase(0, row.find_first_not_of(' '));
        cout << "Reading: " << nums.back() << endl;
      }
      input.push_back(nums);
    } else {
      while (row.size() != 0) {
        op.push_back(row.substr(0, row.find(' ')).at(0));
        if (row.find(' ') == string::npos) {
          op.push_back(row.substr(0, row.find(' ')).at(0));
          break;
        }
        row.erase(0, row.find(' ') + 1);
        row.erase(0, row.find_first_not_of(' '));
      }
    }
        
  }

   cout << day6(input, op) << endl;
 
  MyReadFile.close();
}

uint64_t day6(vector<vector<uint64_t>> input, vector<char> op) {
  uint64_t sum = 0;
  
  for (int j = 0; j < input[0].size(); j++) {
    uint64_t total = 0;
    char operation = op[j];
    if (operation == '+') {
      total = 0;
      for (int i = 0; i < input.size(); i++) {
        total += input[i][j];
        cout << input[i][j] << endl;
        cout << total << endl;
        cout << "Total: " << total << endl;
      }
    } else if (operation == '*') {
      total = 1;
      for (int i = 0; i < input.size(); i++) {
        total *= input[i][j];
        cout << input[i][j] << endl;
        cout << "Total: " << total << endl;
      }
      cout << "Multiplication total: " << total << endl;
    }

    cout << "Total for column " << j << ": " << total << endl;

    sum += total;

    
    
  }
 
  return sum;
}