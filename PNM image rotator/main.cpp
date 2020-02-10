#include <cstdio>
#include <string>
#include <stdexcept>

typedef unsigned char byte;

int PIXEL_SIZE;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        throw std::runtime_error("You should enter input filename, output filename and type of conversion");
    }
    char *input_filename = argv[1];
    char *output_filename = argv[2];
    int conversion_type = -1;
    try {
        conversion_type = std::stoi(argv[3]);
    } catch (std::invalid_argument const &e) {
        throw std::runtime_error("Conversion type should be a number");
    } catch (std::out_of_range const &e) {
        throw std::runtime_error("Conversion type is too big");
    }
    if (conversion_type < 0 || conversion_type > 4) {
        throw std::runtime_error("Conversion type should be in [0-4]");
    }

    FILE *image_file = fopen(input_filename, "rb");
    if (image_file == nullptr) {
        throw std::runtime_error("Can't open an image");
    }
    char format, trash;
    int image_type = 0, width = 0, height = 0, size = 0;
    int readed = fscanf(image_file, "%c%d%d%d%d%c", &format, &image_type, &width, &height, &size, &trash);
    if (readed != 6 || format != 'P' || image_type > 6 || image_type < 5 || width <= 0 || height <= 0 || size != 255) {
        fclose(image_file);
        throw std::runtime_error("Incorrect image format");
    }
    if (image_type == 5) {
        PIXEL_SIZE = 1;
    } else {
        PIXEL_SIZE = 3;
    }
    int read_count = PIXEL_SIZE * width * height;
    byte *image = (byte *) malloc(sizeof(byte) * read_count);
    if (image == nullptr) {
        fclose(image_file);
        throw std::runtime_error("Not enough memory");
    }
    readed = fread(image, 1, read_count, image_file);
    if (readed != read_count) {
        free(image);
        fclose(image_file);
        throw std::runtime_error("Error while trying to read an image");
    }
    fclose(image_file);

    FILE *output_file = fopen(output_filename, "wb");
    if (conversion_type == 0) {
        fprintf(output_file, "P%d\n%d %d\n%d\n", image_type, width, height, size);
        if (ferror(output_file)) {
            free(image);
            throw std::runtime_error("Something happened while converting");
        }
        for (int i = 0; i < readed; ++i) {
            image[i] = ~image[i];
        }
        fwrite(image, sizeof(byte), readed, output_file);
        if (ferror(output_file)) {
            free(image);
            throw std::runtime_error("Something happened while converting");
        }
    }
    if (conversion_type == 1) {
        fprintf(output_file, "P%d\n%d %d\n%d\n", image_type, width, height, size);
        if (ferror(output_file)) {
            free(image);
            throw std::runtime_error("Something happened while converting");
        }
        for (int i = 0, j = height - 1; i < j; ++i, --j) {
            for (int k = 0; k < width; ++k) {
                int f = k * PIXEL_SIZE + i * width * PIXEL_SIZE;
                int s = k * PIXEL_SIZE + j * width * PIXEL_SIZE;
                for (int z = 0; z < PIXEL_SIZE; ++z) {
                    std::swap(image[f + z], image[s + z]);
                }
            }
        }
        fwrite(image, sizeof(byte), readed, output_file);
        if (ferror(output_file)) {
            free(image);
            throw std::runtime_error("Something happened while converting");
        }
    }
    if (conversion_type == 2) {
        fprintf(output_file, "P%d\n%d %d\n%d\n", image_type, width, height, size);
        if (ferror(output_file)) {
            free(image);
            throw std::runtime_error("Something happened while converting");
        }
        for (int i = 0; i < height; ++i) {
            for (int j = 0, k = width - 1; j < k; ++j, --k) {
                int f = j * PIXEL_SIZE + i * width * PIXEL_SIZE;
                int s = k * PIXEL_SIZE + i * width * PIXEL_SIZE;
                for (int z = 0; z < PIXEL_SIZE; ++z) {
                    std::swap(image[f + z], image[s + z]);
                }
            }
        }
        fwrite(image, sizeof(byte), readed, output_file);
        if (ferror(output_file)) {
            free(image);
            throw std::runtime_error("Something happened while converting");
        }
    }
    if (conversion_type == 3) {
        fprintf(output_file, "P%d\n%d %d\n%d\n", image_type, height, width, size);
        if (ferror(output_file)) {
            free(image);
            throw std::runtime_error("Something happened while converting");
        }
        byte *image_res = (byte *) malloc(sizeof(byte) * read_count);
        if (image_res == nullptr) {
            free(image);
            throw std::runtime_error("Out of memory while converting");
        }
        int p = 0;
        for (int i = 0; i < width; ++i) {
            for (int j = height - 1; j >= 0; --j) {
                int f = i * PIXEL_SIZE + j * width * PIXEL_SIZE;
                for (int z = 0; z < PIXEL_SIZE; ++z) {
                    image_res[p++] = image[f + z];
                }
            }
        }
        fwrite(image_res, sizeof(byte), readed, output_file);
        if (ferror(output_file)) {
            free(image);
            free(image_res);
            throw std::runtime_error("Something happened while converting");
        }
        free(image_res);
    }
    if (conversion_type == 4) {
        fprintf(output_file, "P%d\n%d %d\n%d\n", image_type, height, width, size);
        if (ferror(output_file)) {
            free(image);
            throw std::runtime_error("Something happened while converting");
        }
        byte *image_res = (byte *) malloc(sizeof(byte) * read_count);
        if (image_res == nullptr) {
            free(image);
            throw std::runtime_error("Out of memory while converting");
        }
        int p = 0;
        for (int i = width - 1; i >= 0; --i) {
            for (int j = 0; j < height; ++j) {
                int f = i * PIXEL_SIZE + j * width * PIXEL_SIZE;
                for (int z = 0; z < PIXEL_SIZE; ++z) {
                    image_res[p++] = image[f + z];
                }
            }
        }
        fwrite(image_res, sizeof(byte), readed, output_file);
        if (ferror(output_file)) {
            free(image);
            free(image_res);
            throw std::runtime_error("Something happened while converting");
        }
        free(image_res);
    }
    fclose(output_file);
    free(image);

    return 0;
}