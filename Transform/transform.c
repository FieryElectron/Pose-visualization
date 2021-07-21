#include <math.h>
#include "../OLED/oled.h"
#define uchar unsigned char

float right_shift = 0;
float up_shift = 0;
float zoom_rate = 1;

float pivot_X = 0.0;
float pivot_Y = 0.0;
float pivot_Z = 0.0;

typedef struct Point3df
{
	float x;
	float y;
	float z;
} point3df;

typedef struct Point3p2df
{
	float x1;
	float y1;
	float x2;
	float y2;
} point3p2df;

int Line3dArray[14][6] = {
	{-25, 0, 0, 25, 0, 0},
	{0, -20, 0, 0, 20, 0},
	{20, -5, 0, 20, 5, 0},
	{25, 0, 0, 20, -5, 0},
	{25, 0, 0, 20, 5, 0},
	{20, 0, 0, 20, 0, 5},
	{25, 0, 0, 20, 0, 5},
	{0, -20, 0, 17, 0, 0},
	{0, 20, 0, 17, 0, 0},
	{-25, -10, 0, -25, 10, 0},
	{-25, -10, 0, -15, 0, 0},
	{-25, 10, 0, -15, 0, 0},
	{-25, 0, 0, -25, 0, 10},
	{-15, 0, 0, -25, 0, 10}};

point3df trans_Transf3d(int ix, int iy, int iz)
{
	float x = ix;
	float y = iy;
	float z = iz;

	float OriArr[4] = {0};

	OriArr[0] = x;
	OriArr[1] = y;
	OriArr[2] = z;
	OriArr[3] = 1;

	float X = -pivot_X / 180 * 3.14159265359;
	float Y = pivot_Y / 180 * 3.14159265359;
	float Z = pivot_Z / 180 * 3.14159265359;

	float TransArrX[4][4] = {
		{1, 0, 0, 0},
		{0, cos(X), -sin(X), 0},
		{0, sin(X), cos(X), 0},
		{0, 0, 0, 1}};
	float TransArrY[4][4] = {
		{cos(Y), 0, sin(Y), 0},
		{0, 1, 0, 0},
		{-sin(Y), 0, cos(Y), 0},
		{0, 0, 0, 1}};
	float TransArrZ[4][4] = {
		{cos(Z), -sin(Z), 0, 0},
		{sin(Z), cos(Z), 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}};

	OriArr[0] = x * TransArrX[0][0] + y * TransArrX[0][1] + z * TransArrX[0][2] + TransArrX[0][3];
	OriArr[1] = x * TransArrX[1][0] + y * TransArrX[1][1] + z * TransArrX[1][2] + TransArrX[1][3];
	OriArr[2] = x * TransArrX[2][0] + y * TransArrX[2][1] + z * TransArrX[2][2] + TransArrX[2][3];

	x = OriArr[0];
	y = OriArr[1];
	z = OriArr[2];

	OriArr[0] = x * TransArrY[0][0] + y * TransArrY[0][1] + z * TransArrY[0][2] + TransArrY[0][3];
	OriArr[1] = x * TransArrY[1][0] + y * TransArrY[1][1] + z * TransArrY[1][2] + TransArrY[1][3];
	OriArr[2] = x * TransArrY[2][0] + y * TransArrY[2][1] + z * TransArrY[2][2] + TransArrY[2][3];

	x = OriArr[0];
	y = OriArr[1];
	z = OriArr[2];

	OriArr[0] = x * TransArrZ[0][0] + y * TransArrZ[0][1] + z * TransArrZ[0][2] + TransArrZ[0][3];
	OriArr[1] = x * TransArrZ[1][0] + y * TransArrZ[1][1] + z * TransArrZ[1][2] + TransArrZ[1][3];
	OriArr[2] = x * TransArrZ[2][0] + y * TransArrZ[2][1] + z * TransArrZ[2][2] + TransArrZ[2][3];

	point3df p = {OriArr[0], OriArr[1], OriArr[2]};
	return p;
}

point3p2df trans_ConvertReal2D(float x1, float y1, float x2, float y2)
{
	float nx1 = 31 - x1;
	float ny1 = y1 + 31;
	float nx2 = 31 - x2;
	float ny2 = y2 + 31;
	point3p2df p = {nx1, ny1, nx2, ny2};
	return p;
}

point3p2df trans_ConvertPoint3D_2D(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float nx1 = (z1 + up_shift) * zoom_rate;
	float ny1 = (y1 + right_shift) * zoom_rate;
	float nx2 = (z2 + up_shift) * zoom_rate;
	float ny2 = (y2 + right_shift) * zoom_rate;
	point3p2df p = {nx1, ny1, nx2, ny2};
	return p;
}

void trans_DrawLine(float x1, float y1, float x2, float y2)
{
	point3p2df L2p = trans_ConvertReal2D(x1, y1, x2, y2);
	oled_DrawLine((uchar)L2p.x1, (uchar)L2p.y1, (uchar)L2p.x2, (uchar)L2p.y2);
}

void trans_Update3DLines(float x, float y, float z)
{
	pivot_X = x;
	pivot_Y = y;
	pivot_Z = z;

	for (int i = 0; i < 14; ++i)
	{
		int L3dx1 = Line3dArray[i][0];
		int L3dy1 = Line3dArray[i][1];
		int L3dz1 = Line3dArray[i][2];
		int L3dx2 = Line3dArray[i][3];
		int L3dy2 = Line3dArray[i][4];
		int L3dz2 = Line3dArray[i][5];

		point3df p1 = trans_Transf3d(L3dx1, L3dy1, L3dz1);
		point3df p2 = trans_Transf3d(L3dx2, L3dy2, L3dz2);

		point3p2df g2p = trans_ConvertPoint3D_2D(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
		trans_DrawLine(g2p.x1, g2p.y1, g2p.x2, g2p.y2);
	}
}