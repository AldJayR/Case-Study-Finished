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
    bool isDine = false;
    bool newOrder = false;

    printOrderItems();

    string welcomeMeeting = "\nWelcome to Ardee's Canteen. Press 'Y' to get started. ";
    string orderingAgain = "\nPress 'Y' to continue: ";

    orderSystem(welcomeMeeting, isDine, newOrder);

    checkCart(&priceSum);

    char decision;
    cout << "\nWould you like to checkout? (Y/N) ";
    cin >> decision;

    if (toupper(decision) != 'N')
    {
        checkout(&money, &priceSum);
        printReceipt(&money, isDine);
        return 0;
    }

    char orderPrompt = 'Y';
    bool modify = false;

    while (toupper(orderPrompt) != 'N')
    {
        cout << "Do you want to modify your existing orders? (Y/N) ";
        cin >> orderPrompt;
        if (toupper(orderPrompt) != 'N')
        {
            modify = true;
        }

        if (modify)
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
                    newOrder = true;
                    printOrderItems();
                    orderSystem(orderingAgain, isDine, newOrder);
                    break;
                default:
                    cout << "Invalid option! Select again";
                    continue;
            }

            checkCart(&priceSum);

            cout << "Do you want to modify your existing orders? (Y/N) ";
            cin >> orderPrompt;
        }
    }

    if (askCheckout())
    {
        checkout(&money, &priceSum);
        printReceipt(&money, isDine);
    }

    return 0;
}
