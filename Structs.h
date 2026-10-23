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
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TextFormat, key, fontFamily, fontSize);
struct TextLayout 
{
    std::string category;
    std::string name;
    std::string text;
    std::string width;
    std::string height;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TextLayout, category, name, text, width, height);
struct SolidBrush
{
    int key;
    std::string color;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SolidBrush, key, color)
struct BitmapPath 
{
    std::string objectName;
    std::string fileName;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BitmapPath,objectName,fileName)