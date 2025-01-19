#ifndef TRIANGLE_H
#define TRIANGLE_H

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Triangle related methods
 **/

#include <QPainter>
#include "vector2d.h"
#include "determinant.h"
#include "qvectornd.h"
#include <QDebug>
#include <QVector>
#include <QPair>

/**
 * @brief The Triangle class represents a triangle in a 2D plane defined by three vertices.
 * It provides methods for geometric operations such as checking if a point is inside the triangle,
 * computing the circumcircle, and checking the Delaunay property.
 */
class Triangle {
private:
    Vector2D circumCenter; ///< The center of the circumcircle (computed in computeCircle)
    float circumRadius; ///< The radius of the circumcircle (computed in computeCircle)
    QBrush brush; ///< The brush used to draw the triangle
    bool isHighlited = false; ///< Indicates if the triangle is highlighted
    bool isDelaunay = false; ///< Indicates if the triangle satisfies the Delaunay condition
    bool flippable = false; ///< Indicates if the triangle can be flipped (based on its configuration)
    Vector2D *flippPoint = nullptr; ///< The point making this triangle flippable (used during flipping)
    /**
     * @brief Computes the circumcircle for the triangle and sets circumCenter and circumRadius.
     */
    void computeCircle();


public:
    bool isOpen = false;
    QSet<QPair<Vector2D*, Vector2D*>> openSides;
    QSet<QPair<Vector2D*, Vector2D*>> closedSides;
    Vector2D *ptr[3]; ///< Array of 3 pointers to the triangle's vertices
    QString name="";


    /**
     * @brief Constructs a Triangle with the given vertices and a default color (yellow).
     * @param ptr1 Pointer to the first vertex of the triangle
     * @param ptr2 Pointer to the second vertex of the triangle
     * @param ptr3 Pointer to the third vertex of the triangle
     */
    Triangle(Vector2D *ptr1,Vector2D *ptr2,Vector2D *ptr3) {
        ptr[0]=ptr1;
        ptr[1]=ptr2;
        ptr[2]=ptr3;
        name = QString("(%1, %2), (%3, %4), (%5, %6)")
                   .arg(ptr1->x).arg(ptr1->y)   // Sommet 1 : (x1, y1)
                   .arg(ptr2->x).arg(ptr2->y)   // Sommet 2 : (x2, y2)
                   .arg(ptr3->x).arg(ptr3->y);  // Sommet 3 : (x3, y3)
        setColor(Qt::yellow);
        computeCircle();
    }

    void updateName() {
        name = QString("(%1, %2), (%3, %4), (%5, %6)")
        .arg(ptr[0]->x).arg(ptr[0]->y)   // Sommet 1 : (x1, y1)
            .arg(ptr[1]->x).arg(ptr[1]->y)   // Sommet 2 : (x2, y2)
            .arg(ptr[2]->x).arg(ptr[2]->y);  // Sommet 3 : (x3, y3)
    }

    /**
     * @brief Constructs a Triangle with the given vertices and a specified color.
     * @param ptr1 Pointer to the first vertex of the triangle
     * @param ptr2 Pointer to the second vertex of the triangle
     * @param ptr3 Pointer to the third vertex of the triangle
     * @param p_color The color to set for the triangle
     */
    Triangle(Vector2D *ptr1,Vector2D *ptr2,Vector2D *ptr3,const QColor &p_color) {
        ptr[0]=ptr1;
        ptr[1]=ptr2;
        ptr[2]=ptr3;
        name = QString("(%1, %2), (%3, %4), (%5, %6)")
                   .arg(ptr1->x).arg(ptr1->y)   // Sommet 1 : (x1, y1)
                   .arg(ptr2->x).arg(ptr2->y)   // Sommet 2 : (x2, y2)
                   .arg(ptr3->x).arg(ptr3->y);  // Sommet 3 : (x3, y3)
        setColor(p_color);
        computeCircle();
    }


