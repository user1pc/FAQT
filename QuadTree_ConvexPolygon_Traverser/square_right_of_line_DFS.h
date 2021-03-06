
#ifndef _SQUARE_RIGHT_OF_LINE_DFS_H
#define _SQUARE_RIGHT_OF_LINE_DFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int64_t error;
    int64_t dx_error, dy_error;
    int32_t x, y;
    int32_t prev_y;
    int32_t dy_y;
    // True if working on the 1st point, closest to the line,
    // false if working on the 2nd line below 1st line.
    bool working_on_extra_point;
} SquareRightOfLineDFS_LevelTraverser;

SquareRightOfLineDFS_LevelTraverser SquareRightOfLineDFS_LevelTraverser_init(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t square_width);

bool SquareRightOfLineDFS_LevelTraverser_get(const SquareRightOfLineDFS_LevelTraverser* p_traverser, int32_t *out_x, int32_t *out_y);
bool SquareRightOfLineDFS_LevelTraverser_IsDone(const SquareRightOfLineDFS_LevelTraverser* p_traverser);

void SquareRightOfLineDFS_LevelTraverser_update_next(SquareRightOfLineDFS_LevelTraverser* p_traverser);


#ifdef __cplusplus
}
#endif

#endif // _SQUARE_RIGHT_OF_LINE_DFS_H
