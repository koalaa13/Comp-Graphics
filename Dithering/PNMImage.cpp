//
// Created by koalaa13 on 3/28/20.
//

#include "PNMImage.h"

PNMImage::PNMImage(const ImageFile &imageFile, bool grad) {
    data = nullptr;
    char format, trash;
    int imageType;
    int readed = fscanf(imageFile.getData(), "%c%d%d%d%d%c", &format, &imageType, &width, &height, &pixelSize, &trash);
    if (readed != 6 || format != 'P' || imageType != 5 || width <= 0 || height <= 0 || pixelSize != 255) {
        throw std::runtime_error("Incorrect image format");
    }
    IMAGE_SIZE = width * height;
    data = (byte *) malloc(sizeof(byte) * IMAGE_SIZE);
    if (data == nullptr) {
        throw std::runtime_error("Not enough memory to open image");
    }
    if (!grad) {
        readed = fread(data, 1, IMAGE_SIZE, imageFile.getData());
        if (readed != IMAGE_SIZE) {
            throw std::runtime_error("Error while trying to read an image");
        }
    } else {
        fillHorizontalGrad();
    }
}

PNMImage::~PNMImage() {
    if (data != nullptr) {
        free(data);
    }
}

void PNMImage::writeToFile(const ImageFile &imageFile) {
    fprintf(imageFile.getData(), "P5\n%d %d\n%d\n", width, height, pixelSize);
    if (ferror(imageFile.getData())) {
        throw std::runtime_error("Something happened while writing result to output file");
    }
    fwrite(data, sizeof(byte), IMAGE_SIZE, imageFile.getData());
    if (ferror(imageFile.getData())) {
        throw std::runtime_error("Something happened while writing result to output file");
    }
}

void PNMImage::fillHorizontalGrad() {
    int partSize = width / pixelSize, left = width % pixelSize;
    int colAdd = partSize == 0 ? pixelSize / width : 1, colAddLeft = partSize == 0 ? pixelSize % width : 0;
    for (int i = 0, fill = 0, curCol = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            int ind = j * width + i;
            data[ind] = curCol;
        }
        fill++;
        int curPartSize = partSize + (left > 0);
        if (fill == curPartSize) {
            fill = 0;
            left--;
            curCol += colAdd + (colAddLeft > 0);
            colAddLeft--;
        }
    }
}

void PNMImage::ditheringFloydSteinberg(int bit, double gamma) {
    std::vector<byte> values = getValues(bit);
    std::vector<int> error0(width), error1(width);
    double five = 5. / 16., three = 3. / 16., seven = 7. / 16., one = 1. / 16.;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int ind = j + i * width;
            byte curValue = getValueWithGamma(std::min((int) (data[ind]) + error0[j], pixelSize), gamma), newValue;
            int newValueInd = std::lower_bound(values.begin(), values.end(), curValue) - values.begin();
            byte rightValue = values[newValueInd];
            double err;
            if (rightValue == curValue) {
                err = 0;
                newValue = rightValue;
            } else {
                byte leftValue = values[newValueInd - 1];
                if (curValue - leftValue < rightValue - curValue) {
                    newValue = leftValue;
                } else {
                    newValue = rightValue;
                }
                err = curValue - newValue;
            }
            setPixelColor(j, i, newValue);
            error1[j] += five * err;
            if (j - 1 >= 0) {
                error1[j - 1] += three * err;
            }
            if (j + 1 < width) {
                error0[j + 1] += seven * err;
                error1[j + 1] += one * err;
            }
        }
        error1.swap(error0);
        std::fill(error1.begin(), error1.end(), 0);
    }
}

