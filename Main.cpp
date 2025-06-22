#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

#include "Book.h"
#include "LinkedList.h"
#include "BST.h"
#include "Stack.h"
#include "Queue.h"
#include "Search.h"
#include "Sort.h"

using namespace std;

// Utility functions for Book fields
string getISBN(const Book& b) { return b.isbn; }
string getTitle(const Book& b) { return b.title; }
string getAuthor(const Book& b) { return b.author; }
double getPrice(const Book& b) { return b.price; }
int getQuantity(const Book& b) { return b.quantity; }

void printLine(char ch = '=', int width = 45) {
    cout << setfill(ch) << setw(width) << "" << setfill(' ') << endl;
}

void displayMenu() {
    printLine();
    cout << setw(30) << right << "📚 Book Inventory Management System 📚" << endl;
    printLine();
    cout << " 1. Add a Book\n";
    cout << " 2. Delete a Book\n";
    cout << " 3. Update Book Information\n";
    cout << " 4. Search for a Book\n";
    cout << " 5. Display All Books\n";
    cout << " 6. Sort Books\n";
    cout << " 7. Display Recently Updated Books (Stack)\n";
    cout << " 8. Queue Restock Request\n";
    cout << " 9. Process Restock Request\n";
    cout << " 0. Exit\n";
    printLine('-');
    cout << "Choose an option: ";
}

void loadBooks(const string& filename, LinkedList<Book>& bookList, BST<Book, string>& bookBST) {
    ifstream infile(filename);
    string line;
    while (getline(infile, line)) {
        Book b = Book::fromFileString(line);
        bookList.addFront(b);
        bookBST.insert(b);
    }
    infile.close();
}

void saveBooks(const string& filename, const LinkedList<Book>& bookList) {
    ofstream outfile(filename);
    Node<Book>* node = bookList.getHead();
    while (node) {
        outfile << node->data.toFileString() << endl;
        node = node->next;
    }
    outfile.close();
}

string toLower(const string& str) {
    string result = str;
    for (char& c : result)
        if (c >= 'A' && c <= 'Z') c = c + ('a' - 'A');
    return result;
}

void printBook(const Book& b) {
    cout << left << setw(15) << b.isbn
         << setw(25) << b.title.substr(0, 22)
         << setw(15) << b.author.substr(0, 12)
         << setw(8) << b.quantity
         << fixed << setprecision(2) << setw(8) << b.price << endl;
}

void printBookHeader() {
    cout << left << setw(15) << "ISBN"
         << setw(25) << "Title"
         << setw(15) << "Author"
         << setw(8) << "Qty"
         << setw(8) << "Price" << endl;
    printLine('-', 71);
}

