#include "Model.h"
#include <random>
#include <string.h>
Model::Model() 
{
	cur_figure = new point[mx_elems];
	old_figure = new point[mx_elems];
	cr_log_pole();
}


void Model::cr_log_pole()
{
	srand(543);
	pole = new int* [w];
	for (int i = 0; i < w; i++) { pole[i] = new int[h + generate_h]; }
	for (int i = 0; i < w; i++)
		for (int j = 0; j < full_h; j++) pole[i][j] = is_empty;
}
void Model::generate()
{
	int x_st;
	cur_el += 1;
	cur_el = cur_el % 7;
	switch (cur_el)
	{
	case 0:
		x_st = rand() % 15;                                    // ##
		cur_figure[0].x = x_st;                                // ##
		cur_figure[0].y = 0;                                   
		cur_figure[1].x = x_st + 1;                            
		cur_figure[1].y = 0;
		cur_figure[2].x = x_st + 1;
		cur_figure[2].y = 1;
		cur_figure[3].x = x_st;
		cur_figure[3].y = 1;
		break;
	case 1:
		x_st = rand() % 13;
		cur_figure[0].x = x_st;
		cur_figure[0].y = 0;
		cur_figure[1].x = x_st + 1;
		cur_figure[1].y = 0;
		cur_figure[2].x = x_st + 1;
		cur_figure[2].y = 1;
		cur_figure[3].x = x_st + 2;
		cur_figure[3].y = 1;
		break;
	case 2:
		x_st = rand() % 16;
		cur_figure[0].x = x_st;
		cur_figure[0].y = 0;
		cur_figure[1].x = x_st;
		cur_figure[1].y = 1;
		cur_figure[2].x = x_st;
		cur_figure[2].y = 2;
		cur_figure[3].x = x_st;
		cur_figure[3].y = 3;
		break;
	case 3:
		x_st = rand() % 15;
		cur_figure[0].x = x_st;
		cur_figure[0].y = 0;
		cur_figure[1].x = x_st;
		cur_figure[1].y = 1;
		cur_figure[2].x = x_st;
		cur_figure[2].y = 2;
		cur_figure[3].x = x_st + 1;
		cur_figure[3].y = 2;
		break;
	case 4:
		x_st = rand() % 15;
		cur_figure[0].x = x_st;
		cur_figure[0].y = 0;
		cur_figure[1].x = x_st;
		cur_figure[1].y = 1;
		cur_figure[2].x = x_st;
		cur_figure[2].y = 2;
		cur_figure[3].x = x_st + 1;
		cur_figure[3].y = 1;
		break;
	case 5:
		x_st = rand() % 14;
		cur_figure[0].x = x_st;
		cur_figure[0].y = 1;
		cur_figure[1].x = x_st + 1;
		cur_figure[1].y = 1;
		cur_figure[2].x = x_st + 1;
		cur_figure[2].y = 0;
		cur_figure[3].x = x_st + 2;
		cur_figure[3].y = 0;
		break;
	case 6:
		x_st = rand() % 15;
		cur_figure[0].x = x_st;
		cur_figure[0].y = 0;
		cur_figure[1].x = x_st;
		cur_figure[1].y = 1;
		cur_figure[2].x = x_st;
		cur_figure[2].y = 2;
		cur_figure[3].x = x_st + 1;
		cur_figure[3].y = 2;
		break;
	}
	int random_rotate;
	random_rotate = rand() % 4;
	for (int i = 0; i < random_rotate; i++)
	{
		left_Transform();
	}
}
void Model::Get_Start_Pos(point* buf_fig) 
{
	int x_st = 0;
	int el = (cur_el + 1) % 7;
	switch (el)
	{
	case 0:
		buf_fig[0].x = x_st;
		buf_fig[0].y = 0;
		buf_fig[1].x = x_st + 1;
		buf_fig[1].y = 0;
		buf_fig[2].x = x_st + 1;
		buf_fig[2].y = 1;
		buf_fig[3].x = x_st;
		buf_fig[3].y = 1;
		break;
	case 1:
		buf_fig[0].x = x_st;
		buf_fig[0].y = 0;
		buf_fig[1].x = x_st + 1;
		buf_fig[1].y = 0;
		buf_fig[2].x = x_st + 1;
		buf_fig[2].y = 1;
		buf_fig[3].x = x_st + 2;
		buf_fig[3].y = 1;
		break;
	case 2:
		buf_fig[0].x = x_st;
		buf_fig[0].y = 0;
		buf_fig[1].x = x_st;
		buf_fig[1].y = 1;
		buf_fig[2].x = x_st;
		buf_fig[2].y = 2;
		buf_fig[3].x = x_st;
		buf_fig[3].y = 3;
		break;
	case 3:
		buf_fig[0].x = x_st;
		buf_fig[0].y = 0;
		buf_fig[1].x = x_st;
		buf_fig[1].y = 1;
		buf_fig[2].x = x_st;
		buf_fig[2].y = 2;
		buf_fig[3].x = x_st + 1;
		buf_fig[3].y = 2;
		break;
	case 4:
		buf_fig[0].x = x_st;
		buf_fig[0].y = 0;
		buf_fig[1].x = x_st;
		buf_fig[1].y = 1;
		buf_fig[2].x = x_st;
		buf_fig[2].y = 2;
		buf_fig[3].x = x_st + 1;
		buf_fig[3].y = 1;
		break;
	case 5:
		buf_fig[0].x = x_st;
		buf_fig[0].y = 1;
		buf_fig[1].x = x_st + 1;
		buf_fig[1].y = 1;
		buf_fig[2].x = x_st + 1;
		buf_fig[2].y = 0;
		buf_fig[3].x = x_st + 2;
		buf_fig[3].y = 0;
		break;
	case 6:
		buf_fig[0].x = x_st;
		buf_fig[0].y = 0;
		buf_fig[1].x = x_st;
		buf_fig[1].y = 1;
		buf_fig[2].x = x_st;
		buf_fig[2].y = 2;
		buf_fig[3].x = x_st + 1;
		buf_fig[3].y = 2;
		break;
	}
}
bool Model::Is_Collision()
{

	point* copy = Copy_Fig();
	merge_sort(copy, 0, 3, 0);
	int max_x = copy[0].x;
	int min_x = copy[3].x;
	merge_sort(copy, 0, 3, 0, 1);
	int max_y = copy[0].y;
	int min_y = copy[3].y;
	if ((min_x < 0) || (max_x > w - 1)) { error_type = 'o'; return true; }
	if ((min_y < 0) || (max_y > full_h - 1)) { error_type = 'o'; return true; }
	for (int i = 0; i < 4; i++) if (pole[cur_figure[i].x][cur_figure[i].y] != is_empty) { error_type = 's'; return true; }
	delete[] copy;
	return false;
}
point* Model::Copy_Fig()
{
	point* copy_fig = new point[4];
	for (int i = 0; i < 4; i++) copy_fig[i] = cur_figure[i];
	return copy_fig;
}

