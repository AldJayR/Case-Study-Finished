#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include "order_system.h"

constexpr int g_MAX_ORDERS = 15;
string orderCart[g_MAX_ORDERS];
int g_orderIndex = 0;
int price[g_MAX_ORDERS];
const string itemNames[] = {"Fries", "Coke", "Fried Chicken", "Ice Cream", "Menudo", "Burger",
"Pizza", "Salad", "Spaghetti", "Sushi", "Adobo", "Sinigang", "Pancit", "Lumpia", "Rice"};
constexpr int itemPrices[] = {30, 25, 60, 25, 60, 50, 85, 40, 70, 100, 70, 50, 60, 40, 10};
int quantityAmount[g_MAX_ORDERS] = {};
bool isCheckout = false;

using namespace std;

// Print the hardcoded menu of available items
void printOrderItems()
{
    cout << "+------------------------------------------------------------------------------------------+" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "|                                        ARDEE'S                                           |" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "+------------------------------------------------------------------------------------------+" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "|                                   PICK YOUR ORDER                                        |" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "|  1. Fries - P30                   6. Burger - P50          11. Adobo - P70               |" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "|  2. Coke - P25                    7. Pizza - P85           12. Sinigang - P50            |" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "|  3. Fried Chicken - P60           8. Salad - P40           13. Pancit - P60              |" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "|  4. Ice Cream - P25               9. Spaghetti - P70       14. Lumpia - P40              |" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "|  5. Menudo - P60                  10. Sushi - P100         15. Rice - P10                |" << '\n';
    cout << "|                                                                                          |" << '\n';
    cout << "+------------------------------------------------------------------------------------------+" << '\n';
}

// Handle the order system
void orderSystem(string orderMessage, bool &dine, bool &newOrder)
{
    char prompt = 'Y';
    cout << orderMessage;
    cin.get();
    cin.ignore(100, '\n');

    while (toupper(prompt) == 'Y')
    {
        char order;
        // Check if it's a new order and if the order index is 0
        if (!newOrder && g_orderIndex == 0)
        {
            cout << "\nDine in or Take Out (D/T) ";
            cin >> order;
            cin.ignore(100, '\n');
        }

        // Check if the user selected dine-in option

        if (toupper(order) != 'T' && toupper(order) != 'D' && !newOrder)
        {
            cout << "Invalid input! Please enter again";
            continue;
        }
        else if (toupper(order) != 'T')
        {
            dine = true;
            g_orderIndex == 0 ? cout << "DINE IN\n" : cout << "";
        }

        (!dine && g_orderIndex == 0) ? cout << "TAKE OUT" : cout << "";

        cout << "\nWhat would you like to order? (1-15) ";
        int item;
        cin >> item;
        cin.ignore(100, '\n');
        int itemAmount = 0;

        // Validate the user input
        while (cin.fail() || item < 1 || item > 15)
        {
            cout << "Invalid input! Please enter a number between 1 and 15.\n";
            cin.clear();
            cin.ignore(100, '\n');
            cout << "\n\nWhat would you like to order? (1-15) ";
            cin >> item;
            cin.ignore(100, '\n');
        }

        // Calculate the total quantity of the selected item in the order
        int totalQuantity = 0;
        for (int i = 0; i < g_orderIndex; ++i)
        {
            if (orderCart[i] == itemNames[item - 1])
            {
                totalQuantity += quantityAmount[i];
            }
        }

        // Check if the total quantity exceeds the limit
        if (totalQuantity >= 10)
        {
            cout << "You cannot order more than ten items of this type!" << '\n';
            continue;
        }

        cout << "You have ordered " << itemNames[item - 1] << '\n';

        do
        {
            cout << "How many would you like? (cannot exceed more than 10 items) ";
            cin >> itemAmount;
            cin.ignore(100, '\n');

            // Validate the quantity input
            if (cin.fail() || itemAmount <= 0 || itemAmount > 10)
            {
                cout << "\nInvalid amount! Please enter a positive integer less than or equal to 10.\n";
                cin.clear();
                cin.ignore(100, '\n');
            }
        }
        while (cin.fail() || itemAmount <= 0 || itemAmount > 10);

        // Check if the total quantity exceeds the limit after adding the new quantity
        if (totalQuantity + itemAmount > 10)
        {
            cout << "You cannot order more than ten items of this type!\n";
            do
            {
                cout << "How many would you like? (cannot exceed more than 10 items) ";
                cin.clear();
                cin.ignore(100, '\n');
                cin >> itemAmount;
            }
            while (totalQuantity + itemAmount > 10);
        }


        cout << "You have ordered " << itemAmount << " " << itemNames[item - 1] << "(s)." << '\n';

        // Calculate the total price for the ordered item
        int totalPrice = itemPrices[item - 1] * itemAmount;

        // Store the order details
        quantityAmount[g_orderIndex] = itemAmount;
        orderCart[g_orderIndex] = itemNames[item - 1];
        price[g_orderIndex] = totalPrice;
        g_orderIndex++;

        int errorFails = 0;

        do
        {
            if (errorFails == 0)
            {
                cout << "Would you like to order more? (Y/N) ";
                cin >> prompt;
                cin.ignore(100, '\n');
            }
            else
            {
                cout << "Invalid input. Please try again." << '\n';
                cout << "Would you like to order more? (Y/N) ";
                cin >> prompt;
                cin.ignore(100, '\n');
            }
            errorFails++;
        }
        while (toupper(prompt) != 'Y' && toupper(prompt) != 'N' );
    }
}

