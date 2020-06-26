# Converts image from one color space to another.
Keys for a program:\
**-f <from_color_space>** - color space to convert from\
**-t <to_color_space>** - color space to convert to\
**-i** **<**count> <input_file_name>** - <input_file_name> is just a filename, count can be 1 or 3.\
If count equals to 1 image will be taken from <input_file_name>.\
If count equals to 3 and <input_file_name> looks like <name.ext> three color space channels will be taken from files <name_1.ext>, <name_2.ext> and <name_3.ext>.\
For example if <input_file_name> equals "sample.pgm" and user wants to convert from RGB, channel R will be taken from "sample_1.pgm", channel B from "sample_2.pgm" and channel G from "sample_3.pgm".\
**-o** **<**count> <output_file_name>** - is the same as -i.

Available color spaces:\
RGB, HSL, HSV, YCbCr.601, YCbCr.709, YCoCg and CMY
