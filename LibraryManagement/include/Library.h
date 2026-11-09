
#pragma once

#include <vector>
#include "Book.h"

class Library {
private:
    std::vector<Book> Books;

public:
    bool addBook(const Book& bk);
    void displayBooks() const;

    Book* findBook(int id);
    const Book* findBook(int id) const;

    void BorrowBook(int id);
    void returnBook(int id);
    void removeBook(int id);

    std::size_t getBookCount() const;
};
