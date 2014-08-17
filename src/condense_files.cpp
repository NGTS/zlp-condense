#include "condense_files.h"

#include <fstream>
#include <iostream>

using namespace std;

namespace {
    long get_filelist_length(const string &filelist) {
        long counter = 0;
        ifstream infile(filelist);
        if (infile.is_open()) {
            for (string line; getline(infile, line);) {
                counter++;
            }
        }
        infile.close();
        return counter;
    }
}

FileCondenser::FileCondenser(const string &filelist)
: _filelist(filelist) {
    _nfiles = get_filelist_length(_filelist);
    cout << "Working with " << _nfiles << " files" << endl;
}

void FileCondenser::render(const string &output) {
    load_data();
}

void FileCondenser::load_data() {
}
