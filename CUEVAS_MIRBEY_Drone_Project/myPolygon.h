#ifndef MYPOLYGON_H
#define MYPOLYGON_H

/**
 * @author Benoît Piranda
 * @date 7/11/2024
 * @todo Make more tests of the triangulation algorithm with complex shapes.
 **/

#include "vector2d.h"
#include <QPainter>
#include <QDebug>
#include "triangle.h"

/**
 *  @brief MyPolygon class allow to create, draw and manipulate
 *  polygons, especially check if a point is inside and compute
 *  the surface of the polygon.
*/
class MyPolygon {
private:
    int Nmax; ///< maximum number of vertices for the current Polygon
    int N; ///< current number of vertices
    Vector2D *tabPts; ///< array of vertex positions
    QVector<Triangle> triangles; ///< array of triangles for the triangulation process
    QColor currentColor; ///< current drawing color of the polygon
    Vector2D center = Vector2D(0,0); ///< center of the polygon
    QVector<Vector2D*> serverList; ///< list of servers in the polygon
public:
    bool isOpen = false; ///< Indicates whether the triangle is open or closed. Defaults to false (closed).
    QMap<MyPolygon*, QPair<Vector2D, Vector2D>> sharedSides; ///< Shared sides between polygons key = pointer to the polygon and value= pair of Vector2D representing the two points of the shared side.

    /**
     * @brief Constructor of a polygon.
     * @warning The number of vertices added to the polygon must be lower than p_Nmax.
     * The program will crash otherwise.
     * @param p_Nmax maximum number of vertices.
     */
    MyPolygon(int p_Nmax):Nmax(p_Nmax) {
        N=0;
        tabPts = new Vector2D[Nmax+1];
        currentColor=Qt::green;
    }

    /**
     * @brief Destructor
     */
    ~MyPolygon() {
        delete [] tabPts;
    }
    /**
     * @brief Add a new vertex at the end of the list
     * @warning The number of vertices added to the polygon must be lower than Nmax.
     * @param x abscissa of the new vertex
     * @param y ordinate of the new vertex
     */
    Vector2D* getVertexPtr(int i) const{
        return &tabPts[i];
    }

    /**
     * @brief Retrieves the vertex of the object.
     * @return A pointer to the array of vertices.
     */
    Vector2D* getVertex() const{
        return tabPts;
    }

    /**
     * @brief Retrieves the size of the vertex array.
     * @return The number of vertices (size of the array).
     */
    int getVertexSize() const{
        return N;
    }

    /**
     * @brief Add a point in the polygon
     */
    void addVertex(float x,float y);

    /**
     * @brief return the boundig box coordinates of the polygon
     * @return The first vector is the left bottom corner and the second vector is the right top corner of the box.
     */
    QPair<Vector2D,Vector2D> getBoundingBox() const;

    /**
     * @brief Return the center point of the polygon
     */
    Vector2D* getCenter() {return &center;}

    /**
     * @brief Calculates the centroid of the polygon.
     * @return The centroid as a Vector2D.
     */
    Vector2D calculateCentroid();

    /**
     * @brief Set the color of the polygon to c
     * @param c The new color.
     */
    void setColor(const QColor c) { currentColor=c; }

    /**
     * @brief Get the color of the polygon
     * @return The color of the polygon
     */
    QColor getColor(){return currentColor;}

    /**
     * @brief Checks if an edge exists between two points A and B in the polygon.
     * @param A The first point of the edge.
     * @param B The second point of the edge.
     * @return True if the edge exists, false otherwise.
     */
    bool hasEdge(Vector2D A, Vector2D B) const {
        for (int i=0;i<N-1;i++){
            if((tabPts[i]==A && tabPts[i+1]==B) || (tabPts[i+1]==A && tabPts[i]==B)){
                return true;
            }
        }
        return false;
    }


    /**
     * @brief Draw the polygon.
     * @param painter Current painter context.
     * @param showTriangles Shows the internal triangle if true.
     */
    void draw(QPainter &painter,bool showTriangles);

    /**
     * @brief Returns the vertices of the polygon.
     * @return A pointer to the array of vertices.
     */
    const Vector2D* getVertices() const { return tabPts; }

    /**
     * @brief Retrieves shared sides with other polygons.
     * @param polygons A list of polygons to check for shared sides.
     */
    void getSharedSides(QVector<MyPolygon*> polygons);

    /**
     * @brief Checks if a point is inside the polygon.
     * @param point A pointer to the point to be checked.
     * @return True if the point is inside the polygon, false otherwise.
     */
    bool isPointInside(Vector2D* point);

    /**
     * @brief Returns the list of server positions.
     * @return A vector of pointers to the server positions.
     */
    QVector<Vector2D*> getServerList() const{return serverList;}

};

#endif // MYPOLYGON_H
