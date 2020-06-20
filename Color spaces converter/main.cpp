#include <iostream>
#include "Converter.h"
#include "Utils.h"
#include "vector"
#include "string"
#include "unordered_set"

void fromRGBToRGB() {
    char *input = "/home/koalaa13/Desktop/comp_graph/HW4/tiger.pnm";
    char *output = "/home/koalaa13/Desktop/comp_graph/HW4/result.pnm";

    Image res = Converter::convertRGBToYCbCr709(Image(ImageFile(input, "rb")));
    res.writeToFile(ImageFile(output, "wb"));

    char *back = "/home/koalaa13/Desktop/comp_graph/HW4/back.pnm";
    Image res2 = Converter::convertYCbCr709ToRGB(res);
    res2.writeToFile(ImageFile(back, "wb"));
}

std::vector<std::string> getFilenames(std::string const &temp) {
    int ind = (int) temp.size() - 1;
    for (; ind >= 0 && temp[ind] != '.'; --ind) {
    }
    if (ind == -1) {
        throw std::runtime_error("Incorrect template for filenames");
    }
    std::string filename = temp.substr(0, ind);
    std::string ext = temp.substr(ind, temp.size() - ind + 1);
    return {filename + "_1" + ext, filename + "_2" + ext, filename + "_3" + ext};
}

int main(int argc, char *argv[]) {
//    fromRGBToRGB();
    try {
        std::vector<std::string> keys = {"-f", "-t", "-i", "-o"};
        std::unordered_set<std::string> colorSpaces = {"RGB", "HSL", "HSV", "YCbCr.601", "YCbCr.709", "YCoCg", "CMY"};
        std::string fromColorSpace, toColorSpace, inTemp, outTemp;
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
                if (++i == argc) {
                    printError("Should have color space after -f");
                }
                fromColorSpace = argv[i];
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
                if (++i == argc) {
                    printError("Should have color space after -t");
                }
                toColorSpace = argv[i];
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
                if (++i == argc) {
                    printError("Should have a filename after -i");
                }
                inTemp = argv[i];
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
                if (++i == argc) {
                    printError("Should have a filename after -o");
                }
                outTemp = argv[i];
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
        if (inputCount == 1) {
            ins[0] = inTemp;
        } else {
            ins = getFilenames(inTemp);
        }
        if (outputCount == 1) {
            outs[0] = outTemp;
        } else {
            outs = getFilenames(outTemp);
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
                    rgb = Converter::convertHSVToRGB(input);
                }
                if (fromColorSpace == "YCbCr.601") {
                    rgb = Converter::convertYCbCr601ToRGB(input);
                }
                if (fromColorSpace == "YCbCr.709") {
                    rgb = Converter::convertYCbCr709ToRGB(input);
                }
                if (fromColorSpace == "YCoCg") {
                    rgb = Converter::convertYCoCgToRGB(input);
                }
                if (fromColorSpace == "CMY") {
                    rgb = Converter::convertCMYToRGB(input);
                }
            }

            Image res = rgb;
            if (toColorSpace != "RGB") {
                if (toColorSpace == "HSL") {
                    res = Converter::convertRGBToHSL(rgb);
                }
                if (toColorSpace == "HSV") {
                    res = Converter::convertRGBToHSV(rgb);
                }
                if (toColorSpace == "YCbCr.601") {
                    res = Converter::convertRGBToYCbCr601(rgb);
                }
                if (toColorSpace == "YCbCr.709") {
                    res = Converter::convertRGBToYCbCr709(rgb);
                }
                if (toColorSpace == "YCoCg") {
                    res = Converter::convertRGBToYCoCg(rgb);
                }
                if (toColorSpace == "CMY") {
                    res = Converter::convertRGBToCMY(rgb);
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
    } catch (std::exception const &e) {
        printError("Error occurred while converting: " + (std::string) e.what());
    }

    return 0;
}
