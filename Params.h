#pragma once
#include <chrono>

namespace GameParams 
{
	 constexpr float delta_Y = 0.1;
	 constexpr float alpha_pole = 0.75;
	 constexpr float rect_nxt_fig = 0.2;
	 constexpr float delta_X_figure_and_rotate = (1 - alpha_pole - rect_nxt_fig) / 2;
	 constexpr float height_count = 0.2;
	 constexpr float pos_of_count[2] = { alpha_pole + (float)0.01, 0.01 };
	 constexpr float height_of_text_nxt_fig = 0.2;
	 constexpr float pos_of_text_nxt_fig[2] = { alpha_pole + (float)0.01, pos_of_count[1] + height_count + (float)0.01};
	 constexpr float height_of_nxt_fig = 0.2;
	 constexpr float pos_of_nxt_fig[2] = { alpha_pole + (float)0.01, pos_of_text_nxt_fig[1] + height_of_text_nxt_fig + (float)0.01};
	 const float speed_of_fall = 1.0f / 2.0f;
	 const float input_lag = 1.0f / 10.0f;
	 const float DAS_DELAY = 0.050;  // 150 мс до автоповтора
	 const float DAS_RATE = 0.266;  // 30 мс между повторами
	 const auto targetFrameTime = std::chrono::microseconds(16667);
};