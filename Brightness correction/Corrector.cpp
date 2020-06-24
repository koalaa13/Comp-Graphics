//
// Created by koalaa13 on 6/24/20.
//

#include "Corrector.h"

double Corrector::doCorrection(double d, double maxValue, double offset, double factor) {
    return fmax(0, fmin(maxValue, (d - offset) * factor));
}

double Corrector::doAutoCorrection(double d, double maxValue, double mn, double mx) {
    return fmax(0, fmin(maxValue, (d - mn) * maxValue / (mx - mn)));
}

void Corrector::RGBCorrection(Image &image, double offset, double factor) {
    for (int i = 0; i < image.IMAGE_SIZE; ++i) {
        image.data[i] = doCorrection(image.data[i], image.pixelSize, offset, factor);
    }
}

std::tuple<double, double, double> Corrector::toYCbCr601(double r, double g, double b) {
    double y = 16. + 65.481 * r / 255. + 128.553 * g / 255. + 24.966 * b / 255.;
    double cb = 128. - 37.7968 * r / 255. - 74.2032 * g / 255. + 112.0 * b / 255.;
    double cr = 128 + 112.0 * r / 255. - 93.786 * g / 255. - 18.214 * b / 255.;
    y = (y - 16.) / 219. * 255.;
    cb = (cb - 16.) / 224. * 255.;
    cr = (cr - 16.) / 224. * 255.;
    return std::make_tuple(y, cb, cr);
}

std::tuple<double, double, double> Corrector::fromYCbCr601(double y, double cb, double cr) {
    y = (y / 255.) * 219. + 16.;
    cb = (cb / 255.) * 224. + 16.;
    cr = (cr / 255.) * 224. + 16.;
    double r = (255. / 219.) * (y - 16.) + (255. / 224.) * 1.402 * (cr - 128.);
    double g = (255. / 219.) * (y - 16.) - (255. / 224.) * 1.772 * (0.114 / 0.587) * (cb - 128.) -
               (255. / 224.) * 1.402 * (0.299 / 0.587) * (cr - 128.);
    double b = (255. / 219.) * (y - 16.) + (255. / 224.) * 1.772 * (cb - 128.);
    r = fmin(255., fmax(r, 0.));
    g = fmin(255., fmax(g, 0.));
    b = fmin(255., fmax(b, 0.));
    return std::make_tuple(r, g, b);
}

void Corrector::RGBAutoCorrection(Image &image) {
    double mn = image.pixelSize, mx = 0;
    for (int i = 0; i < image.IMAGE_SIZE; ++i) {
        mn = fmin(mn, image.data[i]);
        mx = fmax(mx, image.data[i]);
    }
    for (int i = 0; i < image.IMAGE_SIZE; ++i) {
        image.data[i] = doAutoCorrection(image.data[i], image.pixelSize, mn, mx);
    }
}

void Corrector::RGBSkipAutoCorrection(Image &image) {
    std::vector<int> cnts(image.pixelSize);
    int skip = (int) (0.0039 * (double) (image.IMAGE_SIZE));
    for (int i = 0; i < image.IMAGE_SIZE; ++i) {
        cnts[image.data[i]]++;
    }
    double mn, mx;
    for (int i = 0, curSkip = skip; i < cnts.size(); ++i) {
        if (cnts[i] <= curSkip) {
            curSkip -= cnts[i];
        } else {
            mn = i;
            break;
        }
    }
    for (int i = (int) cnts.size() - 1, curSkip = skip; i >= 0; --i) {
        if (cnts[i] <= curSkip) {
            curSkip -= cnts[i];
        } else {
            mx = i;
            break;
        }
    }
    for (int i = 0; i < image.IMAGE_SIZE; ++i) {
        image.data[i] = doAutoCorrection(image.data[i], image.pixelSize, mn, mx);
    }
}

void Corrector::YCbCrCorrection(Image &image, double offset, double factor) {
    if (image.BYTES_PER_PIXEL != 3) {
        throw std::runtime_error("Should have PNM P6 image for YCbCr corrections");
    }
    for (int i = 0; i < image.IMAGE_SIZE; i += 3) {
        auto res = toYCbCr601(image.data[i], image.data[i + 1], image.data[i + 2]);
        double y = doCorrection(std::get<0>(res), image.pixelSize, offset, factor);
        auto rgb = fromYCbCr601(y, std::get<1>(res), std::get<2>(res));
        image.data[i] = std::get<0>(rgb);
        image.data[i + 1] = std::get<1>(rgb);
        image.data[i + 2] = std::get<2>(rgb);
    }
}

void Corrector::YCbCrAutoCorrection(Image &image) {
    if (image.BYTES_PER_PIXEL != 3) {
        throw std::runtime_error("Should have PNM P6 image for YCbCr corrections");
    }
    double mn = image.pixelSize, mx = 0;
    for (int i = 0; i < image.IMAGE_SIZE; i += 3) {
        double y = std::get<0>(toYCbCr601(image.data[i], image.data[i + 1], image.data[i + 2]));
        mn = fmin(mn, y);
        mx = fmax(mx, y);
    }
    for (int i = 0; i < image.IMAGE_SIZE; i += 3) {
        auto res = toYCbCr601(image.data[i], image.data[i + 1], image.data[i + 2]);
        double y = doAutoCorrection(std::get<0>(res), image.pixelSize, mn, mx);
        auto rgb = fromYCbCr601(y, std::get<1>(res), std::get<2>(res));
        image.data[i] = std::get<0>(rgb);
        image.data[i + 1] = std::get<1>(rgb);
        image.data[i + 2] = std::get<2>(rgb);
    }
}

void Corrector::YCbCrSkipAutoCorrection(Image &image) {
    if (image.BYTES_PER_PIXEL != 3) {
        throw std::runtime_error("Should have PNM P6 image for YCbCr corrections");
    }
    int skip = (int) (0.0039 * (double) (image.IMAGE_SIZE));
    std::vector<double> ys;
    ys.reserve(image.IMAGE_SIZE / 3);
    for (int i = 0; i < image.IMAGE_SIZE; i += 3) {
        ys.push_back(std::get<0>(toYCbCr601(image.data[i], image.data[i + 1], image.data[i + 2])));
    }
    std::sort(ys.begin(), ys.end());
    double mn = ys[skip], mx = ys[ys.size() - 1 - skip];
    for (int i = 0; i < image.IMAGE_SIZE; i += 3) {
        auto res = toYCbCr601(image.data[i], image.data[i + 1], image.data[i + 2]);
        double y = doAutoCorrection(std::get<0>(res), image.pixelSize, mn, mx);
        auto rgb = fromYCbCr601(y, std::get<1>(res), std::get<2>(res));
        image.data[i] = std::get<0>(rgb);
        image.data[i + 1] = std::get<1>(rgb);
        image.data[i + 2] = std::get<2>(rgb);
    }
}
