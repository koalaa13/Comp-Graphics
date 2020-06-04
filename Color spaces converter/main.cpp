#include <iostream>
#include "Converter.h"
#include "Utils.h"
#include "vector"
#include "string"
#include "unordered_set"

void testing() {
    const int diff = 1;
    for (int r = 0; r < 256; ++r) {
        for (int g = 0; g < 256; ++g) {
            for (int b = 0; b < 256; ++b) {
                auto res = Converter::toHSL((double) r / 255., (double) g / 255., (double) b / 255.);
                auto res2 = Converter::fromHSL(std::get<0>(res), std::get<1>(res), std::get<2>(res));
                int r1 = std::get<0>(res2) * 255;
                int g1 = std::get<1>(res2) * 255;
                int b1 = std::get<2>(res2) * 255;
                if (abs(r - r1) > diff || abs(g - g1) > diff || abs(b - b1) > diff) {
                    std::cout << "r = " << r << " g = " << g << " b = " << b << '\n';
                    std::cout << "r1 = " << r1 << " g1 = " << g1 << " b1 = " << b1 << '\n';
                    return;
                }
            }
        }
    }
}

void fromRGBToRGB() {
    char *input = "/home/koalaa13/Desktop/comp_graph/HW4/color_sample.pnm";
    char *output = "/home/koalaa13/Desktop/comp_graph/HW4/result.pnm";

    Image res = Converter::convertRGBToHSL(Image(ImageFile(input, "rb")));
    res.writeToFile(ImageFile(output, "wb"));

    char *back = "/home/koalaa13/Desktop/comp_graph/HW4/back.pnm";
    Image res2 = Converter::convertHSLToRGB(res);
    res2.writeToFile(ImageFile(back, "wb"));
}

int main(int argc, char *argv[]) {
    std::vector<std::string> keys = {"-f", "-t", "-i", "-o"};
    std::unordered_set<std::string> colorSpaces = {"RGB", "HSL", "HSV", "YCbCr.601", "YCbCr.709", "YCoCg", "CMY"};
    std::string fromColorSpace, toColorSpace;
    int inputCount, outputCount, maskCheck = 0;
    std::vector<std::string> ins(3), outs(3);
    if (argc == 1) {
        printError("Incorrect input for a program");
    }
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-f") {
            if (maskCheck & 1) {
                printError("Should have only one key -f");
            }
            maskCheck |= 1;
            fromColorSpace = argv[++i];
            if (std::find(keys.begin(), keys.end(), fromColorSpace) != keys.end()) {
                printError("Should have a name of color space after -f");
            }
            continue;
        }
        if (arg == "-t") {
            if (maskCheck & 2) {
                printError("Should have only one key -t");
            }
            maskCheck |= 2;
            toColorSpace = argv[++i];
            if (std::find(keys.begin(), keys.end(), toColorSpace) != keys.end()) {
                printError("Should have a name of color space after -t");
            }
            continue;
        }
        if (arg == "-i") {
            if (maskCheck & 4) {
                printError("Should have only one key -i");
            }
            maskCheck |= 4;
            try {
                inputCount = std::stoi(argv[++i]);
            } catch (std::invalid_argument const &e) {
                printError("Should have a number after -i");
            } catch (std::out_of_range const &e) {
                printError("Too big number after -i");
            }
            if (inputCount != 1 && inputCount != 3) {
                printError("Number after -i should be equals to 1 or 3");
            }
            for (int j = 0; j < inputCount; ++j) {
                ins[j] = argv[++i];
                if (std::find(keys.begin(), keys.end(), ins[j]) != keys.end()) {
                    printError("Should have filename(s) after -i");
                }
            }
            continue;
        }
        if (arg == "-o") {
            if (maskCheck & 8) {
                printError("Should have only one key -o");
            }
            maskCheck |= 8;
            try {
                outputCount = std::stoi(argv[++i]);
            } catch (std::invalid_argument const &e) {
                printError("Should have a number after -o");
            } catch (std::out_of_range const &e) {
                printError("Too big number after -o");
            }
            if (outputCount != 1 && outputCount != 3) {
                printError("Number after -o should be equals to 1 or 3");
            }
            for (int j = 0; j < outputCount; ++j) {
                outs[j] = argv[++i];
                if (std::find(keys.begin(), keys.end(), outs[j]) != keys.end()) {
                    printError("Should have filename(s) after -o");
                }
            }
            continue;
        }
        printError("Incorrect input for a program");
    }
    if (maskCheck != 15) {
        for (int i = 0; i < 4; ++i) {
            if ((maskCheck & (1 << i)) == 0) {
                printError("Should have key " + keys[i] + " in input");
            }
        }
    }
    if (colorSpaces.count(fromColorSpace) == 0) {
        printError("Incorrect color space for -f");
    }
    if (colorSpaces.count(toColorSpace) == 0) {
        printError("Incorrect color space for -t");
    }
    if (fromColorSpace == toColorSpace) {
        if (inputCount == outputCount) {
            for (int i = 0; i < inputCount; ++i) {
                Image image(ImageFile(ins[i], "rb"));
                image.writeToFile(ImageFile(outs[i], "wb"));
            }
        } else {
            if (inputCount == 1) {
                auto res = Converter::split(Image(ImageFile(ins[0], "rb")));
                std::get<0>(res).writeToFile(ImageFile(outs[0], "wb"));
                std::get<1>(res).writeToFile(ImageFile(outs[1], "wb"));
                std::get<2>(res).writeToFile(ImageFile(outs[2], "wb"));
            } else {
                Image res = Converter::merge(Image(ImageFile(ins[0], "rb")),
                                             Image(ImageFile(ins[1], "rb")),
                                             Image(ImageFile(ins[2], "rb")));
                res.writeToFile(ImageFile(outs[0], "wb"));
            }
        }
    } else {
        Image input = inputCount == 1 ? Image(ImageFile(ins[0], "rb")) :
                      Converter::merge(Image(ImageFile(ins[0], "rb")),
                                       Image(ImageFile(ins[1], "rb")),
                                       Image(ImageFile(ins[2], "rb")));
        Image rgb = input;
        if (fromColorSpace != "RGB") {
            if (fromColorSpace == "HSL") {
                rgb = Converter::convertHSLToRGB(input);
            }
            if (fromColorSpace == "HSV") {

            }
            if (fromColorSpace == "YCbCr.601") {

            }
            if (fromColorSpace == "YCbCr.709") {

            }
            if (fromColorSpace == "YCoCg") {

            }
            if (fromColorSpace == "CMY") {

            }
        }

        Image res = rgb;
        if (toColorSpace != "RGB") {
            if (toColorSpace == "HSL") {
                res = Converter::convertRGBToHSL(rgb);
            }
            if (toColorSpace == "HSV") {

            }
            if (toColorSpace == "YCbCr.601") {

            }
            if (toColorSpace == "YCbCr.709") {

            }
            if (toColorSpace == "YCoCg") {

            }
            if (toColorSpace == "CMY") {

            }
        }
        if (outputCount == 1) {
            res.writeToFile(ImageFile(outs[0], "wb"));
        } else {
            auto ans = Converter::split(res);
            std::get<0>(ans).writeToFile(ImageFile(outs[0], "wb"));
            std::get<1>(ans).writeToFile(ImageFile(outs[1], "wb"));
            std::get<2>(ans).writeToFile(ImageFile(outs[2], "wb"));
        }
    }

    return 0;
}
