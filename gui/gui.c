
typedef struct rect {
  int x, y, width, height;
} rect;

rect rect_new(int x, int y, int width, int height);
rect rect_intersect(rect a, rect b);