std::vector<byte> PNMImage::getValues(int bit) const {
    int count = 1 << bit;
    std::vector<byte> res;
    res.reserve(count);
    int curCount = pixelSize + 1;
    if (count == curCount) {
        for (int i = 0; i <= pixelSize; ++i) {
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
        res.push_back(pixelSize);
    }
    return res;
}

void PNMImage::setPixelColor(int x, int y, byte value) {
    if (x < 0 || x > width || y < 0 || y > height) {
        return;
    }
    int ind = x + y * width;
    data[ind] = value;
}

void PNMImage::ditheringJarvisJudiceNinke(int bit, double gamma) {
    std::vector<byte> values = getValues(bit);
    std::vector<int> error0(width), error1(width), error2(width);
    double one = 1. / 48., three = 3. / 48., five = 5. / 48., seven = 7. / 48.;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int ind = j + i * width;
            byte curValue = getValueWithGamma(std::min(data[ind] + error0[j], pixelSize), gamma), newValue;
            int newValueInd = std::lower_bound(values.begin(), values.end(), curValue) - values.begin();
            byte rightValue = values[newValueInd];
            int err;
            if (rightValue == curValue) {
                err = 0;
                newValue = rightValue;
            } else {
                byte leftValue = values[newValueInd - 1];
                if (curValue - leftValue < rightValue - curValue) {
                    newValue = leftValue;
                } else {
                    newValue = rightValue;
                }
                err = curValue - newValue;
            }
            setPixelColor(j, i, newValue);
            error1[j] += seven * err;
            error2[j] += five * err;
            if (j + 1 < width) {
                error0[j + 1] += seven * err;
                error1[j + 1] += five * err;
                error2[j + 1] += three * err;
            }
            if (j + 2 < width) {
                error0[j + 2] += five * err;
                error1[j + 2] += three * err;
                error2[j + 2] += one * err;
            }
            if (j - 1 >= 0) {
                error1[j - 1] += five * err;
                error2[j - 1] += three * err;
            }
            if (j - 2 >= 0) {
                error1[j - 2] += three * err;
                error2[j - 2] += one * err;
            }
        }
        error0 = error1;
        error1 = error2;
        std::fill(error2.begin(), error2.end(), 0);
    }
}

void PNMImage::randomDithering(int bit, double gamma) {
    std::random_device rd;
    std::mt19937 gen(rd());
    int bound = (1 << (9 - bit));
    std::vector<byte> values = getValues(bit);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int ind = j + i * width;
            unsigned long add = gen() % bound;
            byte curValue = getValueWithGamma(std::min((int) (data[ind] + add), pixelSize), gamma), newValue;
            int newValueInd = std::lower_bound(values.begin(), values.end(), curValue) - values.begin();
            byte rightValue = values[newValueInd];
            if (rightValue == curValue) {
                newValue = rightValue;
            } else {
                byte leftValue = values[newValueInd - 1];
                if (curValue - leftValue < rightValue - curValue) {
                    newValue = leftValue;
                } else {
                    newValue = rightValue;
                }
            }
            setPixelColor(j, i, newValue);
        }
    }
}

void PNMImage::noDithering(int bit, double gamma) {
    std::vector<byte> values = getValues(bit);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int ind = j + i * width;
            byte curValue = getValueWithGamma(data[ind], gamma), newValue;
            int newValueInd = std::lower_bound(values.begin(), values.end(), curValue) - values.begin();
            byte rightValue = values[newValueInd];
            if (rightValue == curValue) {
                newValue = rightValue;
            } else {
                byte leftValue = values[newValueInd - 1];
                if (curValue - leftValue < rightValue - curValue) {
                    newValue = leftValue;
                } else {
                    newValue = rightValue;
                }
            }
            setPixelColor(j, i, newValue);
        }
    }
}

void PNMImage::ditheringSierra3(int bit, double gamma) {
    std::vector<byte> values = getValues(bit);
    std::vector<int> error0(width), error1(width), error2(width);
    double two = 2. / 32., three = 3. / 32., four = 4. / 32., five = 5. / 32.;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int ind = j + i * width;
            byte curValue = getValueWithGamma(std::min(data[ind] + error0[j], pixelSize), gamma), newValue;
            int newValueInd = std::lower_bound(values.begin(), values.end(), curValue) - values.begin();
            byte rightValue = values[newValueInd];
            double err;
            if (rightValue == curValue) {
                err = 0;
                newValue = rightValue;
            } else {
                byte leftValue = values[newValueInd - 1];
                if (curValue - leftValue < rightValue - curValue) {
                    newValue = leftValue;
                } else {
                    newValue = rightValue;
                }
                err = curValue - newValue;
            }
            setPixelColor(j, i, newValue);
            error1[j] += five * err;
            error2[j] += three * err;
            if (j + 1 < width) {
                error0[j + 1] += five * err;
                error1[j + 1] += four * err;
                error2[j + 1] += two * err;
            }
            if (j + 2 < width) {
                error0[j + 2] += three * err;
                error1[j + 2] += two * err;
            }
            if (j - 1 >= 0) {
                error1[j - 1] += four * err;
                error2[j - 1] += two * err;
            }
            if (j - 2 >= 0) {
                error1[j - 2] += two * err;
            }
        }
        error0 = error1;
        error1 = error2;
        std::fill(error2.begin(), error2.end(), 0);
    }
}

