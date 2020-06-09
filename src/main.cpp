#include <docopt.h>
//#include <iostream>
#include "HeaderManager.h"


static const char USAGE2[] =
R"(Usage: onlyoneheader [-ho <file>] [--output=<file>] [--remove_comments] [--no_separators] [--no_timestamp] [--silent] <path>

    -h --help                   show this
    -o <file> --output=<file>   specify output file [default: ./default.h]
    --remove_comments           remove one line comments from the header files
    --no_separators             don't add // <filename> START|END separators
    --no_timestamp              don't add timestamp
    --silent                    no info text
)";
static const char USAGE[] =
        R"(Usage: onlyoneheader [-h] [--output=<file>] [--remove_comments] [--no_separators] [--no_timestamp] [--silent] <path>

    -h --help                   show this
    --output=<file>             specify output file [default: ./allinone.h]
    --remove_comments           remove one line comments from the header files
    --no_separators             don't add // <filename> START|END separators
    --no_timestamp              don't add timestamp
    --silent                    no info text
)";

int main(int argc, char** argv) {

    std::map<std::string, docopt::value> args = docopt::docopt(
        USAGE,
        { argv + 1, argv + argc },
        true, 
        "Only One Header v0.1"
    );

#if 0
    for (auto const& arg : args) {
        std::cout << arg.first << ": " << arg.second << std::endl;
    }
#endif

    OnlyOneHeader::HeaderManager headerManager{args["<path>"].asString()};

    headerManager.setOptionAddSeparatorState(args["--no_separators"].asBool() == false);
    headerManager.setOptionAddTimestampState(args["--no_timestamp"].asBool() == false);
    headerManager.setOptionRemoveCommentsState(args["--remove_comments"].asBool());
    headerManager.setOptionSilentState(args["--silent"].asBool());

    headerManager.process();
    headerManager.output(args["--output"].asString());
}