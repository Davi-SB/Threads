#include <iostream>
#include <set>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    set<string> unique_lines;
    string line;

    while(getline(cin, line)) {
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        unique_lines.insert(line);
    }

    for (const auto& unique_line : unique_lines) cout << unique_line << endl;
    return 0;
}
