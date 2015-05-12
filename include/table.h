#ifndef TABLE_H

#define TABLE_H

#include <string>
#include <fits++.h>
#include <memory>
#include <vector>
#include <map>
#include <set>

class Table {
  public:
    Table(const std::string &tablename);

    void initialise(std::auto_ptr<fitspp::FITSFile> &f,
                    const std::string &filename, long size);

    void addDouble(const std::string &name, double value);
    void addLong(const std::string &name, long value);
    void addInt(const std::string &name, int value);

  private:
    std::string tablename;
    std::set<std::string> colNames;
    std::map<std::string, std::vector<double> > doubleMap;
    std::map<std::string, std::vector<int> > intMap;
    std::map<std::string, std::vector<long> > longMap;
};

class ImageList : public Table {
  public:
    ImageList() : Table("IMAGELIST") {}
};

class Catalogue : public Table {
  public:
    Catalogue() : Table("CATALOGUE") {}
};

#endif /* end of include guard: TABLE_H */
