#include <iostream> 

#include <fstream> 

#include <sstream> 

#include <string> 

#include "Book.h" 

#include "LinkedList.h" 

#include "BST.h" 

#include "Stack.h" 

#include "Queue.h" 

#include "Search.h" 

#include "Sort.h" 

using namespace std;



string getISBN(const Book& b) { return b.isbn; }

string getTitle(const Book& b) { return b.title; }

double getPrice(const Book& b) { return b.price; }

int getQuantity(const Book& b) { return b.quantity; }



void displayMenu() {

    cout << "\n===== Book Inventory Management System =====\n";

    cout << "1. Add a Book\n";

    cout << "2. Delete a Book\n";

    cout << "3. Update Book Information\n";

    cout << "4. Search for a Book\n"; // <- single combined search 

    cout << "5. Display All Books\n";

    cout << "6. Sort Books by Price\n";

    cout << "7. Display Recently Updated Books (Stack)\n";

    cout << "8. Queue Restock Request\n";

    cout << "9. Process Restock Request\n";

    cout << "0. Exit\n";

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

    for (int i = 0; i < result.length(); ++i) {

        if (result[i] >= 'A' && result[i] <= 'Z') {

            result[i] = result[i] + ('a' - 'A');

        }

    }

    return result;

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

        case 1: {

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

                cout << "Book added!\n";

                cout << "Add another book? (y/n): ";

                cin >> cont;

                cin.ignore();

            } while (tolower(cont) == 'y');

            break;

        }

        case 2: {

            char cont;

            do {

                string isbn;

                cout << "Enter ISBN of book to delete: ";

                getline(cin, isbn);



                Node<Book>* node = bookList.getHead();

                Node<Book>* prev = nullptr;

                bool found = false;



                while (node) {

                    if (node->data.isbn == isbn) {

                        found = true;



                        // Remove from list and BST 

                        Book bookToDelete = node->data; // Make a safe copy 

                        bookList.remove(bookToDelete);

                        bookBST.remove(bookToDelete);

                        saveBooks(filename, bookList);



                        cout << "Book deleted from list.\n";

                        break;

                    }

                    node = node->next;

                }



                if (!found) {

                    cout << "Book not found.\n";

                }



                cout << "Delete another book? (y/n): ";

                cin >> cont;

                cin.ignore();

            } while (tolower(cont) == 'y');

            break;

        }



        case 3: {

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

                    cout << "Book updated!\n";

                }

                else {

                    cout << "Book not found.\n";

                }

                cout << "Update another book? (y/n): ";

                cin >> cont;

                cin.ignore();

            } while (tolower(cont) == 'y');

            break;

        }

        case 4: { // Combined search 

            char cont;

            do {

                cout << "Search by:\n";

                cout << "1. ISBN\n";

                cout << "2. Title\n";

                cout << "Choose option: ";

                int searchType;

                cin >> searchType;

                cin.ignore();

                if (searchType == 1) {

                    string isbn;

                    cout << "Enter ISBN: "; getline(cin, isbn);

                    Book* found = bookBST.search(isbn);

                    if (found) cout << "Book found:\n" << *found << endl;

                    else cout << "Book not found.\n";

                }

                else if (searchType == 2) {

                    string keyword;

                    cout << "Enter keyword to search in book title: ";

                    getline(cin, keyword);

                    keyword = toLower(keyword);



                    Node<Book>* node = bookList.getHead();

                    bool found = false;



                    while (node) {

                        string titleLower = toLower(node->data.title);

                        if (titleLower.find(keyword) != string::npos) {

                            cout << node->data << endl;

                            found = true;

                        }

                        node = node->next;

                    }



                    if (!found) cout << "No books matched the keyword.\n";

                }

                else {

                    cout << "Invalid search option.\n";

                }

                cout << "Search another book? (y/n): ";

                cin >> cont;

                cin.ignore();

            } while (tolower(cont) == 'y');

            break;

        }

        case 5:

            cout << "All Books in Inventory:\n";

            bookList.displayAll();

            break;

        case 6: {

            int count = 0;

            Node<Book>* node = bookList.getHead();

            Book tempArr[200];

            while (node && count < 200) {

                tempArr[count++] = node->data;

                node = node->next;

            }

            sortByKey<Book, double>(tempArr, count, getPrice);

            cout << "Books sorted by price:\n";

            for (int i = 0; i < count; ++i) cout << tempArr[i] << endl;

            break;

        }

        case 7: {

            cout << "Recently Updated Books (Stack):\n";

            Stack<Book> tempStack = recentUpdates;

            Book b;

            while (!tempStack.isEmpty()) {

                tempStack.pop(b);

                cout << b << endl;

            }

            break;

        }

        case 8: {

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

                }

                else {

                    cout << "Book not found.\n";

                }

                cout << "Queue another restock? (y/n): ";

                cin >> cont;

                cin.ignore();

            } while (tolower(cont) == 'y');

            break;

        }

        case 9: {

            char cont;

            do {

                Book b;

                if (restockQueue.dequeue(b)) {

                    cout << "Processing restock for book:\n" << b << endl;

                }

                else {

                    cout << "No restock requests in queue.\n";

                }

                cout << "Process next restock request? (y/n): ";

                cin >> cont;

                cin.ignore();

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