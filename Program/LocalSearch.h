//#pragma once
#ifndef _LOCALSEARCH_H
#define _LOCALSEARCH_H

#include "Data.h"

/************************************************************************************
 Method: LocalSearch
 Description: RVND
*************************************************************************************/
TSol LocalSearch(TSol &s, int const& numVertices, vector<vector<double>> const& dist);

TSol LocalSearch2(TSol &s, vector<Vehicle> &v, int const& numVertices, vector<vector<double>> const& dist);

TSol LS1(TSol s, vector<Vehicle> &v, int n, vector<vector<double>> const& dist); //Swap intra-route

TSol LS2(TSol s, vector<Vehicle> &v, int n, vector<vector<double>> const& dist); // Shift intra-route

TSol LS3(TSol s, vector<Vehicle> &v, int n, vector<vector<double>> const& dist); //Swap inter-route

TSol LS4(TSol s, vector<Vehicle> &v, int n, vector<vector<double>> const& dist); // Shift inter-route

TSol LS5(TSol s, vector<Vehicle> &v, int n, vector<vector<double>> const& dist); // 2-Opt

double rand(const double min, const double max);

int irand(const int min, const int max);

void depotFirst(vector<Customer> &route);

/************************************************************************************
 Method: setDistGetFo
 Description: set the dist value for entire route and calculate the objective function
 of the current route
*************************************************************************************/
double setDistGetFo(vector<Customer> &route, vector<vector<double>> const& dist);

void ls2opt(vector <Customer> &route, int n, vector<vector<double>> const& dist);

void lsSwap(vector <Customer> &route, int n, vector<vector<double>> const& dist);

void lsShift(vector <Customer> &route, int n, vector<vector<double>> const& dist);

void lsSwapInter(vector <Customer> &r1, vector <Customer> &r2, int n, int m, vector<vector<double>> const& dist);

void lsShiftInter(vector <Customer> &r1, vector <Customer> &r2, int n, int m, vector<vector<double>> const& dist);

#endif
