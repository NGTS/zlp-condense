#include <iostream>
#include "condense.h"
#include "source_file.h"

using namespace std;
using namespace fitspp;

namespace {
auto_ptr<FITSFile> openFitsFile(const string &filename) {
    return auto_ptr<FITSFile>(FITSFile::openFile(filename));
}

auto_ptr<FITSFile> createFitsFile(const string &filename) {
    return auto_ptr<FITSFile>(FITSFile::createFile(filename));
}

vector<string> imageHDUNames = {"HJD",  "FLUX", "FLUXERR",
                                "CCDX", "CCDY", "SKYBKG"};
}

Condenser::Condenser(const vector<string> &files)
    : files(files), napertures(0), nimages(0) {
    cout << "Number of files: " << files.size() << endl;
}

void Condenser::compute_output_file_dimensions() {
    nimages = files.size();
    auto_ptr<FITSFile> first = openFitsFile(this->files[0]);
    FITSBinaryTable *data_table = first->findBinaryTable("apm-binarytable");
    napertures = data_table->getNumRows();

    cout << "Found " << napertures << " apertures and " << nimages << " images"
         << endl;
}

void Condenser::render(const string &output_filename) {
    cout << "Rendering to " << output_filename << endl;
    compute_output_file_dimensions();

    auto_ptr<FITSFile> output = createFitsFile(output_filename);
    initialiseOutputFile(output);

    for (int i = 0; i < nimages; i++) {
        auto name = files[i];
        auto source_file = SourceFile(name);

        source_file.addMJD(images["HJD"], i, napertures);
        source_file.addData("Aper_flux_3", images["FLUX"], i, napertures);
        source_file.addData("Aper_flux_3_err", images["FLUXERR"], i,
                            napertures);
        source_file.addData("X_coordinate", images["CCDX"], i, napertures);
        source_file.addData("Y_coordinate", images["CCDY"], i, napertures);
        source_file.addData("Sky_level", images["SKYBKG"], i, napertures);

        imagelist.addFromSourceFile(source_file);
    }

    imagelist.render();

    output->closeFile();
}

void Condenser::initialiseOutputFile(auto_ptr<FITSFile> &f) {
    f->addEmptyPrimary();
    for (auto name : imageHDUNames) {
        long axes[] = {nimages, napertures};
        auto image = f->addImage(name, doubleImg, 2, axes);
        images.insert(pair<string, FITSImage *>(name, image));
    }

    imagelist.initialise(f, "imagelist_columns.json", nimages);
    catalogue.initialise(f, "catalogue_columns.json", napertures);
}