    /**
     * @brief Checks if point P is to the left of the line formed by P1 and P2.
     * @param P The point to check
     * @param P1 The first point of the line
     * @param P2 The second point of the line
     * @return True if point P is on the left of the line, false otherwise
     */
    bool isOnTheLeft(const Vector2D *P,const Vector2D *P1,const Vector2D *P2) {
        Vector2D AB = *P2-*P1,
                AP = *P-*P1;

        return (AB.x*AP.y - AB.y*AP.x)>=0;
    }

    /**
     * @brief Checks if the point (x, y) is inside the triangle.
     * @param x The x-coordinate of the point
     * @param y The y-coordinate of the point
     * @return True if the point is inside the triangle, false otherwise
     */
    inline bool isInside(float x,float y) {
        return isInside(Vector2D(x,y));
    }

    /**
     * @brief Checks if the point P is inside the triangle.
     * @param P The point to check
     * @return True if the point is inside the triangle, false otherwise
     */
    bool isInside(const Vector2D &P) {
        return isOnTheLeft(&P,ptr[0],ptr[1]) &&
               isOnTheLeft(&P,ptr[1],ptr[2]) &&
               isOnTheLeft(&P,ptr[2],ptr[0]);
    }

    /**
     * @brief Draws the triangle using the provided QPainter.
     * @param painter The QPainter used to draw the triangle
     */
    void draw(QPainter &painter);

    /**
     * @brief Draws the circumcircle of the triangle using the provided QPainter.
     * @param painter The QPainter used to draw the circumcircle
     */
    void drawCircle(QPainter &painter);

    /**
     * @brief Sets the color of the triangle.
     * @param p_color The new color for the triangle
     */
    void setColor(const QColor &p_color) {
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        brush.setColor(p_color);
    }

    /**
     * @brief Sets whether the triangle should be highlighted.
     * @param v True to highlight the triangle, false to remove the highlight
     */
    inline void setHighlighted(bool v) {
        isHighlited=v;
    }

    /**
     * @brief Checks if the triangle is highlighted.
     * @return True if the triangle is highlighted, false otherwise
     */
    inline bool isHighlighted() {
        return isHighlited;
    }

    /**
     * @brief Checks if the triangle can be flipped.
     * @return True if the triangle is flippable, false otherwise
     */
    inline bool isFlippable() {
        return flippable;
    }

    /**
     * @brief Gets the center of the circumcircle of the triangle.
     * @return The center of the circumcircle
     */
    inline Vector2D getCircleCenter() {
        return circumCenter;
    }

    /**
     * @brief Sets whether the triangle is Delaunay-compliant and whether it is flippable.
     * @param v True if the triangle is Delaunay-compliant, false otherwise
     * @param f True if the triangle is flippable, false otherwise
     */
    inline void setDelaunay(bool v, bool f) {
        isDelaunay=v;
        flippable=f;
    }

    /**
     * @brief Sets the opposite point that makes this triangle flippable.
     * @param o The opposite point
     */
    inline void setOpposite(Vector2D *o){
        flippPoint = o;
    }

    /**
     * @brief Gets the opposite point that makes this triangle flippable.
     * @return The opposite point
     */
    inline Vector2D* getOpposite(){
        return flippPoint;
    }
    /**
     * @brief Checks if the triangle satisfies the Delaunay condition by checking if the determinant of the matrix formed
     *        by the triangle's vertices and a test point is non-positive for all points in tabVertices.
     * @param tabVertices The list of vertices to check against
     * @return True if the triangle satisfies the Delaunay condition, false otherwise
     */

