#pragma once 



#include <iostream> 

#include <string> 

#include <sstream> 

using namespace std;



struct Book {

    string isbn;

    string title;

    string author;

    int quantity;

    double price;



    Book() : isbn(""), title(""), author(""), quantity(0), price(0.0) {}

    Book(string isbn, string title, string author, int quantity, double price)

        : isbn(isbn), title(title), author(author), quantity(quantity), price(price) {}



    bool operator==(const Book& other) const {

        return isbn == other.isbn;

    }



    friend ostream& operator<<(ostream& os, const Book& b) {

        os << "[" << b.isbn << "] " << b.title << " by " << b.author

            << " | Qty: " << b.quantity << " | $" << b.price;

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