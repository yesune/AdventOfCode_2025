#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <numeric>
#include <chrono>
#include <set>

using namespace std;

static int MAX_CONNECTIONS = 1000;

int day8(vector<tuple<int, int, int>> input);
uint64_t day8Part2(vector<tuple<int, int, int>> input);

int main() {
  string myText;
  ifstream MyReadFile("day8TestInput.txt");
  vector<tuple<int, int, int>> input;

  while (getline(MyReadFile, myText)) {
    int first = myText.find(',', 0);
    int second = myText.find(',', first + 1);
    

    int t1 = stoi(myText.substr(0, first));
    int t2 = stoi(myText.substr(first + 1, second));
    int t3 = stoi(myText.substr(second + 1));

    input.push_back(make_tuple(t1, t2, t3));
  }

  cout << day8(input) << endl;
  cout << day8Part2(input) << endl;

  MyReadFile.close();
}

// Directly calculates Euclidiean distance of all points to each other
multiset<tuple<float, int, int>> day8Euclidiean(vector<tuple<int, int, int>> input) {

  // Max heap, but we only insert if its smaller than the max
  multiset<tuple<float, int, int>> distances;

  for (int i = 0; i < input.size(); i++) {
    for (int j = i + 1; j < input.size(); j++) {
      int x1 = get<0>(input[i]);
      int y1 = get<1>(input[i]);
      int z1 = get<2>(input[i]);

      int x2 = get<0>(input[j]);
      int y2 = get<1>(input[j]);
      int z2 = get<2>(input[j]);

      float dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
      //printf("i: %d, j: %d, dist: %f\n", i, j, dist);
      if (distances.size() < MAX_CONNECTIONS)  {
        distances.insert(make_tuple(dist, i, j));
      } else if (get<0>(*distances.rbegin()) > dist) {
        distances.erase(--distances.end());;
        distances.insert(make_tuple(dist, i, j));
      }
    }
  }

  return distances;
}

/*
But what if I hate doing things efficiently and want to use law of cosines?
Much less efficient but might as well show it.
Uses law of cosines to only calculate distance for a set of points, 
then use relative distance to that reference point for the rest
*/
multiset<tuple<float, int, int>> day8Cosine(vector<tuple<int, int, int>> input) {
  // Max heap, but we only insert if its smaller than the max
  multiset<tuple<float, int, int>> distances;

  // distance, index, diff
  vector<tuple<float, int, vector<int>>> reference;

  // Create initial distances and angles
  int x1 = get<0>(input[0]);
  int y1 = get<1>(input[0]);
  int z1 = get<2>(input[0]);
  for (int i = 1; i < input.size(); i++) {
    int x2 = get<0>(input[i]);
    int y2 = get<1>(input[i]);
    int z2 = get<2>(input[i]);
    float dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
    vector<int> diff = {x2 - x1, y2 - y1, z2 - z1};

    reference.push_back(make_tuple(dist, i, diff));
    if (distances.size() < MAX_CONNECTIONS)  {

      distances.insert(make_tuple(dist, 0, i));
    } else if (get<0>(*distances.rbegin()) > dist) {
      distances.erase(--distances.end());;
      distances.insert(make_tuple(dist, 0, i));
    }
  }

  // Calculate the rest using law of cosines
  for (int i = 0; i < reference.size(); i++) {
    float a = get<0>(reference[i]);
    for (int j = i + 1; j < reference.size(); j++) {
      float b = get<0>(reference[j]);

      float dot = inner_product(get<2>(reference[i]).begin(),get<2>(reference[i]).end(), get<2>(reference[j]).begin(), 0.0f);
      float angle = acos(dot / (get<0>(reference[i]) * get<0>(reference[j])));
      float dist = sqrt(pow(a, 2) + pow(b, 2) - 2 * a * b * cos(angle));
      //printf("i: %d, j: %d, dist: %f\n", i + 1, j + 1, dist);
      if (distances.size() < MAX_CONNECTIONS)  {
        distances.insert(make_tuple(dist, i + 1, j + 1));
      } else if (get<0>(*distances.rbegin()) > dist) {
        distances.erase(--distances.end());
        distances.insert(make_tuple(dist, i + 1, j + 1));
      }
    }
  }

  return distances;
}

/*
Euclid Time: 54 ms
Cosine Time: 77 ns
Euclid is more efficient in the end
*/
int day8(vector<tuple<int, int, int>> input) {
  multiset<tuple<float, int, int>> distances = day8Euclidiean(input);
  // multiset<tuple<float, int, int>> distances = day8Cosine(input);

  set<set<int>> circuits;

  for (tuple<float, int, int> distance: distances) {
    bool found = false;
    set<int> foundCircuit;
    for (set<int> circuit: circuits) {
      if (circuit.count(get<1>(distance)) != 0 || circuit.count(get<2>(distance)) != 0) {
        // If we found a previous one, then we have to combine the sets
        if (found) {
          cout << "Found another set " << get<1>(distance) << ", " << get<2>(distance) << endl;
          circuit.insert(foundCircuit.begin(), foundCircuit.end());
          cout << "Combined set size" << circuit.size() << endl;
          circuits.erase(foundCircuit);
        } else {
          cout << "Found a set " << get<1>(distance) << ", " << get<2>(distance) << endl;
          cout << "New set size" << circuit.size() << endl;
          circuit.insert(get<1>(distance));
          circuit.insert(get<2>(distance));
          found = true;
        }
      }
    }
    if (!found) {
      set<int> newCircuit;
      newCircuit.insert(get<1>(distance));
      newCircuit.insert(get<2>(distance));
      cout << get<1>(distance) << ", " << get<2>(distance) << endl;
      circuits.insert(newCircuit);
    }
  }

  // Not most optimal but since its only 1000 connections we can do the easy solution
  vector<int> sizes = {1, 1, 1};
  for (const auto& s : circuits) {
      sizes.push_back(s.size());
  }
  sort(sizes.rbegin(), sizes.rend());
  int result = sizes[0] * sizes[1] * sizes[2];
  cout << sizes[0] << ", " << sizes[1] << ", " << sizes[2] << endl;

  return result;
}

uint64_t day8Part2(vector<tuple<int, int, int>> input) {
  return 0;
}