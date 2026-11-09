
#include "../include/Library.h"

#include <iomanip>
#include <iostream>

bool Library::addBook(const Book& bk) {
    if (findBook(bk.getId()) != nullptr) {
        return false;
    }

    Books.push_back(bk);
    return true;
}

void Library::displayBooks() const {
    if (Books.empty()) {
        std::cout << "The library is empty.\n";
        return;
    }

    std::cout << '\n' << std::left
              << std::setw(10) << "ID"
              << std::setw(50) << "TITLE"
              << std::setw(25) << "AUTHOR"
              << std::setw(15) << "STATUS"
              << '\n';

    for (const Book& book : Books) {
        book.displayBook();
    }
}

Book* Library::findBook(int id) {
    for (Book& book : Books) {
        if (book.getId() == id) {
            return &book;
        }
    }

    return nullptr;
}

const Book* Library::findBook(int id) const {
    for (const Book& book : Books) {
        if (book.getId() == id) {
            return &book;
        }
    }

    return nullptr;
}

void Library::BorrowBook(int id) {
    Book* bk = findBook(id);

    if (bk == nullptr) {
        std::cout << "Book not found.\n";
    }
    else if (bk->borrowBook()) {
        std::cout << "Book has been borrowed successfully!\n";
    }
    else {
        std::cout << "Book is not available at the moment.\n";
    }
}

void Library::returnBook(int id) {
    Book* bk = findBook(id);

    if (bk == nullptr) {
        std::cout << "Book not found.\n";
    }
    else if (bk->returnBook()) {
        std::cout << "Book has been returned successfully!\n";
    }
    else {
        std::cout << "Error: Book hasn't been borrowed.\n";
    }
}

void Library::removeBook(int id) {
    for (auto it = Books.begin(); it != Books.end(); ++it) {
        if (it->getId() == id) {
            Books.erase(it);
            std::cout << "Book removed successfully!\n";
            return;
        }
    }

    std::cout << "Book not found.\n";
}

std::size_t Library::getBookCount() const {
    return Books.size();
}
