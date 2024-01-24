#pragma once
#include "MainFrame.h"
#include "Library.h"
#include <wx/wx.h>
#include <wx/simplebook.h>
#include <string>
#include <list>
#include <iterator>
#include <wx/timer.h>
#include <fstream>
#include <ctime>

using std::string;
using std::to_string;
using std::list;

bool is_digits(const string& str) {
	if (str.size() >= 10) { // checks if number exceed max
		return false;
	}
	if (!(str.find_first_not_of(' ') != string::npos)) // checks if there are spaces
	{
		return false;
	}
	for (char c : str) { // checks if string is only numbers
		if (!isdigit(c)) {
			return false;
		}
	}
	return true;
}

struct AddingBooks {
	int addName = 1;  // bookName
	int addPage = 0;  // pageCount
	int addTotalPages = 0; // totalPages
}state;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	CreateMainMenu();
	CreateRemoveBookPage();
	BindEventHandlers();
	
	library = Library();
	library.ReadFromExternal();
	int bookListSize = library.getBookList().size();
	for (int i = 0; i < bookListSize; i++) {
		AddBookToShelf();
	}

	simpleBook = new wxSimplebook();
	simpleBook->Create(this, wxID_ANY, wxPoint(0, 0), wxSize(1920, 1080));
	simpleBook->ShowNewPage(mainMenu);

}

void MainFrame::CreateMainMenu() {
	wxFont headlineFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	mainMenu = new wxPanel(this);
	mainMenu->SetFont(mainFont);

	inputField = new wxTextCtrl(mainMenu, wxID_ANY, "", wxPoint(478, 95), wxSize(555, 75), wxTE_PROCESS_ENTER);
	inputField->SetFont(wxFontInfo(wxSize(0, 50)));
	inputField->Hide();
	wxSize sz = mainMenu->GetSize();

	//start of image code

	//::wxInitAllImageHandlers();
	//bitMap = wxBitmap(wxImage("data/animation/0.png", wxBITMAP_TYPE_PNG));
	baseImageSrc = "data/animation/";
	currentFrame = 0;
	maxFrame = 15;

	baseBookSrc = "data/bookSprites/b";
	maxBooks = 61;
	booksOnShelf = 0;

	xPos = 0;
	yPos = 0;

	xPosBook = 510;
	yPosBook = 225;

	changeY = 1;
	
	scale = 1;

	row = 0;

	booksYPosition = {0, 11, 23, 35, 47, 59, 71, 83, 95, 107, 119, 131, 143, 155, 167, 179, 191, 203, 215, 227, 239, 251, 263, 275, 287, 299, 311, 323, 335, 347, 359, 369, 381, 393, 405, 417, 429, 441, 453, 466, 478, 488, 500, 512, 523, 535, 547};

	bookBitmap = wxNullBitmap;
	bookBitmapRight = wxNullBitmap;

	maxBooksOnShelf = 2867;

	currentZoom = 0; //out = 0, left = 1, right = 2	

	viewingBooks = false;
	
	for (int frame = 0; frame <= maxFrame; frame++) {

		string imageSrc = baseImageSrc + std::to_string(frame) + ".png";
		wxBitmap bitMap(wxImage(imageSrc, wxBITMAP_TYPE_PNG));

		bitMapList.push_back(bitMap);
		
	}

	for (int bookCount = 1; bookCount <= maxBooks; bookCount++) {
		
		string imageSrcBook = baseBookSrc + std::to_string(bookCount) + ".png";
		wxBitmap bitMap(wxImage(imageSrcBook, wxBITMAP_TYPE_PNG));

		if (bookCount == maxBooks) {
			maxBookBitmap = bitMap;
		}

		bitMapListBook.push_back(bitMap);
		
	}
	
	//bitMapList.push_back(wxBitmap(wxImage(imageSrc, wxBITMAP_TYPE_PNG).Scale(sz.GetWidth(), sz.GetHeight(),wxIMAGE_QUALITY_NORMAL)));
	
	// *** BELOW CODE WORKS!!!***
	//bitMapList.push_back(wxBitmap(wxImage("data/animation/0.png", wxBITMAP_TYPE_PNG)));


	

	//end of image code
}


