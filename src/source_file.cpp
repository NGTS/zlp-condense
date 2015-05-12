#include "source_file.h"
#include <iostream>
#include <algorithm>

using namespace fitspp;
using namespace std;

namespace {
struct AddVal {
    double val;
    AddVal(double v) : val(v) {}

    void operator()(double &elem) const { elem += val; }
};
}

SourceFile::SourceFile(const string &filename)
    : f(FITSFile::openFile(filename)), filename(filename), nullval(NAN) {
    table = f->findBinaryTable("APM-BINARYTABLE");
}

SourceFile::~SourceFile() {
    if (f) {
        delete f;
    }
}

void SourceFile::addData(const string &source_key, FITSImage *imageHDU,
                         int image, int napertures) {
    cout << "Writing data from " << filename << ":" << source_key << endl;
    vector<double> data = readData(source_key, napertures);
    writeData(data, imageHDU, image, napertures);
}

void SourceFile::addMJD(FITSImage *imageHDU, int image, int napertures) {
    cout << "Writing MJD data from " << filename << endl;
    FITSImage *primary = f->findPrimary();
    double mjd = primary->readKeyword("MJD");
    vector<double> hjdCorrection = readData("hjd_correction", napertures);
    for_each(hjdCorrection.begin(), hjdCorrection.end(), AddVal(mjd));
    writeData(hjdCorrection, imageHDU, image, napertures);
}

vector<double> SourceFile::readData(const string &source_key, int napertures) {
    vector<double> data(napertures);
    table->readColumnNull(source_key, &data[0], nullval);
    return data;
}

void SourceFile::writeData(vector<double> &data, FITSImage *imageHDU,
                           int image, int napertures) {
    long fpixel[] = {image + 1, 1};
    long lpixel[] = {image + 1, napertures + 1};
    imageHDU->writeImageSubset(data, fpixel, lpixel);
}
