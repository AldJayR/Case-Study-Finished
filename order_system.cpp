#include <iostream>
#include <string>
// headers for the date and time
#include <ctime>
#include <iomanip>
#include <sstream>
#include "order_system.h"

// Global variables initialization

constexpr int g_MAX_ORDERS = 10;    // Maximum number of orders allowed
string orderCart[g_MAX_ORDERS];     // Array to store ordered items
int g_orderIndex = 0;               // Index for the current order
int price[g_MAX_ORDERS];            // Array to store prices of ordered items
const string itemNames[] = {"Fries", "Coke", "Chicken with Rice", "Ice Cream", "Menudo", "Burger", "Pizza", "Salad", "Spaghetti", "Sushi"}; // Array of item names
constexpr int itemPrices[] = {40, 25, 70, 25, 50, 60, 100, 45, 80, 120}; // Array of item prices
int quantityAmount[g_MAX_ORDERS] = {};  // Array to store quantity of each ordered item
bool isCheckout = false;

using namespace std;

// Print out hardcoded GUI for Menu

void printOrderItems()
{
    // Displaying the menu GUI
    cout << "+------------------------------------------------------------------------------------------+" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "|                                   ARDEE'S CANTEEN                                        |" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "+------------------------------------------------------------------------------------------+"  << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "|                                   PICK YOUR ORDER                                        |" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "|  1. Fries - P40                                  6. Burger - P60                         |" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "|  2. Coke - P25                                   7. Pizza - P100                         |" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "|  3. Chicken with Rice - P70                      8. Salad - P45                          |" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "|  4. Ice Cream - P25                              9. Spaghetti - P80                      |" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "|  5. Menudo - P50                                 10. Sushi - P120                        |" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "+------------------------------------------------------------------------------------------+" << '\n';
}

// Order prompt system

void orderSystem(string orderMessage, bool &dine, bool &newOrder)
{
    char prompt;
    cout << orderMessage;
    cin >> prompt;

    // Asks the user for their order

    while (toupper(prompt) != 'N')
    {
        char order;
        if (!newOrder && g_orderIndex == 0)
        {
            cout << "\nDine in or Take Out (D/T) ";
            cin >> order;
        }


        if (toupper(order) != 'T')
        {
            dine = true;
        }

        cout << "\nWhat would you like to order? (1-10) ";
        int item;
        cin >> item;
        int itemAmount;

        // Error handling

        while (cin.fail() || item < 1 || item > 10)
        {
            cout << "Invalid input! Please enter a number between 1 and 10.\n";
            cin.clear();
            cin.ignore(100, '\n');
            cout << "\n\nWhat would you like to order? (1-10) ";
            cin >> item;
        }

        int totalQuantity = 0;
        for (int i = 0; i < g_orderIndex; ++i)
        {
            if (orderCart[i] == itemNames[item - 1])
            {
                totalQuantity += quantityAmount[i];
            }
        }

        if (totalQuantity >= 10)
        {
            cout << "You cannot order more than ten items of this type!" << '\n';
            continue;
        }

        // Successful order

        cout << "You have ordered " << itemNames[item - 1] << '\n';


        // If customer ordered more than 10 items

        do
        {
            cout << "How much would you like? (cannot exceed more than 10 items) ";
            cin >> itemAmount;

             if (cin.fail() || itemAmount <= 0 || itemAmount > 10)
            {
                cout << "\nInvalid amount! Please enter a positive integer less than or equal to 10.\n";
                cin.clear();
                cin.ignore(100, '\n'); // Clear the input buffer
            }
        }
        while (cin.fail() || itemAmount <= 0 || itemAmount > 10);


        cout << "You have ordered " << itemAmount << " " << itemNames[item - 1] << "(s)." << '\n';

        // Calculate price

        int totalPrice = itemPrices[item - 1] * itemAmount;


        // Create cart and updates as an order is made.

        quantityAmount[g_orderIndex] = itemAmount;
        orderCart[g_orderIndex] = itemNames[item - 1];
        price[g_orderIndex] = totalPrice;
        g_orderIndex++;

        cout << "Would you like to order more? (Y/N) ";
        cin >> prompt;
    }
}

// Checks for duplicate items and joins them

void checkDuplicate()
{
    // Loop through the order cart to check for duplicates
    for (int i = 0; i < g_orderIndex; i++)
    {
        for (int j = i + 1; j < g_orderIndex; j++)
        {
            if (orderCart[i] == orderCart[j])
            {
                quantityAmount[i] += quantityAmount[j];
                price[i] += price[j];

                // Shift elements to fill the gap created by merging duplicates
                for (int k = j; k < g_orderIndex - 1; k++)
                {
                    orderCart[k] = orderCart[k + 1];
                    quantityAmount[k] = quantityAmount[k + 1];
                    price[k] = price[k + 1];
                }

                // Decrease the order index and repeat the check for the current index
                g_orderIndex--;
                j--;
            }
        }
    }
}