void MainFrame::UpdateBackground() {

	if (currentFrame > maxFrame) {
		currentFrame = 0;
	}

	if (currentFrame == 0) {
		if (scale == 1) {
			xPosBook = 510;
			xPosBookRight = 1170;
			yPosBook = 225;
			changeY = 1;
		}
		else if (scale == 3) {
			xPosBook = 160;
			xPosBookRight = 370;
			// ^^
			yPosBook = 75;
			changeY = 1;
		}
		
	}

	

	//wxSize sz = mainMenu->GetSize();

	auto bitMapGetter = bitMapList.begin();

	std::advance(bitMapGetter, currentFrame);

	wxClientDC dc(mainMenu);

	if (zoomInLeft) {
		scale = 3;
		inputField->SetPosition(wxPoint(475, 95));
		inputField->Show();
		xPos = -350;
		yPos = -150;
		
		xPosBook = 160;
		yPosBook = 75;

		zoomInLeft = false;
		currentZoom = 1;
	}
	else if (zoomInRight) {
		scale = 3;
		inputField->SetPosition(wxPoint(1108, 95));
		inputField->Show();
		xPos = -800;
		yPos = -150;

		xPosBookRight = 370;
		yPosBook = 75;

		zoomInRight = false;
		currentZoom = 2;
	}
	else if (zoomOut) {
		scale = 1;
		inputField->Hide();
		xPos = 0;
		yPos = 0;

		xPosBook = 510;
		yPosBook = 225;

		xPosBookRight = 1170;
		zoomOut = false;
		currentZoom = 0;
	}
	dc.SetUserScale(scale, scale);
	
	dc.DrawBitmap(*bitMapGetter, xPos, yPos);
	
	if (booksPerRow.size() != 0) {
		
		for (int i = 0; i < booksPerRow.size(); i++) {
			auto yPosGetter = booksYPosition.begin();

			std::advance(yPosGetter, i);

			if (currentZoom != 2) {

				auto bookPerRowGetter = booksPerRow.begin();

				std::advance(bookPerRowGetter, i);

				if (*bookPerRowGetter == maxBooks) {
					bookBitmap = maxBookBitmap;
				}
				else {
					auto bookGetter = bitMapListBook.begin();

					std::advance(bookGetter, *bookPerRowGetter - 1);

					bookBitmap = *bookGetter;
				}

				dc.DrawBitmap(bookBitmap, xPosBook, yPosBook + *yPosGetter);
			}

			//******************************************************************

			if (booksPerRowRight.size() != 0 && currentZoom != 1 && i < booksPerRowRight.size()) {
				auto bookPerRowRightGetter = booksPerRowRight.begin();

				std::advance(bookPerRowRightGetter, i);

				if (*bookPerRowRightGetter == maxBooks) {
					bookBitmapRight = maxBookBitmap;
				}
				else {
					auto bookRightGetter = bitMapListBook.begin();

					std::advance(bookRightGetter, *bookPerRowRightGetter - 1);

					bookBitmapRight = *bookRightGetter;
				}

				dc.DrawBitmap(bookBitmapRight, xPosBookRight, yPosBook + *yPosGetter);
			}
			
		}
	}
	
	

	
	
	yPosBook += changeY;
	if (scale == 1 && (yPosBook <= 225 || yPosBook >= 233)) {
		changeY *= -1;
	}
	else if (scale == 3 && (yPosBook < 75 || yPosBook >= 83)) {
		changeY *= -1;
	}
	

	currentFrame++;
}

