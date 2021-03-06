//
// Created by koalaa13 on 3/28/20.
//

#ifndef HW2_IMAGEFILE_H
#define HW2_IMAGEFILE_H

#include <stdexcept>
#include <cstdio>
#include "algorithm"
#include "string"

struct ImageFile {
    explicit ImageFile(char *filename, const char *mode);

    explicit ImageFile(std::string const &filename, const char*mode);

    ~ImageFile();

    FILE *getData() const;

private:
    FILE *data;
};

#endif //HW2_IMAGEFILE_H
