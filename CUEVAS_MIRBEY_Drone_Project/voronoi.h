#ifndef VORONOI_H
#define VORONOI_H

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Voronoi related methods
 **/

#include "mypolygon.h"
#include "triangle.h"
#include "vector2d.h"
#include <QVector>
#include <QColor>
#include <QDebug>

/**
 * @brief The Voronoi class
 * Represents a Voronoi diagram based on a set of points and their triangulation.
 */
class Voronoi {
public:
    Voronoi();
    ~Voronoi();

    /**
     * @brief Add a Voronoi polygon to the diagram
     * @param center The center of the polygon
     * @param color The color of the polygon
     * @param triangles The Delaunay triangles of the mesh
     */
    void addPolygon(const Triangle center, const QColor &color, const QVector<Triangle> &triangles);

    /**
     * @brief Draw all polygons in the Voronoi diagram
     * @param painter The QPainter object used for rendering
     */
    void draw(QPainter &painter);

private:
    QVector<MyPolygon *> tabPoly; ///< Array of polygons in the Voronoi diagram
};

#endif // VORONOI_H
