//
// Created by bpiranda on 23/11/2019.
//
#include "determinant.h"

// Function to extract a 2x2 submatrix from a 3x3 matrix by removing a specified row and column.
void Matrix22::get2x2From3x3(const Matrix33 &mat33,int shadowLin, int shadowCol) {
    int l=0; // Row index for the resulting 2x2 matrix

    // Iterate through each row of the 3x3 matrix
    for (int lig=0; lig<3; lig++) {
        if (lig!=shadowLin) { // Skip the row to be removed (shadowLin)
            int c=0; // Column index for the resulting 2x2 matrix

            // Iterate through each column of the 3x3 matrix
            for (int col=0; col<3; col++) {
                if (col!=shadowCol) { // Skip the column to be removed (shadowCol)
                    m[l][c] = mat33.m[lig][col]; // Copy the remaining element
                    c++;
                }
            }
            l++; // Move to the next row in the 2x2 matrix
        }
    }
}

// Function to extract a 3x3 submatrix from a 4x4 matrix by removing a specified row and column.
void Matrix33::get3x3From4x4(const Matrix44 &mat44,int shadowLin, int shadowCol) {
    int l=0; // Row index for the resulting 3x3 matrix

    // Iterate through each row of the 4x4 matrix
    for (int lig=0; lig<4; lig++) {
        if (lig!=shadowLin) { // Skip the row to be removed (shadowLin)
            int c=0; // Column index for the resulting 3x3 matrix

             // Iterate through each column of the 4x4 matrix
            for (int col=0; col<4; col++) {
                if (col!=shadowCol) { // Skip the column to be removed (shadowCol)
                    m[l][c] = mat44.m[lig][col];  // Copy the remaining element
                    c++;
                }
            }
            l++; // Move to the next row in the 3x3 matrix
        }
    }
}

// Function to calculate the determinant of a 3x3 matrix using cofactor expansion along the first column.
float Matrix33::determinant() {
    Matrix22 mat22; // Temporary 2x2 matrix to hold the submatrix
    float det=0;  // Determinant result
    float sign=1; // Sign alternates between positive and negative

    // Perform cofactor expansion along the first column
    for (int i=0; i<3; i++) {
        // Get the 2x2 submatrix by removing the ith row and the 0th column
        mat22.get2x2From3x3(*this,i,0);

        // Add or subtract the cofactor value to the determinant
        det += sign*m[i][0]*mat22.determinant();
        sign = -sign; // Alternate sign for the next term
    }
    return det; // Return the final determinant value
}

// Function to calculate the determinant of a 4x4 matrix using cofactor expansion along the first column.
float  Matrix44::determinant() {
    Matrix33 mat33; // Temporary 3x3 matrix to hold the submatrix
    float det=0; // Determinant result
    float sign=1; // Sign alternates between positive and negative

    // Perform cofactor expansion along the first column
    for (int i=0; i<4; i++) {
        // Get the 3x3 submatrix by removing the ith row and the 0th column
        mat33.get3x3From4x4(*this,i,0);

        // Add or subtract the cofactor value to the determinant
        det += sign*m[i][0]*mat33.determinant();

        // Alternate sign for the next term
        sign = -sign;
    }
    return det; // Return the final determinant value
}
