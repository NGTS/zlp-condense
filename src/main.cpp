#include <iostream>
#include <vector>
#include <string>

#include <tclap/CmdLine.h>

#include "condense.h"

int main(int argc, char *argv[]) {

    try {

        TCLAP::CmdLine cmd("ZLP condense", ' ', "0.1");
        TCLAP::ValueArg<std::string> output_arg(
            "o", "output", "Output file name", true, "", "file", cmd);
        TCLAP::UnlabeledValueArg<std::string> filelist_arg(
            "filelist", "Files to condense", true, "", "file", cmd);
        cmd.parse(argc, argv);

        Condenser(filelist_arg.getValue()).render(output_arg.getValue());

        return 0;

    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId()
                  << std::endl;
        return 1;
    }
}

