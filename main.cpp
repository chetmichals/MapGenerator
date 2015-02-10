#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <unordered_map>
#include "ConvertToBMP.h"
#include "RoadMaker.h"

using namespace std;

int MapSize();
int MakeMaps();
void MapInitilize(vector < vector < char> > &MapArray, int MapHeight, int MapLength);
void SecondarySeeder(vector < vector < char> > &MapArray, char BaseMapType, int MapHeight, int MapLength);
void SecondarySeedGrow(vector < vector < char> > &MapArray, char SecondaryType, int Generation, int StoppingPoint, int MyX, int MyY, int MaxX, int MaxY);
void DumpMapArray(vector < vector < char> > &MapArray, ofstream &myfile, int MapHeight, int MapLength, int Seed);
void HillMaker(vector < vector < char> > &MapArray, int MyX, int MyY, int MaxX, int MaxY, int HillChance, int LastDir);
void WaterMaker(vector < vector < char> > &MapArray, int MapHeight, int MapLength);
void LakeMaker(vector < vector < char> > &MapArray, int Generation, int StoppingPoint, int MyX, int MyY, int MaxX, int MaxY);
void RiverMaker(vector < vector < char> > &MapArray, int MyX, int MyY, int MaxX, int MaxY, int LastDir);
void CityAndFactoryGenerator(vector < vector < char> > &MapArray, int MaxX, int MaxY);
void AirPortGenerator(vector < vector < char> > &MapArray, int MaxX, int MaxY);
void HQMaker(vector < vector < char> > &MapArray, int MaxX, int MaxY);
void HQGrower(vector < vector < char> > &MapArray, int MaxX, int MaxY);
void HQGrowerR(vector < vector < char> > &MapArray, int x, int y);
void HQGrowerB(vector < vector < char> > &MapArray, int X, int Y);
void DockGenerator(vector < vector < char> > &MapArray, int MaxX, int MaxY);
void DockSpotFinder(vector < vector < char> > &MapArray, int MaxX, int MaxY);
int  WaterCounter(vector < vector < char> > MapArray, int MaxX, int MaxY);

int main()
{
	MakeMaps();
	//cout << "How many maps do you wish to make? " << endl;
	//int x;
	//cin >> x;
	//for (int i = 0; i < 10; i++)
	//{
	//	cout << "Map #" << i + 1 << endl;
	//	MakeMaps();

	//}
}

int MakeMaps()
{
	int Seed = (unsigned)time(NULL);
	//Seed = 1318309557;
	srand(Seed); 


	ofstream myfile;

	int MapHeight;
	int MapLength;
	char BaseMapType;

	MapHeight = MapSize();
	MapLength = MapSize();

	cout << "The Map is " << MapHeight << " by " << MapLength << endl;
	
	vector< vector <char> > MapArray (MapHeight, vector <char>(MapLength));
	MapInitilize(MapArray, MapHeight, MapLength);


	BaseMapType = MapArray[0][0];
	for (int j =0; j < 5; j++)
	{
		cout << "#" << j+1 << endl;
		SecondarySeeder(MapArray,BaseMapType,MapHeight,MapLength);
	}
	
	int HillRolls = rand() % 20;
	for (int j = 0; j < HillRolls; j++)
	{
		cout << "Hill set #" << j + 1 << endl;
		int x = rand() % 100;
		if (x < 25)
		{
			int MyX, MyY;
			MyX = rand() % MapLength;
			MyY = rand() % MapHeight;
			HillMaker(MapArray,MyX,MyY,MapLength,MapHeight,rand()%100,rand()%4);
		}
	}

	WaterMaker(MapArray,MapHeight,MapLength);
	CityAndFactoryGenerator(MapArray,MapLength,MapHeight);
	AirPortGenerator(MapArray,MapLength,MapHeight);
	HQMaker(MapArray,MapLength,MapHeight);
	DockGenerator(MapArray,MapLength,MapHeight);
	RoadMaker(MapArray,MapLength,MapHeight);
	HQGrower(MapArray,MapLength,MapHeight);
	DumpMapArray(MapArray, myfile, MapHeight, MapLength, Seed);
	ConvertToBMP(MapArray,MapHeight,MapLength, Seed);
	return 0;
}

