#pragma once
#include <string>
#include "Params.h"
#include "Structs.h"

class Model
{
public:
	Model();
	//Model(const Model& other);
	 void left_Transform();
	 void right_Transform();
	 void left_Move();
	 void Low_Move();
	 void right_Move();
	 void StopFig();
	 void generate();
	 void cr_log_pole();
	 bool Is_Collision();
	 void Get_Start_Pos(point* buf_fig);
	 int type_of_draw = 1; //right
	 float border_hight = 0.05;
	 float border_width = 0.1;
	 int game_state = 0; // 0 - меню, 1 - главный экрна, 2 - игра окончена
	 bool is_lose = false;
	 int h = 32;
	 int w = 16;
	 int** pole = nullptr;
	 int mx_elems = 4;
	 int generate_h = mx_elems;
	 int cur_el = 0;
	 point* cur_figure = nullptr;
private:
	void Buf_Current_Figure();
	void Buf_Current_Figure(point* figure, point* copy);
	bool Is_Pole_Filled();
	bool Is_Lose(int* buf_color);
	void ReturnFigure();
	point* Copy_Fig();
	static void merge_sort(int* A, int start, int end, bool rev = 0);
	static void merge_sort(point* A, int start, int end, bool rev, bool is_y = 0);
	 char lst_but = '0';
	 char error_type = '0';
	 int is_empty = -1;
	 int full_h = h + generate_h;
	 Counter counter_;
	 point* old_figure = nullptr;
};