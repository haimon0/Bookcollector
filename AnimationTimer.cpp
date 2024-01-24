#pragma once
#include "MainFrame.h"
#include "AnimationTimer.h"
#include <wx/timer.h>
#include <wx/wx.h>


AnimationTimer::AnimationTimer(MainFrame* mFPtr): wxTimer() {
	mainFramePtr = mFPtr;
};

void AnimationTimer::Notify() {
	// try to remove if statement later for performance gain
	// issue is that mainframe is deleted before timer
	// so timer still tries to use mainframe methods
	// when mainframe doesn't exist
	// causing read access violation
	if (!(mainFramePtr->IsBeingDeleted())) {
		mainFramePtr->UpdateBackground();
	}
}
