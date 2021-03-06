
#ifndef _LINE_SQUARE_INTERSECTION_TRAVERSER_H
#define _LINE_SQUARE_INTERSECTION_TRAVERSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    int64_t near_value, far_value;
    int64_t dx, dy;
}LineSquareIntersectionTraverser;

LineSquareIntersectionTraverser LineSquareIntersectionTraverser_init(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_x, int32_t square_y, int32_t square_width, int32_t x_move_sign, int32_t y_move_sign);

bool LineSquareIntersectionTraverser_move_get(const LineSquareIntersectionTraverser* p_traverser);
void LineSquareIntersectionTraverser_move_x(LineSquareIntersectionTraverser* p_traverser);
void LineSquareIntersectionTraverser_move_y(LineSquareIntersectionTraverser* p_traverser);

#ifdef __cplusplus
}
#endif

#endif // _LINE_SQUARE_INTERSECTION_TRAVERSER_H
