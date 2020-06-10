//
// Created by koalaa13 on 6/1/20.
//

#include "Converter.h"

std::tuple<double, double, double> Converter::toHSL(double r, double g, double b) {
    double h = 0.;
    double s = 0.;
    double l = 0.;
    double mx = fmax(r, fmax(g, b)); // aka v
    double mn = fmin(r, fmin(g, b));
    double c = mx - mn;
    l = (mx + mn) / 2.;
    if (c == 0.) {
        h = 0.;
    } else {
        if (mx == r) {
            h = 60. * ((g - b) / c);
        } else {
            if (mx == g) {
                h = 60. * (2. + (b - r) / c);
            } else {
                if (mx == b) {
                    h = 60. * (4. + (r - g) / c);
                }
            }
        }
    }
    if (h < 0.) {
        h = 360. + h;
    }
    if (l == 0 || l == 1) {
        s = 0.;
    } else {
        s = (mx - l) / std::min(l, 1 - l);
    }
    return std::make_tuple(h, s, l);
}

std::tuple<double, double, double> Converter::fromHSL(double h, double s, double l) {
    double h1 = h / 60.;
    int h2 = (int) h1;
    double c = (1 - fabs(2 * l - 1)) * s;
    double x = c * (1 - fabs(std::fmod(h1, 2) - 1));
    double r1, g1, b1;
    switch (h2) {
        case 0:
            r1 = c;
            g1 = x;
            b1 = 0;
            break;
        case 1:
            r1 = x;
            g1 = c;
            b1 = 0;
            break;
        case 2:
            r1 = 0;
            g1 = c;
            b1 = x;
            break;
        case 3:
            r1 = 0;
            g1 = x;
            b1 = c;
            break;
        case 4:
            r1 = x;
            g1 = 0;
            b1 = c;
            break;
        case 5:
            r1 = c;
            g1 = 0;
            b1 = x;
            break;
        default:
            r1 = b1 = g1 = 0.;
            break;
    }
    double m = l - c / 2.;
    return std::make_tuple(r1 + m, g1 + m, b1 + m);
}

Image Converter::convertRGBToHSL(const Image &input) {
    Image outputImage = input;
    for (int i = 0; i < input.IMAGE_SIZE; i += 3) {
        double r = (double) input.data[i] / (double) input.pixelSize;
        double g = (double) input.data[i + 1] / (double) input.pixelSize;
        double b = (double) input.data[i + 2] / (double) input.pixelSize;
        std::tuple<double, double, double> hsl = toHSL(r, g, b);
        outputImage.data[i] = std::get<0>(hsl) / 360. * (double) input.pixelSize;
        outputImage.data[i + 1] = std::get<1>(hsl) * (double) input.pixelSize;
        outputImage.data[i + 2] = std::get<2>(hsl) * (double) input.pixelSize;
    }
    return outputImage;
}

Image Converter::convertHSLToRGB(const Image &input) {
    Image outputImage = input;
    for (int i = 0; i < input.IMAGE_SIZE; i += 3) {
        double h = (double) input.data[i] / (double) input.pixelSize * 360.;
        double s = (double) input.data[i + 1] / (double) input.pixelSize;
        double l = (double) input.data[i + 2] / (double) input.pixelSize;
        std::tuple<double, double, double> rgb = fromHSL(h, s, l);
        outputImage.data[i] = std::get<0>(rgb) * (double) input.pixelSize;
        outputImage.data[i + 1] = std::get<1>(rgb) * (double) input.pixelSize;
        outputImage.data[i + 2] = std::get<2>(rgb) * (double) input.pixelSize;
    }
    return outputImage;
}

Image Converter::merge(const Image &im1, const Image &im2, const Image &im3) {
    return Image(im1, im2, im3);
}

std::tuple<Image, Image, Image> Converter::split(const Image &im) {
    return std::make_tuple(Image(im, 0), Image(im, 1), Image(im, 2));
}

