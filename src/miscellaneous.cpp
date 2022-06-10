#include "miscellaneous.h"

void printUsage() {
    printf("very nice\n");
}

void parseOptions(int argc, char* argv[]) {
    cxxopts::Options options("tourGenerator", "Generates interesting tours");

    options.add_options()
        ("a, algorithm", "Algorithm Option", cxxopts::value<std::string>()->default_value("algorithm"))
        ("d, deploy", "Deploy the server")
        ("t, test", "Test an example instance")
        ("g, gpx", "Output file as gpx file")
        ("f, filename", "input file", cxxopts::value<std::string>()->default_value("1kArbeit"));
        // ("t", "tmp_dictionary", cxxopts::value<std::string>()->default_value("D:/GIT/C++/geowordle_core_cmake"))

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        printUsage();
        exit(0);
    }

    if (result.count("deploy"))
        deploy = true;

    if (result.count("test"))
        test = true;

    if (result.count("gpx"))
        gpx = true;

    algoType = result["a"].as<std::string>();
}