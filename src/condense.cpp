#include "condense.h"
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <cmath>
#include "fits_file.h"

using namespace std;

namespace {
void removeNewline(string &s) {
  s.erase(remove(s.begin(), s.end(), '\n'), s.end());
}
double NULLVAL = NAN;
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
    delete outputFile_;
}

void Condenser::render(const string &filename) {
  outputFile_ = FITSFile::createFile(filename);
  initialiseOutputFile();

  for (int i = 0; i < filenames_.size(); i++) {
    string input_filename = filenames_[i];
    cout << "Extracting from " << input_filename << endl;
    FITSFile source(input_filename);
    source.toHDU("APM-BINARYTABLE");

    addToImage(source, "FLUX", "Aper_flux_3", i);
  }
}

void Condenser::initialiseOutputFile() {
  vector<string> imageNames;
  imageNames.push_back("HJD");
  imageNames.push_back("FLUX");
  imageNames.push_back("FLUXERR");
  imageNames.push_back("CCDX");
  imageNames.push_back("CCDY");
  imageNames.push_back("SKYBKG");

  for (auto name : imageNames) {
    outputFile_->addImage(name, nimages_, napertures_);
  }

  vector<pair<string, int> > imagelistColumns;
  imagelistColumns.push_back(pair<string, int>("TMID", TDOUBLE));

  vector<pair<string, int> > catalogueColumns;
  catalogueColumns.push_back(pair<string, int>("RA", TDOUBLE));
  catalogueColumns.push_back(pair<string, int>("DEC", TDOUBLE));

  outputFile_->addTable("IMAGELIST", imagelistColumns, nimages_);
  outputFile_->addTable("CATALOGUE", catalogueColumns, napertures_);
}

void Condenser::addToImage(FITSFile &source, const string &hduname,
                           const string &column_name, long image) {
  source.toHDU("APM-BINARYTABLE");
  vector<double> data(napertures_);

  int colnum = -1;
  fits_get_colnum(source.fptr_, CASEINSEN, (char *)column_name.c_str(), &colnum,
                  &source.status_);
  source.check();
  int anynull = 0;
  fits_read_col(source.fptr_, TDOUBLE, colnum, 1, 1, napertures_, &NULLVAL,
                &data[0], &anynull, &source.status_);
  source.check();

  outputFile_->toHDU(hduname);
  long fpixel[] = { image + 1, 1 };
  long lpixel[] = { image + 1, napertures_ + 1 };
  fits_write_subset(outputFile_->fptr_, TDOUBLE, fpixel, lpixel, &data[0],
                    &outputFile_->status_);
  outputFile_->check();
}
