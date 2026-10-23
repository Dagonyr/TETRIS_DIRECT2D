#pragma once
#include <string>
#include <vector>
#include "Model.h"
#include <unordered_map>
class Background 
{
public:
	std::string link_to_background_png = "";
};
class BLetter {
public:
	int w = 0, h = 0;
	std::vector<uint8_t> data;                 

	BLetter() = default;
	BLetter(int w, int h, std::vector<uint8_t> d) : w(w), h(h), data(std::move(d)) {}

	uint8_t at(int row, int col) const { return data[row * w + col]; }
};
class Title 
{
public:
	std::string TitleText = "";
	std::unordered_map<char, BLetter> letters;
	int center_pos[2];
    float PercentRectSize[2];
	float Percent_width_of_space;
};
class Button 
{
public:
	Button(int Id,std::string ButtonText, std::string link_to_button_png, int center_pos[2], int RectSize[2], int centering_type = 0);
	int Id;
	std::string ButtonText = "";
	std::string link_to_button_png = "";
	int center_pos[2];
	int RectSize[2];
	int centering_type = 0;
	bool is_choosed = 0;
};
class Menu
{
public:
	Menu();
	void DrawMenu_Background();
	void DrawMenu_GameTitle();
	void DrawMenu_Buttons();

private:
	int count_buttons = 3;
	std::vector<Button*> buttons = {};
	Title MenuTitle;
	Background MenuBackground;
};