void Model::Buf_Current_Figure() 
{
	for (int i = 0; i < mx_elems; i++) old_figure[i] = cur_figure[i];
}
void Model::Buf_Current_Figure(point* figure, point* copy) 
{
	for (int i = 0; i < mx_elems; i++) copy[i] = figure[i];
}
void Model::StopFig()
{
	//int* superbuf_color = new int[mx_elems];
	for (int i = 0; i < mx_elems; i++)
	{
		//superbuf_color[i] = pole[cur_figure[i].x][cur_figure[i].y];
		pole[cur_figure[i].x][cur_figure[i].y] = cur_el;
	}
	Is_Pole_Filled();
	//is_lose = Is_Lose(superbuf_color);
	generate();
}
bool Model::Is_Pole_Filled() 
{
	int k = 0;
	int i = full_h-1;
	while (i != generate_h-1)
	{
		k = 0;
		for (int j = 0; j < w; j++)
		{
			if (pole[j][i] != is_empty) k++;
		}
		if (k == w)
		{
			for (int j = 0; j < w; j++) pole[j][i] == is_empty;
			for (int p = i; p > 0; p--)
				for (int j = 0; j < w; j++) pole[j][p] = pole[j][p-1];
			counter_.old_counter = counter_.counter;
			counter_.counter += 20;
		}
		else i--;
	}
	return false;
}
bool Model::Is_Lose(int* buf_color) 
{
	int k = 0;
	if (k == mx_elems)
	{
		return true;
	}
	else return false;
}
void Model::ReturnFigure()
{
	if (error_type == 'o') {
		int delta;
		merge_sort(cur_figure, 0, 3, 1, 1);
		if (cur_figure[0].y < 0)
		{
			delta = -cur_figure[0].y;
			for (int i = 0; i < 4; i++) cur_figure[i].y += delta;
		}
		merge_sort(cur_figure, 0, 3, 0, 0);
		if (cur_figure[0].x > w - 1)
		{
			// max_x = cur_figure[0].x
			delta = w - 1 - cur_figure[0].x;
			for (int i = 0; i < 4; i++) cur_figure[i].x += delta;
		}
		merge_sort(cur_figure, 0, 3, 1, 0);
		if (cur_figure[0].x < 0)
		{
			// min_x = cur_figure[0].x;
			delta = -cur_figure[0].x;
			for (int i = 0; i < 4; i++) cur_figure[i].x += delta;
		}
		merge_sort(cur_figure, 0, 3, 0, 1);
		if (cur_figure[0].y > full_h - 1)
		{
			delta = full_h - 1 - cur_figure[0].y;
			for (int i = 0; i < 4; i++) cur_figure[i].y += delta;
		}
		if (cur_figure[0].y == full_h - 1) { StopFig(); return; } 	// Столкновение с полом
	}
	if (error_type == 's') {
		Buf_Current_Figure(old_figure, cur_figure);
		StopFig();
		return;
	}
}
void Model::merge_sort(point* A, int start, int end, bool rev, bool is_y)
{
	if (end == start) { return; }
	int p = (end + start) / 2;
	merge_sort(A, start, p, rev, is_y);
	merge_sort(A, p + 1, end, rev, is_y);
	point* buf = new point[end - start + 1];
	int i = 0;
	int j = 0;
	if (!rev) {
		if (is_y) {
			while (j != end - p && i != p - start + 1)
			{
				if (A[start + i].y >= A[p + 1 + j].y)
				{
					buf[i + j] = A[start + i];
					i++;
				}
				else
				{
					buf[i + j] = A[p + 1 + j];
					j++;
				}
			}
		}
		else
		{
			while (j != end - p && i != p - start + 1)
			{
				if (A[start + i].x >= A[p + 1 + j].x)
				{
					buf[i + j] = A[start + i];
					i++;
				}
				else
				{
					buf[i + j] = A[p + 1 + j];
					j++;
				}
			}
		}
	}
	else
	{
		if (is_y) {
			while (j != end - p && i != p - start + 1)
			{
				if (A[start + i].y <= A[p + 1 + j].y)
				{
					buf[i + j] = A[start + i];
					i++;
				}
				else
				{
					buf[i + j] = A[p + 1 + j];
					j++;
				}
			}
		}
		else
		{
			while (j != end - p && i != p - start + 1)
			{
				if (A[start + i].x <= A[p + 1 + j].x)
				{
					buf[i + j] = A[start + i];
					i++;
				}
				else
				{
					buf[i + j] = A[p + 1 + j];
					j++;
				}
			}
		}
	}
	if (j == end - p)
	{
		while (i != p - start + 1)
		{
			buf[i + j] = A[start + i];
			i++;
		}
	}
	else
	{
		while (j != end - p)
		{
			buf[i + j] = A[p + 1 + j];
			j++;
		}
	}
	for (int i = start; i <= end; i++) { A[i] = buf[i - start]; }
	delete[] buf;
}
void Model::merge_sort(int* A, int start, int end, bool rev)
{
	if (end == start) { return; }
	int p = (end + start) / 2;
	merge_sort(A, start, p, rev);
	merge_sort(A, p + 1, end, rev);
	int* buf = new int[end - start + 1];
	int i = 0;
	int j = 0;
	if (!rev) {
		while (j != end - p && i != p - start + 1)
		{
			if (A[start + i] >= A[p + 1 + j])
			{
				buf[i + j] = A[start + i];
				i++;
			}
			else
			{
				buf[i + j] = A[p + 1 + j];
				j++;
			}
		}
	}
	else
	{
		while (j != end - p && i != p - start + 1)
		{
			if (A[start + i] <= A[p + 1 + j])
			{
				buf[i + j] = A[start + i];
				i++;
			}
			else
			{
				buf[i + j] = A[p + 1 + j];
				j++;
			}
		}
	}
	if (j == end - p)
	{
		while (i != p - start + 1)
		{
			buf[i + j] = A[start + i];
			i++;
		}
	}
	else
	{
		while (j != end - p)
		{
			buf[i + j] = A[p + 1 + j];
			j++;
		}
	}
	for (int i = start; i <= end; i++) { A[i] = buf[i - start]; }
	delete[] buf;
}

