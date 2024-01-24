#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include "Library.h"
#include <list>
#include <string>
#include <wx/simplebook.h>
#include <fstream>

using std::string;

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
	// IMAGE CODE
	void UpdateBackground();
	// END OF IMAGE CODE
private:
	~MainFrame();
	void RemoveBookPageButtonClicked(wxCommandEvent& evt);
	void ReturnToMainMenuButtonClicked(wxCommandEvent& evt);

	void MouseClick(wxMouseEvent& evt); // for main menu only
	void MouseClickRemovePage(wxMouseEvent& evt); // for "remove book" page only
	void MouseClickListBox(wxMouseEvent& evt); //for "remove book" page only
	void EditText(wxMouseEvent& evt); //for "remove book" page only
	void OnInputEnter(wxCommandEvent& evt);

	void Input();
	void InputRemovePage(wxCommandEvent& evt);

	void CreateMainMenu();
	void CreateRemoveBookPage();

	void BindEventHandlers();
	
	void OnListKeyDown(wxKeyEvent& evt);

	void DeleteSelectedBook();

	// IMAGE CODE
	int xPos, yPos, xPosBook, yPosBook, xPosBookRight;
	int maxFrame;
	int currentFrame;
	int maxBooks;
	int booksOnShelf;
	int booksOnShelfCopy;
	int maxBooksOnShelf;
	int changeY;
	int scale;
	int row;
	int currentZoom;
	int bookCoverSizex;
	int bookCoverSizey;
	
	string baseImageSrc;
	string baseBookSrc;
	bool zoomInLeft;
	bool zoomInRight;
	bool zoomOut;
	bool viewingBooks;

	wxPanel* imagePanel;
	wxBitmap bookBitmap;
	wxBitmap bookBitmapRight;
	wxBitmap maxBookBitmap;
	wxBitmap m_scaledBg;
	wxStaticBitmap* bookCover;
	wxStaticText* bookDetails;
	list<wxBitmap> bitMapList{};
	list<wxBitmap> bitMapListBook{};
	list <int> booksPerRow;
	list <int> booksPerRowRight;
	list <int> booksYPosition;

	void AddBookToShelf();
	void RemoveBookFromShelf();
	// END OF IMAGE CODE


	wxPanel* mainMenu;
	wxPanel* removeBookPage;

	wxTextCtrl* inputField;
	wxTextCtrl* inputFieldRemovePage;

	wxListBox* removeBookBox;

	wxButton* removeBookPageButton;
	wxButton* returnToMainMenuButtonRemove;

	wxSimplebook* simpleBook;

	

	Library library;
	string bookName;
	string pageCount; //pages read
	string totalPages; //total pages in book
	int selectedIndex;
	int field;
};

#endif // MAINFRAME_H

