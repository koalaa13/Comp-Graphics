# Draw a line on PNM(P5) format images.
Argument for program: \<input filename\> \<output filename\> \<gradient\> \<dithering mode\> \<bits per pixel\> \<gamma\>.\
If gamma equals to 0 sRGB will be used.\

Gradient argument:\
0 - take an input image\
1 - take width and height from input image and draw horizontal gradient instead\
\
Available dithering modes:\
0 - no dithering\
1 - ordered 8x8 dithering\
2 - random dithering\
3 - Floyd Steinberg dithering\
4 - Jarvis Judice Ninke dithering\
5 - Sierra(Sierra-3) dithering\
6 - Atkinson dithering\
7 - halftone 4x4 orthogonal dithering\

