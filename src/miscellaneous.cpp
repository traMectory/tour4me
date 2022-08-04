#include "miscellaneous.h"

void printUsage() {
    printf("\n");
}

void parseOptions(int argc, char* argv[]) {
    cxxopts::Options options("tourGenerator", "Generates interesting tours");

    options.add_options()
        ("h, help", "Display help message")
        ("d, deploy", "Deploy the webserver on port 80 (default deployment on 8080)")
        ("t, test", "Test an example instance");
        // ("t", "tmp_dictionary", cxxopts::value<std::string>()->default_value("D:/GIT/C++/geowordle_core_cmake"))

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {      
        std::cout << options.help() << std::endl;
        exit(0);
    }

    if (result.count("deploy"))
        deploy = true;

    if (result.count("test"))
        test = true;

}