//
// Created by koalaa13 on 5/15/20.
//

#ifndef HW2_LINE_H
#define HW2_LINE_H

#include "cmath"

typedef double ld;

struct Line {
    Line(ld x0, ld y0, ld x1, ld y1);

    Line(ld a, ld b, ld c);

    ld getDistToPoint(ld x, ld y) const;
private:
    ld a, b, c;
};

#endif //HW2_LINE_H