    bool checkDelaunay(const QVector<Vector2D> &tabVertices) {
        auto it = tabVertices.begin();
        bool isOk = true;

        // Assume tabVertices has at least three points, A, B, and C
        // Assume tabVertices has at least three points, A, B, and C
        const Vector2D *A = ptr[0];
        const Vector2D *B = ptr[1];
        const Vector2D *C = ptr[2];

        while (it != tabVertices.end() && isOk) {
            Matrix33 mat;
            // PAGE 35 DU COURS GEOMETRIC ALGOITHMS
            const Vector2D D = (*it);
            mat.m[0][0] = A->x - D.x;
            mat.m[0][1] = A->y - D.y;
            mat.m[0][2] = ((A->x * A->x) - (D.x * D.x)) + ((A->y * A->y) - (D.y * D.y));

            mat.m[1][0] = B->x - D.x;
            mat.m[1][1] = B->y - D.y ;
            mat.m[1][2] = ((B->x * B->x) - (D.x * D.x)) + ((B->y * B->y) - (D.y * D.y));

            mat.m[2][0] =   C->x - D.x;
            mat.m[2][1] =  C->y - D.y ;
            mat.m[2][2] = ((C->x * C->x) - (D.x * D.x)) + ((C->y * C->y) - (D.y * D.y));

            isOk = (mat.determinant() <= 0);
            it++;
        };
        isDelaunay=isOk;
        flippable=false;
        //qDebug() << isDelaunay;
        return isDelaunay;


    }

    /**
     * @brief Gets the pointer to the i-th vertex of the triangle.
     * @param i The index of the vertex (0, 1, or 2)
     * @return The pointer to the vertex at index i
     */
    Vector2D* getVertexPtr(int i) const{
        return ptr[i];
    }

    /**
     * @brief Checks if the triangle has the edge defined by points A and B.
     * @param A One point of the edge
     * @param B The other point of the edge
     * @return True if the triangle has the edge (A, B), false otherwise
     */
    bool hasEdge(  Vector2D A, Vector2D B) const{
        return (A==ptr[0] && B==ptr[1]) || (A==ptr[1] && B==ptr[2]) || (A==ptr[2] && B==ptr[0]); //|| (A==ptr[1] && B==ptr[0]) || (A==ptr[2] && B==ptr[1]) || (A==ptr[0] && B==ptr[2]);
    }

    /**
     * @brief Checks if the point M is one of the triangle's vertices.
     * @param M The point to check
     * @return True if M is a vertex of the triangle, false otherwise
     */
    bool contains(const Vector2D &M){
        return M==(*ptr[0]) ||   M==(*ptr[1]) ||  M==(*ptr[2]);
    }

    /**
     * @brief Updates the vertices of the triangle.
     * @param _A The new first vertex
     * @param _B The new second vertex
     * @param _C The new third vertex
     */
    void updateVertices( Vector2D *_A,  Vector2D *_B,  Vector2D *_C){
        ptr[0] = _A;
        ptr[1] = _B;
        ptr[2] =_C;
    }

    /**
     * @brief Checks if the point M is inside the circumcircle of the triangle.
     * @param M The point to check
     * @return True if M is inside the circumcircle, false otherwise
     */
    bool circleContains(const Vector2D *M){
        Matrix33 mat;
        Vector2D *A = ptr[0];
        Vector2D *B = ptr[1];
        Vector2D *C = ptr[2];

        mat.m[0][0] = A->x - M->x;
        mat.m[0][1] = A->y - M->y;
        mat.m[0][2] = ((A->x * A->x) - (M->x * M->x)) + ((A->y * A->y) - (M->y * M->y));

        mat.m[1][0] = B->x - M->x;
        mat.m[1][1] = B->y - M->y ;
        mat.m[1][2] = ((B->x * B->x) - (M->x * M->x)) + ((B->y * B->y) - (M->y * M->y));

        mat.m[2][0] =   C->x - M->x;
        mat.m[2][1] =  C->y - M->y ;
        mat.m[2][2] = ((C->x * C->x) - (M->x * M->x)) + ((C->y * C->y) - (M->y * M->y));

        return mat.determinant()<=0;
    }
    /**
     * @brief Attempts to flip the triangle with an adjacent triangle if possible.
     * @param triangles The list of neighboring triangles to check for a possible flip
     */
    void flippIt(QVector<Triangle*> triangles);