//ДВИЖЕНИЯ
//
void Model::left_Transform()
{
	Buf_Current_Figure();
	int buf;
	merge_sort(cur_figure, 0, 3, 1, 1);
	int min_y_ = cur_figure[0].y;
	merge_sort(cur_figure, 0, 3, 1, 0);
	int min_x_ = cur_figure[0].x;
	for (int i = 0; i < 4; i++)
	{
		buf = cur_figure[i].x;
		cur_figure[i].x = cur_figure[i].y - min_y_;
		cur_figure[i].y = min_x_ - buf;
	}
	merge_sort(cur_figure, 0, 3, 1, 1);
	int min_y = cur_figure[0].y;
	merge_sort(cur_figure, 0, 3, 1, 0);
	int min_x = cur_figure[0].x;
	for (int i = 0; i < 4; i++)
	{
		cur_figure[i].x -= min_x - min_x_;
		cur_figure[i].y -= min_y - min_y_;
	}

	if (Is_Collision()) { ReturnFigure(); }
}
void Model::right_Transform()
{
	Buf_Current_Figure();
	int buf;
	merge_sort(cur_figure, 0, 3, 1, 1);
	int min_y_ = cur_figure[0].y;
	merge_sort(cur_figure, 0, 3, 1, 0);
	int min_x_ = cur_figure[0].x;
	for (int i = 0; i < 4; i++)
	{
		buf = cur_figure[i].x;
		cur_figure[i].x = -cur_figure[i].y + min_y_;
		cur_figure[i].y = -min_x_ + buf;
	}
	merge_sort(cur_figure, 0, 3, 1, 1);
	int min_y = cur_figure[0].y;
	merge_sort(cur_figure, 0, 3, 1, 0);
	int min_x = cur_figure[0].x;
	for (int i = 0; i < 4; i++)
	{
		cur_figure[i].x -= min_x - min_x_;
		cur_figure[i].y -= min_y - min_y_;
	}

	if (Is_Collision()) { ReturnFigure(); }
}
void Model::Low_Move()
{
	Buf_Current_Figure();
	for (int i = 0; i < 4; i++) cur_figure[i].y++;
	if (Is_Collision()) { ReturnFigure(); }

}
void Model::right_Move()
{
	Buf_Current_Figure();
	for (int i = 0; i < 4; i++) cur_figure[i].x++;
	if (Is_Collision()) { ReturnFigure(); }
}
void Model::left_Move()
{
	Buf_Current_Figure();
	for (int i = 0; i < 4; i++) cur_figure[i].x--;
	if (Is_Collision()) { ReturnFigure(); }
}