// Function to check the items in the cart and print the total price

void checkCart(int *priceSum)
{
    // Call the check duplicate function to merge duplicate items
    checkDuplicate();

    // Initialize total to 0
    *priceSum = 0;

    // Display GUI for CART
    cout << '\n';
    cout << "+-------------------------------------------------------+\n";
    cout << "|                        CART                           |\n";
    cout << "+-------------------------------------------------------+\n";

    bool cartEmpty = true; // Flag to check if the cart is empty

    for (int i = 0; i < g_orderIndex; i++)
    {
        if (quantityAmount[i] > 0)
        {
            cout << quantityAmount[i] << "x " << orderCart[i] << " - " << "P" << price[i] << '\n';
            *priceSum += price[i];
            cartEmpty = false; // Set flag to false if there are items in the cart
        }
    }

    if (cartEmpty)
    {
        cout << "You do not have any items in your cart.\n";
    }

    cout << "\n\n---------------------------------------------------------\n";
    cout << "\nTotal Price is: P" << *priceSum << '\n';
    cout << "\n---------------------------------------------------------\n";
}

// Linear search algorithm to find the correct index for the price in the cart

int getItemPriceIndex(string itemName)
{
    for (int i = 0; i < g_MAX_ORDERS; ++i)
    {
        if (itemNames[i] == itemName)
            return i;
    }
    return 1;
}

// Update items

void updateItem()
{
    cout << "Enter Item Number: ";
    int itemNumber;
    cin >> itemNumber;

    if (cin.fail() || itemNumber < 1 || itemNumber > g_orderIndex)
    {
        cout << "Invalid item number!\n";
        return;
    }

    // Selecting an item

    cout << "You have selected " << orderCart[itemNumber - 1] << '\n';
    cout << "+ (Add Item) or - (Subtract Item): ";
    char quantityUpdate;
    cin >> quantityUpdate;

    int itemPriceIndex = getItemPriceIndex(orderCart[itemNumber - 1]);

    switch (quantityUpdate)
    {
    case '+':
        {
            cout << "Enter amount you want to add: ";
            int amountUpdate;
            cin >> amountUpdate;

            int totalQuantity = 0;
            for (int i = 0; i < g_orderIndex; ++i)
            {
                if (orderCart[i] == itemNames[itemPriceIndex])
                {
                    totalQuantity += quantityAmount[i];
                }
            }

            if (totalQuantity >= 10)
            {
                cout << "You cannot order more than ten items of this type!" << '\n';
                return;
            }

            price[itemNumber - 1] += itemPrices[itemPriceIndex] * amountUpdate; // Item price in the array of the selected item is added to the current price
            quantityAmount[itemNumber - 1] += amountUpdate; // Quantity amount is updated

            if (cin.fail() || amountUpdate <= 0)
            {
                cout << "Invalid input! Please enter a positive integer.\n";
                cin.clear();
                cin.ignore(100, '\n');
                return;
            }

            break;
        }
    case '-':
        {
            cout << "Enter amount you want to remove: ";
            int amountMinus;
            cin >> amountMinus;

           if (cin.fail() || amountMinus <= 0)
            {
                cout << "Invalid input! Please enter a positive integer.\n";
                cin.clear();
                cin.ignore(100, '\n'); // Clear the input buffer
                return;
            }

            price[itemNumber - 1] -= itemPrices[itemPriceIndex] * amountMinus;  // subtract the price
            quantityAmount[itemNumber - 1] -= amountMinus; // subtract the quantity amount

            // removes the item entirely if the amount is the same as the quantity amount in the cart

            if (quantityAmount[itemNumber - 1] == 0)
            {
                for (int i = itemNumber - 1; i < g_orderIndex - 1; ++i)
                {
                    orderCart[i] = orderCart[i + 1];
                    quantityAmount[i] = quantityAmount[i + 1];
                    price[i] = price[i + 1];
                }
                orderCart[g_orderIndex - 1] = "";
                quantityAmount[g_orderIndex - 1] = 0;
                price[g_orderIndex - 1] = 0;
                g_orderIndex--;
            }
            break;
        }
    default:
        cout << "Invalid option!" << '\n';
        return;
    }
}

// Delete item function

void deleteItem()
{
    cout << "Enter Item Number: ";
    int itemNumber;
    cin >> itemNumber;

    if (cin.fail() || itemNumber < 1 || itemNumber > g_orderIndex)
    {
        cout << "Invalid item number!" << '\n';
        return;
    }

    cout << "You have selected " << orderCart[itemNumber - 1] << '\n';
    cout << "Are you sure you want to delete this item? (Y/N) ";
    char deleteCart;
    cin >> deleteCart;

    if (toupper(deleteCart) != 'N')
    {
        for (int i = itemNumber - 1; i < g_orderIndex - 1; ++i)
        {
            orderCart[i] = orderCart[i + 1];
            quantityAmount[i] = quantityAmount[i + 1];
            price[i] = price[i + 1];
        }
        orderCart[g_orderIndex - 1] = "";
        quantityAmount[g_orderIndex - 1] = 0;
        price[g_orderIndex - 1] = 0;
        g_orderIndex--;

        cout << "Order deleted successfully!" << '\n';


    }
    else
    {
        cout << "Deletion canceled" << '\n';
    }
}

