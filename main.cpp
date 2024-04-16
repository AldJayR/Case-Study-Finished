#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "order_system.h"

using namespace std;

int main()
{
    // Initialize variables
    int money = 0;           // Variable to store the payment amount
    int priceSum = 0;        // Variable to store the total price of the order
    bool isDine = false;     // Flag to indicate if the order is for dine-in or take-out
    bool newOrder = false;   // Flag to indicate if it's a new order

    // Print the menu of available items
    printOrderItems();

    // Welcome message
    string welcomeMeeting = "\nWelcome to Ardee's Canteen. Press 'Y' to get started. ";

    // Message for ordering again
    string orderingAgain = "\nPress 'Y' to continue: ";

    // Start the order system with the welcome message
    orderSystem(welcomeMeeting, isDine, newOrder);

    // Display the current cart and total price
    checkCart(&priceSum);

    char decision;
    cout << "\nWould you like to checkout? (Y/N) ";
    cin >> decision;

    // If the user wants to checkout
    if (toupper(decision) != 'N')
    {
        // Handle checkout process
        checkout(&money, &priceSum);
        printReceipt(&money, isDine);
        cout << "Thank you for coming to Ardee's. Please come again" << '\n';
        cout << "\nPress Enter to exit...";
        cin.ignore();
        cin.get();
        return 0; // Exit the program
    }

    char orderPrompt = 'Y';
    bool modify = false;

    // Loop to allow order modification
    while (toupper(orderPrompt) != 'N')
    {
        cout << "Do you want to modify your existing orders? (Y/N) ";
        cin >> orderPrompt;

        // If the user wants to modify the order
        if (toupper(orderPrompt) != 'N')
        {
            modify = true;
        }
        else
        {
            break; // Exit the loop if the user doesn't want to modify the order
        }

        // If modification is requested
        if (modify)
        {
            // Print options for modification
            cout << "\n(1) Delete Item"
                 << "\n(2) Update Item"
                 << "\n(3) Order Again"
                 << "\n(4) Go Back" << '\n';

            int option;
            cin >> option;

            // Handle the selected option
            switch (option)
            {
            case 1:
                deleteItem(); // Delete an existing order
                break;
            case 2:
                updateItem(); // Update the quantity of an existing order
                break;
            case 3:
                newOrder = true;     // Set the flag to indicate it's a new order
                printOrderItems();   // Print the menu of available items
                orderSystem(orderingAgain, isDine, newOrder); // Start the order system with the "ordering again" message
                break;
            case 4:
                checkCart(&priceSum);
                orderPrompt = 'N';
                break;
            default:
                cout << "Invalid option! Select again"; // Print an error message for invalid input
                continue;                               // Continue the loop to allow the user to select again
            }

            // Display the updated cart and total price
            if (option != 4)
            {
                checkCart(&priceSum);
                continue; // Continue the loop to allow further modifications
            }

        }
    }

    // If the user wants to checkout after modifying the order
    if (askCheckout())
    {
        checkout(&money, &priceSum);
        printReceipt(&money, isDine);
        cout << "Thank you for coming to Ardee's. Please come again" << '\n';
        cout << "\nPress Enter to exit...";
        cin.ignore();
        cin.get();
    }

    return 0; // Exit the program
}
