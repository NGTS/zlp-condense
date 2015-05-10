#include "condense.h"
#include <iostream>

using namespace std;

Condenser::Condenser(const vector<string> &files) : files(files) {
    cout << "Number of files: " << files.size() << endl;
}

void Condenser::render(const string &output_filename) {
    cout << "Rendering to " << output_filename << endl;
}
