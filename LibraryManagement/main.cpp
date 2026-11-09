
#include <iostream>
#include <cstdlib>
#include <limits>

#include "./include/Library.h"

using namespace std;

int readNumber(const std::string& prompt) {
    int value;

    while (true) {
        std::cout << prompt;

        if (std::cin >> value) {
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );
            return value;
        }

        if (std::cin.eof()) {
            return -1;
        }

        std::cout << "Please enter a valid number.\n";

        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );
    }
}

int readId() {
    return readNumber("Enter book ID: ");
}

int main() {
    Library library;
    int choice;

    while (true) {
        system("clear");

        cout << "========== LIBRARY SYSTEM ==========\n";
        cout << "1. Add a book\n";
        cout << "2. Display all books\n";
        cout << "3. Search for a book\n";
        cout << "4. Borrow a book\n";
        cout << "5. Return a book\n";
        cout << "6. Remove a book\n";
        cout << "7. Show book count\n";
        cout << "0. Exit\n";

        choice = readNumber("Your choice: ");

        if (cin.eof()) break;

        system("clear");

        switch (choice) {
            case 1: {
                int id = readId();
                if (cin.eof()) return 0;

                if (id <= 0) {
                    cout << "Book IDs must be positive.\n";
                    break;
                }

                string title, author;

                cout << "Enter title: ";
                if (!getline(cin, title)) return 0;

                cout << "Enter author: ";
                if (!getline(cin, author)) return 0;

                if (title.empty() || author.empty()) {
                    cout << "Title and author cannot be empty.\n";
                    break;
                }

                Book book(id, title, author);

                if (library.addBook(book))
                    cout << "Book added successfully!\n";
                else
                    cout << "Book ID already exists!\n";

                break;
            }

            case 2:
                library.displayBooks();
                break;

            case 3: {
                int id = readId();
                if (cin.eof()) return 0;

                Book* book = library.findBook(id);

                if (book != nullptr)
                    book->displayBook();
                else
                    cout << "Book not found.\n";

                break;
            }

            case 4: {
                int id = readId();
                if (cin.eof()) return 0;
                library.BorrowBook(id);
                break;
            }

            case 5: {
                int id = readId();
                if (cin.eof()) return 0;
                library.returnBook(id);
                break;
            }

            case 6: {
                int id = readId();
                if (cin.eof()) return 0;
                library.removeBook(id);
                break;
            }

            case 7:
                cout << "Total books: "
                     << library.getBookCount() << '\n';
                break;

            case 0:
                cout << "Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice!\n";
        }

        // Wait before clearing the screen
        cout << "\nPress Enter to continue...";
        cin.get();
    }

    return 0;
}
