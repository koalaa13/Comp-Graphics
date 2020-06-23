//
// Created by koalaa13 on 3/28/20.
//

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

void PNMImage::writeToFile(const ImageFile &imageFile) const {
    writeHeaderToFile(imageFile);
    fwrite(data, 1, IMAGE_SIZE, imageFile.getData());
    if (ferror(imageFile.getData())) {
        throw std::runtime_error("Something happened while writing result to output file");
    }
}

void PNMImage::writeHeaderToFile(const ImageFile &imageFile) const {
    fprintf(imageFile.getData(), "P5\n%d %d\n%d\n", width, height, pixelSize);
    if (ferror(imageFile.getData())) {
        throw std::runtime_error("Something happened while writing result to output file");
    }
}
