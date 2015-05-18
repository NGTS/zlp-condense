#include "condense.h"
#include "fits_file.h"

using namespace std;

Condenser::Condenser(const string &filename)
    : filename_(filename), outputFile_(NULL) {}

Condenser::~Condenser() {
    if (outputFile_) {
        delete outputFile_;
    }
}

void Condenser::render(const string &filename) {
    outputFile_ = FITSFile::createFile(filename);
}
