#pragma once
#include <string>
#include <d2d1.h>
#include <dwrite.h>
#include <nlohmann/json.hpp>
struct point
{
	int x;
	int y;
};
struct floatRect
{
    float prc_height;
    float prc_width;
    float x_start;
    float y_start;
};

struct Counter {
    int counter = 0;
    int old_counter = -1;
};

struct GameLoopState
{
    using Clock = std::chrono::steady_clock;

    Clock::time_point prevTime = Clock::now();
    Clock::time_point frameStart = Clock::now();
    float delayTimer = 0.0f;
    float frameTime = 0.0f;
    float accumulator = 0.0f;
    float dasTimer = 0.0f;

};
struct TextFormat 
{
    std::string key;
    std::string fontFamily;
    float fontSize;
};

struct TextLayout 
{
    std::string category;
    std::string name;
    std::string text;
    std::string width;
    std::string height;
};

struct SolidBrush
{
    int key;
    std::string color;
};

struct BitmapPath 
{
    std::string objectName;
    std::string fileName;
};

struct buttonPadding
{
    float left;
    float top;
    float right;
    float bottom;
};
struct strRect
{
    std::string prc_height;
    std::string prc_width;
    std::string x_start;
    std::string y_start;
};

struct SButton 
{
    std::string button_name;
    std::string texture_name;
    buttonPadding buttonpadding;
    strRect button_rect;

};
struct FButton 
{
    std::string button_name;
    std::string texture_name;
    buttonPadding buttonpadding;
    floatRect button_rect;

};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(strRect, prc_height, prc_width, x_start, y_start)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TextFormat, key, fontFamily, fontSize)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TextLayout, category, name, text, width, height)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SolidBrush, key, color)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BitmapPath, objectName, fileName)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(buttonPadding, left, top, right, bottom)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SButton, button_name, texture_name, buttonpadding)
