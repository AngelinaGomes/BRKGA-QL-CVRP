//#pragma once
#ifndef _DATA_H
#define _DATA_H

#include <vector>
#include <string>
#include <algorithm>
using std::vector;
using std::string;

//------ DEFINITION OF TYPES OF PROBLEM SPECIFIC --------

/**
 * @brief The Vertex struct.
 *
 * This structure is used to store vertex information.
 */
struct Vertex {
    string id;   /**< ID. */
    double lat;  /**< Latitude. */
    double lng;  /**< Longitute. */
    double size; /**< Demand. */

    /**
     * @brief Vertex - Default constructor.
     */
    Vertex() : id(""), lat(0.0), lng(0.0), size(0.0) {}

    /**
     * @brief Vertex - Adapted constructor.
     * @param[in] id - ID.
     * @param[in] lat - Latitude.
     * @param[in] lng - Longitude.
     * @param[in] size - Demand.
     */
    Vertex(string const& id, double const& lat, double const& lng, double const& size) :
        id(id), lat(lat), lng(lng), size(size) {}
};


/**
 * @brief The Customer struct.
 *
 * This structure is used to store the position of customers in the solution
 * and the distances between them and their predecessors.
 */
struct Customer {
    int point;          /**< Point of a route. */
    double dist;        /**< Distance between current customer and its predecessor. */
};

/**
 * @brief The Vehicle struct.
 *
 * This structure is used to store the customers served by a vehicle.
 */
struct Vehicle {
    vector <Customer> route;        /**< Customers served by a vehicle. */
    double load;                    /**< Vehicle occupied capacity. */
    double totalDist;               /**< Total distance traveled on this route. */
};




//------ DEFINITION OF TYPES OF BRKGA-QL --------

/***********************************************************************************
 Struct: TVecSol
 Description: struct to represent the two vector solutions (rk and problem)
************************************************************************************/
struct TVecSol
{
    int sol;                                // position of chromosome
    double rk;                              // random-key of chromosome
};


/***********************************************************************************
 Struct: TSol
 Description: struct to represent a solution problem
************************************************************************************/
struct TSol
{
    std::vector <TVecSol> vec;                      // solution of the problem and random key
    double fo = std::numeric_limits<double>::max(); // objetive function value
    int label;                                      // defines a community solution with a number
    int similar;                                    // indicates if a solution is similar to other (0 no, 1 yes)
    int flag;                                       // indicates if a local search has already been performed on this solution (0 no, 1 yes)
    int promising;                                  // indicates if a solution is promising to apply local search
};


/***********************************************************************************
 Struct: TQ
 Description: struct to represent a quality matrix
************************************************************************************/
struct TQ
{
    int S;
    double pVar;
    double q;
    int k;
};

#endif
