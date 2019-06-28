#include <iostream>
#include <clocale>
#include <string>

#include "radix_tree.h"

using namespace std;
using namespace griha;

int main() {
    wcout.imbue(locale(setlocale(LC_ALL, "")));

    radix_tree<wchar_t> nicknames;
    string line;
    while (getline(cin, line)) {
        nicknames.insert(static_cast<string_view>(line));
    }

    for (auto& nickname : nicknames) {
        wcout << nickname.first << L' ' << nickname.second << endl;
    }

    wcout << endl;
    wcout << nicknames;

    return 0;
}