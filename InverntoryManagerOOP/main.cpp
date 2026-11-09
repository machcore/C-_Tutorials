// Inventory Manager - simple OOP C++ console app with SQLite
// Build: g++ -std=c++17 main.cpp -lsqlite3 -o inventory

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>
#include <stdexcept>
#include <sqlite3.h>

using namespace std;

// ---------------------------------------------------------------
// Product: represents one row of the products table
// ---------------------------------------------------------------
class Product {
private:
    int id;
    string name;
    int quantity;
    double price;

public:
    Product(int id, const string& name, int quantity, double price)
        : id(id), name(name), quantity(quantity), price(price) {}

    int getId() const { return id; }
    const string& getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }

    void setName(const string& n) { if (!n.empty()) name = n; }
    void setQuantity(int q) { if (q >= 0) quantity = q; }
    void setPrice(double p) { if (p >= 0) price = p; }

    double totalValue() const { return quantity * price; }
};

// ---------------------------------------------------------------
// Database: wraps SQLite (RAII) and does all the SQL work
// ---------------------------------------------------------------
class Database {
private:
    sqlite3* db = nullptr;

    // Small RAII wrapper so statements are always finalized
    struct Stmt {
        sqlite3_stmt* s = nullptr;
        Stmt(sqlite3* db, const string& sql) {
            if (sqlite3_prepare_v2(db, sql.c_str(), -1, &s, nullptr) != SQLITE_OK)
                throw runtime_error(sqlite3_errmsg(db));
        }
        ~Stmt() { sqlite3_finalize(s); }
        Stmt(const Stmt&) = delete;
        Stmt& operator=(const Stmt&) = delete;
    };

    static Product rowToProduct(sqlite3_stmt* s) {
        const unsigned char* text = sqlite3_column_text(s, 1);
        return Product(
            sqlite3_column_int(s, 0),
            text ? reinterpret_cast<const char*>(text) : "",
            sqlite3_column_int(s, 2),
            sqlite3_column_double(s, 3));
    }

    void exec(const string& sql) {
        char* err = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
            string msg = err ? err : "unknown error";
            sqlite3_free(err);
            throw runtime_error(msg);
        }
    }

