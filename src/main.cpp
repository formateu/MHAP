#include <SequenceSketchStreamer.hpp>
#include <MinHashSearch.hpp>

#include <boost/program_options.hpp>
#include <iostream>

using namespace boost::program_options;

int main(int argc, char **argv) {
    try {

        options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("input_file", value<std::string>(), "file containing reads to be aligned");

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (!vm.count("input_file")) {
            throw std::runtime_error("No input file has been provided!");
        }

        auto longReadsFile = vm["input_file"].as<std::string>();

        std::size_t MinOlapLength = 116;
        std::size_t KmerSize = 16;
        std::size_t NumHashes = 512;
        std::size_t OrderedKmerSize = 12;
        std::size_t OrderedSketchSize = 1536;
        std::size_t NumMinMatches = 3;
        std::size_t MinStoreLength = 0;
        double MaxShift = 0.2;
        double AcceptScore = 0.78;

        SequenceSketchStreamer seqStreamer(longReadsFile,
                                           MinOlapLength,
                                           KmerSize,
                                           NumHashes,
                                           OrderedKmerSize,
                                           OrderedSketchSize,
                                           true,
                                           0.9);

        MinHashSearch hashSearch(seqStreamer,
                                 NumHashes,
                                 NumMinMatches,
//                                 false,
                                 MinStoreLength,
                                 MaxShift,
                                 AcceptScore,
                                 true);

        auto results = hashSearch.findMatches();
        std::string res;

        for (const auto &result : results) {
            std::cout << result.toString() << '\n';
        }

    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';

        return 1;
    } catch (...) {
        std::cerr << "Unknown Exception has occured!\n";

        return 1;
    }

    return 0;
}
