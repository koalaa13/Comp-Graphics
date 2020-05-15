//
// Created by koalaa13 on 5/15/20.
//

#include "Line.h"
#include "stdio.h"

int main() {
    Line line(0, 0, 1, 1);
    printf("%.5f", line.getDistToPoint(1, 0));

    return 0;
}

