#include "triangle.h"
#include <QThread>

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Triangle related methods
 **/

void Triangle::computeCircle() {
    Vector2D AB = *ptr[1]-*ptr[0];
    Vector2D AC = *ptr[2]-*ptr[0];

    Vector2D OBprim = *ptr[0] + 0.5*AC; // Midpoint of one edge
    Vector2D VAC = AC.orthoNormed(); // Orthogonal unit vector to edge AC

    double k = (AB*AB - AC*AB)/(2*(VAC*AB)); // Formula to compute k

    circumCenter = OBprim + k * VAC; // Final circumcenter
    circumRadius = (circumCenter-ptr[0]).length(); // Circumradius
}

void Triangle::draw(QPainter &painter) {
    QPen pen(Qt::black);
    pen.setWidth(3);
    painter.setPen(pen);

    // Set color depending on the triangle's properties (Delaunay, flippable, etc.)
    QColor c= isDelaunay?Qt::cyan:(flippable?Qt::gray:Qt::yellow);
    //painter.setBrush(isHighlited?(isDelaunay?Qt::green:Qt::red):Qt::yellow);

    if(isHighlighted()){
        float h,s,l;
        c.getHslF(&h,&s,&l);
        c.setHslF(h,s,l*0.75); // Dim the color if highlighted
    }

    painter.setBrush(c); // Set the brush color for filling
    QPointF points[3];
    for (int i=0; i<3; i++) {
        points[i].setX(ptr[i]->x);
        points[i].setY(ptr[i]->y);
    }
    painter.drawPolygon(points,3); // Draw the triangle
}

void Triangle::drawCircle(QPainter &painter) {
    painter.setPen(QPen(Qt::black,3,Qt::DashLine));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(circumCenter.x-circumRadius,circumCenter.y-circumRadius,2.0*circumRadius,2.0*circumRadius); // Draw the circumcircle
}


void Triangle::flippIt(QVector<Triangle*> triangles) {
    QVector<const Vector2D*> commonEdges;
    qDebug()<< "Flip on"<<this->name;
    qDebug() << "OPPOSITE : " << *(this->getOpposite());

    bool flipCompleted = false; // Flag to control when the flip is done
    int i = 0;

    // Iterate over all neighboring triangles using while loop
    while (i < triangles.size() && !flipCompleted) {
        Triangle* tri = triangles[i];

        // Check if triangle meets flippable criteria
        if (tri->isFlippable() && tri != this && tri->contains(this->getOpposite())) {
            // Check each of the 3 edges to find a common edge
            int edgeIndex = 0;

            // Use nested while loop to check edges
            while (edgeIndex < 3 && !flipCompleted) {
                if (edgeIndex == 0 && tri->hasEdge(this->getVertexPtr(1), this->getVertexPtr(0))) {
                    // Common edge is (1, 0)
                    commonEdges = {getVertexPtr(1), getVertexPtr(0)};
                } else if (edgeIndex == 1 && tri->hasEdge(getVertexPtr(2), getVertexPtr(1))) {
                    // Common edge is (2, 1)
                    commonEdges = {getVertexPtr(2), getVertexPtr(1)};
                } else if (edgeIndex == 2 && tri->hasEdge(getVertexPtr(0), getVertexPtr(2))) {
                    // Common edge is (0, 2)
                    commonEdges = {getVertexPtr(0), getVertexPtr(2)};
                }

                // If a common edge is found, perform the flip
                if (!commonEdges.isEmpty()) {
                    this->updateVertices(
                        this->getOpposite(),
                        const_cast<Vector2D*>(commonEdges[0]),
                        tri->getOpposite()
                        );
                    this->updateName();

                    tri->updateVertices(
                        tri->getOpposite(),
                        const_cast<Vector2D*>(commonEdges[1]),
                        this->getOpposite()

                        );

                    tri->updateName();

                    this->computeCircle();
                    tri->computeCircle();
                    flipCompleted = true; // Set flag to indicate flip is done
                }

                edgeIndex++;
            }
        }

        i++;
    }

    // If no flip occurred, still recompute the circle
    if (!flipCompleted) {
        this->computeCircle();
    }

    qDebug() << "FlippIt ended";
}

