//
// Created by koalaa13 on 3/28/20.
//

#ifndef HW2_IMAGEFILE_H
#define HW2_IMAGEFILE_H

#include <stdexcept>
#include <cstdio>

struct ImageFile {
    explicit ImageFile(char *filename, const char *mode);

    ~ImageFile();

    FILE *getData() const;

private:
    FILE *data;
};

#endif //HW2_IMAGEFILE_H
