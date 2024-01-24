#pragma once
#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>
#include "AnimationTimer.h"
#include <wx/timer.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {

	::wxInitAllImageHandlers();

	MainFrame* mainFrame = new MainFrame("BookCollector");
	mainFrame-> SetClientSize(1920, 1080);
	mainFrame->Center();
	mainFrame->Show();
	//mainFrame->ShowFullScreen(true);
	// 
	//Maximise() before Show() make grey bar appear, temporary fix is placing it after 
	mainFrame->Maximize();
	wxSize size = mainFrame->GetSize(); // windows buttons, name of program and task bar cut off image partially
	
	//create timer to update background
	
	animationTimer = new AnimationTimer(mainFrame);
	animationTimer->Start(100);


	return true;
}

int App::OnExit()
{
	wxDELETE(animationTimer);
	return 0;
}

