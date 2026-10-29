#pragma once
#include "TextFormatCache.h"
#include "TextLayoutCache.h"
#include "BrushCache.h"
#include "BitmapCache.h"
class View;
class TotalCache
{
public:
	TotalCache(View* pview, const wchar_t* link)
		: link_to_assets(link),
		TextFormatCache_(pview),
		TextLayoutCache_(pview),
		BrushCache_(pview),
		BitmapCache_(pview)
	{
		ParseAssets();
	}
	std:: wstring link_to_assets;
	TextFormatCache TextFormatCache_;
	TextLayoutCache TextLayoutCache_;
	BrushCache BrushCache_;
	BitmapCache BitmapCache_;
	void ParseAssets();
};