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

string getISBN(const Book& b) { return b.getISBN(); }
string getTitle(const Book& b) { return b.getTitle(); }
string getAuthor(const Book& b) { return b.getAuthor(); }
double getPrice(const Book& b) { return b.getPrice(); }
int getQuantity(const Book& b) { return b.getQuantity(); }

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

void printBookHeader(int isbnW, int titleW, int authorW) {
    cout << left << setw(isbnW) << "ISBN"
         << setw(titleW) << "Title"
         << setw(authorW) << "Author"
         << setw(8) << "Qty"
         << setw(8) << "Price" << endl;
    printLine('-', isbnW + titleW + authorW + 8 + 8);
}

void printBook(const Book& b, int isbnW, int titleW, int authorW) {
    cout << left << setw(isbnW) << b.getISBN()
         << setw(titleW) << b.getTitle()
         << setw(authorW) << b.getAuthor()
         << setw(8) << b.getQuantity()
         << fixed << setprecision(2) << setw(8) << b.getPrice() << endl;
}

// --- Input validation helpers ---
bool isPositiveInteger(const string& s) {
    if (s.empty()) return false;
    for (char c : s)
        if (!isdigit(c)) return false;
    return true;
}

bool isPositiveDouble(const string& s) {
    if (s.empty()) return false;
    int dotCount = 0;
    for (char c : s) {
        if (c == '.') {
            dotCount++;
            if (dotCount > 1) return false;
        }
        else if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

struct RestockRequest {
    Book book;
    int quantity;
};

void getMaxColumnWidths(const LinkedList<Book>& bookList, int& isbnW, int& titleW, int& authorW) {
    isbnW = 4;   // "ISBN"
    titleW = 5;  // "Title"
    authorW = 6; // "Author"
    Node<Book>* node = bookList.getHead();
    while (node) {
        isbnW = max(isbnW, (int)node->data.getISBN().length());
        titleW = max(titleW, (int)node->data.getTitle().length());
        authorW = max(authorW, (int)node->data.getAuthor().length());
        node = node->next;
    }
    // Add some padding
    isbnW += 2;
    titleW += 2;
    authorW += 2;
}

int main() {
    LinkedList<Book> bookList;
    BST<Book, string> bookBST(getISBN);
    Stack<Book> recentUpdates;
    Queue<RestockRequest> restockQueue;
    const string filename = "books.txt";
    loadBooks(filename, bookList, bookBST);

    // Calculate column widths once after loading
    int isbnW, titleW, authorW;
    getMaxColumnWidths(bookList, isbnW, titleW, authorW);

    int choice;
    string input;
    do {
        displayMenu();

        // Input validation for menu choice (0-9)
        while (true) {
            getline(cin, input);
            if (isPositiveInteger(input)) {
                choice = stoi(input);
                if (choice >= 0 && choice <= 9) break;
            }
            cout << "Invalid option. Please enter a number (0-9)." << endl;
            cout << "Choose an option: ";
        }

        switch (choice) {
        case 1: { // Add Book
            char cont;
            do {
                string isbn, title, author;
                int quantity;
                double price;

                cout << "\nEnter ISBN: "; getline(cin, isbn);
                cout << "Enter Title: "; getline(cin, title);
                cout << "Enter Author: "; getline(cin, author);

                // Input validation for quantity
                while (true) {
                    cout << "Enter Quantity: ";
                    getline(cin, input);
                    if (isPositiveInteger(input)) {
                        quantity = stoi(input);
                        break;
                    }
                    cout << "Invalid input for quantity. Please enter a non-negative integer." << endl;
                }

                // Input validation for price
                while (true) {
                    cout << "Enter Price: ";
                    getline(cin, input);
                    if (isPositiveDouble(input)) {
                        price = stod(input);
                        break;
                    }
                    cout << "Invalid input for price. Please enter a non-negative number." << endl;
                }

                Book newBook(isbn, title, author, quantity, price);
                bookList.addFront(newBook);
                bookBST.insert(newBook);
                recentUpdates.push(newBook);
                saveBooks(filename, bookList);
                getMaxColumnWidths(bookList, isbnW, titleW, authorW); // recalc widths
                cout << "✅ Book added!\n";

                cout << "Add another book? (y/n): ";
                while (true) {
                    getline(cin, input);
                    if (input.length() == 1 && (tolower(input[0]) == 'y' || tolower(input[0]) == 'n')) {
                        cont = input[0];
                        break;
                    }
                    cout << "Invalid input. Please enter 'y' or 'n': ";
                }
            } while (tolower(cont) == 'y');
            break;
        }
        case 2: { // Delete Book
            char cont;
            do {
                string isbn;
                cout << "\nEnter ISBN of book to delete: ";
                getline(cin, isbn);
                Node<Book>* node = bookList.getHead();
                bool found = false;
                while (node) {
                    if (toLower(node->data.getISBN()) == toLower(isbn)) {
                        found = true;
                        cout << "\nBook found:\n";
                        printBookHeader(isbnW, titleW, authorW);
                        printBook(node->data, isbnW, titleW, authorW);
                        cout << "Are you sure you want to delete this book? (y/n): ";
                        string confirm;
                        while (true) {
                            getline(cin, confirm);
                            if (confirm.length() == 1 && (tolower(confirm[0]) == 'y' || tolower(confirm[0]) == 'n')) {
                                break;
                            }
                            cout << "Invalid input. Please enter 'y' or 'n': ";
                        }
                        if (tolower(confirm[0]) == 'y') {
                            Book bookToDelete = node->data;
                            bookList.remove(bookToDelete);
                            bookBST.remove(bookToDelete);
                            saveBooks(filename, bookList);
                            getMaxColumnWidths(bookList, isbnW, titleW, authorW); // recalc widths
                            cout << "🗑️ Book deleted from list.\n";
                        } else {
                            cout << "Deletion cancelled.\n";
                        }
                        break;
                    }
                    node = node->next;
                }
                if (!found) cout << "Book not found.\n";
                cout << "Delete another book? (y/n): ";
                while (true) {
                    getline(cin, input);
                    if (input.length() == 1 && (tolower(input[0]) == 'y' || tolower(input[0]) == 'n')) {
                        cont = input[0];
                        break;
                    }
                    cout << "Invalid input. Please enter 'y' or 'n': ";
                }
            } while (tolower(cont) == 'y');
            break;
        }
        case 3: { // Update Book
            char cont;
            do {
                // Display all books first
                cout << "\nAll Books in Inventory:\n";
                printBookHeader(isbnW, titleW, authorW);
                Node<Book>* node = bookList.getHead();
                while (node) {
                    printBook(node->data, isbnW, titleW, authorW);
                    node = node->next;
                }

                string isbn;
                cout << "\nEnter ISBN of book to update: "; 
                getline(cin, isbn);
                node = bookList.getHead();
                while (node && toLower(node->data.getISBN()) != toLower(isbn)) node = node->next;
                if (node) {
                    cout << "\nBook found:\n";
                    printBookHeader(isbnW, titleW, authorW);
                    printBook(node->data, isbnW, titleW, authorW);
                    cout << "Do you want to update this book? (y/n): ";
                    string confirm;
                    while (true) {
                        getline(cin, confirm);
                        if (confirm.length() == 1 && (tolower(confirm[0]) == 'y' || tolower(confirm[0]) == 'n')) {
                            break;
                        }
                        cout << "Invalid input. Please enter 'y' or 'n': ";
                    }
                    if (tolower(confirm[0]) == 'y') {
                        string newIsbn, title, author;
                        int quantity;
                        double price;
                        cout << "Enter New ISBN: "; getline(cin, newIsbn);
                        cout << "Enter New Title: "; getline(cin, title);
                        cout << "Enter New Author: "; getline(cin, author);

                        // Input validation for quantity
                        while (true) {
                            cout << "Enter New Quantity: ";
                            getline(cin, input);
                            if (isPositiveInteger(input)) {
                                quantity = stoi(input);
                                break;
                            }
                            cout << "Invalid input for quantity. Please enter a non-negative integer." << endl;
                        }

                        // Input validation for price
                        while (true) {
                            cout << "Enter New Price: ";
                            getline(cin, input);
                            if (isPositiveDouble(input)) {
                                price = stod(input);
                                break;
                            }
                            cout << "Invalid input for price. Please enter a non-negative number." << endl;
                        }

                        Book updatedBook(newIsbn, title, author, quantity, price);
                        Book oldBook = node->data;
                        bookBST.remove(oldBook);
                        node->data = updatedBook;
                        bookBST.insert(updatedBook);
                        recentUpdates.push(updatedBook);
                        saveBooks(filename, bookList);
                        getMaxColumnWidths(bookList, isbnW, titleW, authorW); // recalc widths
                        cout << "✏️ Book updated!\n";
                    } else {
                        cout << "Update cancelled.\n";
                    }
                }
                else {
                    cout << "Book not found.\n";
                }
                cout << "Update another book? (y/n): ";
                while (true) {
                    getline(cin, input);
                    if (input.length() == 1 && (tolower(input[0]) == 'y' || tolower(input[0]) == 'n')) {
                        cont = input[0];
                        break;
                    }
                    cout << "Invalid input. Please enter 'y' or 'n': ";
                }
            } while (tolower(cont) == 'y');
            break;
        }
        case 4: { // Search
            char cont;
            do {
                int searchType;
                string input;
                bool valid = false;
                do {
                    cout << "\nSearch by:\n";
                    cout << " 1. ISBN\n";
                    cout << " 2. Title\n";
                    cout << " 3. Author\n";
                    cout << "Choose option: ";
                    getline(cin, input);
                    if (isPositiveInteger(input)) {
                        searchType = stoi(input);
                        if (searchType >= 1 && searchType <= 3)
                            valid = true;
                        else
                            cout << "Invalid option! Please enter 1, 2, or 3.\n";
                    }
                    else {
                        cout << "Invalid input! Please enter a number (1, 2, or 3).\n";
                    }
                } while (!valid);

                if (searchType == 1) {
                    string isbn;
                    cout << "Enter ISBN: "; getline(cin, isbn);
                    Book* found = bookBST.search(isbn);
                    if (found) {
                        printBookHeader(isbnW, titleW, authorW);
                        printBook(*found, isbnW, titleW, authorW);
                    }
                    else cout << "Book not found.\n";
                }
                else if (searchType == 2) {
                    string keyword;
                    cout << "Enter keyword to search in book title: ";
                    getline(cin, keyword);
                    keyword = toLower(keyword);
                    Node<Book>* node = bookList.getHead();
                    bool found = false;
                    printBookHeader(isbnW, titleW, authorW);
                    while (node) {
                        string titleLower = toLower(node->data.getTitle());
                        if (titleLower.find(keyword) != string::npos) {
                            printBook(node->data, isbnW, titleW, authorW);
                            found = true;
                        }
                        node = node->next;
                    }
                    if (!found) cout << "No books matched the keyword.\n";
                }
                else if (searchType == 3) {
                    string keyword;
                    cout << "Enter keyword to search in author name: ";
                    getline(cin, keyword);
                    keyword = toLower(keyword);
                    Node<Book>* node = bookList.getHead();
                    bool found = false;
                    printBookHeader(isbnW, titleW, authorW);
                    while (node) {
                        string authorLower = toLower(node->data.getAuthor());
                        if (authorLower.find(keyword) != string::npos) {
                            printBook(node->data, isbnW, titleW, authorW);
                            found = true;
                        }
                        node = node->next;
                    }
                    if (!found) cout << "No books matched the author keyword.\n";
                }

                cout << "Search another book? (y/n): ";
                while (true) {
                    getline(cin, input);
                    if (input.length() == 1 && (tolower(input[0]) == 'y' || tolower(input[0]) == 'n')) {
                        cont = input[0];
                        break;
                    }
                    cout << "Invalid input. Please enter 'y' or 'n': ";
                }
            } while (tolower(cont) == 'y');
            break;
        }
        case 5: // Display All
            cout << "\nAll Books in Inventory:\n";
            printBookHeader(isbnW, titleW, authorW);
            {
                Node<Book>* node = bookList.getHead();
                while (node) {
                    printBook(node->data, isbnW, titleW, authorW);
                    node = node->next;
                }
            }
            break;
        case 6: { // Sort
            char cont;
            do {
                int sortType;
                cout << "\nSort books by:\n";
                cout << " 1. Price\n";
                cout << " 2. Quantity\n";
                cout << " 3. Title\n";
                cout << " 4. ISBN\n";
                cout << " 5. Author\n";
                cout << "Choose option: ";
                while (true) {
                    getline(cin, input);
                    if (isPositiveInteger(input)) {
                        sortType = stoi(input);
                        if (sortType >= 1 && sortType <= 5) break;
                    }
                    cout << "Invalid sort option. Please enter a number from 1 to 5: ";
                }

                int count = 0;
                Node<Book>* node = bookList.getHead();
                Book tempArr[200];
                while (node && count < 200) {
                    tempArr[count++] = node->data;
                    node = node->next;
                }

                switch (sortType) {
                case 1:
                    bubbleSortByKey<Book, double>(tempArr, count, getPrice);
                    cout << "Books sorted by price (Bubble Sort):\n";
                    break;
                case 2:
                    selectionSortByKey<Book, int>(tempArr, count, getQuantity);
                    cout << "Books sorted by quantity (Selection Sort):\n";
                    break;
                case 3:
                    insertionSortByKey<Book, string>(tempArr, count, getTitle);
                    cout << "Books sorted by title (Insertion Sort):\n";
                    break;
                case 4:
                    bubbleSortByKey<Book, string>(tempArr, count, getISBN);
                    cout << "Books sorted by ISBN (Bubble Sort):\n";
                    break;
                case 5:
                    selectionSortByKey<Book, string>(tempArr, count, getAuthor);
                    cout << "Books sorted by author (Selection Sort):\n";
                    break;
                }
                printBookHeader(isbnW, titleW, authorW);
                for (int i = 0; i < count; ++i) printBook(tempArr[i], isbnW, titleW, authorW);

                cout << "Sort again? (y/n): ";
                while (true) {
                    getline(cin, input);
                    if (input.length() == 1 && (tolower(input[0]) == 'y' || tolower(input[0]) == 'n')) {
                        cont = input[0];
                        break;
                    }
                    cout << "Invalid input. Please enter 'y' or 'n': ";
                }
            } while (tolower(cont) == 'y');
            break;
        }
        case 7: { // Recently Updated (Stack)
            cout << "\nRecently Updated Books (Stack):\n";
            Stack<Book> tempStack = recentUpdates;
            Book b;
            printBookHeader(isbnW, titleW, authorW);
            while (!tempStack.isEmpty()) {
                tempStack.pop(b);
                printBook(b, isbnW, titleW, authorW);
            }
            break;
        }
        case 8: { // Queue Restock Request
            char cont;
            do {
                // Display all books first
                cout << "\nAll Books in Inventory:\n";
                printBookHeader(isbnW, titleW, authorW);
                Node<Book>* node = bookList.getHead();
                while (node) {
                    printBook(node->data, isbnW, titleW, authorW);
                    node = node->next;
                }

                string isbn;
                cout << "\nEnter ISBN of book to restock: ";
                getline(cin, isbn);

                node = bookList.getHead();
                while (node && toLower(node->data.getISBN()) != toLower(isbn)) node = node->next;
                if (node) {
                    int restockQty;
                    // Input validation for restock quantity
                    while (true) {
                        cout << "Enter quantity to restock: ";
                        getline(cin, input);
                        if (isPositiveInteger(input)) {
                            restockQty = stoi(input);
                            break;
                        }
                        cout << "Invalid input for quantity. Please enter a non-negative integer." << endl;
                    }
                    RestockRequest req{node->data, restockQty};
                    restockQueue.enqueue(req);
                    cout << "Restock request queued.\n";
                }
                else {
                    cout << "Book not found.\n";
                }
                cout << "Queue another restock? (y/n): ";
                while (true) {
                    getline(cin, input);
                    if (input.length() == 1 && (tolower(input[0]) == 'y' || tolower(input[0]) == 'n')) {
                        cont = input[0];
                        break;
                    }
                    cout << "Invalid input. Please enter 'y' or 'n': ";
                }
            } while (tolower(cont) == 'y');
            break;
        }
        case 9: { // Process Restock Request (Admin)
            // Show all queue first
            cout << "\nRestock Requests in Queue:\n";
            Queue<RestockRequest> tempQueue = restockQueue;
            RestockRequest req;
            int idx = 1;
            if (tempQueue.isEmpty()) {
                cout << "No restock requests in queue.\n";
                break;
            }
            printBookHeader(isbnW, titleW, authorW);
            while (!tempQueue.isEmpty()) {
                tempQueue.dequeue(req);
                cout << idx++ << ". ";
                printBook(req.book, isbnW, titleW, authorW);
                cout << "   Restock Qty: " << req.quantity << endl;
            }

            // Admin authentication
            string adminPass;
            cout << "\nEnter admin password to process restock: ";
            getline(cin, adminPass);
            if (adminPass != "admin123") { // Change password as needed
                cout << "Invalid admin password. Access denied.\n";
                break;
            }

            char cont;
            do {
                if (restockQueue.isEmpty()) {
                    cout << "No restock requests in queue.\n";
                    break;
                }
                cout << "Do you want to process the next restock request? (y/n): ";
                while (true) {
                    getline(cin, input);
                    if (input.length() == 1 && (tolower(input[0]) == 'y' || tolower(input[0]) == 'n')) {
                        cont = input[0];
                        break;
                    }
                    cout << "Invalid input. Please enter 'y' or 'n': ";
                }
                if (tolower(cont) == 'y') {
                    if (restockQueue.dequeue(req)) {
                        // Update book quantity
                        Node<Book>* node = bookList.getHead();
                        while (node && node->data.getISBN() != req.book.getISBN()) node = node->next;
                        if (node) {
                            int newQty = node->data.getQuantity() + req.quantity;
                            node->data.setQuantity(newQty);
                            saveBooks(filename, bookList);
                            getMaxColumnWidths(bookList, isbnW, titleW, authorW); // recalc widths
                            cout << "\nProcessed restock for book:\n";
                            printBookHeader(isbnW, titleW, authorW);
                            printBook(node->data, isbnW, titleW, authorW);
                            cout << "   Restocked Qty: " << req.quantity << endl;
                        } else {
                            cout << "Book not found in inventory (may have been deleted).\n";
                        }
                    }
                    // Show updated queue
                    cout << "\nRestock Requests Remaining in Queue:\n";
                    Queue<RestockRequest> tempQueue2 = restockQueue;
                    int idx2 = 1;
                    if (tempQueue2.isEmpty()) {
                        cout << "No restock requests in queue.\n";
                    } else {
                        printBookHeader(isbnW, titleW, authorW);
                        while (!tempQueue2.isEmpty()) {
                            tempQueue2.dequeue(req);
                            cout << idx2++ << ". ";
                            printBook(req.book, isbnW, titleW, authorW);
                            cout << "   Restock Qty: " << req.quantity << endl;
                        }
                    }
                }
            } while (tolower(cont) == 'y' && !restockQueue.isEmpty());
            break;
        }
        case 0:
            cout << "Exiting...\n";
            break;
        }
    } while (choice != 0);

    return 0;
}