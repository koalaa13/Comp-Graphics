#include "Utils.h"
#include "ImageFile.h"
#include "PNMImage.h"

int main(int argc, char *argv[]) {
    if (argc != 10 && argc != 9) {
        printError(
                "You should enter input filename, output filename, brightness, thickness, coordinates of line and (optionally) gamma");
    }

    try {
        double gamma = 2.2; // Default value for sRGB gamma correction
        ImageFile in(argv[1], "rb");
        PNMImage image(in);
        if (argc == 10) {
            gamma = std::stod(argv[9]);
        }
        image.drawLine(std::stoi(argv[3]),
                       std::stod(argv[4]),
                       (int) std::stod(argv[5]),
                       (int) std::stod(argv[6]),
                       (int) std::stod(argv[7]),
                       (int) std::stod(argv[8]),
                       gamma);
        ImageFile out(argv[2], "wb");
        image.writeToFile(out);
    } catch (std::exception const &e) {
        printError("Error, occurred while drawing a line: " + std::string(e.what()));
    }

    return 0;
}
