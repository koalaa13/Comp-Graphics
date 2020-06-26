#include "PNMImage.h"
#include "Utils.h"
#include "Dither.h"
#include "set"

void testing() {
//    const int N = 8;
//    std::vector<std::vector<double>> matrix(N);
//    matrix[0] = {0, 48, 12, 60, 3, 51, 15, 63};
//    matrix[1] = {32, 16, 44, 28, 35, 19, 47, 31};
//    matrix[2] = {8, 56, 4, 52, 11, 59, 7, 55};
//    matrix[3] = {40, 24, 36, 20, 43, 27, 39, 23};
//    matrix[4] = {2, 50, 14, 62, 1, 49, 13, 61};
//    matrix[5] = {34, 18, 46, 30, 33, 17, 45, 29};
//    matrix[6] = {10, 58, 6, 54, 9, 57, 5, 53};
//    matrix[7] = {42, 26, 38, 22, 41, 25, 37, 21};
//    for (int i = 0; i < N; ++i) {
//        for (int j = 0; j < N; ++j) {
//            matrix[i][j] = ((matrix[i][j] + 0.5) / (double) (N * N) - 0.5);
//        }
//    }
    int cnt = 0;
    PNMImage image(ImageFile("/home/koalaa13/Desktop/comp_graph/HW3/result.pnm", "rb"));
    for (int i = 0; i < image.width; ++i) {
        std::set<byte> have;
        for (int j = 0; j < image.height; ++j) {
            have.insert(image.data[j * image.width + i]);
        }
        if (have.size() == 1) {
//            double col = i * 255. / (image.width - 1);
//            std::cerr << col << ' ';
//            for (int j = 0; j < 8; ++j) {
//                std::cerr << col + matrix[i % N][j] << ' ';
//            }
//            std::cerr << '\n';
              cnt++;
        }
    }
    std::cerr << cnt << ' ' << image.width;
}

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
    testing();

    return 0;
}
