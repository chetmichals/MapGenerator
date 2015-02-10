#include "ConvertToBMP.h"

//Takes in a Maparray object, which is a vector of vectors of chars, each char corasponds to a single colored pixel. Then, make the image 800%x800%.

void GetColors (uint16_t &R, uint16_t &G, uint16_t &B, char x);

struct BITMAPFILEHEADER{
	uint16_t magic;
	uint32_t filesz;
	uint32_t junk;
	uint32_t bmp_offset;
};

struct BITMAPINFOHEADER{
  uint32_t header_sz;
  int32_t width;
  int32_t height;
  uint16_t nplanes;
  uint16_t bitspp;
  uint32_t compress_type;
  uint32_t bmp_bytesz;
  int32_t hres;
  int32_t vres;
  uint32_t ncolors;
  uint32_t nimpcolors;
} ;


void ConvertToBMP(vector < vector < char> > &MapArray, int Height, int Width, int Seed)
{
	stringstream FileNumber;
	FileNumber << Seed;
	string FileName = "image";
	FileName += FileNumber.str();
	FileName += ".bmp";
	ofstream Image (FileName,  ios::out | ios::binary);
	
	//bmfh.magic = 0x424d;
	//bmfh.creator1 = 0000;
	//bmfh.creator2 = 0000;
	//bmfh.bmp_offset = 0x7A000000;

	uint32_t ByteSize = Width * 3;
	ByteSize = ByteSize + (ByteSize%4);
	ByteSize = ByteSize * Height * 64;
	ByteSize = ByteSize + 54;

	BITMAPFILEHEADER bmfh = {19778, ByteSize, 0, 54};
	BITMAPINFOHEADER bmih = {40,Width*8,Height*8,1,24,0,0,0,0,0,0};

	/*bmih.header_sz = 40;
	bmih.width = Width;
	bmih.height = Height;
	bmih.nplanes = 1;
	bmih.bitspp = 24;
	bmih.compress_type = 0;
	bmih.hres = 2835;
	bmih.vres = 2835;
	bmih.ncolors = 0;
	bmih.nimpcolors = 0;*/

	vector <unsigned char> ImageData;
	unsigned char R,G,B;


	for (int i = 0; i < Height; i++)
	{
		for (int a = 0; a < 8; a++)
		{
			int j;
			for (j = 0; j < Width; j++)
			{
				for (int b = 0; b < 8; b++)
				{
					GetColors(R,G,B,MapArray[i][j]);
					ImageData.push_back(B);
					ImageData.push_back(G);
					ImageData.push_back(R);
				}
			}
		}
		//int k = (j*3)%4;
		//while (k != 0) //Pad the lines
		//{
		//	ImageData.push_back(0x00);
		//	k++;
		//	k = k%4;
		//}
	}
	Image.write(reinterpret_cast<char*>(&bmfh.magic), sizeof(bmfh.magic));
	Image.write(reinterpret_cast<char*>(&bmfh.filesz), sizeof(bmfh.filesz));
	Image.write(reinterpret_cast<char*>(&bmfh.junk), sizeof(bmfh.junk));
	Image.write(reinterpret_cast<char*>(&bmfh.bmp_offset), sizeof(bmfh.bmp_offset));
	Image.write(reinterpret_cast<char*>(&bmih), sizeof(bmih));
	for (unsigned int i = 0; i < ImageData.size(); i++)
	{
		Image << ImageData[i];
	}

	Image.close();
}


void GetColors (unsigned char &R, unsigned char &G, unsigned char &B, char x)
{
	if (x == 'p')
	{
		R = 0x8C;
		G = 0xE6;
		B = 0x62;
	}
	else if (x == 'f')
	{
		R = 0x1D;
		G = 0x60;
		B = 0x9;
	}
	else if (x == 'D')
	{
		R = 0xF0;
		G = 0xE0;
		B = 0x7D;
	}
	else if (x == 'W')
	{
		R = 0x18;
		G = 0x0c;
		B = 0xFF;
	}
	else if (x == 'M')
	{
		R = 0x76;
		G = 0x5f;
		B = 0x50;
	}
	else if (x == 'C')
	{
		R = 209;
		G = 209;
		B = 209;
	}
	else if (x == 'R')
	{
		R = 127;
		G = 127;
		B = 127;
	}
	else if (x == 'r')
	{
		R = 255;
		G = 41;
		B = 4;
	}
	else if (x == 'b')
	{
		R = 153;
		G = 217;
		B = 234;
	}
	else if (x == 'v')
	{
		R = 72;
		G = 188;
		B = 217;
	}
	else if (x == 'F')
	{
		R = 255;
		G = 119;
		B = 86;
	}
	else if (x == 'V')
	{
		R = 11;
		G = 49;
		B = 14;
	}
	else if (x == 'g')
	{
		R = 27;
		G = 109;
		B = 131;
	}
	else if (x == 'G')
	{
		R = 152;
		G = 35;
		B = 5;
	}
	else if (x == 'B')
	{
		R = 190;
		G = 176;
		B = 131;
	}
	else if (x == 'A')
	{
		R = 236;
		G = 159;
		B = 255;
	}
	else if (x == 'd')
	{
		R = 244;
		G = 255;
		B = 036;
	}
	else //if (x=='H')
	{
		R = 173;
		G = 151;
		B = 105;
	}
}