int MapSize()
{
	int x = rand ();
	x = x % 60;
	x = x + 15;
	return x;
}

void MapInitilize(vector < vector < char> > &MapArray, int MapHeight, int MapLength)
{
	int i, j, x;
	char y;
	x = rand () % 100;
	if (x < 70)
	{
		y = 'p';
	}
	else if (x < 90)
	{
		y = 'f';
	}
	else
	{
		y = 'D';
	}

	for (i = 0; i < MapHeight; i++)
	{
		for (j = 0; j < MapLength; j++)
		{
			MapArray[i][j] = y;
		}
	}
}

void SecondarySeeder(vector < vector < char> > &MapArray, char BaseMapType, int MapHeight, int MapLength)
{
	char SecondaryType;
	int x = rand ();
	x = x % 100;
	int StartX, StartY;
	int StoppingPoint = (int)floor((double)(MapHeight+MapLength)/6);

	if (x < 50)
	{
		SecondaryType = 'p';
	}
	else if (x < 80)
	{
		SecondaryType = 'f';
	}
	else
	{
		SecondaryType = 'D';
	}

	if (BaseMapType == SecondaryType) return;
	
	StartX = rand() % MapLength;
	StartY = rand() % MapHeight;



	SecondarySeedGrow(MapArray, SecondaryType, 0, StoppingPoint, StartX, StartY,MapLength,MapHeight);

}

void SecondarySeedGrow(vector < vector < char> > &MapArray, char SecondaryType, int Generation, int StoppingPoint, int MyX, int MyY, int MaxX, int MaxY)
{
	if (Generation >= StoppingPoint) 
		return;

	if (MyX >= MaxX || MyY >= MaxY || MyX < 0 || MyY < 0) return;

	float chance = (float)(100 * (1 - (Generation/StoppingPoint)));
	if (chance < (rand() % 100)) return;

	MapArray[MyY][MyX] = SecondaryType;

	if (MyX+1 < MaxX && MapArray[MyY][MyX+1] != SecondaryType)
	{
			SecondarySeedGrow(MapArray, SecondaryType, Generation+1, StoppingPoint, MyX+1, MyY, MaxX, MaxY); //To the Right
	}
	if (MyX-1 >= 0 && MapArray[MyY][MyX-1] != SecondaryType)
	{
		SecondarySeedGrow(MapArray, SecondaryType, Generation+1, StoppingPoint, MyX-1, MyY, MaxX, MaxY); //To the Left
	}
	if (MyY+1 < MaxY && MapArray[MyY+1][MyX] != SecondaryType)
	{
		SecondarySeedGrow(MapArray, SecondaryType, Generation+1, StoppingPoint, MyX, MyY+1, MaxX, MaxY); //Down One
	}
	if (MyY-1 >= 0 && MapArray[MyY-1][MyX] != SecondaryType)
	{
		SecondarySeedGrow(MapArray, SecondaryType, Generation+1, StoppingPoint, MyX, MyY-1, MaxX, MaxY); //Up One
	}
}

//LastDir is the last Dirrection gone. 0 is up, 1 is right, 2 is down, 3 is left
void HillMaker(vector < vector < char> > &MapArray, int MyX, int MyY, int MaxX, int MaxY, int HillChance, int LastDir)
{
	if (MyX < 0 || MyX >=MaxX || MyY < 0 || MyY >=MaxY) return;
	int x;
	int NextDir = LastDir;
	x = rand() % 100;
	if (x < HillChance)
		MapArray[MyY][MyX] = 'H';
	else
		MapArray[MyY][MyX] = 'M';

	x = rand() % 100;
	if (x < 5) return;
	else if (x < 25) 
		NextDir-=1;
	else if (x < 45)
		NextDir+=1;
	NextDir= NextDir % 4;

	if (NextDir == 0)
		HillMaker(MapArray, MyX, MyY-1, MaxX, MaxY, HillChance, NextDir);
	else if (NextDir == 1)
		HillMaker(MapArray, MyX+1, MyY, MaxX, MaxY, HillChance, NextDir);
	else if (NextDir == 2)
		HillMaker(MapArray, MyX, MyY+1, MaxX, MaxY, HillChance, NextDir);
	else
		HillMaker(MapArray, MyX-1, MyY, MaxX, MaxY, HillChance, NextDir);
}

