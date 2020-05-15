//
// Created by koalaa13 on 5/15/20.
//

#include "Line.h"

Line::Line(ld x0, ld y0, ld x1, ld y1) {
    a = y0 - y1;
    b = x1 - x0;
    c = x0 * y1 - x1 * y0;
}

Line::Line(ld a, ld b, ld c) : a(a), b(b), c(c) {}

ld Line::getDistToPoint(ld x, ld y) const {
    ld norm = sqrt(a * a + b * b);
    return fabs((a * x + b * y + c) / norm);
}

