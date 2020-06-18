//
// Created by koalaa13 on 3/28/20.
//

#include "ImageFile.h"

ImageFile::ImageFile(char *filename, const char *mode) {
    data = fopen(filename, mode);
    if (data == nullptr) {
        throw std::runtime_error("Can't open a image file");
    }
}

ImageFile::~ImageFile() {
    fclose(data);
}

FILE *ImageFile::getData() const {
    return data;
}

ImageFile::ImageFile(std::string const &filename, const char *mode) : ImageFile((char *) filename.c_str(), mode) {}