
/// square_right_of_line_traverser.h
/// Provides functions which will traverse a quadtree, searching only the squares that are to the right of a given line.

#ifndef SQUARE_RIGHT_OF_LINE_TRAVERSER_H
#define SQUARE_RIGHT_OF_LINE_TRAVERSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "square_right_of_line.h"

/// User defined function which will be called for every node in a pre-order tree traversal.
/// @param user_data Pointer to any user defined data which will be provided in the user_data parameter in SquareRightOfLineTraversePreOrder().
/// @param square_x X coordinate of the top-left (min x, min y) point where the current node is located in the quadtree.
/// @param square_y Y coordinate of the top-left (min x, min y) point where the current node is located in the quadtree.
/// @param square_width width of the current node in the quadtree. Will always be a power of 2.
/// @param depth depth in the tree. The very first value will be 1, because its impossible to traverse the root node itself,
/// only its children.
/// complete_hit True if the region defined by square_x, square_y, and square_width is completely right of the line.
/// False if the region partially intersects the line. If the line lies exactly on any edge/corner of the square, that is 
/// considered a partial hit and will cause complete_hit to be false.
/// @returns True if the user wishes to traverse further into this node, or false to explore other nodes.
/// Value is ignored if the current node is a leaf node.
typedef bool (*SquareRightOfLineTraverserPreOrderCallback)(void* user_data, int32_t square_x, int32_t square_y, int32_t square_width,
    int32_t depth, bool complete_hit);

/// Traverses (pre-order) a quadtree over all squares that are right of (or exactly on) a line.
/// @param x1 x coordinate of the 1st point on the line.
/// @param y1 y coordinate of the 1st point on the line.
/// @param x2 x coordinate of the 2nd point on the line.
/// @param y2 y coordinate of the 2nd point on the line.
/// @param starting_x starting x coordinate of the traversal. Must be a multiple of 2^starting_width_log2.
/// @param starting_y starting y coordinate of the traversal. Must be a multiple of 2^starting_width_log2.
/// @param starting_width_log2 the log base 2 of the region on the quadtree to traverse. Must be between 1 and 30, inclusive.
/// @param callback User defined function to be called on every node in the tree which is right of the given line.
/// @param user_data Pointer to be passed directly to callback for user defined data.
void SquareRightOfLineTraversePreOrder(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t starting_x, int32_t starting_y, int32_t starting_width_log2,
    SquareRightOfLineTraverserPreOrderCallback callback, void* user_data);




typedef void (*SquareRightOfLineTraverserDFSCallback)(void* user_data, int32_t square_x, int32_t square_y, int32_t square_width, int32_t depth);

void SquareRightOfLineTraverseDepthFirst(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    int32_t starting_x, int32_t starting_y, int32_t starting_width_log2,
    SquareRightOfLineTraverserDFSCallback callback, void* user_data);

#ifdef __cplusplus
}
#endif

#endif // SQUARE_RIGHT_OF_LINE_TRAVERSER_H
