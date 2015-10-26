#ifndef _ED_LINES_LIB_H_
#define _ED_LINES_LIB_H_

struct LS {
  double sx, sy, ex, ey; // Start & end coordinates of the line segment
};

LS *DetectLinesByED(unsigned char *srcImg, int width, int height, int *pNoLS);

#endif