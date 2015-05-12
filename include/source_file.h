#ifndef SOURCE_FILE_H

#define SOURCE_FILE_H

#include <string>

#include "fits++.h"

class SourceFile {
  public:
    SourceFile(const std::string &filename);
    ~SourceFile();

    void addData(const std::string &source_key, fitspp::FITSImage *imageHDU,
                 int image, int napertures);
    void addMJD(fitspp::FITSImage *imageHDU, int image, int napertures);
    fitspp::FITSKeyword readKeyword(const std::string &name) const;
    vector<double> readData(const std::string &source_key, int napertures) const;
    vector<double> readData(const std::string &source_key) const;

  private:
    void writeData(vector<double> &data, fitspp::FITSImage *imageHDU, int image,
                   int napertures);

    fitspp::FITSFile *f;
    fitspp::FITSBinaryTable *table;
    std::string filename;
    double nullval;
};

#endif /* end of include guard: SOURCE_FILE_H */
