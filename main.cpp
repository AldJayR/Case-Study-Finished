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
    bool cartFlag = true;
    bool classStatus = false;

    // Print the menu of available items
    printOrderItems();

    // Welcome message
    string welcomeMeeting = "\nWelcome to Ardee's. Press anything to get started. ";

    // Start the order system with the welcome message
    orderSystem(welcomeMeeting, isDine, newOrder, classStatus);

    // Display the current cart and total price
    checkCart(priceSum, cartFlag);

    char decision;
    cout << "\nWould you like to checkout? (Y/N) ";
    cin >> decision;
    cin.ignore(100, '\n');

    // If the user wants to checkout
    if (toupper(decision) != 'N')
    {
        // Handle checkout process
        directCheckout(money, priceSum, isDine, newOrder, cartFlag, classStatus);
        return 0; // Exit the program
    }

    handleOrderModification(priceSum, isDine, newOrder, cartFlag, classStatus);

    // If the user wants to checkout after modifying the order
    if (askCheckout())
    {
       directCheckout(money, priceSum, isDine, newOrder, cartFlag, classStatus);
    }

    return 0;
}