// Remove duplicate orders and update quantities and prices
void checkDuplicate()
{
    for (int i = 0; i < g_orderIndex; i++)
    {
        for (int j = i + 1; j < g_orderIndex; j++)
        {
            // If duplicate order found
            if (orderCart[i] == orderCart[j])
            {
                // Update the quantity and price of the first occurrence
                quantityAmount[i] += quantityAmount[j];
                price[i] += price[j];

                // Shift the remaining orders to remove the duplicate
                for (int k = j; k < g_orderIndex - 1; k++)
                {
                    orderCart[k] = orderCart[k + 1];
                    quantityAmount[k] = quantityAmount[k + 1];
                    price[k] = price[k + 1];
                }
                // Decrement the order index since a duplicate was removed
                g_orderIndex--;
                j--;
            }
        }
    }
}

// Display the current cart and calculate the total price
void checkCart(int *priceSum)
{
    // Remove duplicate orders and update quantities and prices
    checkDuplicate();

    *priceSum = 0;

    cout << '\n';
    cout << "+----------------------------------------------------------------+" << '\n';
    cout << "|                             CART                               |" << '\n';
    cout << "+----------------------------------------------------------------+" << '\n';

    bool cartEmpty = true;

    cout << "|No. | Qty | Item                           | Price   | Total    |" << '\n';
    cout << "+----+--------------------------------------+---------+----------+" << '\n';

    int itemNumber = 1;

    for (int i = 0; i < g_orderIndex; i++)
    {
        if (quantityAmount[i] > 0)
        {
            int itemPriceIndex = getItemPriceIndex(orderCart[i]);
            cout << "| " << setw(3) << left << itemNumber++ << "| " << setw(3) << left << quantityAmount[i]
                 << " | " << setw(30) << left << orderCart[i] << " | P" << setw(7)
                 << left << itemPrices[itemPriceIndex] << "| P" << setw(8) << left << price[i]  << "|" << '\n';

            *priceSum += price[i];
            cartEmpty = false;
        }
    }

    if (cartEmpty)
    {
        cout << "|                     No items in cart                           |" << '\n';
    }

    cout << "+----------------------------------------------------------------+" << '\n';
    cout << "| Total Price: P" << *priceSum << setw(50 - to_string(*priceSum).length()) << right << "|" << '\n';
    cout << "+----------------------------------------------------------------+" << '\n';

}

// Get the index of an item in the itemNames array
int getItemPriceIndex(string itemName)
{
    for (int i = 0; i < g_MAX_ORDERS; ++i)
    {
        if (itemNames[i] == itemName)
            return i;
    }
    return 1;
}

