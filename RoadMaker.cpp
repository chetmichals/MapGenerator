#include "RoadMaker.h"

void RoadMaker(vector < vector < char> > &MapArray, int MaxX, int MaxY)
{
	vector<Point> PointsHolder; 
	vector<Pairs> MinTree;
	MinTreeMaker(MapArray, MaxX, MaxY, PointsHolder, MinTree);
	RoadDrawer(MapArray, PointsHolder, MinTree);
}

//Creates a Minimum Spanning Tree using Prim’s Algorithm,
void MinTreeMaker(vector < vector < char> > MapArray, int MaxX, int MaxY, vector<Point> &PointsHolder, vector<Pairs> &MinTree)
{
	vector<int> HookedNumbers;
	vector<int> FilledPointA;
	HookedNumbers.push_back(0);

	Point Temp;
	Temp.x = 0;
	Temp.y = 0;
	//while (Temp.y != -1)
	//{
	//	cout << "Please enter the X: ";
	//	cin >> Temp.x;
	//	cout << "Please enter the Y: ";
	//	cin >> Temp.y;
	//	if (Temp.y != -1)
	//		PointsHolder.push_back(Temp);
	//}


	for (int i = 0; i < MaxY; i++)
	{
		for (int k = 0; k < MaxX; k++)
		{
			if (MapArray[i][k] == 'C' || MapArray[i][k] == 'r' || MapArray[i][k] == 'b' || MapArray[i][k] == 'A' || MapArray[i][k] == 'd' || MapArray[i][k] == 'V')
			{
				Temp.x = k;
				Temp.y = i;
				PointsHolder.push_back(Temp);
			}
		}
	}



	vector < vector < int > > Weights (PointsHolder.size(), vector <int>(PointsHolder.size()));

	unsigned int i, j;
	for (i = 0; i < PointsHolder.size(); i++)
	{
		for (j = 0; j < PointsHolder.size(); j++)
		{
			Weights[i][j] = ((int)abs(PointsHolder[i].x - PointsHolder[j].x) + (int)abs(PointsHolder[i].y - PointsHolder[j].y));
		}
	}
	
	
	int NodesLeft = PointsHolder.size() - 1;

	while (NodesLeft != 0)
	{
		int minWeight = 999999;
		Pairs tempPair;
		for (i = 0; i < HookedNumbers.size(); i++)
		{
			for (j = 0; j < PointsHolder.size(); j++)
			{
				if (Weights[HookedNumbers[i]][j] != 0)
				{
					bool IsNumberHooked = false;

					for (unsigned int k = 0; k < HookedNumbers.size(); k++)
					{
						int value = HookedNumbers[k];
						if ( j == value)
						{
							IsNumberHooked = true;
						}
						else 
						{
							for (unsigned int l = 0; l < FilledPointA.size(); l++)
							{
								int value2 = FilledPointA[l];
								if ( HookedNumbers[i] == value2)
								{
									IsNumberHooked = true;
								}
							}
						}
					}

					


					if (!IsNumberHooked)
					{
						if (Weights[HookedNumbers[i]][j] < minWeight)
						{
							tempPair.PointA = HookedNumbers[i];
							tempPair.PointB = j;
							minWeight = Weights[HookedNumbers[i]][j];
						}
					}
				}

			}
		}
		if (minWeight != 999999)
		{
			cout << MinTree.size() << ": " << minWeight << endl;
			tempPair.Weight = minWeight;
			MinTree.push_back(tempPair);
			HookedNumbers.push_back(tempPair.PointB);
			FilledPointA.push_back(tempPair.PointA);
		}

		NodesLeft--;

	}
}


void RoadDrawer(vector < vector < char> > &MapArray, vector<Point> &PointsHolder, vector<Pairs> &MinTree)
{
	for (unsigned int i = 0; i < MinTree.size(); i++)
	{
		if (MinTree[i].Weight != 1)
		{
		int q = rand()%2;
			int MoveDir = 1;
			//decides if road will come out of side, or top, randomly
			int StartX, StartY;
		
			StartX = PointsHolder[MinTree[i].PointA].x;
			if (PointsHolder[MinTree[i].PointA].y > PointsHolder[MinTree[i].PointB].y)
				MoveDir = -1;
			else if (PointsHolder[MinTree[i].PointA].y == PointsHolder[MinTree[i].PointB].y)
				MoveDir = 0;

			StartY = PointsHolder[MinTree[i].PointA].y + MoveDir;

			while (StartY != PointsHolder[MinTree[i].PointB].y)
			{
				if (MapArray[StartY][StartX] != 'C' && MapArray[StartY][StartX] != 'r' && MapArray[StartY][StartX] != 'b' && MapArray[StartY][StartX] != 'A' && MapArray[StartY][StartX] != 'd' && MapArray[StartY][StartX] != 'V')
				{
					if (MapArray[StartY][StartX] != 'W')	
						MapArray[StartY][StartX] = 'R';
					else
						MapArray[StartY][StartX] = 'B';
				}

				StartY += MoveDir;
			}
		
			MoveDir = 1;
			if (PointsHolder[MinTree[i].PointA].x > PointsHolder[MinTree[i].PointB].x)
				MoveDir = -1;
			else if (PointsHolder[MinTree[i].PointA].x == PointsHolder[MinTree[i].PointB].x)
				MoveDir = 0;

			while (StartX != PointsHolder[MinTree[i].PointB].x)
			{
				if (MapArray[StartY][StartX] != 'C' && MapArray[StartY][StartX] != 'r' && MapArray[StartY][StartX] != 'b' && MapArray[StartY][StartX] != 'A' && MapArray[StartY][StartX] != 'd' && MapArray[StartY][StartX] != 'V')
								{
					if (MapArray[StartY][StartX] != 'W')	
						MapArray[StartY][StartX] = 'R';
					else
						MapArray[StartY][StartX] = 'B';
				}

				StartX += MoveDir;
			}
		
		}

	}
}
