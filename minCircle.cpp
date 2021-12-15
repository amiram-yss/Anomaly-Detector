#include "minCircle.h"
#include <vector>

Circle welzl(const vector<Point>& P);

Circle findMinCircle(Point** points,size_t size){

    vector<Point> v = vector<Point>();
    for (int i = 0; i < size; ++i) {
        Point p = *points[i];
        v.push_back(p);
    }

    return welzl(v);
}

//###

#include <algorithm>
#include <assert.h>
#include <math.h>
using namespace std;

// Function to return the euclidean distance
// between two points
float distance(const Point& p1, const Point& p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

}

// Function to check whether a point lies inside
// or on the boundaries of the circle
bool is_inside(const Circle& c, const Point& p)
{
    return distance(c.center, p) <= c.radius;
}

// Helper method to get a circle defined by 3 points
Point get_circle_center(float bx, float by,
                        float cx, float cy)
{
    float B = bx * bx + by * by;
    float C = cx * cx + cy * cy;
    float D = bx * cy - by * cx;
    return { (cy * B - by * C) / (2 * D),
             (bx * C - cx * B) / (2 * D) };
}

// Function to return a unique circle that
// intersects three points
Circle circle_from(const Point& A, const Point& B,
                   const Point& C)
{
    Point I = get_circle_center(B.x - A.x, B.y - A.y,
                                C.x - A.x, C.y - A.y);

    I.x += A.x;
    I.y += A.y;
    return { I, distance(I, A) };
}

// Function to return the smallest circle
// that intersects 2 points
Circle circle_from(const Point& p1, const Point& p2)
{
    // Set the center to be the midpoint of p1 and p2
    Point C = {(p1.x + p2.x) / 2, (p1.y + p2.y) / 2 };

    // Set the radius to be half the distance AB
    return { C, distance(p1, p2) / 2 };
}

// Function to check whether a circle
// encloses the given points
bool is_valid_circle(const Circle& c,
                     const vector<Point>& P)
{

    // Iterating through all the points
    // to check whether the points
    // lie inside the circle or not
    for (const Point& p : P)
        if (!is_inside(c, p))
            return false;
    return true;
}

// Function to return the minimum enclosing
// circle for N <= 3
Circle min_circle_simple(vector<Point>& P)
{
    if (P.empty()) {
        return { { 0, 0 }, 0 };
    }
    else if (P.size() == 1) {
        return { P[0], 0 };
    }
    else if (P.size() == 2) {
        return circle_from(P[0], P[1]);
    }

    // To check if MEC can be determined
    // by 2 points only
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {

            Circle c = circle_from(P[i], P[j]);
            if (is_valid_circle(c, P))
                return c;
        }
    }
    return circle_from(P[0], P[1], P[2]);
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
    if (is_inside(d, p)) {
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


//int main()
//{
//    Circle mec = welzl({ { 0, 0 },
//                         { 0, 1 },
//                         { 1, 0 } });

//    return 0;
//}
