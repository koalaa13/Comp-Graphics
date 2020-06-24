//
// Created by koalaa13 on 6/22/20.
//

#include "Dither.h"

void Dither::randomDithering(const PNMImage &image, bool grad, std::string const &output, int bit, double gamma) {
    auto *data = getData(image, grad);
    const int bound = 1e7;
    std::random_device rd;
    std::mt19937 gen(rd());
    ImageFile out(output, "wb");
    image.writeHeaderToFile(out);
    std::vector<double> values = getValues(image.pixelSize, bit);
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            int ind = j + i * image.width;
            double leftValue = getValueWithGamma(getLeft(values, data[ind]), image.pixelSize, gamma);
            double rightValue = getValueWithGamma(getRight(values, data[ind]), image.pixelSize, gamma);
            double dist = rightValue - leftValue;
            auto add = (double) ((double) (gen() % (2 * bound)) - bound) / (double) bound * dist / 2;
            double value = getValueWithGamma(data[ind], image.pixelSize, gamma) + add;
            byte res;
            if (fabs(value - rightValue) < fabs(value - leftValue)) {
                res = (byte) fmin(image.pixelSize, fmax(0., rightValue));
            } else {
                res = (byte) fmin(image.pixelSize, fmax(0., leftValue));
            }
            fwrite(&res, 1, 1, out.getData());
        }
    }
}

std::vector<double> Dither::getValues(int maxValue, int bit) {
    int count = 1 << bit;
    std::vector<double> res;
    res.reserve(count);
    int curCount = maxValue + 1;
    if (count == curCount) {
        for (int i = 0; i <= maxValue; ++i) {
            res.push_back(i);
        }
    } else {
        res.push_back(0);
        curCount -= 2;
        count -= 2;
        int cntParts = count + 1;
        curCount -= count;
        int div = curCount / cntParts, mod = curCount % cntParts;
        for (byte cur = 1; count > 0; --count) {
            cur += div;
            if (mod > 0) {
                cur++;
                mod--;
            }
            res.push_back(cur);
        }
        res.push_back(maxValue);
    }
    return res;
}

void Dither::drawGrad(double *data, PNMImage const &image) noexcept {
    double k = 255. / (image.width - 1);
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            data[j + i * image.width] = (double) j * k;
        }
    }
}

double Dither::getLeft(const std::vector<double> &values, double value) {
    auto it = std::lower_bound(values.begin(), values.end(), value);
    if (*it == value) {
        return value;
    }
    return *(it - 1);
}

double Dither::getRight(const std::vector<double> &values, double value) {
    auto it = std::upper_bound(values.begin(), values.end(), value);
    return it == values.end() ? *values.end() : *it;
}

double Dither::getValueWithGamma(double value, double maxValue, double gamma) {
    if (gamma == 0.) {
        return sRGB(value / maxValue) * maxValue;
    } else {
        return pow(value / maxValue, gamma) * maxValue;
    }
}

double Dither::sRGB(double value) {
    if (value <= 0.04045) {
        return value / 12.92;
    } else {
        return pow((200 * value + 11) / 211, 12. / 5.);
    }
}

void Dither::noDithering(const PNMImage &image, bool grad, const std::string &output, int bit, double gamma) {
    auto *data = getData(image, grad);
    ImageFile out(output, "wb");
    image.writeHeaderToFile(out);
    std::vector<double> values = getValues(image.pixelSize, bit);
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            int ind = j + i * image.width;
            double leftValue = getValueWithGamma(getLeft(values, data[ind]), image.pixelSize, gamma);
            double rightValue = getValueWithGamma(getRight(values, data[ind]), image.pixelSize, gamma);
            double value = getValueWithGamma(data[ind], image.pixelSize, gamma);
            byte res;
            if (fabs(value - rightValue) < fabs(value - leftValue)) {
                res = (byte) fmin(image.pixelSize, fmax(0., rightValue));
            } else {
                res = (byte) fmin(image.pixelSize, fmax(0., leftValue));
            }
            fwrite(&res, 1, 1, out.getData());
        }
    }
}

double *Dither::getData(PNMImage const &image, bool grad) {
    auto *data = (double *) malloc(sizeof(double) * image.IMAGE_SIZE);
    if (data == nullptr) {
        throw std::runtime_error("Not enough memory to dither an image");
    }
    if (grad) {
        drawGrad(data, image);
    } else {
        for (int i = 0; i < image.IMAGE_SIZE; ++i) {
            data[i] = (double) image.data[i];
        }
    }
    return data;
}

