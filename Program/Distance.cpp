/** /distance.cpp
 * This file is used to compute the distance matrix.
 *
 * Developed by:
 *
 * Angelina Gomes Santos - angelina.gomes@unifesp.br
 * Antônio Augusto Chaves - antonio.chaves@unifesp.br
 * Cleder Marcos Schenekemberg - cledercms@hotmail.com
 */

#include "Distance.h"
#include <cmath>

/**
 * @brief getEuclideanDistance - Computes the distance between (lat1, lng1) and (lat2, lng2) using the Euclidean formula.
 * @param[in] lat1 - Latitude 1.
 * @param[in] lng1 - longitude 1.
 * @param[in] lat2 - Latitude 2.
 * @param[in] lng2 - longitude 2.
 * @return Returns the the distance between (lat1, lng1) and (lat2, lng2).
 */
double getEuclideanDistance(double const& lat1, double const& lng1, double const& lat2, double const& lng2) {
    return sqrt(pow((lat1 - lat2), 2.0) + pow((lng1 - lng2), 2.0));
}

/**
 * @brief getRoundedEuclideanDistance - Computes the distance between (lat1, lng1) and (lat2, lng2) using the rounded Euclidean formula.
 * @param[in] lat1 - Latitude 1.
 * @param[in] lng1 - longitude 1.
 * @param[in] lat2 - Latitude 2.
 * @param[in] lng2 - longitude 2.
 * @return Returns the the distance between (lat1, lng1) and (lat2, lng2).
 */
double getRoundedEuclideanDistance(double const& lat1, double const& lng1, double const& lat2, double const& lng2) {
    double euclideanDistance = getEuclideanDistance(lat1, lng1, lat2, lng2);
    return floor(euclideanDistance + 0.5);
}

/**
 * @brief getManhattanEuclideanDistance - Computes the distance between (lat1, lng1) and (lat2, lng2) using the Manhattan formula.
 * @param[in] lat1 - Latitude 1.
 * @param[in] lng1 - longitude 1.
 * @param[in] lat2 - Latitude 2.
 * @param[in] lng2 - longitude 2.
 * @return Returns the the distance between (lat1, lng1) and (lat2, lng2).
 */
double getManhattanDistance(double const& lat1, double const& lng1, double const& lat2, double const& lng2) {
    return std::abs(lat1 - lat2) + std::abs(lng1 - lng2);
}

/**
 * @brief getHarvesineDistance_Miles - Computes the distance between (lat1, lng1) and (lat2, lng2) using the Haversine formula (in Miles).
 * @param[in] lat1 - Latitude 1.
 * @param[in] lng1 - longitude 1.
 * @param[in] lat2 - Latitude 2.
 * @param[in] lng2 - longitude 2.
 * @return Returns the the distance between (lat1, lng1) and (lat2, lng2) in Miles.
 */
double getHarvesineDistance_Miles(double const& lat1, double const& lng1, double const& lat2, double const& lng2) {

    // Constants.
    const double C_PI = 3.14159265358979;
    const double C_RADIUS_EARTH = 6370.97327862;
    const double C_MILES_PER_KM = 0.621371;
    const double diff_lat = (lat2 - lat1) * C_PI / 180.0;
    const double diff_lng = (lng2 - lng1) * C_PI / 180.0;

    // Convert to radians: radians = (degrees / 180) * PI.
    const double val_lat1 = lat1 * C_PI / 180.0;
    const double val_lat2 = lat2 * C_PI / 180.0;

    // Get the central spherial angle.
    double delta = 2.0 * sin(sqrt(pow(sin(diff_lat / 2.0), 2.0) + pow(sin(diff_lng / 2.0), 2.0) * cos(val_lat1) * cos(val_lat2)));

    return delta * C_RADIUS_EARTH * C_MILES_PER_KM;
}

/**
 * @brief getHarvesineDistance_KM - Computes the distance between (lat1, lng1) and (lat2, lng2) using the Haversine formula (in KM).
 * @param[in] lat1 - Latitude 1.
 * @param[in] lng1 - longitude 1.
 * @param[in] lat2 - Latitude 2.
 * @param[in] lng2 - longitude 2.
 * @return Returns the the distance between (lat1, lng1) and (lat2, lng2) in KM.
 */
double getHarvesineDistance_KM(double const& lat1, double const& lng1, double const& lat2, double const& lng2) {

    // Constants.
    const double C_PI = 3.14159265358979;
    const double C_RADIUS_EARTH = 6370.97327862;
    const double diff_lat = (lat2 - lat1) * C_PI / 180.0;
    const double diff_lng = (lng2 - lng1) * C_PI / 180.0;

    // Convert to radians: radians = (degrees / 180) * PI.
    const double val_lat1 = lat1 * C_PI / 180.0;
    const double val_lat2 = lat2 * C_PI / 180.0;

    // Get the central spherial angle.
    double delta = 2.0 * sin(sqrt(pow(sin(diff_lat / 2.0), 2.0) + pow(sin(diff_lng / 2.0), 2.0) * cos(val_lat1) * cos(val_lat2)));

    return delta * C_RADIUS_EARTH;
}

/**
 * @brief getDistMatrixByEuclidean - This function computes the distance matrix using the Euclidean formula.
 * @param[in] vertex - Vector of vertex information.
 * @return Returns a two-dimensional matrix containing the distance matrix.
 *
 * Useful link on the Euclidean formula:
 * https://sciencing.com/calculate-area-using-coordinates-8265405.html
 */
