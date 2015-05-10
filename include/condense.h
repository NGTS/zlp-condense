#ifndef CONDENSE_H

#define CONDENSE_H

#include <vector>
#include <string>

class Condenser {
  public:
    Condenser(const std::vector<std::string> &files);
    void render(const std::string &output_filename);

  private:
    std::vector<std::string> files;
};

#endif /* end of include guard: CONDENSE_H */
