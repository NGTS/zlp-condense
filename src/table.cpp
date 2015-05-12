#include "table.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <rapidjson/document.h>

Table::Table(const string &tablename) : tablename(tablename), table(NULL) {}

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
        typeMap.insert(pair<string, string>(name, description));

        ss << name << "/" << description;
        if (itr != end - 1) {
            ss << " ";
        }
    }
    auto defs = ss.str();
    cout << defs << endl;
    table = f->addBinaryTable(tablename, defs);
}

void Table::add(const std::string &name, double value) {
    ASSERT_PRESENT;
    doubleMap[name].push_back(value);
}
void Table::add(const std::string &name, long value) {
    ASSERT_PRESENT;
    longMap[name].push_back(value);
}
void Table::add(const std::string &name, int value) {
    ASSERT_PRESENT;
    intMap[name].push_back(value);
}

void Table::addFromSourceFile(const SourceFile &s) {
    for (auto type : typeMap) {
        if (type.second == "1D") {
            add(type.first, double(s.readKeyword(type.first)));
        } else if (type.second == "1J") {
            add(type.first, long(s.readKeyword(type.first)));
        } else if (type.second == "1I") {
            add(type.first, int(s.readKeyword(type.first)));
        }
    }
}

void Table::render() {
    for (auto type : typeMap) {
        if (type.second == "1D") {
            table->writeColumn(type.first, doubleMap[type.first], 0);
        } else if (type.second == "1J") {
            table->writeColumn(type.first, longMap[type.first], 0);
        } else if (type.second == "1I") {
            table->writeColumn(type.first, intMap[type.first], 0);
        }
    }
}
