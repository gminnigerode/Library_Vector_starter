#include "../includes_usr/fileIO.h"
#include <fstream>
#include <sstream>

using namespace std;
/* clears, then loads books from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries in books
 * 			SUCCESS if all data is loaded
 * */
int loadBooks(std::vector<book> &books, const char* filename)
{
	ifstream bookStream(filename, ios_base::in);
	if(!bookStream.is_open()){
		return COULD_NOT_OPEN_FILE;
	}
	books.clear();
	string line;
	while(!bookStream.eof()){
		getline(bookStream, line);
		stringstream ss(line);
			string tempStr;
			book tempBook;
			int count = 0;
			while(getline(ss,tempStr,',')){
				if(tempStr.size() == 0){
						break;
				}
				stringstream sToInt(tempStr);
				switch(count){
				case 0:
					int bookID;
					sToInt >> bookID;
					tempBook.book_id = bookID;
					count++;
					break;
				case 1:
					tempBook.title = tempStr;
					count++;
					break;
				case 2:
					tempBook.author = tempStr;
					count++;
					break;
				case 3:
					int bookState;
					sToInt >> bookState;
					if(bookState == 1){
						tempBook.state = book_checkout_state::IN;
						count++;
						break;
					}
					else{
						tempBook.state = book_checkout_state::OUT;
						count++;
						break;
					}
				case 4:
					int patronID;
					sToInt >> patronID;
					tempBook.loaned_to_patron_id = patronID;
					break;
				}
			}
			if(tempBook.book_id!=UNINITIALIZED){
				books.push_back(tempBook);
			}
	}
	if(books.size()==0){
		return NO_BOOKS_IN_LIBRARY;
	}
	return SUCCESS;
}

/* serializes books to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries books (do not create file)
 * 			SUCCESS if all data is saved
 * */
int saveBooks(std::vector<book> &books, const char* filename)
{
	if(books.size() == 0){
		return NO_BOOKS_IN_LIBRARY;
	}
	ofstream outStream(filename, ios_base::out);
	if(!outStream.is_open()){
		return COULD_NOT_OPEN_FILE;
	}
	for (book b:books){
		int state;
		switch(b.state){
		case book_checkout_state::IN:
			state = 1;
			break;

		case book_checkout_state::OUT:
			state = 2;
			break;

		case book_checkout_state::UNKNOWN:
			state = 0;
			break;
		}
		outStream << to_string(b.book_id) + "," + b.title + "," + b.author + "," + to_string(state) + "," + to_string(b.loaned_to_patron_id) + "\n";
	}
	return SUCCESS;
}

/* clears, then loads patrons from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons
 * 			SUCCESS if all data is loaded
 * */
int loadPatrons(std::vector<patron> &patrons, const char* filename)
{
	ifstream patronStream(filename, ios_base::in);
	if(!patronStream.is_open()){
		return COULD_NOT_OPEN_FILE;
	}
	patrons.clear();
	string line;
	while(!patronStream.eof()){
		getline(patronStream, line);
		stringstream ss(line);
			string tempStr;
			patron tempPatron;
			int count = 0;
			while(getline(ss,tempStr,',')){
				if(tempStr.size() == 0){
						break;
				}
				stringstream sToInt(tempStr);
				switch(count){
				case 0:
					int patronID;
					sToInt >> patronID;
					tempPatron.patron_id = patronID;
					count++;
					break;
				case 1:
					tempPatron.name = tempStr;
					count++;
					break;
				case 2:
					int numBooksOut;
					sToInt >> numBooksOut;
					tempPatron.number_books_checked_out = numBooksOut;
					break;
				}
			}
			if(tempPatron.patron_id!=UNINITIALIZED){
				patrons.push_back(tempPatron);
			}
	}
	if(patrons.size() == 0){
		return NO_PATRONS_IN_LIBRARY;
	}
	return SUCCESS;
}

/* serializes patrons to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons  (do not create file)
 * 			SUCCESS if all data is saved
 * */
int savePatrons(std::vector<patron> &patrons, const char* filename)
{
	if(patrons.size() == 0){
		return NO_PATRONS_IN_LIBRARY;
	}
	ofstream outStream(filename, ios_base::out);
	if(!outStream.is_open()){
		return COULD_NOT_OPEN_FILE;
	}
	for (patron p:patrons){
		outStream <<  to_string(p.patron_id) + "," + p.name + "," + to_string(p.number_books_checked_out) + "\n";
	}
	return SUCCESS;
}
