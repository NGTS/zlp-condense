#include "condense.h"
#include <iostream>
#include <memory>

#include <fits++.h>

using namespace std;
using namespace fitspp;

typedef auto_ptr<FITSFile> pFITSFile;

namespace {
    pFITSFile openFitsFile(const string &filename) {
        return auto_ptr<FITSFile>(FITSFile::openFile(filename));
    }

    pFITSFile createFitsFile(const string &filename) {
        return auto_ptr<FITSFile>(FITSFile::createFile(filename));
    }
}

Condenser::Condenser(const vector<string> &files)
    : files(files), napertures(0), nimages(0) {
    cout << "Number of files: " << files.size() << endl;
}

void Condenser::compute_output_file_dimensions() {
    nimages = files.size();
    pFITSFile first = openFitsFile(this->files[0]);
    FITSBinaryTable *data_table = first->findBinaryTable("apm-binarytable");
    napertures = data_table->getNumRows();

    cout << "Found " << napertures << " apertures and " << nimages << " images"
         << endl;
}

void Condenser::render(const string &output_filename) {
    cout << "Rendering to " << output_filename << endl;
    compute_output_file_dimensions();

    pFITSFile output = createFitsFile(output_filename);
    output->closeFile();
}
