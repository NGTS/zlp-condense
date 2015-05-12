#include "table.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <rapidjson/document.h>

Table::Table(const string &tablename) : tablename(tablename) {}

#define ASSERT_PRESENT                                                         \
    if (colNames.find(name) == colNames.end()) {                               \
        throw runtime_error("Invalid column name");                            \
    }

void Table::initialise(auto_ptr<fitspp::FITSFile> &f, const string &filename,
                       long size) {
    rapidjson::Document document;
    ifstream in(filename);
    if (in) {
        ostringstream contents;
        contents << in.rdbuf();
        in.close();
        document.Parse(contents.str().c_str());
    }

    stringstream ss;
    auto end = document["columns"].MemberEnd();
    for (auto itr = document["columns"].MemberBegin(); itr != end; ++itr) {
        auto name = itr->name.GetString();
        auto description = itr->value["type"].GetString();

        colNames.insert(name);

        ss << name << "/" << description;
        if (itr != end - 1) {
            ss << " ";
        }
    }
    auto defs = ss.str();
    cout << defs << endl;
    auto table = f->addBinaryTable(tablename, defs, size);
}

void Table::addDouble(const std::string &name, double value) {
    ASSERT_PRESENT;
    doubleMap[name].push_back(value);
}
void Table::addLong(const std::string &name, long value) {
    ASSERT_PRESENT;
    longMap[name].push_back(value);
}
void Table::addInt(const std::string &name, int value) {
    ASSERT_PRESENT;
    intMap[name].push_back(value);
}