void DumpMapArray(vector < vector < char> > &MapArray, ofstream &myfile, int MapHeight, int MapLength, int Seed)
{
	stringstream FileNumber;
	FileNumber << Seed;
	string FileName = "Map";
	FileName += FileNumber.str();
	FileName += ".txt";

	myfile.open(FileName,ios::out);

	int i, j;
	for (i = 0; i < MapHeight; i++)
	{
		for (j = 0; j < MapLength; j++)
		{
			myfile << MapArray[i][j];
		}
		myfile << endl;
	}
	myfile << "Seed Number: " << Seed << endl;
}

void WaterMaker(vector < vector < char> > &MapArray, int MapHeight, int MapLength)
{
	int x;
	int StoppingPoint = (int)floor((double)(MapHeight+MapLength)/4);
	//Rolls for lakes

	for (int i = 0; i < 3; i++)
	{
		cout << "Lake #" << i + 1 << endl;
		x = rand() % 100;
		if (x < 40)
		{
			LakeMaker(MapArray,0,StoppingPoint, rand() % MapLength, rand() % MapHeight,MapLength, MapHeight);
		}
	}
	//Rolls for Rivers
	for (int i = 0; i < 5; i++)
	{
		cout << "River #" << i + 1 << endl;
		x = rand() % 100;
		if (x < 30)
		{
			int y;
			x = rand() % 4;			
			if (x == 0)
			{
				y = rand() % MapLength;
				RiverMaker(MapArray,y,0,MapLength,MapHeight,2);
			}
			else if (x == 1)
			{
				y = rand() % MapHeight;
				RiverMaker(MapArray,0,y,MapLength,MapHeight,3);
			}
			else if (x == 2)
			{
				y = rand() % MapLength;
				RiverMaker(MapArray,y,MapHeight-1,MapLength,MapHeight,0);
			}
			else
			{
				y = rand() % MapHeight;
				RiverMaker(MapArray,MapLength-1,y,MapLength,MapHeight,1);
			}

		}
	}
}

void LakeMaker(vector < vector < char> > &MapArray, int Generation, int StoppingPoint, int MyX, int MyY, int MaxX, int MaxY)
{
	if (Generation >= StoppingPoint) 
		return;

	if (MyX >= MaxX || MyY >= MaxY || MyX < 0 || MyY < 0) return;

	float chance = (float)(100 * (1 - (Generation/StoppingPoint)));
	if (chance < (rand() % 100)) return;

	MapArray[MyY][MyX] = 'W';

	if (MyX+1 < MaxX && MapArray[MyY][MyX+1] != 'W')
	{
			LakeMaker(MapArray, Generation+1, StoppingPoint, MyX+1, MyY, MaxX, MaxY); //To the Right
	}
	if (MyX-1 >= 0 && MapArray[MyY][MyX-1] != 'W')
	{
		LakeMaker(MapArray, Generation+1, StoppingPoint, MyX-1, MyY, MaxX, MaxY); //To the Left
	}
	if (MyY+1 < MaxY && MapArray[MyY+1][MyX] != 'W')
	{
		LakeMaker(MapArray, Generation+1, StoppingPoint, MyX, MyY+1, MaxX, MaxY); //Down One
	}
	if (MyY-1 >= 0 && MapArray[MyY-1][MyX] != 'W')
	{
		LakeMaker(MapArray, Generation+1, StoppingPoint, MyX, MyY-1, MaxX, MaxY); //Up One
	}
}

void RiverMaker(vector < vector < char> > &MapArray, int MyX, int MyY, int MaxX, int MaxY, int LastDir)
{
	if (MyX < 0 || MyX >=MaxX || MyY < 0 || MyY >=MaxY) return;
	int x;
	int NextDir = LastDir;
	x = rand() % 100;
	MapArray[MyY][MyX] = 'W';

	x = rand() % 100;
	if (x < 25) 
		NextDir-=1;
	else if (x < 50)
		NextDir+=1;
	NextDir = NextDir % 4;

	if (NextDir == 0)
		RiverMaker(MapArray, MyX, MyY-1, MaxX, MaxY, NextDir);
	else if (NextDir == 1)
		RiverMaker(MapArray, MyX+1, MyY, MaxX, MaxY, NextDir);
	else if (NextDir == 2)
		RiverMaker(MapArray, MyX, MyY+1, MaxX, MaxY, NextDir);
	else
		RiverMaker(MapArray, MyX-1, MyY, MaxX, MaxY, NextDir);
}