void MainFrame::AddBookToShelf() {
	booksOnShelf += 1;
	booksOnShelfCopy = booksOnShelf;

	// maxRow is used to know how many rows are need to fit x amount of books
	// the loop then iterates that many times, filling each row with books

	int maxRow = 1 + ((booksOnShelf - 1) / maxBooks);
	int maxRowRight = maxRow - 47;
	


	int booksOnLeftShelf = 0;
	int booksOnRightShelf = 0;
	booksPerRow.clear();
	booksPerRowRight.clear();

	for (int i = 1; i <= maxRow && booksOnLeftShelf < maxBooksOnShelf; i++) {
		if (booksOnShelfCopy - 61 >= 0) {
			booksOnShelfCopy -= 61;
			booksOnLeftShelf += 61;

			booksPerRow.push_back(61);
		}
		else {
			booksPerRow.push_back(booksOnShelfCopy);
		}
	}

	for (int i = 1; i <= maxRowRight && booksOnRightShelf < maxBooksOnShelf; i++) {
		if (booksOnShelfCopy - 61 >= 0) {
			booksOnShelfCopy -= 61;
			booksOnRightShelf += 61;

			booksPerRowRight.push_back(61);
		}
		else {
			booksPerRowRight.push_back(booksOnShelfCopy);
		}
	}

	if (booksOnShelf <= 0) {
		booksPerRow.clear();
	}
}

void MainFrame::RemoveBookFromShelf() {
	booksOnShelf -= 2;
	AddBookToShelf();
}

void MainFrame::CreateRemoveBookPage() {

	returnToMainMenuButtonRemove = new wxButton(mainMenu, wxID_ANY, "BACK", wxPoint(1, 80), wxSize(100, 35));
	returnToMainMenuButtonRemove->Hide();

	removeBookBox = new wxListBox(mainMenu, wxID_ANY, wxPoint(400, 300), wxSize(600, 400));
	removeBookBox->Hide();

	bookCoverSizex = 350;
	bookCoverSizey = 500;

	imagePanel = new wxPanel(mainMenu, wxID_ANY, wxPoint(1050, 150), wxSize(bookCoverSizex, bookCoverSizey));
	bookCover = new wxStaticBitmap(imagePanel, wxID_ANY, wxBitmap(wxImage("data/images/Empty.png", wxBITMAP_TYPE_PNG).Scale(bookCoverSizex, bookCoverSizey)), wxPoint(0, 0), wxSize(bookCoverSizex, bookCoverSizey));
	imagePanel->Hide();

	bookDetails = new wxStaticText(mainMenu, wxID_ANY, "", wxPoint(1050, 655), wxSize(350, -1), wxALIGN_LEFT);
	bookDetails->SetFont(wxFont(wxFontInfo(wxSize(0, 20))));
	bookDetails->Hide();

	inputFieldRemovePage = new wxTextCtrl(mainMenu, wxID_ANY, "", wxPoint(780, 620), wxSize(50, 25), wxTE_PROCESS_ENTER);
	inputFieldRemovePage->SetFont(wxFontInfo(wxSize(0, 15)));
	inputFieldRemovePage->Hide();
}

void MainFrame::BindEventHandlers() {


	returnToMainMenuButtonRemove->Bind(wxEVT_BUTTON, &MainFrame::ReturnToMainMenuButtonClicked, this);

	inputField->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnter, this);

	removeBookBox->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDown, this);

	removeBookBox->Bind(wxEVT_LEFT_DOWN, &MainFrame::MouseClickListBox, this);

	mainMenu->Bind(wxEVT_LEFT_DOWN, &MainFrame::MouseClick, this);

	bookDetails->Bind(wxEVT_LEFT_DOWN, &MainFrame::EditText, this);

	inputFieldRemovePage->Bind(wxEVT_TEXT_ENTER, &MainFrame::InputRemovePage, this);
}

void MainFrame::EditText(wxMouseEvent& evt) {
	int x = evt.GetX();
	int y = evt.GetY();

	if (x >= 42 && x <= 265 && y >= 3 && y <= 21) {
		inputFieldRemovePage->SetPosition(wxPoint(bookDetails->GetPosition().x+198, bookDetails->GetPosition().y));
		inputFieldRemovePage->SetSize(wxSize(50, 25));
		inputFieldRemovePage->Show();
		inputFieldRemovePage->SetFocus();
		field = 0;
	}
	else if(x >= 8 && x <= 290 && y >= 22 && y <= 50) {
		inputFieldRemovePage->SetPosition(wxPoint(bookDetails->GetPosition().x + 158, bookDetails->GetPosition().y+25));
		inputFieldRemovePage->SetSize(wxSize(120, 25));
		inputFieldRemovePage->Show();
		inputFieldRemovePage->SetFocus();
		field = 1;
	}
}

