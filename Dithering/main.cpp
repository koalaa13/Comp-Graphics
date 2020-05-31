#include "PNMImage.h"
#include "Utils.h"

int main(int argc, char *argv[]) {
    if (argc != 7) {
        printError(
                "You should enter input filename, output filename, gradient, dithering type, bits per pixel and gamma");
    }

    try {
        ImageFile in(argv[1], "rb");
        PNMImage image(in, std::stoi(argv[3]));
        int type = std::stoi(argv[4]);
        int bit = std::stoi(argv[5]);
        double gamma = std::stod(argv[6]);
        if (gamma == 0) {
            gamma = 2.2;
        }
        if (type == 0) { // OK
            fprintf(stdout, (char *) "You've chosen no dithering mode.\n");
            image.noDithering(bit, gamma);
        }
        if (type == 1) { // OK
            fprintf(stdout, (char *) "You've chosen ordered dithering.\n");
            image.orderedDithering(bit, gamma);
        }
        if (type == 2) { // OK
            fprintf(stdout, (char *) "You've chosen random dithering.\n");
            image.randomDithering(bit, gamma);
        }
        if (type == 3) { // OK
            fprintf(stdout, (char *) "You've chosen Floyd Steinberg dithering algorithm.\n");
            image.ditheringFloydSteinberg(bit, gamma);
        }
        if (type == 4) { // OK
            fprintf(stdout, (char *) "You've chosen Jarvis Judice Ninke dithering algorithm.\n");
            image.ditheringJarvisJudiceNinke(bit, gamma);
        }
        if (type == 5) { // OK
            fprintf(stdout, (char *) "You've chosen Sierra(Sierra-3) dithering algorithm.\n");
            image.ditheringSierra3(bit, gamma);
        }
        if (type == 6) { // OK
            fprintf(stdout, (char *) "You've chosen Atkinson dithering algorithm.\n");
            image.ditheringAtkinson(bit, gamma);
        }
        if (type == 7) { // OK
            fprintf(stdout, (char *) "You've chosen halftone 4x4 orthogonal algorithm.\n");
            image.halftone4x4Dithering(bit, gamma);
        }
        ImageFile out(argv[2], "wb");
        image.writeToFile(out);
    } catch (std::exception const &e) {
        printError("Error occurred while dithering an image: " + (std::string) e.what());
    }

    return 0;
}
