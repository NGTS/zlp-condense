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

    private:
        std::string _fname;
        std::vector<std::string> _files;
};

#endif // FILELIST_H_