int main() {
    LinkedList<Book> bookList;
    BST<Book, string> bookBST(getISBN);
    Stack<Book> recentUpdates;
    Queue<Book> restockQueue;
    const string filename = "books.txt";
    loadBooks(filename, bookList, bookBST);

    int choice;
    do {
        displayMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: { // Add Book
            char cont;
            do {
                string isbn, title, author;
                int quantity;
                double price;
                cout << "Enter ISBN: "; getline(cin, isbn);
                cout << "Enter Title: "; getline(cin, title);
                cout << "Enter Author: "; getline(cin, author);
                cout << "Enter Quantity: "; cin >> quantity;
                cout << "Enter Price: "; cin >> price; cin.ignore();
                Book newBook(isbn, title, author, quantity, price);
                bookList.addFront(newBook);
                bookBST.insert(newBook);
                recentUpdates.push(newBook);
                saveBooks(filename, bookList);
                cout << "✅ Book added!\n";
                cout << "Add another book? (y/n): ";
                cin >> cont; cin.ignore();
            } while (tolower(cont) == 'y');
            break;
        }
        case 2: { // Delete Book
            char cont;
            do {
                string isbn;
                cout << "Enter ISBN of book to delete: ";
                getline(cin, isbn);
                Node<Book>* node = bookList.getHead();
                bool found = false;
                while (node) {
                    if (node->data.isbn == isbn) {
                        found = true;
                        Book bookToDelete = node->data;
                        bookList.remove(bookToDelete);
                        bookBST.remove(bookToDelete);
                        saveBooks(filename, bookList);
                        cout << "🗑️ Book deleted from list.\n";
                        break;
                    }
                    node = node->next;
                }
                if (!found) cout << "Book not found.\n";
                cout << "Delete another book? (y/n): ";
                cin >> cont; cin.ignore();
            } while (tolower(cont) == 'y');
            break;
        }
        case 3: { // Update Book
            char cont;
            do {
                string isbn;
                cout << "Enter ISBN of book to update: "; getline(cin, isbn);
                Node<Book>* node = bookList.getHead();
                while (node && node->data.isbn != isbn) node = node->next;
                if (node) {
                    string title, author;
                    int quantity;
                    double price;
                    cout << "Enter New Title: "; getline(cin, title);
                    cout << "Enter New Author: "; getline(cin, author);
                    cout << "Enter New Quantity: "; cin >> quantity;
                    cout << "Enter New Price: "; cin >> price; cin.ignore();
                    Book updatedBook(isbn, title, author, quantity, price);
                    Book oldBook = node->data;
                    bookBST.remove(oldBook);
                    node->data = updatedBook;
                    bookBST.insert(updatedBook);
                    recentUpdates.push(updatedBook);
                    saveBooks(filename, bookList);
                    cout << "✏️ Book updated!\n";
                } else {
                    cout << "Book not found.\n";
                }
                cout << "Update another book? (y/n): ";
                cin >> cont; cin.ignore();
            } while (tolower(cont) == 'y');
            break;
        }
        case 4: { // Search
            char cont;
            do {
                cout << "Search by:\n";
                cout << " 1. ISBN\n";
                cout << " 2. Title\n";
                cout << " 3. Author\n";
                cout << "Choose option: ";
                int searchType;
                cin >> searchType; cin.ignore();
                if (searchType == 1) {
                    string isbn;
                    cout << "Enter ISBN: "; getline(cin, isbn);
                    Book* found = bookBST.search(isbn);
                    if (found) {
                        printBookHeader();
                        printBook(*found);
                    } else cout << "Book not found.\n";
                } else if (searchType == 2) {
                    string keyword;
                    cout << "Enter keyword to search in book title: ";
                    getline(cin, keyword);
                    keyword = toLower(keyword);
                    Node<Book>* node = bookList.getHead();
                    bool found = false;
                    printBookHeader();
                    while (node) {
                        string titleLower = toLower(node->data.title);
                        if (titleLower.find(keyword) != string::npos) {
                            printBook(node->data);
                            found = true;
                        }
                        node = node->next;
                    }
                    if (!found) cout << "No books matched the keyword.\n";
                } else if (searchType == 3) {
                    string keyword;
                    cout << "Enter keyword to search in author name: ";
                    getline(cin, keyword);
                    keyword = toLower(keyword);
                    Node<Book>* node = bookList.getHead();
                    bool found = false;
                    printBookHeader();
                    while (node) {
                        string authorLower = toLower(node->data.author);
                        if (authorLower.find(keyword) != string::npos) {
                            printBook(node->data);
                            found = true;
                        }
                        node = node->next;
                    }
                    if (!found) cout << "No books matched the author keyword.\n";
                } else {
                    cout << "Invalid search option.\n";
                }
                cout << "Search another book? (y/n): ";
                cin >> cont; cin.ignore();
            } while (tolower(cont) == 'y');
            break;
        }
        case 5: // Display All
            cout << "All Books in Inventory:\n";
            printBookHeader();
            {
                Node<Book>* node = bookList.getHead();
                while (node) {
                    printBook(node->data);
                    node = node->next;
                }
            }
            break;
        case 6: { // Sort
            cout << "Sort books by:\n";
            cout << " 1. Price\n";
            cout << " 2. Quantity\n";
            cout << " 3. Title (Name)\n";
            cout << " 4. ISBN\n";
            cout << " 5. Author\n";
            cout << "Choose option: ";
            int sortType;
            cin >> sortType; cin.ignore();

            int count = 0;
            Node<Book>* node = bookList.getHead();
            Book tempArr[200];
            while (node && count < 200) {
                tempArr[count++] = node->data;
                node = node->next;
            }

            switch (sortType) {
                case 1:
                    sortByKey<Book, double>(tempArr, count, getPrice);
                    cout << "Books sorted by price:\n";
                    break;
                case 2:
                    sortByKey<Book, int>(tempArr, count, getQuantity);
                    cout << "Books sorted by quantity:\n";
                    break;
                case 3:
                    sortByKey<Book, string>(tempArr, count, getTitle);
                    cout << "Books sorted by title:\n";
                    break;
                case 4:
                    sortByKey<Book, string>(tempArr, count, getISBN);
                    cout << "Books sorted by ISBN:\n";
                    break;
                case 5:
                    sortByKey<Book, string>(tempArr, count, getAuthor);
                    cout << "Books sorted by author:\n";
                    break;
                default:
                    cout << "Invalid sort option.\n";
                    return 0;
            }
            printBookHeader();
            for (int i = 0; i < count; ++i) printBook(tempArr[i]);
            break;
        }
        case 7: { // Recently Updated (Stack)
            cout << "Recently Updated Books (Stack):\n";
            Stack<Book> tempStack = recentUpdates;
            Book b;
            printBookHeader();
            while (!tempStack.isEmpty()) {
                tempStack.pop(b);
                printBook(b);
            }
            break;
        }
        case 8: { // Queue Restock Request
            char cont;
            do {
                string isbn;
                cout << "Enter ISBN of book to restock: ";
                getline(cin, isbn);
                Node<Book>* node = bookList.getHead();
                while (node && node->data.isbn != isbn) node = node->next;
                if (node) {
                    restockQueue.enqueue(node->data);
                    cout << "Restock request queued.\n";
                } else {
                    cout << "Book not found.\n";
                }
                cout << "Queue another restock? (y/n): ";
                cin >> cont; cin.ignore();
            } while (tolower(cont) == 'y');
            break;
        }
        case 9: { // Process Restock Request
            char cont;
            do {
                Book b;
                if (restockQueue.dequeue(b)) {
                    cout << "Processing restock for book:\n" << b << endl;
                } else {
                    cout << "No restock requests in queue.\n";
                }
                cout << "Process next restock request? (y/n): ";
                cin >> cont; cin.ignore();
            } while (tolower(cont) == 'y');
            break;
        }
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}