#include <iostream>
#include <fitsio.h>
#include <fits++.h>
#include <tclap/CmdLine.h>

int main(int argc, char *argv[]) {

    try {

        TCLAP::CmdLine cmd("ZLP condense", ' ', "0.1");
        TCLAP::ValueArg<std::string> output_arg(
            "o", "output", "Output file name", true, "", "file", cmd);
        TCLAP::UnlabeledMultiArg<std::string> multi(
            "files", "Files to condense", true, "file", cmd);
        cmd.parse(argc, argv);

        return 0;

    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId()
                  << std::endl;
        return 1;
    }
}
