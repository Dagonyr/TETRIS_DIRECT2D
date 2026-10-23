#include "Menu.h"
Button::Button(int Id,std::string ButtonText, std::string link_to_button_png, int center_pos[2], int RectSize[2], int centering_type)
{
	this->ButtonText = ButtonText;
	this->link_to_button_png = link_to_button_png;
	for (int i = 0; i < 2; i++) 
	{
		this->center_pos[i] = center_pos[i];
		this->RectSize[i] = RectSize[i];
	}
	this->centering_type = centering_type;
	this->Id = Id;
}