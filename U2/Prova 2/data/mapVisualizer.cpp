#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

bool greaterThan(const pair<string, int>& a, const pair<string, int>& b) {
    if(a.second == b.second) return (a.first < b.first);
    return (a.second > b.second);
}

int main() {
    map<string, int> mapCount;
    ifstream rawFile("raw.txt");
    ofstream mapFile("map.txt");

    if (!rawFile.is_open()) { cerr << "Erro ao abrir o arquivo" << endl; exit(1); }

    string line;
    while (getline(rawFile, line)) mapCount[line]++;
    rawFile.close();

    vector<pair<string, int>> vectorCount(mapCount.begin(), mapCount.end());
    sort(vectorCount.begin(), vectorCount.end(), greaterThan);

    for (const auto& curr : vectorCount) {
        mapFile << curr.second << " - " << curr.first << endl;
    }
    return 0;
}