// Function to ask for checkout

bool askCheckout()
{
    cout << "\nWould you like to check out? ";
    char decision;
    cin >> decision;

    return (toupper(decision) != 'N');
}

// Checkout function, uses pointers and references to reduce use of global variables

int dropOrders()
{
    cout << "\nOrders dropped!" << '\n';

    g_orderIndex = 0;

    for (int i = 0; i < g_MAX_ORDERS; i++)
    {
        price[i] = 0;
        orderCart[i] = "";
        quantityAmount[i] = 0;
    }

    cout << "Thank you for coming to Ardee's Canteen!" << '\n';

    return 1;
}

void checkout(int *money, int *priceSum)
{
    if (*priceSum != 0)
    {
        cout << "\nPayment: ";
        cin >> *money;

        while (cin.fail() || *money < 0)
        {
            cout << "Invalid input! Please enter a positive number.\n";
            cin.clear();
            cin.ignore(100, '\n'); // Clear the input buffer
            cout << "\nPayment: ";
            cin >> *money;
        }

        if (*money < *priceSum)
        {
            cout << "You do not have enough cash!"
                 << "\nCancel order? (Y/N) ";
            char continueOrder;
            cin >> continueOrder;
            if (toupper(continueOrder) != 'Y')
            {
                checkout(money, priceSum);
            }
            else
            {
                dropOrders();
            }

        }
        else
        {
            isCheckout = true;
        }

    }
    else
    {
        cout << "\nYou do not have enough items in your cart!" << '\n';
    }
}

string getCurrentDateTime()
{
    // Get the current system time
    time_t currentTime = time(nullptr);

    // Convert the system time to a local time struct
    tm* localTime = localtime(&currentTime);

    // Format the time in 12-hour format with AM/PM
    ostringstream oss;
    oss << put_time(localTime, "%a %b %e %I:%M:%S %p %Y");
    return oss.str();
}


// Print receipt function

// use int *money as a parameter here

void printReceipt(int *money, bool &dine)
{
    string orderTime = getCurrentDateTime();

    if (isCheckout)
    {
        system("CLS");
        cout << '\n';
        cout << "+------------------------------------------------------------+\n";
        cout << "|                  ARDEE'S CANTEEN                           |\n";
        cout << "|              " << orderTime << "                   |\n";
        cout << "|               San Antonio, Nueva Ecija                     |\n";
        cout << "|                   +63902020202                             |\n";
        cout << "+------------------------------------------------------------+\n";
        cout << "| QTY    | ITEM                      | PRICE   |    TOTAL    |\n";
        cout << "+------------------------------------------------------------+\n";
        cout << "|                                                            |\n";
        cout << "|-----------------------" << (dine ? "DINE IN-" : "TAKE OUT") << "-----------------------------|\n";
        cout << "|                                                            |\n";

        // Loop through the orderCart array and print each item along with its quantity and price
        int totalAmount = 0;

        for (int i = 0; i < g_orderIndex; i++)
        {
            cout << "| " << setw(5) << left << quantityAmount[i] << "  | " << setw(25) << left << orderCart[i] << " | P" << setw(6) << left << price[i] / quantityAmount[i] << " | P" << setw(8) << left << price[i] << "   |\n";
            totalAmount += price[i];
        }

        double vat = 0.12;
        double vatSales = *money * vat;

        cout << "+------------------------------------------------------------+\n";
        cout << "|                            Vatable Sales: P" << setw(8) << right << totalAmount - vatSales << "        |" << '\n';
        cout << "|                            VAT:           P" << setw(8) << right << vatSales << "        |" << '\n';
        cout << "|                            TOTAL:         P" << setw(8) << right << totalAmount << "        |" << '\n';
        cout << "|                            CASH:          P" << setw(8) << right << *money << "        |" << '\n';
        cout << "|                            CHANGE:        P" << setw(8) << right << *money - totalAmount << "        |" << '\n';
        cout << "+------------------------------------------------------------+\n";

        cout << "|                                                            |" << '\n';
        cout << "|                                                            |" << '\n';
        cout << "|               Thank you, please come again                 |" << '\n';
        cout << "|             This serves as an OFFICIAL RECEIPT             |" << '\n';
        for (int i = 0; i < 4; i++)
        {
            cout << "|                                                            |" << '\n';
        }
        cout << "+------------------------------------------------------------+" << '\n';
    }
}
