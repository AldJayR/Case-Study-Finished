#ifndef ORDER_SYSTEM_H_INCLUDED
#define ORDER_SYSTEM_H_INCLUDED

#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <regex>

using namespace std;

void displayCenteredAsciiArt();
void printOrderItems();
void orderSystem(string orderMessage, bool &dine, bool &newOrder, bool &classStatus);
bool isValidInput(const string& input);
void checkDuplicate();
void checkCart(int &priceSum, bool &emptyCart);
int getItemPriceIndex(string itemName);
void updateItem();
void deleteItem(bool &emptyCart);
bool askCheckout();
int dropOrders();
void saveOrdersToCSV();
void checkout(int &money, int &priceSum, bool &isDine, bool &newOrder, bool &cartFlag, bool &classStatus);
string getCurrentDateTime();
void printReceipt(int &money, bool &dine, bool &classStatus);
void directCheckout(int &money, int &priceSum, bool &dine, bool &newOrder, bool &cartFlag, bool &classStatus);
void exitSystem();
void handleOrderModification(int &priceSum, bool &isDine, bool &newOrder, bool &cartFlag, bool &classStatus);
#endif // ORDER_SYSTEM_H_INCLUDED
