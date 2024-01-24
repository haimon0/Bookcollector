#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "Library.h"
#include <vector>
#include <sstream>
using std::string;
using std::cout;
using std::to_string;
using std::cin;
using std::list;

Book::Book(int ID, string& bN, int& pC, int& tP, string& sD) {
	bookName = bN;
	pageCount = pC;
	totalPages = tP;
	startDate = sD;
	bookID = ID;
}

//Used to return a "null" book when a book is not found
Book::Book(int ID){
	bookID = ID;
}

string Book::getBookName() {
	return bookName;
}

string Book::getStartDate() {
	return startDate;
}

int Book::getID() {
	return bookID;
}

int Book::getPageCount() {
	return pageCount;
}
int Book::getTotalPages() {
	return totalPages;
}

void Book::setPageCount(int pC) {
	pageCount = pC;
}

void Book::setStartDate(string cD) {
	startDate = cD;
}

int Library::getLastID() {
	return lastID;
}

void Library::setLastID(int ID) {
	lastID = ID;
}

list<Book> Library::getBookList()
{
	return bookList;
}

Library::Library() {
	lastID = 0;
}

//Adding Books From UserInput
void Library::AddBook(string bN, string p, string tP, string startDate) {
	int pages = stoi(p);
	int totalPages = stoi(tP);
	lastID++;
	Book book(lastID, bN, pages, totalPages, startDate);
	bookList.push_back(book);
}

//Adding Books From External File
void Library::AddBook(string ID, string bN, string p, string tP, string startDate) {
	int bID = stoi(ID);
	int pages = stoi(p);
	int totalPages = stoi(tP);

	Book book(bID, bN, pages, totalPages, startDate);
	bookList.push_back(book);
}

list<string> Library::viewBooks() {
	cout << "\n";
	int index = 1;
	list<string> bookDetailsList;
	string i;
	string bookDetails;
	if (bookList.size() != 0) {
		for (Book book : bookList) {
			i = to_string(index);
			bookDetails = book.getBookName();//+ " " + to_string(book.getPageCount()) + " " + book.getStartDate() ***add more details here if needed i.e. author or rating
			bookDetailsList.push_back(bookDetails);
			index++;
		}
	}
	return bookDetailsList;
}

void Library::UpdatePages() {
	cout << "\n<Update pages>\n";
	viewBooks();
	int currentPage;
	cout << "\n enter current page: ";
	cin >> currentPage;
	cout << "\n";
}

Book* Library::GetBook(int index) {
	bool iterate = true;
	if (bookList.size() != 0) {
		if (0 <= index && index <= bookList.size()) {
			list<Book>::iterator book = bookList.begin();
			for (int i = 0; i < index; i++) {
				++book;
			}
			return &*book;
		}
	}
	return new Book(-1); //is this safe? check later
}

void Library::RemoveBook(int removeNumber) {
	bool iterate = true;
	if (bookList.size() != 0) {
		if (0 <= removeNumber && removeNumber <= bookList.size()) {
			list<Book>::iterator book = bookList.begin();
			for (int i = 0; i < removeNumber; i++) {
				++book;
			}
			bookList.erase(book);
		}
		
	}
}

void Library::SaveToExternal() {
	if (bookList.size() > 0) {
		std::fstream fileOut;
		fileOut.open("data/database/BooksDatabase.csv", std::ios::out, std::ios::trunc);
		for (Book b : bookList) {
			fileOut << std::to_string(b.getID()) + "," + b.getBookName() + "," + std::to_string(b.getPageCount()) + "," + std::to_string(b.getTotalPages()) + "," + b.getStartDate();
			fileOut << "\n";
		}
		fileOut.close();
	}
	else {
		remove("data/database/BooksDatabase.csv");
	}
}

void Library::ReadFromExternal() {
	
	std::fstream fileIn;
	fileIn.open("data/database/BooksDatabase.csv", std::ios::in);
		
	// Read the Data from the file
	// as String Vector
	std::vector<string> row;
	string line = "";
	string word, temp;

	while (getline(fileIn, line)) {

		row.clear();

		// read an entire row and
		// store it in a string variable 'line'
		

		// used for breaking words
		std::stringstream ss(line);

		// read every column data of a row and
		// store it in a string variable, 'word'
		string bookDetails;
		while (getline(ss, word, ',')) {

			// add all the column data
			// of a row to a vector
			row.push_back(word);
		}
		lastID = stoi(row[0]);
		AddBook(row[0], row[1], row[2], row[3], row[4]);
	}
	fileIn.close();
}
