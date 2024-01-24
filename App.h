#pragma once
#include <wx/wx.h>
#include "AnimationTimer.h"
class App : public wxApp
{
public:
	bool OnInit();
	int OnExit();
private:
	AnimationTimer* animationTimer;
};

