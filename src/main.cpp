#include <iostream>
#include <vector>
#include <string>

#include <tclap/CmdLine.h>
#include <fitsio.h>
#include <fits++.h>

void condense_files(const std::vector<std::string> &files,
        const std::string &output_filename) {
}

int main(int argc, char *argv[]) {

    try {

        TCLAP::CmdLine cmd("ZLP condense", ' ', "0.1");
        TCLAP::ValueArg<std::string> output_arg(
            "o", "output", "Output file name", true, "", "file", cmd);
        TCLAP::UnlabeledMultiArg<std::string> input_files_arg(
            "files", "Files to condense", true, "file", cmd);
        cmd.parse(argc, argv);

        condense_files(input_files_arg.getValue(), output_arg.getValue());

        return 0;

    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId()
                  << std::endl;
        return 1;
    }
}