void
Dither::FloydSteinbergDithering(const PNMImage &image, bool grad, const std::string &output, int bit, double gamma) {
    auto *data = getData(image, grad);
    ImageFile out(output, "wb");
    image.writeHeaderToFile(out);
    std::vector<double> values = getValues(image.pixelSize, bit), error0(image.width), error1(image.width);
    double one = 1. / 16., three = 3. / 16., five = 5. / 16., seven = 7. / 16.;
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            int ind = j + i * image.width;
            double leftValue = getValueWithGamma(getLeft(values, data[ind]), image.pixelSize, gamma);
            double rightValue = getValueWithGamma(getRight(values, data[ind]), image.pixelSize, gamma);
            double value = getValueWithGamma(data[ind], image.pixelSize, gamma) + error0[j];
            byte res;
            double error;
            if (fabs(value - rightValue) < fabs(value - leftValue)) {
                error = value - rightValue;
                res = (byte) fmin(image.pixelSize, fmax(0., rightValue));
            } else {
                error = value - leftValue;
                res = (byte) fmin(image.pixelSize, fmax(0., leftValue));
            }
            fwrite(&res, 1, 1, out.getData());
            error1[j] += error * five;
            if (j + 1 < image.width) {
                error0[j + 1] += error * seven;
                error1[j + 1] += error * one;
            }
            if (j - 1 > 0) {
                error1[j - 1] += error * three;
            }
        }
        error0.swap(error1);
        std::fill(error1.begin(), error1.end(), 0);
    }
}

void
Dither::JarvisJudiceNinkeDithering(const PNMImage &image, bool grad, const std::string &output, int bit, double gamma) {
    auto *data = getData(image, grad);
    ImageFile out(output, "wb");
    image.writeHeaderToFile(out);
    std::vector<double> values = getValues(image.pixelSize, bit), error0(image.width), error1(image.width), error2(
            image.width);
    double one = 1. / 48., three = 3. / 48., five = 5. / 48., seven = 7. / 48.;
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            int ind = j + i * image.width;
            double leftValue = getValueWithGamma(getLeft(values, data[ind]), image.pixelSize, gamma);
            double rightValue = getValueWithGamma(getRight(values, data[ind]), image.pixelSize, gamma);
            double value = getValueWithGamma(data[ind], image.pixelSize, gamma) + error0[j];
            byte res;
            double error;
            if (fabs(value - rightValue) < fabs(value - leftValue)) {
                error = value - rightValue;
                res = (byte) fmin(image.pixelSize, fmax(0., rightValue));
            } else {
                error = value - leftValue;
                res = (byte) fmin(image.pixelSize, fmax(0., leftValue));
            }
            fwrite(&res, 1, 1, out.getData());
            error1[j] += error * seven;
            error2[j] += error * five;
            if (j + 1 < image.width) {
                error0[j + 1] += error * seven;
                error1[j + 1] += error * five;
                error2[j + 1] += error * three;
            }
            if (j + 2 < image.width) {
                error0[j + 2] += error * five;
                error1[j + 2] += error * three;
                error2[j + 2] += error * one;
            }
            if (j - 1 > 0) {
                error1[j - 1] += error * five;
                error2[j - 1] += error * three;
            }
            if (j - 2 > 0) {
                error1[j - 2] += error * three;
                error2[j - 2] += error * one;
            }
        }
        error0 = error1;
        error1 = error2;
        std::fill(error2.begin(), error2.end(), 0);
    }
}

void Dither::Sierra3Dithering(const PNMImage &image, bool grad, const std::string &output, int bit, double gamma) {
    auto *data = getData(image, grad);
    ImageFile out(output, "wb");
    image.writeHeaderToFile(out);
    std::vector<double> values = getValues(image.pixelSize, bit), error0(image.width), error1(image.width), error2(
            image.width);
    double one = 1. / 32., two = 2. / 32., three = 3. / 32., four = 4. / 32., five = 5. / 32.;
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            int ind = j + i * image.width;
            double leftValue = getValueWithGamma(getLeft(values, data[ind]), image.pixelSize, gamma);
            double rightValue = getValueWithGamma(getRight(values, data[ind]), image.pixelSize, gamma);
            double value = getValueWithGamma(data[ind], image.pixelSize, gamma) + error0[j];
            byte res;
            double error;
            if (fabs(value - rightValue) < fabs(value - leftValue)) {
                error = value - rightValue;
                res = (byte) fmin(image.pixelSize, fmax(0., rightValue));
            } else {
                error = value - leftValue;
                res = (byte) fmin(image.pixelSize, fmax(0., leftValue));
            }
            fwrite(&res, 1, 1, out.getData());
            error1[j] += error * five;
            error2[j] += error * three;
            if (j + 1 < image.width) {
                error0[j + 1] += error * five;
                error1[j + 1] += error * four;
                error2[j + 1] += error * two;
            }
            if (j + 2 < image.width) {
                error0[j + 2] += error * three;
                error1[j + 2] += error * two;
            }
            if (j - 1 > 0) {
                error1[j - 1] += error * four;
                error2[j - 1] += error * two;
            }
            if (j - 2 > 0) {
                error1[j - 2] += error * two;
            }
        }
        error0 = error1;
        error1 = error2;
        std::fill(error2.begin(), error2.end(), 0);
    }
}

