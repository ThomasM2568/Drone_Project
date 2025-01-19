#include "mypolygon.h"
#include <QDebug>

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Polygon related methods
 **/

// Function to add a new vertex to the polygon.
void MyPolygon::addVertex(float x, float y) {
    assert(N<Nmax); // Ensure the maximum number of vertices has not been exceeded.
    /*center.x = (center.x == 0) ? x : (center.x + x) / 2;
    center.y = (center.y == 0) ? y : (center.y + y) / 2;*/

    // Add the new vertex to the array of points.
    tabPts[N].x=x;
    tabPts[N].y=y;
    N++; // Increment the number of vertices.

    // Close the polygon by adding the first point at the end of the array.
    tabPts[N].x=tabPts[0].x;
    tabPts[N].y=tabPts[0].y;
}

// Function to draw the polygon and its associated triangles (if applicable).
void MyPolygon::draw(QPainter &painter,bool showTriangles) {
    // Set up the pen for the polygon's border (black, width 3).
    QPen pen(Qt::black);
    pen.setWidth(3);

    // Create an array of QPoint to convert polygon points to QPainter-compatible format.
    QPoint *points=new QPoint[N];
    for (int i=0; i<N; i++) {
        points[i].setX(tabPts[i].x); // Convert x to QPoint format
        points[i].setY(tabPts[i].y); // Convert y to QPoint format
    }

     // Set brush color for the polygon and set the pen.
    painter.setBrush(currentColor); // Use the current color for the polygon fill.
    painter.setPen(pen); // Set the pen to outline the polygon.

    // Draw the polygon using the points array.
    painter.drawPolygon(points,N,Qt::OddEvenFill); // Use OddEvenFill rule for filling.

    // If showTriangles is true, display the edges of the triangles.
    if (showTriangles) {
        painter.setPen(Qt::blue); // Set the pen color for triangles to blue.
    } else {
        painter.setPen(Qt::NoPen); // Hide the edges if showTriangles is false.
    }

    // Iterate through all triangles and draw their edges.
    for (auto &t:triangles) {
        // Draw the edges of each triangle using its points.
        painter.drawLine(t.ptr[0]->x,t.ptr[0]->y,t.ptr[1]->x,t.ptr[1]->y);
        painter.drawLine(t.ptr[1]->x,t.ptr[1]->y,t.ptr[2]->x,t.ptr[2]->y);
        painter.drawLine(t.ptr[2]->x,t.ptr[2]->y,t.ptr[0]->x,t.ptr[0]->y);
    }
    // Clean up the dynamically allocated memory for the points array.
    delete [] points;
}

// Function to get the bounding box of the polygon (the smallest rectangle that encloses it).
QPair<Vector2D,Vector2D> MyPolygon::getBoundingBox() const {
    // Initialize the minimum and maximum points with the first point of the polygon.
    Vector2D min=tabPts[0],max=tabPts[0];

    // Iterate over all the points of the polygon to find the bounding box.
    for (int i=0; i<N; i++) {
        // Update the minimum and maximum x and y values.
        if (tabPts[i].x<min.x) min.x = tabPts[i].x;
        if (tabPts[i].y<min.y) min.y = tabPts[i].y;
        if (tabPts[i].x>max.x) max.x = tabPts[i].x;
        if (tabPts[i].y>max.y) max.y = tabPts[i].y;
    }
    // Return the bounding box as a pair of Vector2D (min, max).
    return QPair<Vector2D,Vector2D>(min,max);
}

// Get all sides of this polygon that are shared with other polygons
void MyPolygon::getSharedSides(QVector<MyPolygon*> polygons) {
    sharedSides.clear();
    // Loop through all sides
    for (int i = 0; i < N; i++) {  // Loop from 0 to N-1, including the last side (from N-1 to 0)
        // Calculate index for points X and X+1
        int nextIndex = (i + 1) % N;  // (i + 1) % N ensures we loop back to the first point at the end

        //qDebug() << "Side:" << tabPts[i] << "," << tabPts[nextIndex];

        // Is side shared, default is false
        bool found = false;

        // Loop through all other polygons
        int j = 0;
        while (j < polygons.size() && !found) {  // Continue checking polygons until a shared side is found
            MyPolygon* poly = polygons[j];
            // Ignore the current polygon (this)
            if (this != poly) {
                // Loop through all sides of the other polygon
                int k = 0;
                while (k < poly->N && !found) {  // Continue checking sides of the current polygon
                    int nextPolyIndex = (k + 1) % poly->N;

                    // Check if the current side (tabPts[i], tabPts[nextIndex]) matches the side (poly->tabPts[k], poly->tabPts[nextPolyIndex])
                    if ((tabPts[i] == poly->tabPts[k] && tabPts[nextIndex] == poly->tabPts[nextPolyIndex]) ||
                        (tabPts[i] == poly->tabPts[nextPolyIndex] && tabPts[nextIndex] == poly->tabPts[k])) {
                        // If side is shared, print a message and set 'found' to true
                        //qDebug() << "Shared side found between current polygon and polygon " << poly;
                        found = true;  // Set as a shared side

                        // Add the shared side to the QMap
                        sharedSides.insert(poly, qMakePair(tabPts[i], tabPts[nextIndex]));
                        //qDebug() << "Added shared side to QMap:" << tabPts[i] << "," << tabPts[nextIndex];
                    }
                    k++;  // Increment k for the next side in the other polygon
                }
            }
            j++;  // Increment j to check the next polygon
        }
    }
    //qDebug()<<"sharedSides size for "<<this<<"is"<<sharedSides.size();
}

// Calculate the center position of the polygon
Vector2D MyPolygon::calculateCentroid() {
    float sumX = 0;
    float sumY = 0;
    float A = 0;  // Polygon area

    for (int i = 0; i < N; i++) {
        int j = (i + 1) % N;
        float xi = tabPts[i].x;
        float yi = tabPts[i].y;
        float xj = tabPts[j].x;
        float yj = tabPts[j].y;

        // Calculate area (A) of the polygon
        float crossProduct = xi * yj - xj * yi;
        A += crossProduct;

        // Calculate centroid coordinates
        sumX += (xi + xj) * crossProduct;
        sumY += (yi + yj) * crossProduct;
    }

    A /= 2;

    // Centroid coordinates calculation
    float centroidX = sumX / (6 * A);
    float centroidY = sumY / (6 * A);

    center = Vector2D(centroidX, centroidY);
    return center;
}

// Return if a point is inside of a polygon using comparison to left
bool MyPolygon::isPointInside(Vector2D* point) {
    bool inside = true;  // Default value, point is considered inside
    int i = 0;
    while (i < N && inside) {
        int j = (i + 1) % N;
        Vector2D p1 = tabPts[i];
        Vector2D p2 = tabPts[j];

        // Sides vertex
        Vector2D edge = p2 - p1;
        Vector2D pointVec = *point - p1;

        // Vectorial product : (p2 - p1) x (point - p1)
        float crossProduct = edge.x * pointVec.y - edge.y * pointVec.x;

        // If vectorial product is negative, it means the point is on the right
        if (crossProduct < 0) {
            inside = false;  // Point is outside
        }

        i++;
    }


    if (inside && !serverList.contains(point)) {
        // If point is inside and not already in serverList we add it
            serverList.push_back(point);
    }

    return inside;
}



