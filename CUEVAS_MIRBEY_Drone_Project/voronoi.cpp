#include "voronoi.h"
/*
Voronoi::Voronoi() {}

Voronoi::~Voronoi() {
    for (auto &p : tabPoly) {
        delete p;
    }
}

void Voronoi::addPolygon(const Triangle center, const QColor &color, const QVector<Triangle> &triangles) {
    MyPolygon *poly = new MyPolygon(20); // Maximum of 20 vertices for this polygon
    poly->setColor(color);

    QVector<const Triangle *> L;
    for (auto &t : triangles) {
        if (t.contains(center)) {
            L.push_back(&t);
        }
    }

    if (L.isEmpty()) {
        qDebug() << "No triangles found for the given center!";
        delete poly;
        return;
    }

    // Sort triangles around the center to form a closed polygon
    QVector<const Triangle *> Lordered;
    const Triangle *current = L.front();
    Lordered.push_back(current);
    L.pop_front();

    while (!L.isEmpty()) {
        const Triangle *next = nullptr;
        for (const Triangle *t : L) {
            if (current->sharesEdge(*t)) {
                next = t;
                break;
            }
        }

        if (next) {
            Lordered.push_back(next);
            L.removeOne(next);
            current = next;
        } else {
            qDebug() << "Error: Could not close the polygon loop.";
            break;
        }
    }

    // Add vertices of ordered triangles to the polygon
    for (const Triangle *t : Lordered) {
        poly->addVertex(t->getCircleCenter().x, t->getCircleCenter().y);
    }

    tabPoly.push_back(poly);
}

void Voronoi::draw(QPainter &painter) {
    for (const auto &poly : tabPoly) {
        poly->draw(painter, false); // Draw without showing triangles inside
    }
}
*/
