#include "condense.h"
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include "fits_file.h"

using namespace std;

namespace {
    void removeNewline(string &s) {
        s.erase(remove(s.begin(), s.end(), '\n'), s.end());
    }
}

Condenser::Condenser(const string &filename)
    : filename_(filename), outputFile_(NULL) {
    ifstream infile(filename.c_str());
    string tmp;
    while (infile >> tmp) {
        removeNewline(tmp);
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

    for (auto input_filename : filenames_) {
        cout << "Extracting from " << input_filename << endl;
        FITSFile source(input_filename);
        /* source.toHDU("APM-BINARYTABLE"); */
    }
}

void Condenser::initialiseOutputFile() {
    vector<string> imagelistNames;
    imagelistNames.push_back("HJD");
    imagelistNames.push_back("FLUX");
    imagelistNames.push_back("FLUXERR");
    imagelistNames.push_back("CCDX");
    imagelistNames.push_back("CCDY");
    imagelistNames.push_back("SKYBKG");

    for (auto name : imagelistNames) {
        outputFile_->addImage(name, nimages_, napertures_);
    }

    vector<pair<string, int>> imagelistColumns;
    imagelistColumns.push_back(pair<string, int>("TMID", TDOUBLE));

    vector<pair<string, int>> catalogueColumns;
    catalogueColumns.push_back(pair<string, int>("RA", TDOUBLE));
    catalogueColumns.push_back(pair<string, int>("DEC", TDOUBLE));

    outputFile_->addTable("IMAGELIST", imagelistColumns, nimages_);
    outputFile_->addTable("CATALOGUE", catalogueColumns, napertures_);
}
