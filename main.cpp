#include "order_system.h"
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

int main()
{
    int money = 0;
    int priceSum = 0;

    printOrderItems();

    string welcomeMeeting = "\nWelcome to Ardee's Canteen. Press 'Y' to get started. ";
    string orderingAgain = "\nPress 'Y' to continue: ";

    orderSystem(welcomeMeeting);

    checkCart(&priceSum);

    char decision;
    cout << "\nWould you like to checkout? (Y/N) ";
    cin >> decision;

    if (toupper(decision) != 'N')
    {
        checkout(&money, &priceSum);
        printReceipt(&money);
        return 0;
    }

    char orderPrompt = 'Y';
    while (toupper(orderPrompt) != 'N')
    {
        cout << "\n(1) Delete Item"
             << "\n(2) Update Item"
             << "\n(3) Order Again" << '\n';
        int option;
        cin >> option;

        switch (option)
        {
        case 1:
            deleteItem();
            break;
        case 2:
            updateItem();
            break;
        case 3:
            printOrderItems();
            orderSystem(orderingAgain);
            break;
        default:
            cout << "Invalid option! Select again";
            continue;
        }

        checkCart(&priceSum);

        cout << "Do you want to modify your existing orders? (Y/N) ";
        cin >> orderPrompt;
    }

    if (askCheckout())
    {
        checkout(&money, &priceSum);
        printReceipt(&money);
    }

    return 0;
}

