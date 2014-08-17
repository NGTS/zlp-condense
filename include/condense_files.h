#ifndef CONDENSE_FILES_H_
#define CONDENSE_FILES_H_

#include <string>
#include <memory>
#include <vector>
#include <fitsio.h>

#include "filelist.h"

struct Catalogue {
    std::vector<std::string> obj_id;
    std::vector<double> mean_flux;
    std::vector<long> npts;

    Catalogue(long n) :
        obj_id(std::vector<std::string>(n)),
        mean_flux(std::vector<double>(n)),
        npts(std::vector<long>(n)) {}
};

struct Imagelist {
    std::vector<double> airmass;

    Imagelist(long n) :
        airmass(std::vector<double>(n)) {}
};

class FileCondenser {
    public:
        FileCondenser(const std::string &filelist_name);
        void render(const std::string &output);

    private:
        std::string _filelist_name;
        long _nfiles, _napertures;
        std::unique_ptr<FileList> _filelist;

        std::vector<double> _mjd_arr, _flux_arr, _skybkg_arr, _ccdx_arr, _ccdy_arr;
        std::unique_ptr<Catalogue> _catalogue;
        std::unique_ptr<Imagelist> _imagelist;

        void load_data();
        void read_file(const std::string &fname, long index);
        void initialize();
};


#endif //  CONDENSE_FILES_H_
