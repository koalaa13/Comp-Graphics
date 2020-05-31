//
// Created by koalaa13 on 3/28/20.
//

#include <vector>
#include "PNMImage.h"

PNMImage::PNMImage(const ImageFile &imageFile) {
    data = nullptr;
    char format, trash;
    int imageType;
    int readed = fscanf(imageFile.getData(), "%c%d%d%d%d%c", &format, &imageType, &width, &height, &pixelSize, &trash);
    if (readed != 6 || format != 'P' || imageType != 5 || width <= 0 || height <= 0 || pixelSize != 255) {
        throw std::runtime_error("Incorrect image format");
    }
    IMAGE_SIZE = width * height;
    data = (byte *) malloc(sizeof(byte) * IMAGE_SIZE);
    if (data == nullptr) {
        throw std::runtime_error("Not enough memory to open image");
    }
    readed = fread(data, 1, IMAGE_SIZE, imageFile.getData());
    if (readed != IMAGE_SIZE) {
        throw std::runtime_error("Error while trying to read an image");
    }
}

PNMImage::~PNMImage() {
    if (data != nullptr) {
        free(data);
    }
}

void PNMImage::writeToFile(const ImageFile &imageFile) {
    fprintf(imageFile.getData(), "P5\n%d %d\n%d\n", width, height, pixelSize);
    if (ferror(imageFile.getData())) {
        throw std::runtime_error("Something happened while writing result to output file");
    }
    fwrite(data, sizeof(byte), IMAGE_SIZE, imageFile.getData());
    if (ferror(imageFile.getData())) {
        throw std::runtime_error("Something happened while writing result to output file");
    }
}

void PNMImage::setPixelColor(int x, int y, int lineColor, int brightness, double gamma, bool srgb) {
    if (x < 0 || x > width || y < 0 || y > height) {
        return;
    }
    int ind = x + y * width;
    double intensity = 1. - ((double) lineColor) / pixelSize;
    double backgroundColor = gammaCorrection(((double) data[ind]) / pixelSize, gamma, srgb, false);
    data[ind] =
            gammaCorrection(intensity * (((double) brightness) / pixelSize) + (1. - intensity) * backgroundColor,
                            1. / gamma, srgb, true) * pixelSize;
}

void PNMImage::drawLine(int brightness, double thickness, int x0, int y0, int x1, int y1, double gamma, bool srgb) {
    int begX = std::min(x0, x1), endX = std::max(x0, x1);
    int begY = std::min(y0, y1), endY = std::max(y0, y1);
    Line line(x0, y0, x1, y1);

    if (x0 == x1) {
        begX -= (int) thickness / 2;
        endX += (int) thickness / 2;
    }

    if (y0 == y1) {
        begY -= (int) thickness / 2;
        endY += (int) thickness / 2;
    }

    for (int curX = begX; curX <= endX; ++curX) {
        for (int curY = begY; curY <= endY; ++curY) {
            setPixelColor(curX, curY, getLineColor(line, curX, curY, thickness), brightness, gamma, srgb);
        }
    }
}

byte PNMImage::getLineColor(const Line &line, int x, int y, double thickness) const {
    double dist = line.getDistToPoint(x, y);
    if (thickness >= 1.) {
        if (dist > thickness) {
            return pixelSize;
        }
        return (byte) (pixelSize - ((thickness - dist) / thickness * pixelSize));
    } else {
        if (dist > 0.5) {
            return pixelSize;
        }
        return (byte) (pixelSize - thickness * pixelSize);
    }
}

double PNMImage::sRGB(double col) {
    if (col <= 0.0031308) {
        return 12.92 * col;
    } else {
        return (211 * pow(col, 5. / 12.) - 11) / 200;
    }
}

double PNMImage::revsRGB(double col) {
    if (col <= 0.04045) {
        return col / 12.92;
    } else {
        return pow((200 * col + 11) / 211, 12. / 5.);
    }
}

double PNMImage::gammaCorrection(double u, double gamma, bool srgb, bool rev) {
    if (srgb) {
        if (rev) {
            return revsRGB(u);
        } else {
            return sRGB(u);
        }
    } else {
        return pow(u, gamma);
    }
}
