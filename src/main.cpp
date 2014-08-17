#include <iostream>
#include <memory>
#include <fitsio.h>
#include <tclap/CmdLine.h>

#include "condense_files.h"

int main(int argc, const char *argv[]) {
    try {
        TCLAP::CmdLine cmd("Condense", ' ', "0.0.1");

        TCLAP::UnlabeledValueArg<std::string> filelist_arg("filelist", "List of files to condense", true, "", "filelist", cmd);
        TCLAP::ValueArg<std::string> output_arg("o", "output", "Resulting output file", true, "", "fits file", cmd);

        cmd.parse(argc, argv);

        FileCondenser(filelist_arg.getValue()).render(output_arg.getValue());

        return 0;

    } catch (TCLAP::ArgException &e) {
        std::cerr << "Argument error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }
}
