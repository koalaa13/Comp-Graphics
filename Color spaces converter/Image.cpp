//
// Created by koalaa13 on 3/28/20.
//

#include "Image.h"

Image::Image(const ImageFile &imageFile) {
    data = nullptr;
    char format, trash;
    int imageType;
    int readed = fscanf(imageFile.getData(), "%c%d%d%d%d%c", &format, &imageType, &width, &height, &pixelSize, &trash);
    if (readed != 6 || format != 'P' || imageType > 6 || imageType < 5 || width <= 0 || height <= 0 ||
        pixelSize != 255) {
        throw std::runtime_error("Incorrect image format");
    }
    BYTES_PER_PIXEL = imageType == 5 ? 1 : 3;
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
                                   BYTES_PER_PIXEL(other.BYTES_PER_PIXEL), IMAGE_SIZE(other.IMAGE_SIZE) {
    data = (byte *) malloc(sizeof(byte) * IMAGE_SIZE);
    if (data == nullptr) {
        throw std::runtime_error("Not enough memory to copy image");
    }
    memcpy(data, other.data, IMAGE_SIZE);
}

Image::Image(const Image &image1, const Image &image2, const Image &image3) : width(image1.width),
                                                                              height(image1.height),
                                                                              pixelSize(image1.pixelSize) {
    if (image1.BYTES_PER_PIXEL != 1 || image2.BYTES_PER_PIXEL != 1 || image3.BYTES_PER_PIXEL != 1) {
        throw std::runtime_error("Constructor from three images requires three format P5 images");
    }
    if (image1.height != image2.height || image1.height != image3.height || image1.width != image2.width ||
        image1.width != image3.width) {
        throw std::runtime_error("Images should have equal heights and widths");
    }
    IMAGE_SIZE = image1.IMAGE_SIZE * 3;
    BYTES_PER_PIXEL = 3;
    data = (byte *) malloc(sizeof(byte) * IMAGE_SIZE);
    if (data == nullptr) {
        throw std::runtime_error("Not enough memory to create image");
    }
    for (int i = 0, j = 0; i < image1.IMAGE_SIZE; ++i) {
        data[j++] = image1.data[i];
        data[j++] = image2.data[i];
        data[j++] = image3.data[i];
    }
}

Image::Image(const Image &other, int ind) : width(other.width),
                                            height(other.height),
                                            pixelSize(other.pixelSize) {
    if (other.BYTES_PER_PIXEL != 3) {
        throw std::runtime_error("Constructor from image and integer requires format P6 image");
    }
    BYTES_PER_PIXEL = 1;
    IMAGE_SIZE = width * height;
    data = (byte *) malloc(sizeof(byte) * IMAGE_SIZE);
    if (data == nullptr) {
        throw std::runtime_error("Not enough memory to create image");
    }
    for (int i = ind, j = 0; i < other.IMAGE_SIZE; i += 3) {
        data[j++] = other.data[i];
    }
}

Image &Image::operator=(const Image &other) {
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
