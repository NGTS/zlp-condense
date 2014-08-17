#ifndef CONDENSE_FILES_H_
#define CONDENSE_FILES_H_

#include <string>

class FileCondenser {
    public:
        FileCondenser(const std::string &filelist);
        void render(const std::string &output);

    private:
        std::string _filelist;
        long _nfiles, _napertures;

        void load_data();
};


#endif //  CONDENSE_FILES_H_
