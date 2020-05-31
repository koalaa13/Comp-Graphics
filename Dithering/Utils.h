//
// Created by koalaa13 on 3/28/20.
//

#ifndef HW2_UTILS_H
#define HW2_UTILS_H

#include <string>

typedef unsigned char byte;

void printError(const std::string &msg) {
    fprintf(stderr, "%s", msg.c_str());
    exit(1);
}

#endif //HW2_UTILS_H
