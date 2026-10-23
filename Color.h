#pragma once
class Color 
{
public:
	static int GetCFC(Color color);
	static Color GetCFI(int color);
private:
	unsigned char color[4];
};