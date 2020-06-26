# Adjusts the brightness of the image.
Program arguments: \<input_file_name\> \<output_file_name\> \<mode\> \[\<offset\> \<factor\>\]\

Available color modes:\
0 - apply \<offset\> \<factor\> in color space RGB to every channel.
1 - apply \<offset\> \<factor\> in color space YCbCr.601 to Y channel.
2 - as same as 0 but \<offset\> \<factor\> calculates automatically using min and max values.
3 - as same as 2 but in color space YCbCr.601.
4 - as same as 2 but skips 0.39% of smallest and biggest values when calculating min and max.
5 - as same as 4 but in color space YCbCr.601
Modes 0 and 1 use formula y=(x-\<offset\>)*\<factor\>
