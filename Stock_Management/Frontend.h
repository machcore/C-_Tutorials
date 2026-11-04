#pragma once
#include "Oper.h"
#include <iomanip>

void AfficherAllPrd(const vector<stProduct>& stock) {
	cout << endl << right << setw(30) << "List de Produits" << endl;
	cout << string(45, '-') << endl;
	cout << left
		<< setw(8) << "ID"
		<< setw(24) << "Nom"
		<< setw(8) << "Prix"
		<< setw(10) << "qte" << endl;;
	cout << string(45, '-') << endl;
	for (const auto& prd : stock)
	cout << left
		<< setw(8) << prd.id
		<< setw(24) << prd.name
		<< setw(8) << prd.price
		<< setw(10) << prd.qte << endl;;

}

void afficherPrd(vector<stProduct>& stock) {
	int id;
	stProduct prd;
	do {
		cout << "Enter Product Id : ";
		cin >> id;
		if (!prdExist(&stock, id)) {
			cout << "Invalid Id : Product doesn't exist\n";
		}
	} while (!prdExist(&stock, id));

	prd = searchByID(stock, id);

	cout << endl << right << setw(28) << "Product Page" << endl;
	cout << string(45, '-') << endl << endl;
	cout << left
		<< setw(10) << "ID " << ": " << prd.id << endl
		<< setw(10) << "Name " << ": " << prd.name << endl
		<< setw(10) << "Price" << ": " << prd.price << endl
		<< setw(10) << "Qte" << ": " << prd.qte << endl;

}

void ModifierQte(vector<stProduct>* stock) {
	int id, updQte;
	do {
		cout << "Enter Product Id : ";
		cin >> id;
		if (!prdExist(stock, id)) {
			cout << "Invalid Id : Product doesn't exist\n";
		}
	} while (!prdExist(stock, id));

	cout << "Enter New Quantity : ";
	cin >> updQte;

	updateQte(*stock, id, updQte);

}

void deletePrd(vector<stProduct>* stock) {
	int id;
	do {
		cout << "Enter Product Id : ";
		cin >> id;
		if (!prdExist(stock, id)) {
			cout << "Invalid Id : Product doesn't exist\n";
		}
	} while (!prdExist(stock, id));
	supprimerPrd(*stock, id);
}

void menu(vector<stProduct>& stock) {
	int choix;

	do {
		cout << "\n========== GESTION DE STOCK ==========\n";
		cout << "1. Ajouter un produit\n";
		cout << "2. Afficher tous les produits\n";
		cout << "3. Rechercher un produit\n";
		cout << "4. Modifier la quantite\n";
		cout << "5. Supprimer un produit\n";
		cout << "6. Quitter\n";

		cout << "Votre choix : ";

		if (!(cin >> choix)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Choix invalide !\n";
			continue;
		}

		switch (choix) {
		case 1:
			ajouterPrd(&stock, readProduct(&stock));
			break;

		case 2:
			AfficherAllPrd(stock);
			break;

		case 3:
			afficherPrd(stock);
			break;

		case 4:
			ModifierQte(&stock);
			break;

		case 5:
			deletePrd(&stock);
			break;

		case 6:
			cout << "Au revoir !\n";
			break;

		default:
			cout << "Choix invalide !\n";
		}

	} while (choix != 6);
}


