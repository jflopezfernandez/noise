#include "noise.hpp"

namespace Options = boost::program_options;

int main(int argc, char *argv[])
{
    // Configuration options
    size_t points = 0;

    double position  = 0.0000;
    double increment = 0.0314;

    bool noisy = false;

    double noise_range = 0.20;

    Options::options_description generic("Generic Options");
    generic.add_options()
        ("help", "Display this help menu and exit.")
        ("version", "Display program version information and exit.")
    ;

    Options::options_description exec("Execution Options");
    exec.add_options()
        ("verbose,v", "Display detailed information during program execution")
    ;

    Options::options_description config("Configuration Options");
    config.add_options()
        ("points", Options::value<size_t>(&points)->default_value(100), "Number of points of the wave to generate")
        ("intial-position", Options::value<double>()->default_value(0.0), "Initial position of the graph")
        ("increment", Options::value<double>()->default_value(0.031400), "Change in x-value from point to point")
        ("noisy", "Include random noise in the function output")
        ("noise-range", Options::value<double>(&noise_range)->default_value(0.20), "Noise range should be plus/minus this value")
    ;

    Options::options_description desc("Program Options");
    desc
        .add(generic)
        .add(exec)
        .add(config);
    
    Options::positional_options_description pos;
    pos.add("points", -1);

    Options::variables_map vm;
    Options::store(
        Options::command_line_parser(argc, argv)
            .options(desc)
            .positional(pos)
            .run(),
        vm);
    Options::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;

        return EXIT_SUCCESS;
    }

    if (vm.count("version")) {
        std::cout << "<Program Version Info>" << std::endl;

        return EXIT_SUCCESS;
    }

    if (vm.count("noisy")) {
        noisy = true;
    }

    // Initialize random number generation apparatus
    std::random_device entropy;
    std::mt19937_64 generator(entropy());
    std::uniform_real_distribution dist(-(noise_range), (noise_range));

    for (size_t i = 0; i <= points; ++i) {
        // Get position coordinates
        double x_position = position;
        double y_position = sin(position);

        // If generating noisy wave, incorporate noise here
        if (noisy) {
            // Generate random noise value
            const double noise = dist(generator);

            // Add it to the current y position
            y_position += noise;
        }

        // Print current position
        std::cout << x_position << " " << y_position << std::endl;

        // Move position by the increment value
        position += increment;
    }

    return EXIT_SUCCESS;
}