    /**
 * @brief Checks if the triangle is open based on its shared edges with other triangles.
 * @param triangles A list of triangles to check against for shared edges.
 * This function checks the edges of the current triangle and determines whether each edge is shared with another triangle.
 * If at least one edge is not shared, the triangle is considered "open" and is added to the `openSides` set.
 * It updates the `isOpen` status and inserts non-shared edges into the `openSides` set.
 */
    void checkIsOpen(QVector<Triangle*> triangles) {

        // Boolean flags to track if the edges are shared with other triangles
        bool edge01Shared = false;
        bool edge02Shared = false;
        bool edge12Shared = false;

        // Loop through all triangles to check if edge 0-1 is shared
        int i = 0;
        while (i < triangles.size() && !edge01Shared) {
            if (triangles[i] != this) {  // Ensure we're not checking against the current triangle
                // Check if edge 0-1 or 1-0 is shared with another triangle
                if (triangles[i]->hasEdge(this->getVertexPtr(0), this->getVertexPtr(1)) ||
                    triangles[i]->hasEdge(this->getVertexPtr(1), this->getVertexPtr(0))) {
                    edge01Shared = true;  // Mark this edge as shared
                }
            }
            ++i;  // Move to the next triangle
        }

        // Reset the counter and check for edge 0-2
        i = 0;
        while (i < triangles.size() && !edge02Shared) {
            if (triangles[i] != this) {
                // Check if edge 0-2 or 2-0 is shared with another triangle
                if (triangles[i]->hasEdge(this->getVertexPtr(0), this->getVertexPtr(2)) ||
                    triangles[i]->hasEdge(this->getVertexPtr(2), this->getVertexPtr(0))) {
                    edge02Shared = true;  // Mark this edge as shared
                }
            }
            ++i;
        }

        // Reset the counter and check for edge 1-2
        i = 0;
        while (i < triangles.size() && !edge12Shared) {
            if (triangles[i] != this) {
                // Check if edge 1-2 or 2-1 is shared with another triangle
                if (triangles[i]->hasEdge(this->getVertexPtr(1), this->getVertexPtr(2)) ||
                    triangles[i]->hasEdge(this->getVertexPtr(2), this->getVertexPtr(1))) {
                    edge12Shared = true;  // Mark this edge as shared
                }
            }
            ++i;
        }

        // A triangle is open if at least one of its edges is not shared with other triangles
        this->isOpen = !(edge01Shared && edge02Shared && edge12Shared);

        // If the triangle is open, add its non-shared edges to the openSides set
        if (this->isOpen) {
            if (!edge01Shared) {
                // Edge 0-1 is not shared, add it to openSides
                openSides.insert(qMakePair(this->getVertexPtr(0), this->getVertexPtr(1)));
            }
            if (!edge02Shared) {
                // Edge 0-2 is not shared, add it to openSides
                openSides.insert(qMakePair(this->getVertexPtr(0), this->getVertexPtr(2)));
            }
            if (!edge12Shared) {
                // Edge 1-2 is not shared, add it to openSides
                openSides.insert(qMakePair(this->getVertexPtr(1), this->getVertexPtr(2)));
            }
        }
    }








    Vector2D getSegmentMidpoint(Vector2D v1, Vector2D v2) const {

        float midX = (v1.x + v2.x) / 2.0f;
        float midY = (v1.y + v2.y) / 2.0f;

        return Vector2D(midX, midY);
    }

    Vector2D getOrthogonalFromMidpoint(Vector2D v1, Vector2D v2,Vector2D midpoint) const {
        //Vector2D midpoint = getSegmentMidpoint(v1, v2);

        float dx = v2.x - v1.x;
        float dy = v2.y - v1.y;

        Vector2D orthogonal(dy, -dx);

        return midpoint + orthogonal;
    }


};

#endif // TRIANGLE_H
