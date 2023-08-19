//#pragma once
#ifndef _DECODER_H
#define _DECODER_H

#define INFINITO 999999999

#include <iostream>
#include <map>
#include <math.h>
#include <cstdio>
#include "Data.h"

using namespace std;

#define MAX(x,y) ((x)<(y) ? (y) : (x))

/************************************************************************************
 Method: Decoder()
 Description: Convert a random key solution in a real problem solution
*************************************************************************************/
TSol Decoder(TSol &s, vector<Vehicle> &v, int const& n, double const& capacity, vector<Vertex> const& vertex, vector<vector<double>> const& dist);

TSol Dec1(TSol &s, vector<Vehicle> &v, int const& n, double const& capacity, vector<Vertex> const& vertex, vector<vector<double>> const& dist);

TSol Dec2(TSol &s, vector<Vehicle> &v, int const& n, double const& capacity, vector<Vertex> const& vertex, vector<vector<double>> const& dist);

TSol Dec3(TSol &s, vector<Vehicle> &v, int const& n, double const& capacity, vector<Vertex> const& vertex, vector<vector<double>> const& dist, int const& minVehicles);

TSol Dec4(TSol &s, vector<Vehicle> &v, int const& n, double const& capacity, vector<Vertex> const& vertex, vector<vector<double>> const& dist, int const& minVehicles);

#endif