void PNMImage::ditheringAtkinson(int bit, double gamma) {
    std::vector<byte> values = getValues(bit);
    std::vector<int> error0(width), error1(width), error2(width);
    double one = 1. / 8.;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int ind = j + i * width;
            byte curValue = getValueWithGamma(std::min(data[ind] + error0[j], pixelSize), gamma), newValue;
            int newValueInd = std::lower_bound(values.begin(), values.end(), curValue) - values.begin();
            byte rightValue = values[newValueInd];
            double err;
            if (rightValue == curValue) {
                err = 0;
                newValue = rightValue;
            } else {
                byte leftValue = values[newValueInd - 1];
                if (curValue - leftValue < rightValue - curValue) {
                    newValue = leftValue;
                } else {
                    newValue = rightValue;
                }
                err = curValue - newValue;
            }
            setPixelColor(j, i, newValue);
            error1[j] += one * err;
            error2[j] += one * err;
            if (j + 1 < width) {
                error0[j + 1] += one * err;
                error1[j + 1] += one * err;
            }
            if (j + 2 < width) {
                error0[j + 2] += one * err;
            }
            if (j - 1 >= 0) {
                error1[j - 1] += one * err;
            }
        }
        error0 = error1;
        error1 = error2;
        std::fill(error2.begin(), error2.end(), 0);
    }
}

void PNMImage::orderedDithering(int bit, double gamma) {
    const int N = 8;
    std::vector<std::vector<byte>> matrix(N);
    matrix[0] = {0, 48, 12, 60, 3, 51, 15, 63};
    matrix[1] = {32, 16, 44, 28, 35, 19, 47, 31};
    matrix[2] = {8, 56, 4, 52, 11, 59, 7, 55};
    matrix[3] = {40, 24, 36, 20, 43, 27, 39, 23};
    matrix[4] = {2, 50, 14, 62, 1, 49, 13, 61};
    matrix[5] = {34, 18, 46, 30, 33, 17, 45, 29};
    matrix[6] = {10, 58, 6, 54, 9, 57, 5, 53};
    matrix[7] = {42, 26, 38, 22, 41, 25, 37, 21};
    std::vector<byte> values = getValues(bit);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int ind = j + i * width;
            byte curValue = getValueWithGamma(std::min(pixelSize, data[ind] + matrix[i % N][j % N]), gamma), newValue;
            int newValueInd = std::lower_bound(values.begin(), values.end(), curValue) - values.begin();
            byte rightValue = values[newValueInd];
            if (rightValue == curValue) {
                newValue = rightValue;
            } else {
                byte leftValue = values[newValueInd - 1];
                if (curValue - leftValue < rightValue - curValue) {
                    newValue = leftValue;
                } else {
                    newValue = rightValue;
                }
            }
            setPixelColor(j, i, newValue);
        }
    }
}

void PNMImage::halftone4x4Dithering(int bit, double gamma) {
    const int N = 4;
    std::vector<std::vector<byte>> matrix(N);
    matrix[0] = {7, 13, 11, 4};
    matrix[1] = {12, 16, 14, 8};
    matrix[2] = {10, 15, 6, 2};
    matrix[3] = {5, 9, 3, 1};
    std::vector<byte> values = getValues(bit);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int ind = j + i * width;
            byte curValue = getValueWithGamma(std::min(pixelSize, data[ind] + matrix[i % N][j % N]), gamma), newValue;
            int newValueInd = std::lower_bound(values.begin(), values.end(), curValue) - values.begin();
            byte rightValue = values[newValueInd];
            if (rightValue == curValue) {
                newValue = rightValue;
            } else {
                byte leftValue = values[newValueInd - 1];
                if (curValue - leftValue < rightValue - curValue) {
                    newValue = leftValue;
                } else {
                    newValue = rightValue;
                }
            }
            setPixelColor(j, i, newValue);
        }
    }
}

byte PNMImage::getValueWithGamma(byte value, double gamma) const {
    return pow(((double) value) / ((double) pixelSize), gamma) * pixelSize;
}


