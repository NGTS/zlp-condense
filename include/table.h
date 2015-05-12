#ifndef TABLE_H

#define TABLE_H

#include <string>
#include <fits++.h>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include "source_file.h"

class Table {
  public:
    Table(const std::string &tablename);

    void initialise(fitspp::FITSFile *f,
                    const std::string &filename, long size);

    void add(const std::string &name, double value);
    void add(const std::string &name, long value);
    void add(const std::string &name, int value);
    void render();
    void addFromSourceFile(const SourceFile &s);

  protected:
    std::set<std::string> colNames;
    fitspp::FITSBinaryTable *table;

  private:
    std::string tablename;
    std::map<std::string, std::string> typeMap;
    std::map<std::string, std::vector<double>> doubleMap;
    std::map<std::string, std::vector<int>> intMap;
    std::map<std::string, std::vector<long>> longMap;
};

class ImageList : public Table {
  public:
    ImageList() : Table("IMAGELIST") {}
};

class Catalogue : public Table {
  public:
    Catalogue() : Table("CATALOGUE") {}
    void render(const SourceFile &s);
};

#endif /* end of include guard: TABLE_H */
