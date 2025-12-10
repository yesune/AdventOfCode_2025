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

int day4(vector<vector<char>>);
int day4Part2(vector<vector<char>> input);

int main() {
  // Read File
  string myText;
  vector<vector<char>> input;
  ifstream MyReadFile("day4Input.txt");
  while(getline(MyReadFile, myText)) {
    vector<char> v(myText.begin(), myText.end());
    input.push_back(v);
  }
 
  // Fun logic!
  cout << day4(input) << endl;
  cout << day4Part2(input) << endl;
 
  MyReadFile.close();
}

int isNeighborPaper(char c) {
    if (c == '@') {
        return 1;
    }
    return 0;
}

int day4(vector<vector<char>> input) {
    int sum = 0;

    // No reasonable speedup besides sharing neighbors, I'm too lazy for that
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            char c = input[i][j];
            int neighbor = 0;
            if (c == '@') {
                // Check surroundings
                
                // Are we top
                if (j != 0) {
                    neighbor += isNeighborPaper(input[i][j - 1]);
                }

                // Are we bottom
                if (j != input[i].size() - 1) {
                    neighbor += isNeighborPaper(input[i][j + 1]);
                }

                // Are we right
                if (i != input.size() - 1) {
                    neighbor += isNeighborPaper(input[i + 1][j]);
                }

                // Are we left
                if (i != 0) {
                    neighbor += isNeighborPaper(input[i - 1][j]);
                }
                
                // Are we top-left
                if (i != 0 && j != 0) {
                    neighbor += isNeighborPaper(input[i - 1][j - 1]);
                }

                // Are we top-right
                if (i != input.size() - 1 && j != 0) {
                    neighbor += isNeighborPaper(input[i + 1][j - 1]);
                }

                // Are we bottom-left
                if (i != 0 && j != input[i].size() - 1) {
                    neighbor += isNeighborPaper(input[i - 1][j + 1]);
                }

                // Are we bottom-right
                if (i != input.size() - 1 && j != input[i].size() - 1) {
                    neighbor += isNeighborPaper(input[i + 1][j + 1]);
                }

                if (neighbor < 4) {
                    sum += 1;
                }
            }
        }
    }

    return sum;

}

int day4Part2(vector<vector<char>> input) {
    int prevSum = -1;
    int sum = 0;

    while (prevSum != sum) {
        prevSum = sum;
        // No reasonable speedup besides sharing neighbors, I'm too lazy for that
        for (int i = 0; i < input.size(); i++) {
            for (int j = 0; j < input[i].size(); j++) {
                char c = input[i][j];
                int neighbor = 0;
                if (c == '@') {
                    // Check surroundings
                    
                    // Are we top
                    if (j != 0) {
                        neighbor += isNeighborPaper(input[i][j - 1]);
                    }

                    // Are we bottom
                    if (j != input[i].size() - 1) {
                        neighbor += isNeighborPaper(input[i][j + 1]);
                    }

                    // Are we right
                    if (i != input.size() - 1) {
                        neighbor += isNeighborPaper(input[i + 1][j]);
                    }

                    // Are we left
                    if (i != 0) {
                        neighbor += isNeighborPaper(input[i - 1][j]);
                    }
                    
                    // Are we top-left
                    if (i != 0 && j != 0) {
                        neighbor += isNeighborPaper(input[i - 1][j - 1]);
                    }

                    // Are we top-right
                    if (i != input.size() - 1 && j != 0) {
                        neighbor += isNeighborPaper(input[i + 1][j - 1]);
                    }

                    // Are we bottom-left
                    if (i != 0 && j != input[i].size() - 1) {
                        neighbor += isNeighborPaper(input[i - 1][j + 1]);
                    }

                    // Are we bottom-right
                    if (i != input.size() - 1 && j != input[i].size() - 1) {
                        neighbor += isNeighborPaper(input[i + 1][j + 1]);
                    }

                    if (neighbor < 4) {
                        sum += 1;
                        input[i][j] = 'x';
                    }
                }
            }
        }
    }

    return sum;

}