public:
    explicit Database(const string& path) {
        if (sqlite3_open(path.c_str(), &db) != SQLITE_OK) {
            string msg = sqlite3_errmsg(db);
            sqlite3_close(db);
            throw runtime_error(msg);
        }
        exec("CREATE TABLE IF NOT EXISTS products ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT,"
             "name TEXT NOT NULL,"
             "quantity INTEGER NOT NULL CHECK(quantity >= 0),"
             "price REAL NOT NULL CHECK(price >= 0));");
    }

    ~Database() { sqlite3_close(db); }
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    int addProduct(const string& name, int quantity, double price) {
        Stmt st(db, "INSERT INTO products(name, quantity, price) VALUES(?,?,?);");
        sqlite3_bind_text(st.s, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(st.s, 2, quantity);
        sqlite3_bind_double(st.s, 3, price);
        if (sqlite3_step(st.s) != SQLITE_DONE)
            throw runtime_error(sqlite3_errmsg(db));
        return static_cast<int>(sqlite3_last_insert_rowid(db));
    }

    vector<Product> getAll() {
        vector<Product> result;
        Stmt st(db, "SELECT id, name, quantity, price FROM products ORDER BY id;");
        while (sqlite3_step(st.s) == SQLITE_ROW)
            result.push_back(rowToProduct(st.s));
        return result;
    }

    vector<Product> search(const string& term) {
        vector<Product> result;
        Stmt st(db, "SELECT id, name, quantity, price FROM products "
                    "WHERE name LIKE ? ORDER BY id;");
        string pattern = "%" + term + "%";
        sqlite3_bind_text(st.s, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);
        while (sqlite3_step(st.s) == SQLITE_ROW)
            result.push_back(rowToProduct(st.s));
        return result;
    }

    vector<Product> lowStock(int threshold) {
        vector<Product> result;
        Stmt st(db, "SELECT id, name, quantity, price FROM products "
                    "WHERE quantity <= ? ORDER BY quantity;");
        sqlite3_bind_int(st.s, 1, threshold);
        while (sqlite3_step(st.s) == SQLITE_ROW)
            result.push_back(rowToProduct(st.s));
        return result;
    }

    bool getById(int id, Product& out) {
        Stmt st(db, "SELECT id, name, quantity, price FROM products WHERE id = ?;");
        sqlite3_bind_int(st.s, 1, id);
        if (sqlite3_step(st.s) == SQLITE_ROW) {
            out = rowToProduct(st.s);
            return true;
        }
        return false;
    }

    bool update(const Product& p) {
        Stmt st(db, "UPDATE products SET name=?, quantity=?, price=? WHERE id=?;");
        sqlite3_bind_text(st.s, 1, p.getName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(st.s, 2, p.getQuantity());
        sqlite3_bind_double(st.s, 3, p.getPrice());
        sqlite3_bind_int(st.s, 4, p.getId());
        if (sqlite3_step(st.s) != SQLITE_DONE)
            throw runtime_error(sqlite3_errmsg(db));
        return sqlite3_changes(db) > 0;
    }

    bool remove(int id) {
        Stmt st(db, "DELETE FROM products WHERE id = ?;");
        sqlite3_bind_int(st.s, 1, id);
        if (sqlite3_step(st.s) != SQLITE_DONE)
            throw runtime_error(sqlite3_errmsg(db));
        return sqlite3_changes(db) > 0;
    }
};

// ---------------------------------------------------------------
// InventoryManager: the console UI / application logic
// ---------------------------------------------------------------
class InventoryManager {
private:
    Database db;

    // ---- input helpers ----
    static string readLine(const string& prompt) {
        cout << prompt;
        string s;
        getline(cin, s);
        return s;
    }

    static int readInt(const string& prompt) {
        while (true) {
            cout << prompt;
            string s;
            getline(cin, s);
            try {
                size_t pos;
                int v = stoi(s, &pos);
                if (pos == s.size()) return v;
            } catch (...) {}
            cout << "  Invalid number, try again.\n";
        }
    }

    static double readDouble(const string& prompt) {
        while (true) {
            cout << prompt;
            string s;
            getline(cin, s);
            try {
                size_t pos;
                double v = stod(s, &pos);
                if (pos == s.size()) return v;
            } catch (...) {}
            cout << "  Invalid number, try again.\n";
        }
    }

    // ---- display helpers ----
    static void printTable(const vector<Product>& items) {
        if (items.empty()) {
            cout << "  (no products)\n";
            return;
        }
        cout << left << setw(5) << "ID" << setw(24) << "Name"
             << right << setw(8) << "Qty" << setw(12) << "Price"
             << setw(14) << "Total" << "\n";
        cout << string(63, '-') << "\n";
        double grand = 0;
        cout << fixed << setprecision(2);
        for (const auto& p : items) {
            cout << left << setw(5) << p.getId() << setw(24) << p.getName().substr(0, 22)
                 << right << setw(8) << p.getQuantity() << setw(12) << p.getPrice()
                 << setw(14) << p.totalValue() << "\n";
            grand += p.totalValue();
        }
        cout << string(63, '-') << "\n";
        cout << right << setw(49) << "Inventory value: " << setw(14) << grand << "\n";
    }

    // ---- actions ----
    void addProduct() {
        string name = readLine("Name: ");
        if (name.empty()) { cout << "  Name cannot be empty.\n"; return; }
        int qty = readInt("Quantity: ");
        double price = readDouble("Price: ");
        if (qty < 0 || price < 0) { cout << "  Values cannot be negative.\n"; return; }
        int id = db.addProduct(name, qty, price);
        cout << "  Added with ID " << id << ".\n";
    }

    void listProducts() { printTable(db.getAll()); }

    void searchProducts() {
        string term = readLine("Search name: ");
        printTable(db.search(term));
    }

    void updateProduct() {
        int id = readInt("Product ID to update: ");
        Product p(0, "", 0, 0);
        if (!db.getById(id, p)) { cout << "  Product not found.\n"; return; }

        cout << "  Current: " << p.getName() << " | qty " << p.getQuantity()
             << " | price " << p.getPrice() << "\n";

        string name = readLine("New name (Enter to keep): ");
        p.setName(name);  // ignored if empty
        p.setQuantity(readInt("New quantity: "));
        p.setPrice(readDouble("New price: "));

        cout << (db.update(p) ? "  Updated.\n" : "  Nothing updated.\n");
    }

    void deleteProduct() {
        int id = readInt("Product ID to delete: ");
        cout << (db.remove(id) ? "  Deleted.\n" : "  Product not found.\n");
    }

    void lowStockReport() {
        int threshold = readInt("Show products with quantity <= ");
        printTable(db.lowStock(threshold));
    }

    static void printMenu() {
        cout << "\n===== Inventory Manager =====\n"
             << "1. Add product\n"
             << "2. List products\n"
             << "3. Search by name\n"
             << "4. Update product\n"
             << "5. Delete product\n"
             << "6. Low stock report\n"
             << "0. Exit\n";
    }

public:
    explicit InventoryManager(const string& dbPath) : db(dbPath) {}

    void run() {
        while (true) {
            printMenu();
            int choice = readInt("Choice: ");
            try {
                switch (choice) {
                    case 1: addProduct(); break;
                    case 2: listProducts(); break;
                    case 3: searchProducts(); break;
                    case 4: updateProduct(); break;
                    case 5: deleteProduct(); break;
                    case 6: lowStockReport(); break;
                    case 0: cout << "Goodbye!\n"; return;
                    default: cout << "  Unknown option.\n";
                }
            } catch (const exception& e) {
                cout << "  Error: " << e.what() << "\n";
            }
        }
    }
};

// ---------------------------------------------------------------
int main() {
    try {
        InventoryManager app("inventory.db");
        app.run();
    } catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}