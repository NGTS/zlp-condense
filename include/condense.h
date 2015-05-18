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
  void initialiseOutputFile();
  void addToImage(FITSFile &source, const std::string &hduname,
                  const std::string &column_name, long image);

  std::string filename_;
  FITSFile *outputFile_;
  long nimages_, napertures_;
  std::vector<std::string> filenames_;
};

#endif /* end of include guard: CONDENSE_H */
