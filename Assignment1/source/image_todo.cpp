#include "image.h"
#include "line_segments.h"
#include <Eigen/Dense>
#include <vector>

void ImageProcessor::BeierNeelyMorph(
    const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &source,
    const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &destination,
    const Image::OrientedLineSegmentPairs &olsp,
    float timeStep,
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &result) const
{
    // Students will implement the Beier-Neely Morphing algorithm here
    Image::OrientedLineSegmentPairs destinationShape;
    Image::OrientedLineSegmentPairs destinationShape2;
    for (const auto &pair : olsp)
    {
        Image::OrientedLineSegment interpolatedLine =
            pair.first * (1.0f - timeStep) + pair.second * timeStep;

        destinationShape.emplace_back(pair.first, interpolatedLine);
        destinationShape2.emplace_back(pair.second, interpolatedLine);
    }

    // Warp source image to destination shape
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> warpedA;
    warp(source, destinationShape, warpedA);

    // Warp destination image to destination shape
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> warpedB;
    warp(destination, destinationShape2, warpedB);

    CrossDissolve(warpedA, warpedB, timeStep, result);
}

void ImageProcessor::warp(
    const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &source,
    const Image::OrientedLineSegmentPairs &olsp,
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &result) const
{
    // Students will implement the warp function here
    result.resize(source.rows(), source.cols());

    int rows = source.rows();
    int cols = source.cols();

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            Eigen::Vector2d destination(x, y);

            Eigen::Vector2d sourcePos = olsp.getSourcePosition(destination);

            if (sourcePos.x() < 0 || sourcePos.x() >= cols || sourcePos.y() < 0 || sourcePos.y() >= rows)
            {
                result(y, x) = 255;
                continue;
            }

            int source_y = static_cast<int>(std::floor(sourcePos.y()));
            int source_x = static_cast<int>(std::floor(sourcePos.x()));
            result(y, x) = source(source_y, source_x);
        }
    }
}

void ImageProcessor::CrossDissolve(
    const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &source,
    const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &destination,
    float timeStep,
    Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> &result) const
{
    // Students will implement the Cross Dissolve function here
    int rows = source.rows();
    int cols = source.cols();
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            float blended = (1.0f - timeStep) * static_cast<float>(source(y, x)) +
                            timeStep * static_cast<float>(destination(y, x));

            result(y, x) = static_cast<unsigned char>(std::round(std::clamp(blended, 0.0f, 255.0f)));
        }
    }
}
