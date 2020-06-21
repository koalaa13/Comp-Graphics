//
// Created by koalaa13 on 3/28/20.
//

#include "Image.h"

Image::Image(const ImageFile &imageFile, bool colorSpace) {
    data = nullptr;
    char format, trash;
    int imageType;
    int readed = fscanf(imageFile.getData(), "%c%d%d%d%d%c", &format, &imageType, &width, &height, &pixelSize, &trash);
    if (readed != 6 || format != 'P' || imageType > 6 || imageType < 5 || width <= 0 || height <= 0 ||
        pixelSize != 255) {
        throw std::runtime_error("Incorrect image format");
    }
    BYTES_PER_PIXEL = imageType == 5 ? 1 : 3;
    this->colorSpace = colorSpace;
    if (imageType == 5 && colorSpace) {
        throw std::runtime_error("Incorrect color space for image: PNM P5 Image can't be in color space YCbCr");
    }
    IMAGE_SIZE = BYTES_PER_PIXEL * width * height;
    data = (byte *) malloc(sizeof(byte) * IMAGE_SIZE);
    if (data == nullptr) {
        throw std::runtime_error("Not enough memory to open image");
    }
    readed = fread(data, 1, IMAGE_SIZE, imageFile.getData());
    if (readed != IMAGE_SIZE) {
        throw std::runtime_error("Error while trying to read an image");
    }
}

Image::~Image() {
    if (data != nullptr) {
        free(data);
    }
}

void Image::writeToFile(const ImageFile &imageFile) const {
    fprintf(imageFile.getData(), "P%d\n%d %d\n%d\n", BYTES_PER_PIXEL == 1 ? 5 : 6, width, height, pixelSize);
    if (ferror(imageFile.getData())) {
        throw std::runtime_error("Something happened while writing result to output file");
    }
    fwrite(data, sizeof(byte), IMAGE_SIZE, imageFile.getData());
    if (ferror(imageFile.getData())) {
        throw std::runtime_error("Something happened while writing result to output file");
    }
}

Image::Image(const Image &other) : width(other.width), height(other.height), pixelSize(other.pixelSize),
                                   BYTES_PER_PIXEL(other.BYTES_PER_PIXEL), IMAGE_SIZE(other.IMAGE_SIZE),
                                   colorSpace(other.colorSpace) {
    data = (byte *) malloc(sizeof(byte) * IMAGE_SIZE);
    if (data == nullptr) {
        throw std::runtime_error("Not enough memory to copy image");
    }
    memcpy(data, other.data, IMAGE_SIZE);
}

Image &Image::operator=(const Image &other) {
    colorSpace = other.colorSpace;
    width = other.width;
    height = other.height;
    BYTES_PER_PIXEL = other.BYTES_PER_PIXEL;
    IMAGE_SIZE = other.IMAGE_SIZE;
    pixelSize = other.pixelSize;
    if (data != nullptr) {
        free(data);
    }
    data = (byte *) malloc(sizeof(byte) * IMAGE_SIZE);
    if (data == nullptr) {
        throw std::runtime_error("Not enough memory to copy a image");
    }
    memcpy(data, other.data, IMAGE_SIZE);
    return *this;
}

void Image::correction(double offset, double factor) {
    if (BYTES_PER_PIXEL == 1) {
        for (int i = 0; i < IMAGE_SIZE; ++i) {
            data[i] = doCorrection((double) data[i], offset, factor);
        }
    } else {
        if (colorSpace) {
            for (int i = 0; i < IMAGE_SIZE; i += 3) {
                data[i] = doCorrection((double) data[i], offset, factor);
            }
        } else {
            for (int i = 0; i < IMAGE_SIZE; ++i) {
                data[i] = doCorrection((double) data[i], offset, factor);
            }
        }
    }
}

double Image::doCorrection(double const &d, const double &offset, const double &factor) const {
    return fmax(0, fmin(pixelSize, (d - offset) * factor));
}

void Image::autoCorrection() {
    double mn = 256., mx = 0;
    if (BYTES_PER_PIXEL == 1) {
        for (int i = 0; i < IMAGE_SIZE; ++i) {
            mn = fmin(mn, data[i]);
            mx = fmax(mx, data[i]);
        }
        for (int i = 0; i < IMAGE_SIZE; ++i) {
            data[i] = doAutoCorrection(data[i], mn, mx);
        }
    } else {
        if (colorSpace) {
            for (int i = 0; i < IMAGE_SIZE; i += 3) {
                mn = fmin(mn, data[i]);
                mx = fmax(mx, data[i]);
            }
            for (int i = 0; i < IMAGE_SIZE; i += 3) {
                data[i] = doAutoCorrection(data[i], mn, mx);
            }
        } else {
            for (int i = 0; i < IMAGE_SIZE; i += 3) {
                double y = getBrightness(data[i], data[i + 1], data[i + 2]);
                mn = fmin(mn, y);
                mx = fmax(mx, y);
            }
            for (int i = 0; i < IMAGE_SIZE; ++i) {
                data[i] = doAutoCorrection(data[i], mn, mx);
            }
        }
    }
}

double Image::doAutoCorrection(double const &d, double const &mn, double const &mx) const {
    return fmin(pixelSize, fmax(0., (d - mn) * pixelSize / (mx - mn)));
}

void Image::autoSkipCorrection() {
    std::vector<double> values;
    values.reserve(IMAGE_SIZE / BYTES_PER_PIXEL);
    int skip = (int) (0.0039 * (double) (IMAGE_SIZE / BYTES_PER_PIXEL));
    double mn = 256., mx = 0.;
    if (BYTES_PER_PIXEL == 1) {
        for (int i = 0; i < IMAGE_SIZE; ++i) {
            values.push_back(data[i]);
        }
        std::sort(values.begin(), values.end());
        mn = values[skip];
        mx = values[values.size() - 1 - skip];
        for (int i = 0; i < IMAGE_SIZE; ++i) {
            data[i] = doAutoCorrection(data[i], mn, mx);
        }
    } else {
        if (colorSpace) {
            for (int i = 0; i < IMAGE_SIZE; i += 3) {
                values.push_back(data[i]);
            }
            std::sort(values.begin(), values.end());
            mn = values[skip];
            mx = values[values.size() - 1 - skip];
            for (int i = 0; i < IMAGE_SIZE; i += 3) {
                data[i] = doAutoCorrection(data[i], mn, mx);
            }
        } else {
            for (int i = 0; i < IMAGE_SIZE; i += 3) {
                values.push_back(getBrightness(data[i], data[i + 1], data[i + 2]));
            }
            std::sort(values.begin(), values.end());
            mn = values[skip];
            mx = values[values.size() - 1 - skip];
            for (int i = 0; i < IMAGE_SIZE; ++i) {
                data[i] = doAutoCorrection(data[i], mn, mx);
            }
        }
    }
}

double Image::getBrightness(double const &r, double const &g, double const &b) {
    return 0.299 * r + 0.587 * g + 0.114 * b;
}

bool operator==(Image const &a, Image const &b) {
    bool eq = a.colorSpace == b.colorSpace && a.IMAGE_SIZE == b.IMAGE_SIZE && a.BYTES_PER_PIXEL == b.BYTES_PER_PIXEL &&
              a.pixelSize == b.pixelSize && a.width == b.width && a.height == b.height;
    if (!eq) {
        return eq;
    }
    for (int i = 0; i < a.IMAGE_SIZE; ++i) {
        eq &= a.data[i] == b.data[i];
    }
    return eq;
}

