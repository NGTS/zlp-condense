#include "condense_files.h"

#include <fstream>
#include <iostream>

#include "filelist.h"

using namespace std;


FileCondenser::FileCondenser(const string &filelist)
: _filelist(filelist) {
}

void FileCondenser::render(const string &output) {
    load_data();
}

void FileCondenser::load_data() {
    FileList f(_filelist);
    f.parse();

    _nfiles = f.nfiles();
    _napertures = f.napertures();

    cout << "Condensing " << _nfiles << " files" << endl;
    cout << "Condensing " << _napertures << " apertures" << endl;
}
