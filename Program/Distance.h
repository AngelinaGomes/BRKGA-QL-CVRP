/** /distance.h
 * This file is used to compute the distance matrix.
 *
 * Developed by:
 *
 * Angelina Gomes Santos - angelina.gomes@unifesp.br
 * Antônio Augusto Chaves - antonio.chaves@unifesp.br
 * Cleder Marcos Schenekemberg - cledercms@hotmail.com
 */

#ifndef DISTANCE_H
#define DISTANCE_H

#include <vector>
#include "Data.h"

using std::vector;

/**
 * @brief getDistMatrixByEuclidean - This function computes the distance matrix using the Euclidean formula.
 * @param[in] vertex - Vector of vertex information.
 * @return Returns a two-dimensional matrix containing the distance matrix.
 *
 * Useful link on the Euclidean formula:
 * https://sciencing.com/calculate-area-using-coordinates-8265405.html
 */
vector<vector<double>> getDistMatrixByEuclidean(vector<Vertex> const& vertex);

/**
 * @brief getDistMatrixByRoundedEuclidean - This function computes the distance matrix using the rounded Euclidean formula.
 * @param[in] vertex - Vector of vertex information.
 * @return Returns a two-dimensional matrix containing the distance matrix.
 *
 * Useful link on the roudend Euclidean formula:
 * https://sciencing.com/calculate-area-using-coordinates-8265405.html
 */
vector<vector<double>> getDistMatrixByRoundedEuclidean(vector<Vertex> const& vertex);

/**
 * @brief getDistMatrixByManhattan - This function computes the distance matrix using the Manhattan formula.
 * @param[in] vertex - Vector of vertex information.
 * @return Returns a two-dimensional matrix containing the distance matrix.
 *
 * Useful link on the Manhattan formula:
 * https://xlinux.nist.gov/dads/HTML/manhattanDistance.html
 */
vector<vector<double>> getDistMatrixByManhattan(vector<Vertex> const& vertex);

/**
 * @brief getDistMatrixByHaversine_Miles - This function computes the distance matrix using the Haversine formula (in Miles).
 * @param[in] vertex - Vector of vertex information.
 * @return Returns a two-dimensional matrix containing the distance matrix.
 *
 * Useful link on the Haversine formula:
 * https://www.geeksforgeeks.org/haversine-formula-to-find-distance-between-two-points-on-a-sphere/
 */
vector<vector<double>> getDistMatrixByHaversine_Miles(vector<Vertex> const& vertex);

/**
 * @brief getDistMatrixByHaversine_KM - This function computes the distance matrix using the Haversine formula (in KM).
 * @param[in] vertex - Vector of vertex information.
 * @return Returns a two-dimensional matrix containing the distance matrix.
 *
 * Useful link on the Haversine formula:
 * https://www.geeksforgeeks.org/haversine-formula-to-find-distance-between-two-points-on-a-sphere/
 */
vector<vector<double>> getDistMatrixByHaversine_KM(vector<Vertex> const& vertex);

#endif // DISTANCE_H
