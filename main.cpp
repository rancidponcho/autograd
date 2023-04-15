#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <string>
#include <sstream>

double f(double x) {
    return 3.f*pow(x, 2.f) - 4.f*x + 5.f;
}

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

void plot(const std::vector<double>& x, const std::vector<double>& y) {
    const int plotWidth = 100;
    const int plotHeight = 25;
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
                    std::cout << x_axis_char; // x-axis
                } else {
                    std::cout << blankChar;
                }
            }
        }
        std::cout << std::endl;
    }

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

}

int main() {
    // std::vector<double> x = arange(-5.f, 5.f, 0.25f);
    // std::vector<double> y = apply_function(x, f_of);
    // plot(x, y);

    double h = 0.00000001f;
    // double x = 2./3.;
    // std::cout << (f(x + h) - f(x))/h << std::endl;

    double a = 2.;
    double b = -3.;
    double c = 10.;

    double d1 = a*b + c;
    a += h;
    double d2 = a*b + c;

    std::cout << (d2 - d1)/h << std::endl;

    return 0;
}