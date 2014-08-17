#ifndef CONDENSE_FILES_H_
#define CONDENSE_FILES_H_

#include <string>

class FileCondenser {
    public:
        FileCondenser(const std::string &filelist);
        void render(const std::string &output);
};


#endif //  CONDENSE_FILES_H_
