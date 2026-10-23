#pragma once
#include <windows.h>
#include <iostream>
#include <stack>
#include <string>
#include <d2d1.h>
using namespace std;
int get_op(const string& func)
{
	if (func[0] == '(' || func[0] == ')') return 0;
	if (func[0] == '+' || func[0] == '-') return 2;
	if (func[0] == '/' || func[0] == '*') return 3;
	if (func[0] == '^') return 5;
	return 1;
}
bool isbinop(char c)
{
	return c == '*' || c == '+' || c == '-' || c == '^' || c == '/';
}
bool isunop(string& input_str, int i)
{
	char c = input_str[i];
	if (c == '-')
	{
		if (i == 0) return true;
		char u = input_str[i - 1];
		if (u == '(' || u == ')') return true;
		if (isbinop(u)) return true;
	}
	return false;
}
bool isprefixop(const string& func)
{
	return func == "exp" || func == "sh" || func == "ch" || func == "cos" || func == "sin" || func == "-u";
}
string parcer(string input_str)
{
	std::stack<string> list;
	int i = 0;
	string out;
	string current_operation;
	int n = input_str.length();
	while (i < n)
	{
		if (input_str[i] == 32) {
			i++;
			continue;
		}
		if (input_str[i] == '.' || input_str[i] == ',' || isdigit(input_str[i]))
		{
			while (input_str[i] == '.' || input_str[i] == ',' || isdigit(input_str[i]))
			{
				out += input_str[i];
				i++;
			}
			out += ' ';
			continue;
		} // ввод чисел
		if (isalpha(input_str[i]))
		{
			current_operation = "";
			while (i < n && (isalpha(input_str[i]) || isdigit(input_str[i])))
			{
				current_operation += input_str[i];
				i++;
			}
			if (input_str[i] == '(') list.push(current_operation);
			else
			{
				out += current_operation;
				out += ' ';
			}
		}
		if (isunop(input_str, i))
		{
			if (input_str[i] == '-') current_operation = "-u";
			list.push(current_operation);
			i++;
			continue;
		}
		if (isbinop(input_str[i]))
		{
			current_operation = input_str[i];
			while (!list.empty() && (isprefixop(list.top()) || get_op(list.top()) >= get_op(current_operation)))
			{
				out += list.top();
				out += ' ';
				list.pop();
			}
			list.push(current_operation);
			i++;
			continue;
		}
		if (input_str[i] == '(')
		{
			current_operation = '(';
			list.push(current_operation);
		}
		if (input_str[i] == ')')
		{
			while (!list.empty() && list.top()[0] != '(')
			{
				out += list.top();
				out += ' ';
				list.pop();
			}
			list.pop();
		}
		i++;
	}

	while (!list.empty())
	{
		out += list.top();
		out += ' ';
		list.pop();
	}
	return out;
}
wstring Utf8ToWide(const std::string& s)
{
    if (s.empty()) return {};
    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
    std::wstring w(n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), w.data(), n);
    return w;
}
D2D1::ColorF FromHexToColorF(const std::string& hex)
{
	if (hex.size() != 7 || hex[0] != '#') return D2D1::ColorF(0, 0, 0);
	unsigned long rgb = std::stoul(hex.substr(1), nullptr, 16);
	return D2D1::ColorF(((rgb >> 16) & 0xFF) / 255.0f,
		((rgb >> 8) & 0xFF) / 255.0f,
		(rgb & 0xFF) / 255.0f);
}