// Update the quantity of an existing order
void updateItem()
{
    cout << "Enter Item Number: ";
    int itemNumber;
    cin >> itemNumber;
    cin.ignore(100, '\n');

    // Validate the item number
    if (cin.fail() || itemNumber < 1 || itemNumber > g_orderIndex)
    {
        cout << "Invalid item number!\n";
        return;
    }

    cout << "You have selected " << orderCart[itemNumber - 1] << '\n';
    cout << "+ (Add Item) or - (Subtract Item): ";
    char quantityUpdate;
    cin >> quantityUpdate;

    int itemPriceIndex = getItemPriceIndex(orderCart[itemNumber - 1]);

    switch (quantityUpdate)
    {
    case '+':
        {
            cout << "Enter quantity you want to add: ";
            int amountUpdate;
            cin >> amountUpdate;

            // Calculate the total quantity of the selected item
            int totalQuantity = 0;
            for (int i = 0; i < g_orderIndex; ++i)
            {
                if (orderCart[i] == itemNames[itemPriceIndex])
                {
                    totalQuantity += quantityAmount[i];
                }
            }

            // Check if the total quantity exceeds the limit
            if (totalQuantity >= 10)
            {
                cout << "You cannot order more than ten items of this type!" << '\n';
                return;
            }

            if (totalQuantity + amountUpdate > 10)
            {
            cout << "You cannot order more than ten items of this type!\n";
                do
                {
                    cout << "Enter quantity you want to add: ";
                    cin.clear();
                    cin.ignore(100, '\n');
                    cin >> amountUpdate;
                }
                while (totalQuantity + amountUpdate > 10);
            }


            // Update the price and quantity
            price[itemNumber - 1] += itemPrices[itemPriceIndex] * amountUpdate;
            quantityAmount[itemNumber - 1] += amountUpdate;

            // Validate the quantity input
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
            cout << "Enter quantity you want to remove: ";
            int amountMinus;
            cin.ignore(100, '\n');
            cin >> amountMinus;

            // Validate the quantity input
           if (cin.fail() || amountMinus <= 0)
            {
                cout << "Invalid input! Please enter a positive integer.\n";
                cin.clear();
                cin.ignore(100, '\n');
                return;
            }

            // Update the price and quantity
            price[itemNumber - 1] -= itemPrices[itemPriceIndex] * amountMinus;
            quantityAmount[itemNumber - 1] -= amountMinus;

            // If the quantity becomes zero, remove the order
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

// Delete an existing order
void deleteItem()
{
    cout << "Enter Item Number: ";
    int itemNumber;
    cin >> itemNumber;
    cin.ignore(100, '\n');

    // Validate the item number
    if (cin.fail() || itemNumber < 1 || itemNumber > g_orderIndex)
    {
        cout << "Invalid item number!" << '\n';
        return;
    }

    cout << "You have selected " << orderCart[itemNumber - 1] << '\n';
    cout << "Are you sure you want to delete this item? (Y/N) ";
    char deleteCart;
    cin >> deleteCart;

    // Delete the order if the user confirms
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

// Ask the user if they want to check out
bool askCheckout()
{
    cout << "\nWould you like to check out? ((Y/N) ";
    char decision;
    cin >> decision;

    return (toupper(decision) != 'N');
}

// Drop all orders and reset the order system
int dropOrders()
{
    exitSystem();
    cout << "\nOrders dropped!" << '\n';

    g_orderIndex = 0;

    for (int i = 0; i < g_MAX_ORDERS; i++)
    {
        price[i] = 0;
        orderCart[i] = "";
        quantityAmount[i] = 0;
    }

    return 1;
}

// Handle the checkout process
void checkout(int *money, int *priceSum)
{
    if (*priceSum != 0)
    {
        cout << "\nPayment (Cash): ";
        cin >> *money;
        cin.ignore(100, '\n');

        // Validate the payment input
        while (cin.fail() || *money < 0)
        {
            cout << "Invalid input! Please enter a positive number.\n";
            cin.clear();
            cin.ignore(100, '\n');
            cout << "\nPayment (Cash): ";
            cin >> *money;
            cin.ignore(100, '\n');
        }

        // Check if the payment is sufficient
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

// Get the current date and time
string getCurrentDateTime()
{
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);
    ostringstream oss;
    oss << put_time(localTime, "%a %b %e %I:%M:%S %p %Y");
    return oss.str();
}

// Print the receipt
void printReceipt(int *money, bool &dine)
{
    // Array of cashier names
    const string cashierNames[] = {"Jannel", "Amiel", "Justine", "Jairo",
    "Aldwin", "Hans", "Russel", "Melgine", "King", "Sherilyn" };

    string orderTime = getCurrentDateTime();

    if (isCheckout)
    {
        // Randomly select a cashier name
        srand(time(nullptr));
        int index = rand() % 5; // 5 is the number of names in the array
        string cashierName = cashierNames[index];

        system("CLS");
        cout << '\n';
        cout << "+------------------------------------------------------------+" << '\n';
        cout << "|                       ARDEE'S                              |" << '\n';
        cout << "|               " << orderTime << "                    |" << '\n';
        cout << "|                San Antonio, Nueva Ecija                    |" << '\n';
        cout << "|                    +63902020202                            |" << '\n';
        cout << "+------------------------------------------------------------+" << '\n';
        cout << "| QTY    | ITEM                      | PRICE   |    TOTAL    |" << '\n';
        cout << "+------------------------------------------------------------+" << '\n';
        cout << "|                                                            |" << '\n';
        cout << "|-----------------------" << (dine ? "DINE IN-" : "TAKE OUT") << "-----------------------------|" << '\n';
        cout << "|                                                            |" << '\n';

        int totalAmount = 0;

        for (int i = 0; i < g_orderIndex; i++)
        {
            cout << "| " << setw(5) << left << quantityAmount[i] << "  | " << setw(25) << left << orderCart[i] << " | P" <<
                setw(6) << left << fixed << setprecision(2) << static_cast<double>(price[i] / quantityAmount[i]) <<
                    " | P" << setw(8) << left << fixed << setprecision(2) <<  static_cast<double>(price[i]) << "   |" << '\n';
            totalAmount += price[i];
        }

        double vat = 1.12;
        double vatSales = totalAmount / vat;
        double VAT_tax = totalAmount - vatSales;

        cout << "+------------------------------------------------------------+" << '\n';
        cout << "|                            TOTAL: (" << g_orderIndex << ")                      |" << '\n';
        cout << "|                            Vatable Sales: P" << fixed << setprecision(2) << setw(8) << right << vatSales << "        |" << '\n';
        cout << "|                            VAT (12%):     P" << setw(8) << right << VAT_tax << "        |" << '\n';
        cout << "|                            TOTAL:         P" << setw(8) << right << fixed << setprecision(2) << static_cast<double>(totalAmount) << "        |" << '\n';
        cout << "|                            CASH:          P" << setw(8) << right << fixed << setprecision(2) << static_cast<double>(*money) << "        |" << '\n';
        cout << "|                            CHANGE:        P" << setw(8) << right << fixed << setprecision(2) << static_cast<double>(*money - totalAmount) << "        |" << '\n';
        cout << "+------------------------------------------------------------+" << '\n';

        cout << "|                                                            |" << '\n';
        cout << "|                                                            |" << '\n';
        cout << "|               Thank you, please come again                 |" << '\n';
        cout << "|             This serves as an OFFICIAL RECEIPT             |" << '\n';
        for (int i = 0; i < 4; i++)
        {
            cout << "|                                                            |" << '\n';
        }
        // Include cashier name in the receipt
        cout << "|  Sold by: " << setw(49) << left << cashierName << "|" << '\n';
        cout << "|  Sold to: _________________________                        |" << '\n';
        cout << "|  Name: ____________________________                        |" << '\n';
        cout << "|  Address: _________________________                        |" << '\n';
        cout << "|  TIN: _____________________________                        |" << '\n';
        for (int i = 0; i < 4; i++)
        {
            cout << "|                                                            |" << '\n';
        }
        cout << "+------------------------------------------------------------+" << '\n';

    }
}


void directCheckout(int *money, int *priceSum, bool &dine)
{
    checkout(money, priceSum);
    printReceipt(money, dine);
    cout << "Thank you for coming to Ardee's. Please come again" << '\n';
    cout << "\nPress Enter to exit...";
    cin.ignore();
    cin.get();
}

void exitSystem()
{
    cout << "Canceling order";
    for (int i = 0; i < 3; ++i)
    {
        cout << '.';
        this_thread::sleep_for(chrono::milliseconds(300));
    }
    cout << '\n';
}
