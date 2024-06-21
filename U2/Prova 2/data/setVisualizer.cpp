#include <iostream>
#include <fstream>
#include <string>
#include <set>
using namespace std;

int main() {
    ifstream rawFile("raw.txt");
    ofstream setFile("set.txt");
    set<string> set;

    if (!rawFile.is_open()) { cout << "erro em abrir o arquivo" << endl; exit(1); }

    string line;
    while (getline(rawFile, line)) set.insert(line);
    rawFile.close();

    for (const string& str : set) setFile << str << endl;
    return 0;
}