void CityAndFactoryGenerator(vector < vector < char> > &MapArray, int MaxX, int MaxY)
{
	int x = rand() % 40;
	x = x + 10;
	int xSpot;
	int ySpot;
	int FactoryChance;

	for (int i = 0; i < x; i++)
	{
		xSpot = rand() % MaxX;
		ySpot = rand() % MaxY;
		FactoryChance = rand() % 4;
		if (FactoryChance) //If not zero, make a City. 3/4 chance
			MapArray[ySpot][xSpot] = 'C';
		else //If zero, make a factory, 1/4 chance. 
			MapArray[ySpot][xSpot] = 'V';
	}
	cout << x << " Cities" << endl;
}

void HQMaker(vector < vector < char> > &MapArray, int MaxX, int MaxY)
{
	int x = rand () % MaxX;
	int y = rand () % MaxY;
	
	//Makes sure X isn't in the "Buffer Zone", which is the outter 2 rows collumns, and the inner 4 rows and collumns
	if (x < 2) 
		x = 2;
	else if (x > MaxX - 3)
		x = x - 2;
	else if (x > MaxX/2 - 3 && x <= MaxX/2)
		x = x - 4;
	else if (x < MaxX/2 + 3 && x > MaxX/2)
		x = x + 3;

	//Do the same for Y
	if (y < 2) 
		y = 2;
	else if (y > MaxY - 3)
		y = y - 2;
	else if (y > MaxY/2 - 3 && y < MaxY/2)
		y = y - 3;
	else if (y < MaxY/2 + 3 && y > MaxY/2)
		y = y + 3;


	int ReflectType = 2;

	//Stuff for mirroring HQ location
	int RedX, RedY;
	RedX = MaxX - x;
	RedY = MaxY - y;

	if (RedX < 2)
		RedX +=1;
	if (RedX > MaxX - 3)
		RedX -=1;
	if (RedY < 2)
		RedY +=1;
	if (RedY > MaxY - 3)
		RedY -=1;

	//Horizontal Flip
	if (ReflectType == 0)
	{
		RedX = x;
		MapArray[y][x] = 'b';
		MapArray[RedY][RedX] = 'r';
	}
	//Virticle Flip
	else if (ReflectType == 1)
	{
		RedY = y;
		MapArray[y][x] = 'b';
		MapArray[RedY][RedX] = 'r';
	}
	//Diagonal Flip
	else if (ReflectType == 2)
	{
		MapArray[y][x] = 'b';
		MapArray[RedY][RedX] = 'r';
	}
}

void HQGrower(vector < vector < char> > &MapArray, int MaxX, int MaxY)
{
	int CountToTwo = 0;
	for (int i = 0; i < MaxY; i++)
	{
		for (int j = 0; j < MaxX; j++)
		{
			if (MapArray[i][j] == 'r')
			{	
				HQGrowerR(MapArray, j, i);
				CountToTwo++;
				cout << "Found Red HQ" << endl;
			}
			else if (MapArray[i][j] == 'b')
			{
				HQGrowerB(MapArray, j, i);
				CountToTwo++;
				cout << "Found Blue HQ" << endl;
			}
			else if (CountToTwo == 2)
				return;
		}	
	}

}

void HQGrowerR(vector < vector < char> > &MapArray, int X, int Y)
{
	MapArray[Y-2][X-2] = 'F';
	MapArray[Y+2][X+2] = 'F';
	MapArray[Y+2][X-2] = 'G';
	MapArray[Y-2][X+2] = 'G';

	MapArray[Y-2][X-1] = 'R';
	MapArray[Y-2][X] = 'R';
	MapArray[Y-2][X+1] = 'R';

	MapArray[Y+2][X-1] = 'R';
	MapArray[Y+2][X] = 'R';
	MapArray[Y+2][X+1] = 'R';

	MapArray[Y-1][X-2] = 'R';
	MapArray[Y][X-2] = 'R';
	MapArray[Y+1][X-2] = 'R';

	MapArray[Y-1][X+2] = 'R';
	MapArray[Y][X+2] = 'R';
	MapArray[Y+1][X+2] = 'R';

	MapArray[Y-1][X] = 'R';
	MapArray[Y+1][X] = 'R';

	MapArray[Y-1][X-1] = 'p';
	MapArray[Y][X-1] = 'p';
	MapArray[Y+1][X-1] = 'p';

	MapArray[Y-1][X+1] = 'p';
	MapArray[Y][X+1] = 'p';
	MapArray[Y+1][X+1] = 'p';
}

