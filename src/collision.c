
typedef enum {
    COLLISION_BOTTOM,
    COLLISION_TOP,
    COLLISION_LEFT,
    COLLISION_RIGHT,
} CollisionType;

typedef struct {
    float x1, y1, x2, y2;
} Line; Line l1, l2;

typedef struct {
    bool isIntersecting; 	// yes or no, are the lines intersecting?
    float uA, uB;  			// Intersection parameters along each line
    float cx, cy;  			// Intersection coordinates
} IntersectionResult;

IntersectionResult islinesintersecting(Line l1, Line l2) { // source: https://www.jeffreythompson.org/collision-detection/line-line.php
    IntersectionResult result;
    result.isIntersecting = false;

    // Calculate the denominator (determinant)
    float denom = ((l2.y2 - l2.y1) * (l1.x2 - l1.x1)) - ((l2.x2 - l2.x1) * (l1.y2 - l1.y1));

    // If the denominator is close to zero, lines are parallel or coincident
    if (fabs(denom) < 1e-6) return result;

    // Calculate uA and uB
    float uA = (((l2.x2 - l2.x1) * (l1.y1 - l2.y1)) - ((l2.y2 - l2.y1) * (l1.x1 - l2.x1))) / denom;
    float uB = (((l1.x2 - l1.x1) * (l1.y1 - l2.y1)) - ((l1.y2 - l1.y1) * (l1.x1 - l2.x1))) / denom;

    // Calculate intersection coordinates
    float cx = l1.x1 + (uA * (l1.x2 - l1.x1));
    float cy = l1.y1 + (uA * (l1.y2 - l1.y1));

    // Check if the intersection is within both line segments
    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
        result.isIntersecting = true;
        result.uA = uA;
        result.uB = uB;
        result.cx = cx;
        result.cy = cy;
    }

    return result;
}
