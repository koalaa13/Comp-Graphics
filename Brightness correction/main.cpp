#include "Utils.h"
#include "Corrector.h"

void checkEqImage() {
    Image im1(ImageFile("/home/koalaa13/Desktop/comp_graph/HW5/tiger.pnm", "rb"));
    Image im2(ImageFile("/home/koalaa13/Desktop/comp_graph/HW5/result.pnm", "rb"));
    if (im1 == im2) {
        printf("Images are equal");
    } else {
        printf("Images are different");
    }
}


int main(int argc, char **argv) {
//    checkEqImage();
    try {
        if (argc != 6 && argc != 4) {
            printError("Incorrect count of arguments, see usage");
        }
        int conversionType;
        try {
            conversionType = std::stoi(argv[3]);
        } catch (std::invalid_argument const &e) {
            printError("Conversion type should be an integer number");
        } catch (std::out_of_range const &e) {
            printError("Incorrect conversion type");
        }
        if (conversionType < 0 || conversionType > 5) {
            printError("Conversion type should be in [0;5]");
        }
        if (conversionType < 2 && argc != 6) {
            printError("Should have a offset and factor for conversions 0 and 1");
        }
        Image image(ImageFile(argv[1], "rb"));
        if (conversionType < 2) {
            double offset, factor;
            try {
                offset = std::stod(argv[4]);
                factor = std::stod(argv[5]);
            } catch (std::invalid_argument const &e) {
                printError("Offset and factor should be numbers");
            } catch (std::out_of_range const &e) {
                printError("Incorrect offset or factor");
            }
            if (conversionType == 0) {
                Corrector::RGBCorrection(image, offset, factor);
            } else {
                Corrector::YCbCrCorrection(image, offset, factor);
            }
        }
        if (conversionType == 2) {
            Corrector::RGBAutoCorrection(image);
        }
        if (conversionType == 3) {
            Corrector::YCbCrAutoCorrection(image);
        }
        if (conversionType == 4) {
            Corrector::RGBSkipAutoCorrection(image);
        }
        if (conversionType == 5) {
            Corrector::YCbCrSkipAutoCorrection(image);
        }
        image.writeToFile(ImageFile(argv[2], "wb"));
    } catch (std::exception const &e) {
        printError("Error occurred while doing job: " + (std::string) e.what());
    }

    return 0;
}