void MainFrame::MouseClickListBox(wxMouseEvent& evt) {
	inputFieldRemovePage->Hide();
	evt.Skip();
	selectedIndex = removeBookBox->HitTest(evt.GetPosition());
	if (selectedIndex != wxNOT_FOUND) {
		int bookID = library.GetBook(selectedIndex)->getID();
		int pageCount = library.GetBook(selectedIndex)->getPageCount();
		int totalPages = library.GetBook(selectedIndex)->getTotalPages();
		string startDate = library.GetBook(selectedIndex)->getStartDate();
		string pathToImage = "data/bookCovers/" + to_string(bookID) + ".jpg";
		string details = "Pages:      ";
		details += to_string(pageCount);
		details += " / " + to_string(totalPages) + "\nStart Date:     " + startDate;


		if(std::ifstream (pathToImage.c_str())){
			bookCover->SetBitmap(wxBitmap(wxImage(pathToImage, wxBITMAP_TYPE_JPEG).Scale(bookCoverSizex, bookCoverSizey)));
		}
		else {
			bookCover->SetBitmap(wxBitmap(wxImage("data/bookCovers/notFound.jpg", wxBITMAP_TYPE_JPEG).Scale(bookCoverSizex, bookCoverSizey)));
		}

		imagePanel->Show();
		bookDetails->SetLabelText(details);
		bookDetails->Show();
	}
}

void MainFrame::MouseClickRemovePage(wxMouseEvent& evt) {
	inputFieldRemovePage->Hide();
}

void MainFrame::OnInputEnter(wxCommandEvent& evt) {
	Input();
}


void MainFrame::OnListKeyDown(wxKeyEvent& evt) {
	switch (evt.GetKeyCode()) {
	case WXK_DELETE:
		DeleteSelectedBook();
		break;
	}
}

void MainFrame::DeleteSelectedBook() {
	selectedIndex = removeBookBox->GetSelection();
	if (selectedIndex != wxNOT_FOUND) {
		removeBookBox->Delete(selectedIndex);
		int bookID = library.GetBook(selectedIndex)->getID();
		if (bookID != -1) {
			imagePanel->Hide();
			bookDetails->Hide();
			string pathToImage = "data/bookCovers/" + std::to_string(bookID) + ".jpg";
			remove(pathToImage.c_str());
			
		}
		library.RemoveBook(selectedIndex);

		if (library.getBookList().size() == 0) {
			library.setLastID(0);
		}
		else if (bookID == library.getLastID()) {
			library.setLastID(bookID-1);
		}
		RemoveBookFromShelf();
	}
}



// THIS METHOD NEED TO BE EDITED AND USED IN MOUSE CLICK FUNCTION
void MainFrame::RemoveBookPageButtonClicked(wxCommandEvent& evt) {
	simpleBook->ShowNewPage(removeBookPage);
	removeBookBox->Clear();
	for (string details : library.viewBooks()) {
		removeBookBox->Insert(details, removeBookBox->GetCount());
	}
}

void MainFrame::ReturnToMainMenuButtonClicked(wxCommandEvent& evt) {
	//removeBookPage->Hide();
	//simpleBook->ShowNewPage(mainMenu);
	imagePanel->Hide();
	bookDetails->Hide();
}

void MainFrame::MouseClick(wxMouseEvent& evt) {
	if (viewingBooks) {
		int x = evt.GetX();
		int y = evt.GetY();
		std::cout << "stop";
		if (x <= 400 || x >= 1000 || y <= 300 || y >= 700) {
				removeBookBox->Hide();
				imagePanel->Hide();
				bookDetails->Hide();
				inputFieldRemovePage->Hide();
				viewingBooks = false;
		}
	}
	else if (currentZoom == 0) {
		if (evt.GetY() >= 225 && evt.GetY() <= 825) {
			if ((evt.GetX() >= 520 && evt.GetX() <= 695)) {
				zoomInLeft = true;
			}
			else if (evt.GetX() >= 1175 && evt.GetX() <= 1350) {
				zoomInRight = true;
			}
			else {
				zoomOut = true;
			}
		}
		else {
			zoomOut = true;

		}
	}
	else if (currentZoom == 1) {
		if (evt.GetY() >= 226 && evt.GetX()>= 475 && evt.GetX() <= 1028) {

			inputField->Hide();
			removeBookBox->Show();

			removeBookBox->Clear();
			for (string details : library.viewBooks()) {
				removeBookBox->Insert(details, removeBookBox->GetCount());
			}

			viewingBooks = true;

		}else {
			zoomOut = true;
		}
	}
	else if (currentZoom == 2) {
		if (evt.GetY() >= 226 && evt.GetX() >= 1108 && evt.GetX() <= 1662) {

			inputField->Hide();
			removeBookBox->Show();

			removeBookBox->Clear();
			for (string details : library.viewBooks()) {
				removeBookBox->Insert(details, removeBookBox->GetCount());
			}

			viewingBooks = true;

		}
		else {
			zoomOut = true;
		}
	}
}

