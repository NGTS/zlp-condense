#ifndef CONDENSE_H

#define CONDENSE_H

#include <vector>
#include <string>

class Condenser {
  public:
    Condenser(const std::vector<std::string> &files);
    void render(const std::string &output_filename);

  private:

    void compute_output_file_dimensions();


    std::vector<std::string> files;
    long napertures, nimages;
};

#endif /* end of include guard: CONDENSE_H */
