#include "condense.h"
#include <iostream>

#include <fits++.h>

using namespace std;
using namespace fitspp;

Condenser::Condenser(const vector<string> &files)
    : files(files), napertures(0), nimages(0) {
    cout << "Number of files: " << files.size() << endl;
}

void Condenser::render(const string &output_filename) {
    cout << "Rendering to " << output_filename << endl;
    compute_output_file_dimensions();
}

void Condenser::compute_output_file_dimensions() {
    auto first = FITSFile::openFile(this->files[0]);
}
