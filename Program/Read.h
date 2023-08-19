//#pragma once
#ifndef _READ_H
#define _READ_H

#include "Data.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>

void ReadData(char nameTable[], string &name, string &region, double &capacity, vector<Vertex> &vertex, vector<vector<double>> &dist);

void printReport(string const& name, string const& region, double const& capacity, vector<Vertex> const& vertex);

void printDistMatrix(string const& name, string const& region, vector<Vertex> const& vertex, vector<vector<double>> const& dist);

void printSolution(string const& name, string const& region, double const& capacity, vector<int> const& vec, vector<Vertex> const& vertex, vector<vector<double>> const& dist);

void printSolutionLoggi(string const& name, string const& region, double const& capacity, vector<int> const& vec, vector<Vertex> const& vertex, vector<vector<double>> const& dist, vector<Vehicle> v);

void FreeMemoryProblem(vector<Vertex> &vertex, vector<vector<double>> &dist);

#endif
