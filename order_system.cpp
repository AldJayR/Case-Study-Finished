#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <thread>
#include <windows.h>
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
int g_idNumber;
string className;

using namespace std;

int getConsoleHeight()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

// Function to display centered ASCII art
void displayCenteredAsciiArt()
{
    const string art[] = {
        "                   _           _     ",
        "     /\\           | |         ( )    ",
        "    /  \\   _ __ __| | ___  ___|/ ___ ",
        "   / /\\ \\ | '__/ _` |/ _ \\/ _ \\ / __|",
        "  / ____ \\| | | (_| |  __/  __/ \\__ \\",
        " /_/    \\_\\_|  \\__,_|\\___|\\___| |___/"
    };

    int consoleHeight = getConsoleHeight();
    int artHeight = static_cast<int>(sizeof(art) / sizeof(art[0]));
    int startLine = (consoleHeight - artHeight) / 2;

    for (int i = 0; i < startLine; ++i)
    {
        cout << endl;
    }

    for (const string& line : art)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.dwSize.X;
        int indent = (consoleWidth - line.length()) / 2;
        cout << setw(indent) << "" << line << endl;
    }


}


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
void orderSystem(string orderMessage, bool &dine, bool &newOrder, bool &classStatus)
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

        string types[3] = {"PWD", "SENIOR CITIZEN", "NONE"};
        bool typeMatch = false;

        do
        {
            if (g_orderIndex == 0)
            {
                cout << "PWD or Senior Citizen (Enter None if applicable): ";
                string classType;
                getline(cin >> ws, classType);

                // Convert input string to uppercase
                for (char& c : classType)
                {
                    c = toupper(c);
                }

                // Check if the input matches any of the types

                for (const string& type : types)
                {
                    if (type == classType)
                    {
                        typeMatch = true;

                        if (type != "NONE")
                        {
                            cout << "Enter ID Number: ";
                            string stringIdNumber;
                            getline(cin >> ws, stringIdNumber);

                            bool isError = false;

                            do
                            {
                                if (!isValidInput(stringIdNumber) || stringIdNumber.length() < 6 || stringIdNumber.length() > 6)
                                {
                                    isError = true;
                                    cout << "ID number invalid. Try again: ";
                                    getline(cin >> ws, stringIdNumber);
                                }
                                else
                                {
                                    isError = false;
                                }
                            }
                            while (isError);


                            g_idNumber = stoi(stringIdNumber);

                            do
                            {
                                cout << "Enter Full Name (FIRST/LAST): ";
                                getline(cin >> ws, className);

                                // Check if the input is valid
                                bool isValid = true;
                                for (char c : className)
                                {
                                    if (!isalpha(c) && c != ' ')
                                    {
                                        isValid = false;
                                        break;
                                    }
                                }

                                if (!isValid)
                                {
                                    cout << "Invalid Name. Try again\n";
                                }
                                else
                                {
                                    classStatus = true;
                                    break; // Break out of the loop if the input is valid
                                }
                            }
                            while (true);
                        }
                        else
                        {
                           break;
                        }
                    }
                }
                if (!typeMatch) {
                    cout << "Invalid input! Please enter PWD or SENIOR CITIZEN.\n";
                }
            }
            else
            {
                break;
            }
        }
        while (!typeMatch);


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
        string choice;
        getline(cin >> ws, choice);

        // Validate the user input
        while (!isValidInput(choice) || stof(choice) < 1 || stof(choice) > 15)
        {
            cout << "Invalid input! Please enter a number between 1 and 15.\n";
            cout << "\n\nWhat would you like to order? (1-15) ";
            getline(cin >> ws, choice);
        }

        int item = stoi(choice);

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

        string amountChoice;
        bool error = false;
        do
        {
            cout << "How many would you like? (cannot exceed more than 10 items) ";
            getline(cin >> ws, amountChoice);

            // Validate the quantity input
            if (!isValidInput(amountChoice) || stof(amountChoice) <= 0 || stof(amountChoice) > 10)
            {
                cout << "\nInvalid amount! Please enter a positive integer less than or equal to 10.\n";
                error = true;
            }
            else
            {
                error = false;
            }

        }
        while (error);

        int itemAmount = stoi(amountChoice);

        // Check if the total quantity exceeds the limit after adding the new quantity
        if (totalQuantity + itemAmount > 10)
        {
            cout << "You cannot order more than ten items of this type!\n";
            do
            {
                cout << "How many would you like? (cannot exceed more than 10 items) ";
                string newAmountChoice;
                getline(cin >> ws, newAmountChoice);

                if (!isValidInput(newAmountChoice) || stoi(newAmountChoice) <= 0 || stoi(newAmountChoice) > 10)
                {
                    cout << "\nInvalid amount! Please enter a positive integer less than or equal to 10.\n";
                }
                else
                {
                    itemAmount = stoi(newAmountChoice);
                }
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

bool isValidInput(const string& input)
{
    const regex choicePattern("^[0-9]+$");

    return regex_match(input, choicePattern);
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
void checkCart(int &priceSum, bool &emptyCart)
{
    // Remove duplicate orders and update quantities and prices
    checkDuplicate();

    priceSum = 0;

    cout << '\n';
    cout << "+----------------------------------------------------------------+" << '\n';
    cout << "|                             CART                               |" << '\n';
    cout << "+----------------------------------------------------------------+" << '\n';

    emptyCart = true;

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

            priceSum += price[i];
            emptyCart = false;
        }
    }

    if (emptyCart)
    {
        cout << "|                     No items in cart                           |" << '\n';
    }

    cout << "+----------------------------------------------------------------+" << '\n';
    cout << "| Total Price: P" << priceSum << setw(50 - to_string(priceSum).length()) << right << "|" << '\n';
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
    string choice;
    getline(cin >> ws, choice);

    // Validate the item number
    if (!isValidInput(choice) || stof(choice) < 1 || stof(choice) > g_orderIndex)
    {
        cout << "Invalid item number!\n";
        return;
    }

    int itemNumber = stoi(choice);

    cout << "You have selected " << orderCart[itemNumber - 1] << '\n';
    cout << "+ (Add Item) or - (Subtract Item): ";
    char quantityUpdate;
    cin >> quantityUpdate;

    int itemPriceIndex = getItemPriceIndex(orderCart[itemNumber - 1]);

    switch (quantityUpdate)
    {
    case '+':
    {
        int amountUpdate = 0;
        bool error = false;

        do
        {
            cout << "Enter quantity you want to add: ";
            string amountUpdateChoice;
            getline(cin >> ws, amountUpdateChoice);

            if (!isValidInput(amountUpdateChoice) || stof(amountUpdateChoice) <= 0)
            {
                cout << "Invalid input! Please enter a positive integer.\n";
                error = true;
            }
            else
            {
                amountUpdate = stoi(amountUpdateChoice);
                error = false;
            }
        }
        while (error);

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
            string newAmountUpdateChoice;
            do
            {
                cout << "Enter quantity you want to add: ";
                getline(cin >> ws, newAmountUpdateChoice);

                // Validate the quantity input
                if (!isValidInput(newAmountUpdateChoice) || stof(newAmountUpdateChoice) <= 0)
                {
                    cout << "Invalid input! Please enter a positive integer.\n";
                    continue;
                }
                amountUpdate = stoi(newAmountUpdateChoice);

                // Check if the new amount update still exceeds the limit
                if (totalQuantity + amountUpdate <= 10)
                {
                    break;
                }

                cout << "You cannot order more than ten items of this type!\n";
            }
            while (true);
        }

        // Update the price and quantity
        price[itemNumber - 1] += itemPrices[itemPriceIndex] * amountUpdate;
        quantityAmount[itemNumber - 1] += amountUpdate;

        break;
    }
    case '-':
    {
        int amountMinus = 0;
        bool error = false;

        do
        {
            cout << "Enter quantity you want to remove: ";
            string amountMinusChoice;
            getline(cin >> ws, amountMinusChoice);

            if (!isValidInput(amountMinusChoice) || stof(amountMinusChoice) <= 0 || stof(amountMinusChoice) > quantityAmount[itemNumber - 1])
            {
                cout << "Invalid input! Please enter a valid input.\n";
                error = true;
            }
            else
            {
                amountMinus = stoi(amountMinusChoice);
                error = false;
            }
        }
        while (error);

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
void deleteItem(bool &emptyCart)
{
    string choice;

   if (!emptyCart)
   {
        do
        {
            cout << "Enter Item Number: ";
            getline(cin >> ws, choice);

            // Validate the item number
            if (!isValidInput(choice) || stof(choice) < 1 || stof(choice) > g_orderIndex)
            {
                cout << "Invalid item number!" << '\n';
            }
            else
            {
                break;
            }
        }
        while (true);


        int itemNumber = stoi(choice);

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
    else
    {
        cout << "\nYou do not have any items in your cart!" << '\n';
    }
}

// Ask the user if they want to check out
bool askCheckout()
{
    cout << "\nWould you like to check out? (Y/N) ";
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
void checkout(int &money, int &priceSum, bool &isDine, bool &newOrder, bool &cartFlag, bool &classStatus)
{
    if (priceSum != 0)
    {
        string paymentInput;
        bool error = false;

        do
        {
            cout << "\nPayment (Cash): ";
            getline(cin >> ws, paymentInput);

            // Validate the payment input
            if (!isValidInput(paymentInput) || stoi(paymentInput) < 0)
            {
                cout << "Invalid input! Please enter a valid integer.\n";
                error = true;
            }
            else
            {
                money = stoi(paymentInput);
                error = false;
            }
        }
        while (error);

        // Check if the payment is sufficient
        if (money < priceSum)
        {
            cout << "You do not have enough cash!"
                 << "\nCancel order? (Y/N) ";
            char continueOrder;
            cin >> continueOrder;
            cin.ignore(100, '\n');
            if (toupper(continueOrder) == 'N')
            {
              handleOrderModification(priceSum, isDine, newOrder, cartFlag, classStatus);
              askCheckout();
            }
            else if (toupper(continueOrder) == 'Y')
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
void printReceipt(int &money, bool &dine, bool &classStatus)
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
        cout << "|                            CASH:          P" << setw(8) << right << fixed << setprecision(2) << static_cast<double>(money) << "        |" << '\n';
        cout << "|                            CHANGE:        P" << setw(8) << right << fixed << setprecision(2) << static_cast<double>(money - totalAmount) << "        |" << '\n';
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
        cout << "|  Name: " << setw(52) << left << (classStatus ? className :  "_________________________    ") << "|" << '\n';
        cout << "|  Address: _________________________                        |" << '\n';
        cout << "|  TIN: _____________________________                        |" << '\n';
        for (int i = 0; i < 4; i++)
        {
            cout << "|                                                            |" << '\n';
        }
        cout << "+------------------------------------------------------------+" << '\n';

    }
}


void directCheckout(int &money, int &priceSum, bool &dine, bool &newOrder, bool &cartFlag, bool &classStatus)
{
    checkout(money, priceSum, dine, newOrder, cartFlag, classStatus);
    printReceipt(money, dine, classStatus);
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

void handleOrderModification(int &priceSum, bool &isDine, bool &newOrder, bool &cartFlag, bool &classStatus)
{
    char orderPrompt = 'Y';
    bool modify = false;

    // Loop to allow order modification
    while (toupper(orderPrompt) != 'N')
    {
        cout << "Do you want to modify your existing orders? (Y/N) ";
        cin >> orderPrompt;
        cin.ignore(100, '\n');

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
        while (modify)
        {
            // Print options for modification
            cout << "\n(1) Delete Item"
                 << "\n(2) Update Item"
                 << "\n(3) Order Again"
                 << "\n(4) Go Back \n>> ";

            int option;
            cin >> option;

            while (cin.fail())
            {
                cin.clear();
                cin.ignore(100, '\n');
                option = 5;
            }

            // Handle the selected option
            switch (option)
            {
                case 1:
                    deleteItem(cartFlag); // Delete an existing order
                    modify = false;
                    break;
                case 2:
                    updateItem(); // Update the quantity of an existing order
                    modify = false;
                    break;
                case 3:
                    newOrder = true;     // Set the flag to indicate it's a new order
                    printOrderItems();   // Print the menu of available items
                    orderSystem("", isDine, newOrder, classStatus); // Start the order system with the "ordering again" message
                    modify = false;
                    break;
                case 4:
                    checkCart(priceSum, cartFlag);
                    orderPrompt = 'N';
                    modify = false;
                    break;
                default:
                    cout << "Invalid option! Select again\n"; // Print an error message for invalid input
                    continue;                               // Continue the loop to allow the user to select again
            }

            // Display the updated cart and total price
            if (option != 4)
            {
                checkCart(priceSum, cartFlag);
                continue; // Continue the loop to allow further modifications
            }
        }
    }
}