void HQGrowerB(vector < vector < char> > &MapArray, int X, int Y)
{
	MapArray[Y-2][X-2] = 'v';
	MapArray[Y+2][X+2] = 'v';
	MapArray[Y+2][X-2] = 'g';
	MapArray[Y-2][X+2] = 'g';

	MapArray[Y-2][X-1] = 'R';
	MapArray[Y-2][X] = 'R';
	MapArray[Y-2][X+1] = 'R';

	MapArray[Y+2][X-1] = 'R';
	MapArray[Y+2][X] = 'R';
	MapArray[Y+2][X+1] = 'R';

	MapArray[Y-1][X-2] = 'R';
	MapArray[Y][X-2] = 'R';
	MapArray[Y+1][X-2] = 'R';

	MapArray[Y-1][X+2] = 'R';
	MapArray[Y][X+2] = 'R';
	MapArray[Y+1][X+2] = 'R';

	MapArray[Y-1][X] = 'R';
	MapArray[Y+1][X] = 'R';

	MapArray[Y-1][X-1] = 'p';
	MapArray[Y][X-1] = 'p';
	MapArray[Y+1][X-1] = 'p';

	MapArray[Y-1][X+1] = 'p';
	MapArray[Y][X+1] = 'p';
	MapArray[Y+1][X+1] = 'p';
}

void AirPortGenerator(vector < vector < char> > &MapArray, int MaxX, int MaxY)
{
	//Since the current smallest map randomly generated is 15x15, there will be at least 2 airports, and upto 56 in a 75x75 map
	int AirportNumber = (MaxX * MaxY);

	AirportNumber = (int)(log((float)AirportNumber) / log(1.5));
	AirportNumber-=11;

	for (int i = 0; i < AirportNumber; i++)
	{
		int xSpot = rand() % MaxX;
		int ySpot = rand() % MaxY;
		MapArray[ySpot][xSpot] = 'A';
	}
	cout << "Number of Airports: "<< AirportNumber << endl;
}

void DockGenerator(vector < vector < char> > &MapArray, int MaxX, int MaxY)
{
	int DockNumber = (MaxX * MaxY);

	DockNumber = (int)(log((float)DockNumber) / log(5.0));
	DockNumber*=2;

	int WaterNumber = WaterCounter(MapArray, MaxX ,MaxY);

	if ( WaterNumber < 20 ) return;

	for (int i = 0; i < DockNumber; i++)
	{
		DockSpotFinder(MapArray,MaxX, MaxY);
	}
	cout << "Number of Airports: "<< DockNumber << endl;
}

void DockSpotFinder(vector < vector < char> > &MapArray, int MaxX, int MaxY)
{
	int SpotCheck = 0;
	//In the event there is no valid tile to place a dock, give up!
	while (SpotCheck < 10000)
	{
		int x = rand () % MaxX;
		int y = rand () % MaxY;
		if (MapArray[y][x] == 'p' || MapArray[y][x] == 'D' || MapArray[y][x] == 'f')
		{
			if ((x > 0 && MapArray[y][x-1] == 'W')||(x < MaxX - 1 && MapArray[y][x+1] == 'W')||(y > 0 && MapArray[y-1][x] == 'W')||(y < MaxY -1 && MapArray[y+1][x] == 'W'))
			{
				MapArray[y][x] = 'd';
				return;
			}
			else
				SpotCheck++;
		}
	}

}

int  WaterCounter(vector < vector < char> > MapArray, int MaxX, int MaxY)
{
	int counter = 0;
	for (int i = 0; i < MaxY ; i++)
	{
		for (int j = 0; j < MaxX; j++)
		{
			if (MapArray[i][j] = 'W')
				counter++;
		}
	}
	return counter;
}
