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

struct AddVal {
  double val;
  AddVal(double v) : val(v) {}
  void operator()(double &elem) const { elem += val; }
};

double get_mjd(const string &filename) {
  FITSFile source(filename);
  double mjd = -1;
  fits_read_key(source.fptr_, TDOUBLE, "MJD", &mjd, NULL, &source.status_);
  source.check();
  return mjd;
}

}

Condenser::Condenser(const string &filename)
    : filename_(filename), outputFile_(NULL) {
  ifstream infile(filename.c_str());
  string tmp;
  vector<pair<string, double> > sortList;
  while (infile >> tmp) {
    removeNewline(tmp);
    sortList.push_back(pair<string, double>(tmp, get_mjd(tmp)));
  }

  sort(sortList.begin(), sortList.end(),
       [](const pair<string, double> & left,
          const pair<string, double> & right) {
    return left.second < right.second;
  });

  for (auto row : sortList) {
      filenames_.push_back(row.first);
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

Condenser::~Condenser() { delete outputFile_; }

void Condenser::render(const string &filename) {
  outputFile_ = FITSFile::createFile(filename);
  initialiseOutputFile();

  for (int i = 0; i < filenames_.size(); i++) {
    string input_filename = filenames_[i];
    cout << "Extracting from " << input_filename << endl;
    FITSFile source(input_filename);
    source.toHDU(1);
    double mjd = -1;
    fits_read_key(source.fptr_, TDOUBLE, "MJD", &mjd, NULL, &source.status_);
    source.check();

    source.toHDU("APM-BINARYTABLE");
    vector<double> hjd_correction(napertures_);
    int colnum = -1;
    fits_get_colnum(source.fptr_, CASEINSEN, "hjd_correction", &colnum,
                    &source.status_);
    fits_read_col(source.fptr_, TDOUBLE, colnum, 1, 1, napertures_, NULL,
                  &hjd_correction[0], NULL, &source.status_);
    source.check();
    for_each(hjd_correction.begin(), hjd_correction.end(), AddVal(mjd));

    addToImage(hjd_correction, "HJD", i);
    addToImage(source, "FLUX", "Aper_flux_3", i);
    addToImage(source, "FLUXERR", "Aper_flux_3_err", i);
    addToImage(source, "SKYBKG", "Sky_level", i);
    addToImage(source, "CCDX", "X_coordinate", i);
    addToImage(source, "CCDY", "Y_coordinate", i);

    addToImage(source, "FLUX_1", "Aper_flux_2", i);
    addToImage(source, "FLUX_2", "Aper_flux_4", i);
    addToImage(source, "FLUX_3", "Aper_flux_5", i);
    addToImage(source, "FLUX_4", "Aper_flux_6", i);

    addToImage(source, "ERROR_1", "Aper_flux_2_err", i);
    addToImage(source, "ERROR_2", "Aper_flux_4_err", i);
    addToImage(source, "ERROR_3", "Aper_flux_5_err", i);
    addToImage(source, "ERROR_4", "Aper_flux_6_err", i);
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

  imageNames.push_back("FLUX_1");
  imageNames.push_back("FLUX_2");
  imageNames.push_back("FLUX_3");
  imageNames.push_back("FLUX_4");

  imageNames.push_back("ERROR_1");
  imageNames.push_back("ERROR_2");
  imageNames.push_back("ERROR_3");
  imageNames.push_back("ERROR_4");

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

  addToImage(data, hduname, image);
}

void Condenser::addToImage(const vector<double> &data, const string &hduname,
                           long image) {
  outputFile_->toHDU(hduname);
  long fpixel[] = { image + 1, 1 };
  long lpixel[] = { image + 1, napertures_ + 1 };
  fits_write_subset(outputFile_->fptr_, TDOUBLE, fpixel, lpixel,
                    (void *)&data[0], &outputFile_->status_);
  outputFile_->check();
}
