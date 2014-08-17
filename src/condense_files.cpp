#include "condense_files.h"

#include <fstream>
#include <iostream>

#include "filelist.h"

using namespace std;


FileCondenser::FileCondenser(const string &filelist_name)
: _filelist_name(filelist_name) {
}

void FileCondenser::render(const string &output) {
    load_data();
}

void FileCondenser::load_data() {
    _filelist.reset(new FileList(_filelist_name));
    _filelist->parse();

    _nfiles = _filelist->nfiles();
    _napertures = _filelist->napertures();

    cout << "Condensing " << _nfiles << " files" << endl;
    cout << "Condensing " << _napertures << " apertures" << endl;
}