void Dither::AtkinsonDithering(const PNMImage &image, bool grad, const std::string &output, int bit, double gamma) {
    auto *data = getData(image, grad);
    ImageFile out(output, "wb");
    image.writeHeaderToFile(out);
    std::vector<double> values = getValues(image.pixelSize, bit), error0(image.width), error1(image.width), error2(
            image.width);
    double one = 1. / 8.;
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            int ind = j + i * image.width;
            double leftValue = getValueWithGamma(getLeft(values, data[ind]), image.pixelSize, gamma);
            double rightValue = getValueWithGamma(getRight(values, data[ind]), image.pixelSize, gamma);
            double value = getValueWithGamma(data[ind], image.pixelSize, gamma) + error0[j];
            byte res;
            double error;
            if (fabs(value - rightValue) < fabs(value - leftValue)) {
                error = value - rightValue;
                res = (byte) fmin(image.pixelSize, fmax(0., rightValue));
            } else {
                error = value - leftValue;
                res = (byte) fmin(image.pixelSize, fmax(0., leftValue));
            }
            fwrite(&res, 1, 1, out.getData());
            error1[j] += error * one;
            error2[j] += error * one;
            if (j + 1 < image.width) {
                error0[j + 1] += error * one;
                error1[j + 1] += error * one;
            }
            if (j + 2 < image.width) {
                error0[j + 2] += error * one;
            }
            if (j - 1 > 0) {
                error1[j - 1] += error * one;
            }
        }
        error0 = error1;
        error1 = error2;
        std::fill(error2.begin(), error2.end(), 0);
    }
}

void Dither::ordered8x8Dithering(PNMImage const &image, bool grad, std::string const &output, int bit, double gamma) {
    const int N = 8;
    std::vector<std::vector<double>> matrix(N);
    matrix[0] = {0, 48, 12, 60, 3, 51, 15, 63};
    matrix[1] = {32, 16, 44, 28, 35, 19, 47, 31};
    matrix[2] = {8, 56, 4, 52, 11, 59, 7, 55};
    matrix[3] = {40, 24, 36, 20, 43, 27, 39, 23};
    matrix[4] = {2, 50, 14, 62, 1, 49, 13, 61};
    matrix[5] = {34, 18, 46, 30, 33, 17, 45, 29};
    matrix[6] = {10, 58, 6, 54, 9, 57, 5, 53};
    matrix[7] = {42, 26, 38, 22, 41, 25, 37, 21};
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = (matrix[i][j] + 0.5) / (double) (N * N) * (double) image.pixelSize;
        }
    }
    auto *data = getData(image, grad);
    ImageFile out(output, "wb");
    image.writeHeaderToFile(out);
    std::vector<double> values = getValues(image.pixelSize, bit);
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            int ind = j + i * image.width;
            double leftValue = getValueWithGamma(getLeft(values, data[ind]), image.pixelSize, gamma);
            double rightValue = getValueWithGamma(getRight(values, data[ind]), image.pixelSize, gamma);
            double value = getValueWithGamma(data[ind], image.pixelSize, gamma);
            byte res;
            if (value > matrix[j % N][i % N]) {
                res = (byte) fmin(image.pixelSize, fmax(0., rightValue));
            } else {
                res = (byte) fmin(image.pixelSize, fmax(0., leftValue));
            }
            fwrite(&res, 1, 1, out.getData());
        }
    }
}

void Dither::halftone4x4Dithering(const PNMImage &image, bool grad, const std::string &output, int bit, double gamma) {
    const int N = 4;
    std::vector<std::vector<double>> matrix(N);
    matrix[0] = {7, 13, 11, 4};
    matrix[1] = {12, 16, 14, 8};
    matrix[2] = {10, 15, 6, 2};
    matrix[3] = {5, 9, 3, 1};
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = (matrix[i][j] + 0.5) / (double) (N * N) * (double) image.pixelSize;
        }
    }
    auto *data = getData(image, grad);
    ImageFile out(output, "wb");
    image.writeHeaderToFile(out);
    std::vector<double> values = getValues(image.pixelSize, bit);
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            int ind = j + i * image.width;
            double leftValue = getValueWithGamma(getLeft(values, data[ind]), image.pixelSize, gamma);
            double rightValue = getValueWithGamma(getRight(values, data[ind]), image.pixelSize, gamma);
            double value = getValueWithGamma(data[ind], image.pixelSize, gamma);
            byte res;
            if (value > matrix[j % N][i % N]) {
                res = (byte) fmin(image.pixelSize, fmax(0., rightValue));
            } else {
                res = (byte) fmin(image.pixelSize, fmax(0., leftValue));
            }
            fwrite(&res, 1, 1, out.getData());
        }
    }
}
