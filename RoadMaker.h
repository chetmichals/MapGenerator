#ifndef ROADMAKER_H
#define ROADMAKER_H

#include <iostream>
#include <vector>
#include <cmath>

struct Point {
	int x;
	int y;
};

struct Pairs{
	int PointA;
	int PointB;
	int Weight;
};

using namespace std;

void RoadMaker(vector < vector < char> > &MapArray, int MaxX, int MaxY);
void MinTreeMaker(vector < vector < char> > MapArray, int MaxX, int MaxY, vector<Point> &PointsHolder, vector<Pairs> &MinTree);
void RoadDrawer(vector < vector < char> > &MapArray, vector<Point> &PointsHolder, vector<Pairs> &MinTree);


#endif