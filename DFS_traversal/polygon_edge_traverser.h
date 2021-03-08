
#ifndef POLYGON_EDGE_TRAVERSER_H
#define POLYGON_EDGE_TRAVERSER_H

#include <stdint.h>
#include <stdlib.h>



typedef struct 
{
    // Values for traversing the edge
    int64_t clockwiseness;
    int64_t dx_clockwiseness;
    int64_t dy_clockwiseness;
    int32_t x, y;
    int32_t dx_x, dy_y;
    int32_t end_x, end_y;

    // The intersection calculations with the previous edge in the polygon.
    // This is more optimized than caution edges because we know more info about
    // it. Because one point is shared between this and the previous edge.
    // So, theres no need to worry about the bounds of the line, it can be
    // treated as infinite.
    int64_t prev_value;
    int64_t dx_prev_value;
    int64_t dy_prev_value;

    // List of other edges that may intersect the line we are traversing,
    // which we need to keep track of to avoid hitting any nodes multiple times.
    uint32_t num_caution_edges;
    PolygonEdgeTraverserCautionEdge *caution_edges;
} PolygonEdgeTraverser;

PolygonEdgeTraverser PolygonEdgeTraverser_init(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
    uint32_t num_caution_edges,
    int32_t *caution_x1s, int32_t *caution_y1s, int32_t *caution_x2s, int32_t *caution_y2s)
{
    PolygonEdgeTraverser traverser;
    traverser.num_caution_edges = num_caution_edges;
    traverser.caution_edges = NULL;
    if (num_caution_edges != 0)
        traverser.caution_edges = (PolygonEdgeTraverserCautionEdge*)malloc(num_caution_edges * sizeof(PolygonEdgeTraverserCautionEdge));

}

void PolygonEdgeTraverser_release(PolygonEdgeTraverser *p_traverser)
{
    free(p_traverser->caution_edges);
}

#endif // POLYGON_EDGE_TRAVERSER_H
