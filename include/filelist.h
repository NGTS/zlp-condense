#ifndef FILELIST_H_
#define FILELIST_H_

#include <string>
#include <vector>

class FileList {
    public:
        FileList(const std::string &fname);
        long nfiles();
        long napertures();
        void parse();
        const std::string first() const;

        std::vector<std::string>::iterator begin() { return _files.begin(); }
        std::vector<std::string>::iterator end() { return _files.end(); }

    private:
        std::string _fname;
        std::vector<std::string> _files;
};

#endif // FILELIST_H_