std::tuple<double, double, double> Converter::fromHSV(double h, double s, double v) {
    double c = v * s;
    double h1 = h / 60.;
    double x = c * (1 - fabs(std::fmod(h1, 2) - 1));
    double r1 = 0., g1 = 0., b1 = 0.;
    if (h1 >= 0. && h1 <= 1.) {
        r1 = c;
        g1 = x;
        b1 = 0;
    }
    if (h1 > 1. && h1 <= 2.) {
        r1 = x;
        g1 = c;
        b1 = 0;
    }
    if (h1 > 2. && h1 <= 3.) {
        r1 = 0;
        g1 = c;
        b1 = x;
    }
    if (h1 > 3. && h1 <= 4.) {
        r1 = 0;
        g1 = x;
        b1 = c;
    }
    if (h1 > 4. && h1 <= 5.) {
        r1 = x;
        g1 = 0;
        b1 = c;
    }
    if (h1 > 5. && h1 <= 6.) {
        r1 = c;
        g1 = 0;
        b1 = x;
    }
    double m = v - c;
    return std::make_tuple(r1 + m, g1 + m, b1 + m);
}

std::tuple<double, double, double> Converter::toHSV(double r, double g, double b) {
    double h = 0.;
    double s = 0.;
    double mx = fmax(r, fmax(g, b)); // aka v
    double mn = fmin(r, fmin(g, b));
    double c = mx - mn;
    if (c == 0.) {
        h = 0.;
    } else {
        if (mx == r) {
            h = 60. * ((g - b) / c);
        } else {
            if (mx == g) {
                h = 60. * (2. + (b - r) / c);
            } else {
                if (mx == b) {
                    h = 60. * (4. + (r - g) / c);
                }
            }
        }
    }
    if (h < 0.) {
        h = 360. + h;
    }
    if (mx == 0.) {
        s = 0.;
    } else {
        s = c / mx;
    }
    return std::make_tuple(h, s, mx);
}

Image Converter::convertRGBToHSV(const Image &input) {
    Image outputImage = input;
    for (int i = 0; i < input.IMAGE_SIZE; i += 3) {
        double r = (double) input.data[i] / (double) input.pixelSize;
        double g = (double) input.data[i + 1] / (double) input.pixelSize;
        double b = (double) input.data[i + 2] / (double) input.pixelSize;
        std::tuple<double, double, double> hsl = toHSV(r, g, b);
        outputImage.data[i] = std::get<0>(hsl) / 360. * (double) input.pixelSize;
        outputImage.data[i + 1] = std::get<1>(hsl) * (double) input.pixelSize;
        outputImage.data[i + 2] = std::get<2>(hsl) * (double) input.pixelSize;
    }
    return outputImage;
}

Image Converter::convertHSVToRGB(const Image &input) {
    Image outputImage = input;
    for (int i = 0; i < input.IMAGE_SIZE; i += 3) {
        double h = (double) input.data[i] / (double) input.pixelSize * 360.;
        double s = (double) input.data[i + 1] / (double) input.pixelSize;
        double v = (double) input.data[i + 2] / (double) input.pixelSize;
        std::tuple<double, double, double> rgb = fromHSV(h, s, v);
        outputImage.data[i] = std::get<0>(rgb) * (double) input.pixelSize;
        outputImage.data[i + 1] = std::get<1>(rgb) * (double) input.pixelSize;
        outputImage.data[i + 2] = std::get<2>(rgb) * (double) input.pixelSize;
    }
    return outputImage;
}

std::tuple<double, double, double> Converter::toYCbCr601(double r, double g, double b) {
    double y = 16. + 65.481 * r / 255. + 128.553 * g / 255. + 24.966 * b / 255.;
    double cb = 128. - 37.7968 * r / 255. - 74.2032 * g / 255. + 112.0 * b / 255.;
    double cr = 128 + 112.0 * r / 255. - 93.786 * g / 255. - 18.214 * b / 255.;
    return std::make_tuple(y, cb, cr);
}

