using namespace std;
#include "Frontend.h"
#include "Oper.h"




int main()
{
    vector<stProduct> stock = {
        { 1, "Clavier", 150.0f, 10 },
        { 2, "Souris", 80.0f, 25 },
        { 3, "Ecran", 1200.0f, 5 }
    };

    menu(stock);

    return 0;
}