#include "minCircle.h"
#include <vector>
#include <cmath>
using namespace std;

Circle welzl(const vector<Point>& P);

Circle findMinCircle(Point** points,size_t size){

    vector<Point> v = vector<Point>();
    for (int i = 0; i < size; ++i) {
        Point p = *points[i];
        v.push_back(p);
    }

    return welzl(v);
}

/**
 * Gets 2 points and calculates the distance between them.
 * @param p1    point
 * @param p2    point
 * @return      distance [square root of |x1-x2|^2 to |y1-y2|^2]
 */
float distance(const Point &p1, const Point &p2)
{
    return sqrt(pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2));
}

/**
 * Is the point inside the given circle?
 * @param c     circle
 * @param p     point
 * @return      true if point is inside the circle, false otherwise.
 */
bool is_pt_in_circ (const Circle &c, const Point &p){
    //If the distance is smaller than the radius, the point is inside the circ.
    return distance(c.center, p) <= c.radius;
}

/**
 * Gets x,y values of 2 of the points on the circle
 * (as p1 is normalized to (0,0)), and returns the center of the blocking
 * circle.
 * Decided to work with arguments (and not pointers), to reduce runtime.
 * @param p2x   p2.x
 * @param p2y   p2.y
 * @param p3x   p3.x
 * @param p3y   p3.y
 * @return      Center of circle.
 */
Point get_circle_center
    (const float &p2x, const float &p2y, const float &p3x, const float &p3y)
{
    float p2dist = p2x * p2x + p2y * p2y;
    float p3dist = p3x * p3x + p3y * p3y;
    float delta = p2x * p3y - p2y * p3x;
    return {(p3y * p2dist - p2y * p3dist) / (2 * delta),
            (p2x * p3dist - p3x * p2dist) / (2 * delta) };
}

/**
 * Creates a circle blocking 3 points.
 * @param p1    point
 * @param p2    point
 * @param p3    point
 * @return      Circle that has 3 points on it.
 */
Circle circle_from
    (const Point& p1, const Point& p2, const Point& p3)
{
    Point ctr = get_circle_center
            (p2.x - p1.x,
             p2.y - p1.y,
             p3.x - p1.x,
             p3.y - p1.y);
    ctr.x += p1.x;
    ctr.y += p1.y;
    return {ctr, distance(ctr, p1) };
}

/**
 * Creates a circle that p1 and p2 are one of his diameters,
 * and the center sits in the middle of the 2.
 * @param p1    point
 * @param p2    point
 * @return      Smallest circle including the 2 points.
 */
Circle circle_from(const Point& p1, const Point& p2)
{
    return {
            Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2 ),
            distance(p1, p2) / 2
    };
}

/**
 * Check if a circle contains all of the points in the set
 * @param c     circle
 * @param P     dataset of 2d points
 * @return      True if all points are inside the circle, false otherwise.
 */
bool is_valid_circle(const Circle& c,
                     const vector<Point>& P)
{
    for (const Point& p : P)
        if (!is_pt_in_circ(c, p))
            return false;
    return true;
}

/**
 * Creates a minimum-sized circle fitting all relevant points in |dataset|<=3
 * @param relevant_pts  max of 3 points
 * @return              smallest circle holding each one of the points.
 */
Circle min_circle_simple(vector<Point>& relevant_pts)
{
    if (relevant_pts.empty()) {
        return { { 0, 0 }, 0 };
    }
    else if (relevant_pts.size() == 1) {
        return {relevant_pts[0], 0 };
    }
    else if (relevant_pts.size() == 2) {
        return circle_from(relevant_pts[0], relevant_pts[1]);
    }

    // To check if MEC can be determined
    // by 2 points only
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {

            Circle c = circle_from(relevant_pts[i], relevant_pts[j]);
            if (is_valid_circle(c, relevant_pts))
                return c;
        }
    }
    return circle_from(relevant_pts[0], relevant_pts[1], relevant_pts[2]);
}

// Returns the MEC using Welzl's algorithm
// Takes a set of input points P and a set R
// points on the circle boundary.
// n represents the number of points in P
// that are not yet processed.
Circle welzl_helper(vector<Point>& P,
                    vector<Point> R, int n)
{
    // Base case when all points processed or |R| = 3
    if (n == 0 || R.size() == 3) {
        return min_circle_simple(R);
    }

    // pick point from the end of P
    Point p = P[n-1];

    // Get the MEC circle d from the
    // set of points P - {p}
    Circle d = welzl_helper(P, R, n - 1);

    // If d contains p, return d
    if (is_pt_in_circ(d, p)) {
        return d;
    }

    // Otherwise, must be on the boundary of the MEC
    R.push_back(p);

    // Return the MEC for P - {p} and R U {p}
    return welzl_helper(P, R, n - 1);
}

Circle welzl(const vector<Point>& P)
{
    vector<Point> P_copy = P;
    return welzl_helper(P_copy, {}, P_copy.size());
}
