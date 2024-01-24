#pragma once
#include <string>
#include <list>
#include <fstream>
using std::string;
using std::list;

class Book {
public:
	Book(int ID, string& bN, int& pC, int& tp, string& dTF);
	Book(int ID);

	string getBookName();
	int getPageCount();
	string getStartDate();
	int getID();
	int getTotalPages();

	void setPageCount(int pC);
	void setStartDate(string sD);

private:
	string bookName;
	int pageCount; //pages read
	int totalPages; //total pages in book
	string startDate;
	int bookID;
};

class Library {
public:
	Library();
	void AddBook(string bN, string p, string tP, string startDate);
	void AddBook(string ID, string bN, string p, string tP, string startDate);
	void UpdatePages();
	Book* GetBook(int index);
	void RemoveBook(int removeNumber);

	void setLastID(int ID);
	int getLastID();

	void SaveToExternal();
	void ReadFromExternal();

	list<string> viewBooks();
	list<Book> getBookList();

private:
	list<Book> bookList{};
	int lastID;
};
