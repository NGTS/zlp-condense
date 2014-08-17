#ifndef CONDENSE_FILES_H_
#define CONDENSE_FILES_H_

#include <string>
#include <memory>

class FileList;

class FileCondenser {
    public:
        FileCondenser(const std::string &filelist_name);
        void render(const std::string &output);

    private:
        std::string _filelist_name;
        long _nfiles, _napertures;
        std::unique_ptr<FileList> _filelist;

        void load_data();
};


#endif //  CONDENSE_FILES_H_
