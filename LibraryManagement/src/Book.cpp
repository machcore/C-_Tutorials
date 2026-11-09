
#include "../include/Book.h"

#include <iomanip>
#include <iostream>
#include <utility>

Book::Book(int id, std::string title, std::string author)
    : id(id), title(std::move(title)),
      author(std::move(author)), available(true) {
}

void Book::displayBook() const {
    std::cout << std::left
              << std::setw(10) << id
              << std::setw(50) << title
              << std::setw(25) << author
              << std::setw(15)
              << (available ? "Available" : "Not Available")
              << '\n';
}

bool Book::borrowBook() {
    if (!available) {
        return false;
    }

    available = false;
    return true;
}

bool Book::returnBook() {
    if (available) {
        return false;
    }

    available = true;
    return true;
}

int Book::getId() const {
    return id;
}

bool Book::isAvailable() const {
    return available;
}