void MainFrame::Input() {
	wxString userInput = inputField->GetValue();

	if (!userInput.IsEmpty()) {
		inputField->Clear();
	}

	if (state.addName == 1) {
		state.addName = 0;
		state.addPage = 1;
		bookName = userInput;
	}
	else if (state.addPage == 1) {
		pageCount = userInput;
		wxString message = "you entered: " + userInput;
		if (!(is_digits(pageCount))) {
			wxMessageDialog notInt(this, "please enter an integer", "error", wxCANCEL);
			notInt.ShowModal();
		}else {
			state.addPage = 0;
			state.addTotalPages = 1;
		}
	}
	else if (state.addTotalPages == 1) {
		totalPages = userInput;
		wxString message = "you entered: " + userInput;
		if (!(is_digits(pageCount))) {
			wxMessageDialog notInt(this, "please enter an integer", "error", wxCANCEL);
			notInt.ShowModal();
		}
		else {
			state.addTotalPages = 0;
			state.addName = 1;

			time_t t = time(0);
			tm* timePtr = localtime(&t);
			string startDate = to_string(timePtr->tm_mday) + "/" + to_string(timePtr->tm_mon + 1);
			startDate += "/" + to_string(timePtr->tm_year + 1900);

			library.AddBook(bookName, pageCount, totalPages, startDate);


			AddBookToShelf();
		}
	}
	inputField->SetFocus();
}

void MainFrame::InputRemovePage(wxCommandEvent& evt) {
	wxString userInput = inputFieldRemovePage->GetValue();
	if (field == 0) {
		string numberOfPages = userInput.ToStdString();
		if (!(is_digits(numberOfPages))) {
			wxMessageDialog notInt(this, "please enter an integer", "error", wxCANCEL);
			notInt.ShowModal();
		}
		else {
			library.GetBook(selectedIndex)->setPageCount(stoi(numberOfPages));
			inputFieldRemovePage->Hide();
			int pageCountDisplay = library.GetBook(selectedIndex)->getPageCount();
			string startDate = library.GetBook(selectedIndex)->getStartDate();
			int totalPages = library.GetBook(selectedIndex)->getTotalPages();
			string details = "Pages:      ";
			details += to_string(pageCountDisplay);
			details += " / " + to_string(totalPages) + "\nStart Date:     " + startDate;
			bookDetails->SetLabelText(details);
		}
	}
	else if (field == 1) {
		 string startDate = userInput.ToStdString();
		 bool valid = true;

		 if (startDate.size() <= 10) {
			 for (char c : startDate) { // checks if string is only numbers or /
				 if (!isdigit(c) && c != '/') {
					 valid = false;
				 }
			 }
		 }
		 else {
			 valid = false;
		 }
		 

		 if (valid) {
			 library.GetBook(selectedIndex)->setStartDate(startDate);
			 inputFieldRemovePage->Hide();
			 int pageCountDisplay = library.GetBook(selectedIndex)->getPageCount();
			 string startDate = library.GetBook(selectedIndex)->getStartDate();
			 string details = "Pages:     ";
			 details += " ";
			 details += " / " + to_string(pageCountDisplay) + "\nStart Date:     " + startDate;
			 bookDetails->SetLabelText(details);
		 }
	}
	inputFieldRemovePage->Clear();
}

MainFrame::~MainFrame() {
	library.SaveToExternal();
}