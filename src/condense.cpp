#include "condense.h"
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "fits_file.h"

using namespace std;

Condenser::Condenser(const string &filename)
    : filename_(filename), outputFile_(NULL) {
    ifstream infile(filename);
    string tmp;
    while (infile >> tmp) {
        filenames_.push_back(tmp);
    }
    nimages_ = filenames_.size();

    /* Get image dimensions */
    FITSFile first(filenames_[0]);
    first.toHDU("APM-BINARYTABLE");
    fits_get_num_rows(first.fptr_, &napertures_, &first.status_);
    first.check();

    cout << "Number of images: " << nimages_ << endl;
    cout << "Number of apertures: " << napertures_ << endl;
}

Condenser::~Condenser() {
    if (outputFile_) {
        delete outputFile_;
    }
}

void Condenser::render(const string &filename) {
    outputFile_ = FITSFile::createFile(filename);
    initialiseOutputFile();
}

void Condenser::initialiseOutputFile() {
    const vector<string> imagelistNames = {
        "HJD", "FLUX", "FLUXERR", "CCDX", "CCDY", "SKYBKG",
    };
    for (auto name : imagelistNames) {
        outputFile_->addImage(name, nimages_, napertures_);
    }

    const vector<pair<string, int>> imagelistColumns = {
        {"TMID", TDOUBLE},
    };

    const vector<pair<string, int>> catalogueColumns = {
        {"RA", TDOUBLE}, {"DEC", TDOUBLE},
    };

    outputFile_->addTable("IMAGELIST", imagelistColumns, nimages_);
    outputFile_->addTable("CATALOGUE", catalogueColumns, napertures_);
}
