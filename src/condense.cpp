#include <iostream>
#include <fstream>
#include "condense.h"
#include "source_file.h"

using namespace std;
using namespace fitspp;

namespace {
string imageHDUNames[] = { "HJD", "FLUX", "FLUXERR", "CCDX", "CCDY", "SKYBKG" };
}

Condenser::Condenser(const string &filelist)
    : napertures(0), nimages(0) {
    ifstream infile(filelist.c_str());
    string name;
    while (infile >> name) {
        files.push_back(name);
    }
    cout << "Number of files: " << files.size() << endl;
}

void Condenser::compute_output_file_dimensions() {
  nimages = files.size();
  auto first = FITSFile::openFile(this->files[0]);
  FITSBinaryTable *data_table = first->findBinaryTable("apm-binarytable");
  napertures = data_table->getNumRows();

  cout << "Found " << napertures << " apertures and " << nimages << " images"
       << endl;
}

void Condenser::render(const string &output_filename) {
  cout << "Rendering to " << output_filename << endl;
  compute_output_file_dimensions();

  auto output = FITSFile::createFile(output_filename);
  initialiseOutputFile(output);

  for (int i = 0; i < nimages; i++) {
    auto name = files[i];
    auto source_file = SourceFile(name);

    source_file.addMJD(images["HJD"], i, napertures);
    source_file.addData("Aper_flux_3", images["FLUX"], i, napertures);
    source_file.addData("Aper_flux_3_err", images["FLUXERR"], i, napertures);
    source_file.addData("X_coordinate", images["CCDX"], i, napertures);
    source_file.addData("Y_coordinate", images["CCDY"], i, napertures);
    source_file.addData("Sky_level", images["SKYBKG"], i, napertures);

    imagelist.addFromSourceFile(source_file);
  }

  imagelist.render();
  catalogue.render(SourceFile(files[0]));

  output->closeFile();
}

void Condenser::initialiseOutputFile(FITSFile *f) {
  f->addEmptyPrimary();
  for (auto name : imageHDUNames) {
    long axes[] = { nimages, napertures };
    auto image = f->addImage(name, doubleImg, 2, axes);
    images.insert(pair<string, FITSImage *>(name, image));
  }

  imagelist.initialise(f, "imagelist_columns.json", nimages);
  catalogue.initialise(f, "catalogue_columns.json", napertures);
}
