#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <filesystem>
#include <igl/stb/read_image.h>
#include <igl/stb/write_image.h>
#include "image.h"
#include "line_segments.h"
#include <iomanip>

namespace fs = std::filesystem; // Alias for easier reference to filesystem

int main(int argc, char *argv[])
{
    using namespace Eigen;
    using namespace std;
    using namespace Image;

    // Parse command line inputs
    string input_file;
    string dest_file;
    string dest_folder;
    const char *yellowColor = "\033[33m";
    const char *resetColor = "\033[0m";

    for (int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);
        if (arg == "--in" && i + 1 < argc)
        {
            input_file = argv[++i];
        }
        else if (arg == "--dest" && i + 1 < argc)
        {
            dest_file = argv[++i];
        }
        else if (arg == "--out" && i + 1 < argc)
        {
            dest_folder = argv[++i];
        }
        else
        {
            std::cerr << yellowColor << "Invalid argument: " << arg << resetColor << std::endl;
            return 1;
        }
    }

    if (input_file.empty() || dest_file.empty() || dest_folder.empty())
    {
        std::cerr << yellowColor << "Usage: " << argv[0] << " --in input.jpg --dest destination.jpg --out destination_folder" << resetColor << std::endl;
        return 1;
    }

    // Ensure destination folder exists, or create it
    if (!fs::exists(dest_folder))
    {
        if (!fs::create_directories(dest_folder))
        {
            std::cerr << yellowColor << "Error: Failed to create destination folder " << dest_folder << resetColor << std::endl;
            return 1;
        }
    }

    // Read source image
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> source_R, source_G, source_B, source_Alpha;
    if (!igl::stb::read_image(input_file, source_R, source_G, source_B, source_Alpha))
    {
        std::cerr << yellowColor << "Error: Failed to read the source image from " << input_file << resetColor << std::endl;
        return 1;
    }

    // Read destination image
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> destination_R, destination_G, destination_B, destination_Alpha;
    if (!igl::stb::read_image(dest_file, destination_R, destination_G, destination_B, destination_Alpha))
    {
        std::cerr << yellowColor << "Error: Failed to read the destination image from " << dest_file << resetColor << std::endl;
        return 1;
    }

    // TODO: Define the line segment pairs for morphing, these numbers are placeholders
    OrientedLineSegmentPairs olsp;
    const Eigen::MatrixXd F_src = (Eigen::MatrixXd(11, 4) <<
                                       // For circle to triangle
                                       // 50, 150, 50, 50,
                                       //    150, 150, 150, 50,
                                       //    50, 150, 150, 150
                                       // For F to F_rotated
                                       //    232, 178, 232, 281,
                                       //    232, 281, 145, 271,
                                       //    145, 271, 84, 178,
                                       //    84, 178, 232, 178
                                       // For CA to CS
                                       287,
                                   101, 245, 103,
                                   267, 86, 267, 123,
                                   245, 103, 141, 96,
                                   278, 53, 227, 18,
                                   227, 18, 222, 84,
                                   218, 120, 177, 169,
                                   177, 169, 117, 139,
                                   141, 96, 69, 75,
                                   69, 75, 33, 90,
                                   141, 96, 69, 124,
                                   69, 124, 33, 133)
                                      .finished();
    const Eigen::MatrixXd F_dest = (Eigen::MatrixXd(11, 4) <<
                                        // For circle to triangle
                                        // 50, 150, 100, 50,
                                        // 150, 150, 100, 50,
                                        // 50, 150, 150, 150
                                        // For F to F_rotated
                                        // 159, 23, 250, 76,
                                        // 250, 76, 196, 147,
                                        // 196, 147, 84, 151,
                                        // 84, 151, 159, 23
                                        // For CA to CS
                                        288,
                                    100, 235, 100,
                                    264, 85, 264, 124,
                                    235, 100, 108, 87,
                                    298, 63, 244, 17,
                                    244, 17, 207, 80,
                                    207, 121, 184, 182,
                                    184, 182, 143, 131,
                                    108, 87, 60, 55,
                                    60, 55, 26, 63,
                                    108, 87, 56, 128,
                                    56, 128, 18, 141)
                                       .finished();
    Eigen::MatrixXd src = F_src, dst = F_dest;
    for (int i = 0; i < src.rows(); ++i)
    {
        Eigen::Vector2d p1_src(src(i, 0), src(i, 1)), p2_src(src(i, 2), src(i, 3));
        Eigen::Vector2d p1_dst(dst(i, 0), dst(i, 1)), p2_dst(dst(i, 2), dst(i, 3));
        olsp.push_back(std::make_pair(OrientedLineSegment(p1_src, p2_src), OrientedLineSegment(p1_dst, p2_dst)));
    }

    // Prepare image processor
    ImageProcessor imgProcessor;

    // Loop over 30 frames, saving each one
    for (int FrameNum = 0; FrameNum <= 30; ++FrameNum)
    {
        float timeStep = FrameNum / 30.0f;

        // Perform morphing for each color channel
        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> morphed_R = source_R;
        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> morphed_G = source_G;
        Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> morphed_B = source_B;

        imgProcessor.BeierNeelyMorph(source_R, destination_R, olsp, timeStep, morphed_R);
        imgProcessor.BeierNeelyMorph(source_G, destination_G, olsp, timeStep, morphed_G);
        imgProcessor.BeierNeelyMorph(source_B, destination_B, olsp, timeStep, morphed_B);

        // Generate output filename based on time step
        stringstream output_path;
        output_path << dest_folder << "/output_" << setw(2) << setfill('0') << FrameNum << ".jpg"; // "output_00.jpg", "output_01.jpg", etc.

        if (!igl::stb::write_image(output_path.str(), morphed_R, morphed_G, morphed_B, source_Alpha))
        {
            std::cerr << yellowColor << "Error: Failed to write the output image to " << output_path.str() << resetColor << std::endl;
            return 1;
        }

        std::cout << "Morphing result saved to: " << output_path.str() << std::endl;
    }

    return 0;
}
