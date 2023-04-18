#pragma once

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "value.hpp"

std::vector<float> arange(float start, float end, float step) {
    std::vector<float> result;
    for (float i = start; i < end; i += step) {
        result.push_back(i);
    }
    return result;
}

std::vector<float> apply_function(const std::vector<float>& input, float (*func)(float)) {
    std::vector<float> result;
    result.reserve(input.size());
    for (const auto& i : input) {
        result.push_back(func(i));
    }
    return result;
}

void plot(const std::vector<float>& x, const std::vector<float>& y, const int plotWidth = 100, const int plotHeight = 25) {
    const char x_axis_char = '_';
    const char y_axis_char = '|';
    const char plotChar = '*';
    const char blankChar = ' ';

    // Find min and max values for both x and y axes
    float minX = *min_element(x.begin(), x.end());
    float maxX = *max_element(x.begin(), x.end());
    float minY = *min_element(y.begin(), y.end());
    float maxY = *max_element(y.begin(), y.end());

    // Calculate the increment for axis labels
    float xIncrement = (maxX - minX) / plotWidth;
    float yIncrement = (maxY - minY) / plotHeight;

    // x-axis labels
    // Determine the number of decimal places needed for x-axis labels
    int xDecimals = 0;
    float xStep = xIncrement;
    while (xStep < 1) {
        xStep *= 10;
        ++xDecimals;
    }

    int numLabels = x.size();
    int labelSpacing = plotWidth / numLabels;

    // Print the plot
    for (int i = plotHeight; i >= 0; --i) {
        for (int j = 0; j <= plotWidth; ++j) {
            // Print y-axis
            if (j == 0) {
                std::cout << std::fixed << std::setprecision(1) << minY + i * yIncrement;
                std::cout << "\t";

                std::cout << y_axis_char;
            }

            bool plotted = false;
            for (size_t k = 0; k < x.size(); ++k) {
                int xCoord = static_cast<int>((x[k] - minX) / xIncrement);
                int yCoord = static_cast<int>((y[k] - minY) / yIncrement);

                if (xCoord == j && yCoord == i) {
                    std::cout << plotChar;
                    plotted = true;
                    break;
                }
            }

            if (!plotted) {
                if (i == 0) {
                    std::cout << x_axis_char;  // x-axis
                } else {
                    std::cout << blankChar;
                }
            }
        }
        std::cout << std::endl;
    }

    // update this so last label is printed
    // Print x-axis labels
    std::cout << "\t";
    for (int i = 0; i <= plotWidth; ++i) {
        if (i % (plotWidth / 10) == 0) {
            float xValue = minX + i * xIncrement;
            std::cout << std::fixed << std::setprecision(xDecimals) << xValue;

            // calculate the length of the number label string
            std::stringstream ss;
            ss << std::fixed << std::setprecision(xDecimals) << xValue;
            std::string label = ss.str();
            int labelLength = label.length();

            // adjust index to skip '_' character
            i += (labelLength - 1);
        } else {
            std::cout << blankChar;
        }
    }
    std::cout << std::endl;
}