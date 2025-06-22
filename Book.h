#pragma once
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

class Book {
private:
    string isbn;
    string title;
    string author;
    int quantity;
    double price;

public:
    Book() : isbn(""), title(""), author(""), quantity(0), price(0.0) {}
    Book(string isbn, string title, string author, int quantity, double price)
        : isbn(isbn), title(title), author(author), quantity(quantity), price(price) {}

    // Getters
    string getISBN() const { return isbn; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }

    // Setters
    void setISBN(const string& val) { isbn = val; }
    void setTitle(const string& val) { title = val; }
    void setAuthor(const string& val) { author = val; }
    void setQuantity(int val) { quantity = val; }
    void setPrice(double val) { price = val; }

    bool operator==(const Book& other) const {
        return isbn == other.isbn;
    }

    inline friend ostream& operator<<(ostream& os, const Book& b) {
        os << "[" << b.getISBN() << "] " << b.getTitle() << " by " << b.getAuthor()
           << " | Qty: " << b.getQuantity() << " | $" << b.getPrice();
        return os;
    }

    string toFileString() const {
        return isbn + ";" + title + ";" + author + ";" + to_string(quantity) + ";" + to_string(price);
    }

    static Book fromFileString(const string& line) {
        stringstream ss(line);
        string isbn, title, author, quantityStr, priceStr;
        getline(ss, isbn, ';');
        getline(ss, title, ';');
        getline(ss, author, ';');
        getline(ss, quantityStr, ';');
        getline(ss, priceStr, ';');
        return Book(isbn, title, author, stoi(quantityStr), stod(priceStr));
    }
};