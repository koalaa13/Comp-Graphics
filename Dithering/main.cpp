#include "PNMImage.h"
#include "Utils.h"
#include "Dither.h"
#include "set"

int main(int argc, char *argv[]) {
    if (argc != 7) {
        printError(
                "You should enter input filename, output filename, gradient, dithering type, bits per pixel and gamma");
    }

    try {
        ImageFile in(argv[1], "rb");
        bool grad = std::stoi(argv[3]);
        PNMImage image(in);
        int type = std::stoi(argv[4]);
        if (type < 0 || type > 7) {
            printError("Incorrect dithering type, see usage");
        }
        int bit = std::stoi(argv[5]);
        if (bit < 1 || bit > 8) {
            printError("Incorrect bitness for dithering, see usage");
        }
        double gamma = std::stod(argv[6]);
        if (type == 0) {
            Dither::noDithering(image, grad, argv[2], bit, gamma);
            fprintf(stdout, (char *) "You've chosen no dithering mode.\n");
        }
        if (type == 1) {
            Dither::ordered8x8Dithering(image, grad, argv[2], bit, gamma);
            fprintf(stdout, (char *) "You've chosen ordered dithering.\n");
        }
        if (type == 2) {
            Dither::randomDithering(image, grad, argv[2], bit, gamma);
            fprintf(stdout, (char *) "You've chosen random dithering.\n");
        }
        if (type == 3) {
            Dither::FloydSteinbergDithering(image, grad, argv[2], bit, gamma);
            fprintf(stdout, (char *) "You've chosen Floyd Steinberg dithering algorithm.\n");
        }
        if (type == 4) {
            Dither::JarvisJudiceNinkeDithering(image, grad, argv[2], bit, gamma);
            fprintf(stdout, (char *) "You've chosen Jarvis Judice Ninke dithering algorithm.\n");
        }
        if (type == 5) {
            Dither::Sierra3Dithering(image, grad, argv[2], bit, gamma);
            fprintf(stdout, (char *) "You've chosen Sierra(Sierra-3) dithering algorithm.\n");
        }
        if (type == 6) {
            Dither::AtkinsonDithering(image, grad, argv[2], bit, gamma);
            fprintf(stdout, (char *) "You've chosen Atkinson dithering algorithm.\n");
        }
        if (type == 7) {
            Dither::halftone4x4Dithering(image, grad, argv[2], bit, gamma);
            fprintf(stdout, (char *) "You've chosen halftone 4x4 orthogonal algorithm.\n");
        }
    } catch (std::exception const &e) {
        printError("Error occurred while dithering an image: " + (std::string) e.what());
    }

    return 0;
}
