
#ifndef SQUARE_RIGHT_OF_LINE_TRAVERSER_H
#define SQUARE_RIGHT_OF_LINE_TRAVERSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "square_right_of_line.h"

typedef bool (*SquareRightOfLineTraverserPreOrderCallback)(void* user_data, int32_t square_x, int32_t square_y, int32_t square_width,
    int32_t square_width_log2, bool complete_hit);
void SquareRightOfLineTraversePreOrder(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t starting_x, int32_t starting_y, int32_t starting_width_log2,
    SquareRightOfLineTraverserPreOrderCallback callback, void* user_data);

#ifdef __cplusplus
}
#endif

#endif // SQUARE_RIGHT_OF_LINE_TRAVERSER_H
