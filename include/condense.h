#ifndef CONDENSE_H

#define CONDENSE_H

#include <vector>
#include <string>
#include <memory>
#include <map>

#include <fits++.h>

#include "table.h"

class Condenser {
  public:
    Condenser(const std::vector<std::string> &files);
    void render(const std::string &output_filename);

  private:
    void compute_output_file_dimensions();
    void initialiseOutputFile(unique_ptr<fitspp::FITSFile> &f);

    std::vector<std::string> files;
    long napertures, nimages;
    std::map<std::string, fitspp::FITSImage*> images;
    ImageList imagelist;
    Catalogue catalogue;
};

#endif /* end of include guard: CONDENSE_H */
