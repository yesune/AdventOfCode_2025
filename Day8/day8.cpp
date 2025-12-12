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
#include <tuple>
#include <climits>

using namespace std;

static int MAX_CONNECTIONS_TESTING = 10;
static int MAX_CONNECTIONS = 1000;
static int MAX_INT_CONNECTIONS = INT_MAX;
static int INPUT_SIZE = 1000; // Too lazy to get this properly

int day8(vector<tuple<int, int, int>> input);
uint64_t day8Part2(vector<tuple<int, int, int>> input);

int main() {
  string myText;
  ifstream MyReadFile("day8Input.txt");
  vector<tuple<int, int, int>> input;

  while (getline(MyReadFile, myText)) {
    int first = myText.find(',', 0);
    int second = myText.find(',', first + 1);
    

    int t1 = stoi(myText.substr(0, first));
    int t2 = stoi(myText.substr(first + 1, second));
    int t3 = stoi(myText.substr(second + 1));

    input.push_back(make_tuple(t1, t2, t3));
  }

  // cout << day8(input) << endl;
  cout << day8Part2(input) << endl;

  MyReadFile.close();
}

// Directly calculates Euclidiean distance of all points to each other
multiset<tuple<float, int, int, int, int>> day8Euclidiean(vector<tuple<int, int, int>> input) {

  // Max heap, but we only insert if its smaller than the max
  multiset<tuple<float, int, int, int, int>> distances;

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
      if (distances.size() < MAX_INT_CONNECTIONS)  { // replace with constant you need for testing, part 1, and part 2
        distances.insert(make_tuple(dist, i, j, x1, x2));
      } else if (get<0>(*distances.rbegin()) > dist) {
        distances.erase(--distances.end());;
        distances.insert(make_tuple(dist, i, j, x1, x2));
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

// Helper - Thanks ChatGPT
void printSet(const std::set<int> &s) {
    std::cout << "{ ";
    for (int x : s) {
        std::cout << x << " ";
    }
    std::cout << "}" << std::endl;
}

/*
Euclid Time: 54 ms
Cosine Time: 77 ns
Euclid is more efficient in the end
*/
int day8(vector<tuple<int, int, int>> input) {
  multiset<tuple<float, int, int, int, int>> distances = day8Euclidiean(input); 
  // multiset<tuple<float, int, int>> distances = day8Cosine(input);

  vector<set<int>> circuits;

  for (tuple<float, int, int, int, int> distance: distances) {
    bool found = false;
    vector<set<int>> foundCircuit;
    vector<set<int>> newCircuits;
    for (set<int> circuit: circuits) {
      if (circuit.count(get<1>(distance)) != 0 || circuit.count(get<2>(distance)) != 0) {
        // If we see that an element exists, add it to circuits we have to merge
        // cout << "Found a set to merge: " << get<1>(distance) << ", " << get<2>(distance) << " in ";
        // printSet(circuit);
        foundCircuit.push_back(circuit);
      } else {
        // Easiest solution is to just recreate the vector (I am not working with iterators!!)
        newCircuits.push_back(circuit);
      }
    }

    // Merge all found circuits
    set<int> merged = {get<1>(distance), get<2>(distance)};
    for (set<int> fc: foundCircuit) {
      merged.insert(fc.begin(), fc.end());
    }
    newCircuits.push_back(merged);
    circuits = newCircuits;
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
  multiset<tuple<float, int, int, int, int>> distances = day8Euclidiean(input); 
  // multiset<tuple<float, int, int>> distances = day8Cosine(input);

  vector<set<int>> circuits;
  pair<uint64_t, uint64_t> lastConnection = {0, 0};

  for (tuple<float, int, int, int, int> distance: distances) {
    bool found = false;
    vector<set<int>> foundCircuit;
    vector<set<int>> newCircuits;
    for (set<int> circuit: circuits) {
      if (circuit.count(get<1>(distance)) != 0 || circuit.count(get<2>(distance)) != 0) {
        // If we see that an element exists, add it to circuits we have to merge
        // cout << "Found a set to merge: " << get<1>(distance) << ", " << get<2>(distance) << " in ";
        // printSet(circuit);
        foundCircuit.push_back(circuit);
      } else {
        // Easiest solution is to just recreate the vector (I am not working with iterators!!)
        newCircuits.push_back(circuit);
      }
    }

    // Merge all found circuits
    set<int> merged = {get<1>(distance), get<2>(distance)};
    for (set<int> fc: foundCircuit) {
      merged.insert(fc.begin(), fc.end());
    }
    if (merged.size() == INPUT_SIZE) {
        cout << "Exceeded max connections with size: " << merged.size() << endl;
        lastConnection = {get<3>(distance), get<4>(distance)};
        break;
    }
    newCircuits.push_back(merged);
    circuits = newCircuits;
  }

  uint64_t result = uint64_t(get<0>(lastConnection) * get<1>(lastConnection));
  cout << lastConnection.first << ", " << lastConnection.second << endl;

  return result;
}