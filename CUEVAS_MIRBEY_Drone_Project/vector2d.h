#ifndef VECTOR2D_H
#define VECTOR2D_H

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Vector2D related methods
 **/

#include <cmath>
#include <QDebug>
#include <QColor>

class Vector2D {
public:
    float x,y; ///< The x and y components of the vector
    QColor color = QColor("#0eeb0e");
    QString name="";

    /**
     * @brief Constructs a Vector2D object with the given x and y components.
     * @param p_x The x component of the vector
     * @param p_y The y component of the vector
     */
    Vector2D(float p_x,float p_y):x(p_x),y(p_y) {};

    /**
     * @brief Constructs a Vector2D object with the given x and y components.
     * @param p_x The x component of the vector
     * @param p_y The y component of the vector
     */
    Vector2D(float p_x,float p_y, QString p_name):x(p_x),y(p_y),name(p_name) {};

    /**
     * @brief Default constructor that initializes the vector to (0, 0).
     */
    Vector2D() { x=y=0.0; }

    /**
     * @brief Constructs a Vector2D object by copying the values from another Vector2D object.
     * @param p Pointer to another Vector2D object to copy values from
     */
    Vector2D(Vector2D *p):x(p->x),y(p->y) {};


    /**
     * @brief Sets the x and y components of the vector to the specified values.
     * @param p_x The new x component
     * @param p_y The new y component
     */
    void set(double p_x,double p_y) { x=p_x; y=p_y; }


    /**
     * @brief Calculates the length (magnitude) of the vector.
     * @return The length of the vector
     */
    double length() const {
        return sqrt(x*x+y*y);
    }
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    /**
     * @brief Computes the orthogonal (perpendicular) unit vector of the vector.
     * @return A new Vector2D object representing the orthonormal vector
     */
    Vector2D orthoNormed() const {
        float l=length();
        return Vector2D(y/l,-x/l);
    }

    /**
     * @brief Overloads the [] operator to access the x (index 0) or y (index 1) component of the vector.
     * @param i Index (0 or 1) to access x or y component
     * @return The value of the corresponding component (x or y)
     */
    float operator[](const int i) const {
        return (i==0)?x:y;
    };

    // Friend functions for common vector operations:

    /**
     * @brief Computes the dot product of two vectors.
     * @param v1 The first vector
     * @param v2 The second vector
     * @return The dot product of the two vectors
     */
    friend double operator *(const Vector2D&,const Vector2D&);

    /**
     * @brief Multiplies a vector by a scalar (from the left).
     * @param scalar The scalar value to multiply the vector by
     * @param vec The vector to multiply
     * @return The resulting scaled vector
     */
    friend const Vector2D operator *(double,const Vector2D&);

    /**
     * @brief Adds two vectors together.
     * @param v1 The first vector
     * @param v2 The second vector
     * @return The resulting vector from adding v1 and v2
     */
    friend const Vector2D operator +(const Vector2D&,const Vector2D&);

    /**
     * @brief Subtracts the second vector from the first vector.
     * @param v1 The first vector
     * @param v2 The second vector
     * @return The resulting vector from subtracting v2 from v1
     */
    friend const Vector2D operator -(const Vector2D&,const Vector2D&);

    /**
     * @brief Negates the vector (multiplies each component by -1).
     * @param v The vector to negate
     * @return The negated vector
     */
    friend const Vector2D operator -(const Vector2D&);

    /**
     * @brief Computes the cross product (2D determinant) of two vectors.
     * @param v1 The first vector
     * @param v2 The second vector
     * @return The scalar value representing the cross product
     */
    friend double operator ^(const Vector2D&,const Vector2D&);

    /**
     * @brief Compares two vectors for equality.
     * @param v1 The first vector
     * @param v2 The second vector
     * @return True if the vectors are equal, false otherwise
     */
    friend bool operator ==(const Vector2D&,const Vector2D&);

    /**
     * @brief Compares two vectors for inequality.
     * @param v1 The first vector
     * @param v2 The second vector
     * @return True if the vectors are not equal, false otherwise
     */
    friend bool operator !=(const Vector2D&,const Vector2D&);

    /**
     * @brief Outputs a string representation of the vector to a QDebug stream.
     * @param dbg The QDebug object to output the vector to
     * @param v The vector to output
     * @return The QDebug object after outputting the vector
     */
    friend QDebug operator<<(QDebug dbg, const Vector2D& v) {
        dbg.nospace() << "Vector2D(" << v.x << ", " << v.y << ")";
        return dbg.space();
    }

};

// Function prototypes for operator overloads

/**
 * @brief Multiplies a vector by a scalar (from the left).
 * @param scalar The scalar value to multiply the vector by
 * @param vec The vector to multiply
 * @return The resulting scaled vector
 */
const Vector2D operator *(double,const Vector2D&);

/**
 * @brief Computes the dot product of two vectors.
 * @param v1 The first vector
 * @param v2 The second vector
 * @return The dot product of the two vectors
 */
double operator *(const Vector2D&,const Vector2D&);

/**
 * @brief Adds two vectors together.
 * @param v1 The first vector
 * @param v2 The second vector
 * @return The resulting vector from adding v1 and v2
 */
const Vector2D operator +(const Vector2D&,const Vector2D&);

/**
 * @brief Subtracts the second vector from the first vector.
 * @param v1 The first vector
 * @param v2 The second vector
 * @return The resulting vector from subtracting v2 from v1
 */
const Vector2D operator -(const Vector2D&,const Vector2D&);

/**
 * @brief Negates the vector (multiplies each component by -1).
 * @param v The vector to negate
 * @return The negated vector
 */
const Vector2D operator -(const Vector2D&);

/**
 * @brief Computes the cross product (2D determinant) of two vectors.
 * @param v1 The first vector
 * @param v2 The second vector
 * @return The scalar value representing the cross product
 */
double operator ^(const Vector2D&,const Vector2D&);

/**
 * @brief Compares two vectors for equality.
 * @param v1 The first vector
 * @param v2 The second vector
 * @return True if the vectors are equal, false otherwise
 */
bool operator==(const Vector2D&,const Vector2D&);

/**
 * @brief Compares two vectors for inequality.
 * @param v1 The first vector
 * @param v2 The second vector
 * @return True if the vectors are not equal, false otherwise
 */
bool operator !=(const Vector2D&,const Vector2D&);



#endif // VECTOR2D_H
