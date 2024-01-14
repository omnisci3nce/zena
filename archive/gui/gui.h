#ifndef GUI_H
#define GUI_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_ELEMENTS 64

// ----- core

// the below struct inspired by vurtun's comment on Twitter

/* boundary in a direction X/Y */
struct gui_bnd {
  int min, mid, max, ext;
};

/* base class that all gui things inherit from.
 * defines core behaviour and properties of gui
 * elements. */
struct element {
  uint32_t id;
  bool visible;
  bool focused;
};
typedef struct element element;

struct gui_ctx {
  struct element elements[MAX_ELEMENTS];
  bool running;
};
typedef struct gui_ctx gui_ctx;

void gui_draw(gui_ctx* ctx);

// ----- components

// component types
// 1. layout          - doesn't render any pixels directly but influences the layout
//                      of child elements
// 2. box             - like a <div> it's a glorified styleable rectangle
// 3. text            - render text
// 4. input           - interactable components
// 5. multimedia      - embed images

// layout components

// similar to flexbox row
struct hbox {
  element el;
};

struct gui_box {
  element el;
  // bounding box / rect
  struct gui_bnd x;
  struct gui_bnd y;
};

// ----- theme

#endif // !GUI_H
