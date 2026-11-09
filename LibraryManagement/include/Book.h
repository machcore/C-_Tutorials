
#pragma once

#include <string>

class Book {
private:
    int id;
    std::string title;
    std::string author;
    bool available;

public:
    Book(int id, std::string title, std::string author);

    void displayBook() const;
    bool borrowBook();
    bool returnBook();

    int getId() const;
    bool isAvailable() const;
};
