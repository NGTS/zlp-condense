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
    nimages = files.size();
    FITSFile *first = FITSFile::openFile(this->files[0]);
    FITSBinaryTable *data_table = first->findBinaryTable("apm-binarytable");
    napertures = data_table->getNumRows();

    cout << "Found " << napertures << " apertures and " << nimages << " images"
         << endl;
}
