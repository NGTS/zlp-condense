#ifndef CONDENSE_H

#define CONDENSE_H

#include <string>
#include <vector>
#include "fits_file.h"

class Condenser {
  public:
    Condenser(const std::string &filename);
    ~Condenser();

    void render(const std::string &filename);

  private:
    std::string filename_;
    FITSFile *outputFile_;
    long nimages_, napertures_;
    std::vector<std::string> filenames_;
};

#endif /* end of include guard: CONDENSE_H */
