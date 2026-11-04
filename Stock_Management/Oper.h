#pragma once
using namespace std;
#include <string>
#include <iostream>
#include <vector>
#include <limits>

struct stProduct {
    int id;
    string name;
    float price;
    int qte;
};

bool prdExist(vector<stProduct>* stock, int id) {
    int len = stock->size();
    for (int i = 0; i < len; i++) {
        if ((*stock)[i].id == id) {
            return true;
        }
    }
    return false;
}

stProduct searchByID(const vector<stProduct>& stock, int id) {
    int len = stock.size();
    for (int i = 0; i < len; i++) {
        if (stock[i].id == id) {
            return stock[i];
        }
    }
}

void updateQte(vector<stProduct>& stock, int id, int updateQte) {
    int len = stock.size();
    for (int i = 0; i < len; i++) {
        if (stock[i].id == id) {
            stock[i].qte = updateQte;
        }
    }
    cout << "Product Doesn't Exist";
}

stProduct readProduct(vector<stProduct>* stock) {
    stProduct prd;
    do {
        cout << "Enter a unique Id : ";
        cin >> prd.id;
        if (prdExist(stock, prd.id)) {
            cout << "Id already Exist, Try Again\n";
        }
    } while (prdExist(stock, prd.id));
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Name: ";
    getline(cin, prd.name);

    cout << "Enter Price: ";
    cin >> prd.price;

    cout << "Enter Quantity: ";
    cin >> prd.qte;

    return prd;

}

void ajouterPrd(vector<stProduct>* stock, stProduct prd) {
    stock->push_back(prd);
}

void supprimerPrd(vector<stProduct>& stock, int id) {
    int len = stock.size();
    for (int i = 0; i < len; i++) {
        if (stock[i].id == id) {
            stock.erase(stock.begin() + i);
            cout << "Product has been deleted successfully\n";
            return;
        }
    }

    cout << "Product does not exist\n";
}