std::tuple<double, double, double> Converter::fromYCbCr601(double y, double cb, double cr) {
    double r = (255. / 219.) * (y - 16.) + (255. / 224.) * 1.402 * (cr - 128.);
    double g = (255. / 219.) * (y - 16.) - (255. / 224.) * 1.772 * (0.114 / 0.587) * (cb - 128.) -
               (255. / 224.) * 1.402 * (0.299 / 0.587) * (cr - 128.);
    double b = (255. / 219.) * (y - 16.) + (255. / 224.) * 1.772 * (cb - 128.);
    r = fmax(r, 0.);
    g = fmax(g, 0.);
    b = fmax(b, 0.);
    return std::make_tuple(r, g, b);
}

Image Converter::convertRGBToYCbCr601(const Image &input) {
    Image outputImage = input;
    for (int i = 0; i < input.IMAGE_SIZE; i += 3) {
        std::tuple<double, double, double> res = toYCbCr601((double) input.data[i],
                                                            (double) input.data[i + 1],
                                                            (double) input.data[i + 2]);
        outputImage.data[i] = std::get<0>(res);
        outputImage.data[i + 1] = std::get<1>(res);
        outputImage.data[i + 2] = std::get<2>(res);
    }
    return outputImage;
}

Image Converter::convertYCbCr601ToRGB(const Image &input) {
    Image outputImage = input;
    for (int i = 0; i < input.IMAGE_SIZE; i += 3) {
        std::tuple<double, double, double> rgb = fromYCbCr601(input.data[i], input.data[i + 1], input.data[i + 2]);
        outputImage.data[i] = std::get<0>(rgb);
        outputImage.data[i + 1] = std::get<1>(rgb);
        outputImage.data[i + 2] = std::get<2>(rgb);
    }
    return outputImage;
}

std::tuple<double, double, double> Converter::toYCbCr709(double r, double g, double b) {
    double y = 16. + 46.5594 * r / 255. + 156.629 * g / 255. + 15.8118 * b / 255.;
    double cb = 128. - 25.6642 * r / 255. - 86.3358 * g / 255. + 112.0 * b / 255.;
    double cr = 128 + 112.0 * r / 255. - 101.73 * g / 255. - 10.2697 * b / 255.;
    return std::make_tuple(y, cb, cr);
}

std::tuple<double, double, double> Converter::fromYCbCr709(double y, double cb, double cr) {
    double r = (255. / 219.) * (y - 16.) + (255. / 224.) * 1.5748 * (cr - 128.);
    double g = (255. / 219.) * (y - 16.) - (255. / 224.) * 0.187324 * (cb - 128.) -
               (255. / 224.) * 0.468124 * (cr - 128.);
    double b = (255. / 219.) * (y - 16.) + (255. / 224.) * 1.8556 * (cb - 128.);
    r = fmax(r, 0.);
    g = fmax(g, 0.);
    b = fmax(b, 0.);
    return std::make_tuple(r, g, b);
}

Image Converter::convertRGBToYCbCr709(const Image &input) {
    Image outputImage = input;
    for (int i = 0; i < input.IMAGE_SIZE; i += 3) {
        std::tuple<double, double, double> res = toYCbCr709((double) input.data[i],
                                                            (double) input.data[i + 1],
                                                            (double) input.data[i + 2]);
        outputImage.data[i] = std::get<0>(res);
        outputImage.data[i + 1] = std::get<1>(res);
        outputImage.data[i + 2] = std::get<2>(res);
    }
    return outputImage;
}

Image Converter::convertYCbCr709ToRGB(const Image &input) {
    Image outputImage = input;
    for (int i = 0; i < input.IMAGE_SIZE; i += 3) {
        std::tuple<double, double, double> rgb = fromYCbCr709(input.data[i], input.data[i + 1], input.data[i + 2]);
        outputImage.data[i] = std::get<0>(rgb);
        outputImage.data[i + 1] = std::get<1>(rgb);
        outputImage.data[i + 2] = std::get<2>(rgb);
    }
    return outputImage;
}