vector<vector<double>> getDistMatrixByEuclidean(vector<Vertex> const& vertex) {

    // Number of elements.
    size_t numElems = vertex.size();

    // Distance matrix.
    // Useful indexes:
    // Rows:    0 to (numElems - 1).
    // Columns: 0 to (numElems - 1).
    vector<vector<double>> distMatrix(numElems, vector<double>(numElems, 0.0));

    // Computes the distance matrix using Euclidean formula.
    for (size_t i = 0; i < numElems; i++) {
        for (size_t j = 0; j < numElems; j++) {
            if (i == j) continue;
            distMatrix[i][j] = getEuclideanDistance(vertex[i].lat, vertex[i].lng, vertex[j].lat, vertex[j].lng);
        }
    }

    return distMatrix;
}

/**
 * @brief getDistMatrixByRoundedEuclidean - This function computes the distance matrix using the rounded Euclidean formula.
 * @param[in] vertex - Vector of vertex information.
 * @return Returns a two-dimensional matrix containing the distance matrix.
 *
 * Useful link on the roudend Euclidean formula:
 * https://sciencing.com/calculate-area-using-coordinates-8265405.html
 */
vector<vector<double>> getDistMatrixByRoundedEuclidean(vector<Vertex> const& vertex) {

    // Number of elements.
    size_t numElems = vertex.size();

    // Distance matrix.
    // Useful indexes:
    // Rows:    0 to (numElems - 1).
    // Columns: 0 to (numElems - 1).
    vector<vector<double>> distMatrix(numElems, vector<double>(numElems, 0.0));

    // Computes the distance matrix using rounded Euclidean formula.
    for (size_t i = 0; i < numElems; i++) {
        for (size_t j = 0; j < numElems; j++) {
            if (i == j) continue;
            distMatrix[i][j] = getRoundedEuclideanDistance(vertex[i].lat, vertex[i].lng, vertex[j].lat, vertex[j].lng);
        }
    }

    return distMatrix;
}

/**
 * @brief getDistMatrixByManhattan - This function computes the distance matrix using the Manhattan formula.
 * @param[in] vertex - Vector of vertex information.
 * @return Returns a two-dimensional matrix containing the distance matrix.
 *
 * Useful link on the Manhattan formula:
 * https://xlinux.nist.gov/dads/HTML/manhattanDistance.html
 */
vector<vector<double>> getDistMatrixByManhattan(vector<Vertex> const& vertex) {

    // Number of elements.
    size_t numElems = vertex.size();

    // Distance matrix.
    // Useful indexes:
    // Rows:    0 to (numElems - 1).
    // Columns: 0 to (numElems - 1).
    vector<vector<double>> distMatrix(numElems, vector<double>(numElems, 0.0));

    // Computes the distance matrix using Manhattan formula.
    for (size_t i = 0; i < numElems; i++) {
        for (size_t j = 0; j < numElems; j++) {
            if (i == j) continue;
            distMatrix[i][j] = getManhattanDistance(vertex[i].lat, vertex[i].lng, vertex[j].lat, vertex[j].lng);
        }
    }

    return distMatrix;
}

/**
 * @brief getDistMatrixByHaversine_Miles - This function computes the distance matrix using the Haversine formula (in Miles).
 * @param[in] vertex - Vector of vertex information.
 * @return Returns a two-dimensional matrix containing the distance matrix.
 *
 * Useful link on the Haversine formula:
 * https://www.geeksforgeeks.org/haversine-formula-to-find-distance-between-two-points-on-a-sphere/
 */
vector<vector<double>> getDistMatrixByHaversine_Miles(vector<Vertex> const& vertex) {

    // Number of elements.
    size_t numElems = vertex.size();

    // Distance matrix.
    // Useful indexes:
    // Rows:    0 to (numElems - 1).
    // Columns: 0 to (numElems - 1).
    vector<vector<double>> distMatrix(numElems, vector<double>(numElems, 0.0));

    // Computes the distance matrix using Haversine formula (in Miles).
    for (size_t i = 0; i < numElems; i++) {
        for (size_t j = 0; j < numElems; j++) {
            if (i == j) continue;
            distMatrix[i][j] = getHarvesineDistance_Miles(vertex[i].lat, vertex[i].lng, vertex[j].lat, vertex[j].lng);
        }
    }

    return distMatrix;
}

/**
 * @brief getDistMatrixByHaversine_KM - This function computes the distance matrix using the Haversine formula (in KM).
 * @param[in] vertex - Vector of vertex information.
 * @return Returns a two-dimensional matrix containing the distance matrix.
 *
 * Useful link on the Haversine formula:
 * https://www.geeksforgeeks.org/haversine-formula-to-find-distance-between-two-points-on-a-sphere/
 */
vector<vector<double>> getDistMatrixByHaversine_KM(vector<Vertex> const& vertex) {

    // Number of elements.
    size_t numElems = vertex.size();

    // Distance matrix.
    // Useful indexes:
    // Rows:    0 to (numElems - 1).
    // Columns: 0 to (numElems - 1).
    vector<vector<double>> distMatrix(numElems, vector<double>(numElems, 0.0));

    // Computes the distance matrix using Haversine formula (in KM).
    for (size_t i = 0; i < numElems; i++) {
        for (size_t j = 0; j < numElems; j++) {
            if (i == j) continue;
            distMatrix[i][j] = getHarvesineDistance_KM(vertex[i].lat, vertex[i].lng, vertex[j].lat, vertex[j].lng);
        }
    }

    return distMatrix;
}
