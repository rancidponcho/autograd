#pragma once

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "value.hpp"

std::vector<double> arange(double start, double end, double step) {
    std::vector<double> result;
    for (double i = start; i < end; i += step) {
        result.push_back(i);
    }
    return result;
}

std::vector<double> apply_function(const std::vector<double>& input, double (*func)(double)) {
    std::vector<double> result;
    result.reserve(input.size());
    for (const auto& i : input) {
        result.push_back(func(i));
    }
    return result;
}

void plot(const std::vector<double>& x, const std::vector<double>& y, const int plotWidth = 100, const int plotHeight = 25) {
    const char x_axis_char = '_';
    const char y_axis_char = '|';
    const char plotChar = '*';
    const char blankChar = ' ';

    // Find min and max values for both x and y axes
    double minX = *min_element(x.begin(), x.end());
    double maxX = *max_element(x.begin(), x.end());
    double minY = *min_element(y.begin(), y.end());
    double maxY = *max_element(y.begin(), y.end());

    // Calculate the increment for axis labels
    double xIncrement = (maxX - minX) / plotWidth;
    double yIncrement = (maxY - minY) / plotHeight;

    // x-axis labels
    // Determine the number of decimal places needed for x-axis labels
    int xDecimals = 0;
    double xStep = xIncrement;
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
            double xValue = minX + i * xIncrement;
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

// recursively print the graph of a value node
void graph(const Value& v, std::unordered_set<const Value*>& visited, int indent, int precision) {
    // Print the current value node

    std::cout << std::string(indent, ' ') << "+---" << v.get_label() << "[ " << std::fixed << std::setprecision(precision) << v.get() << " ][ " << v.get_grad() << " ]" << (v.get_op().empty() ? "" : "(" + v.get_op() + ")") << std::endl;

    // Recursively print the previous value nodes
    for (const Value& prev : v.get_prev()) {
        // If we haven't visited this node yet, print it
        if (!visited.count(&prev)) {
            visited.insert(&prev);
            graph(prev, visited, indent + 4, precision);
        }
        // Otherwise, just print an arrow to indicate a previously visited node
        else {
            std::cout << std::string(indent + 3, ' ') << "|   +" << std::endl;
        }
    }
}

void graph(const Value& v, int precision = 4) {
    std::unordered_set<const Value*> visited;
    visited.insert(&v);
    graph(v, visited, 0, precision);
}