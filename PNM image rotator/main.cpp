#include <cstdio>
#include <string>
#include <stdexcept>

typedef unsigned char byte;

void printError(const std::string &msg) {
    fprintf(stderr, "%s", msg.c_str());
    exit(1);
}

struct ImageFile {
    explicit ImageFile(char *filename, const char *mode) {
        data = fopen(filename, mode);
        if (data == nullptr) {
            printError("Can't open an image file");
        }
    }

    ~ImageFile() {
        fclose(data);
    }

    FILE *getData() const {
        return data;
    }

private:
    FILE *data;
};

struct Image {
    explicit Image(const ImageFile &imageFile) {
        data = nullptr;
        char format, trash;
        int imageType;
        int readed = fscanf(imageFile.getData(), "%c%d%d%d%d%c", &format, &imageType, &width, &height, &size, &trash);
        if (readed != 6 || format != 'P' || imageType > 6 || imageType < 5 || width <= 0 || height <= 0 ||
            size != 255) {
            printError("Incorrect image format");
        }
        PIXEL_SIZE = (imageType == 5) ? 1 : 3;
        IMAGE_SIZE = PIXEL_SIZE * width * height;
        data = (byte *) malloc(sizeof(byte) * IMAGE_SIZE);
        if (data == nullptr) {
            printError("Not enough memory to open image");
        }
        readed = fread(data, 1, IMAGE_SIZE, imageFile.getData());
        if (readed != IMAGE_SIZE) {
            printError("Error while trying to read an image");
        }
    }

    ~Image() {
        if (data != nullptr) {
            free(data);
        }
    }

    void inversion() {
        for (int i = 0; i < IMAGE_SIZE; ++i) {
            data[i] = ~data[i];
        }
    }

    void horizontal() {
        for (int i = 0; i < height; ++i) {
            for (int j = 0, k = width - 1; j < k; ++j, --k) {
                int f = j * PIXEL_SIZE + i * width * PIXEL_SIZE;
                int s = k * PIXEL_SIZE + i * width * PIXEL_SIZE;
                for (int z = 0; z < PIXEL_SIZE; ++z) {
                    std::swap(data[f + z], data[s + z]);
                }
            }
        }
    }

    void vertical() {
        for (int i = 0, j = height - 1; i < j; ++i, --j) {
            for (int k = 0; k < width; ++k) {
                int f = k * PIXEL_SIZE + i * width * PIXEL_SIZE;
                int s = k * PIXEL_SIZE + j * width * PIXEL_SIZE;
                for (int z = 0; z < PIXEL_SIZE; ++z) {
                    std::swap(data[f + z], data[s + z]);
                }
            }
        }
    }

    void clockwise() {
        byte *buff = (byte *) malloc(sizeof(byte) * IMAGE_SIZE);
        if (buff == nullptr) {
            printError("Out of memory while converting");
        }
        size_t p = 0;
        for (int i = 0; i < width; ++i) {
            for (int j = height - 1; j >= 0; --j) {
                int f = i * PIXEL_SIZE + j * width * PIXEL_SIZE;
                for (int z = 0; z < PIXEL_SIZE; ++z) {
                    buff[p++] = data[f + z];
                }
            }
        }
        std::swap(data, buff);
        std::swap(width, height);
        free(buff);
    }

    void counterClockwise() {
        byte *buff = (byte *) malloc(sizeof(byte) * IMAGE_SIZE);
        if (buff == nullptr) {
            printError("Out of memory while converting");
        }
        size_t p = 0;
        for (int i = width - 1; i >= 0; --i) {
            for (int j = 0; j < height; ++j) {
                int f = i * PIXEL_SIZE + j * width * PIXEL_SIZE;
                for (int z = 0; z < PIXEL_SIZE; ++z) {
                    buff[p++] = data[f + z];
                }
            }
        }
        std::swap(data, buff);
        std::swap(height, width);
        free(buff);
    }

    void writeToFile(const ImageFile &imageFile) {
        fprintf(imageFile.getData(), "P%d\n%d %d\n%d\n", PIXEL_SIZE == 1 ? 5 : 6, width, height, size);
        if (ferror(imageFile.getData())) {
            printError("Something happened while writing result to output file");
        }
        fwrite(data, sizeof(byte), IMAGE_SIZE, imageFile.getData());
        if (ferror(imageFile.getData())) {
            printError("Something happened while writing result to output file");
        }
    }

private:
    int width, height, size, PIXEL_SIZE, IMAGE_SIZE;
    byte *data;
};

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printError("You should enter input filename, output filename and type of conversion");
    }
    int conversionType = -1;
    try {
        conversionType = std::stoi(argv[3]);
    } catch (std::invalid_argument const &e) {
        printError("Conversion type should be a number");
    } catch (std::out_of_range const &e) {
        printError("Conversion type is too big");
    }
    if (conversionType < 0 || conversionType > 4) {
        printError("Conversion type should be in [0-4]");
    }

    ImageFile imageFile(argv[1], "rb");
    Image image(imageFile);
    ImageFile resultImageFile(argv[2], "wb");
    if (conversionType == 0) {
        image.inversion();
    }
    if (conversionType == 1) {
        image.horizontal();
    }
    if (conversionType == 2) {
        image.vertical();
    }
    if (conversionType == 3) {
        image.clockwise();
    }
    if (conversionType == 4) {
        image.counterClockwise();
    }
    image.writeToFile(resultImageFile);
    return 0;
}