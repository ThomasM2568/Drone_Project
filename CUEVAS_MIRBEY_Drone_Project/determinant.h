#ifndef GEOMETRY_TRIANGULATION_GL_DETERMINANT_H
#define GEOMETRY_TRIANGULATION_GL_DETERMINANT_H

/**
 * @author Kyllian Cuevas & Thomas Mirbey
 * @date 19/11/2024
 * @todo Determinant related methods
 **/

/**
 * @brief The Matrix33 class allows to create a matrix of 3x3 size.
 * It provides methods for matrix manipulation and determinant calculation.
 */
class Matrix33;

/**
 * @brief The Matrix44 class allows to create a matrix of 4x4 size.
 * It provides methods for matrix manipulation and determinant calculation.
 */
class Matrix44;

/**
 * @brief The Matrix22 class allows to create a matrix of 2x2 size.
 * It provides methods for matrix manipulation and determinant calculation.
 */
class Matrix22 {
public:
    float m[2][2]; ///< List of points representing the 2x2 matrix.

    /**
     * @brief Extracts a 2x2 submatrix from a 3x3 matrix by removing a specified row and column.
     * @param mat33 The 3x3 matrix from which to extract the submatrix.
     * @param shadowLin The row to exclude.
     * @param shadowCol The column to exclude.
     */
    void get2x2From3x3(const Matrix33 &mat33, int shadowLin, int shadowCol);

    /**
     * @brief Calculates the determinant of the 2x2 matrix.
     * @return The determinant of the matrix.
     */
    inline float determinant() {
        return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    }
};

/**
 * @brief The Matrix33 class represents a 3x3 matrix and allows matrix manipulation.
 * It provides a method for calculating the matrix determinant.
 */
class Matrix33 {
public:
    float m[3][3]; ///< List of points representing the 3x3 matrix.

    /**
     * @brief Extracts a 3x3 submatrix from a 4x4 matrix by removing a specified row and column.
     * @param mat44 The 4x4 matrix from which to extract the submatrix.
     * @param shadowLin The row to exclude.
     * @param shadowCol The column to exclude.
     */
    void get3x3From4x4(const Matrix44 &mat44, int shadowLin, int shadowCol);

    /**
     * @brief Calculates the determinant of the 3x3 matrix.
     * @return The determinant of the matrix.
     */
    float determinant();
};

/**
 * @brief The Matrix44 class represents a 4x4 matrix and allows matrix manipulation.
 * It provides a method for calculating the matrix determinant.
 */
class Matrix44 {
public:
    float m[4][4]; ///< List of points representing the 4x4 matrix.

    /**
     * @brief Calculates the determinant of the 4x4 matrix.
     * @return The determinant of the matrix.
     */
    float determinant();
};

#endif // GEOMETRY_TRIANGULATION_GL_DETERMINANT_H
