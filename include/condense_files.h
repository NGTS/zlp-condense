#ifndef CONDENSE_FILES_H_
#define CONDENSE_FILES_H_

#include <string>
#include <memory>
#include <vector>

#include "filelist.h"

class FileCondenser {
    public:
        FileCondenser(const std::string &filelist_name);
        void render(const std::string &output);

    private:
        std::string _filelist_name;
        long _nfiles, _napertures;
        std::unique_ptr<FileList> _filelist;

        std::vector<double> _mjd_arr, _flux_arr, _skybkg_arr;

        void load_data();
        void read_file(const std::string &fname, long index);
        void initialize();
};


#endif //  CONDENSE_FILES_H_
