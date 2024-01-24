#pragma once
#include <wx/wx.h>
#include "MainFrame.h"
#include <wx/timer.h>



class AnimationTimer: public wxTimer
{
public:
	AnimationTimer(MainFrame* mainFramePtr);
	void Notify();
private:
	MainFrame* mainFramePtr;
};


