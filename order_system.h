#ifndef ORDER_SYSTEM_H_INCLUDED
#define ORDER_SYSTEM_H_INCLUDED

#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

void printOrderItems();
void orderSystem(string orderMessage, bool &dine, bool &newOrder);
void checkDuplicate();
void checkCart(int *priceSum);
int getItemPriceIndex(string itemName);
void updateItem();
void deleteItem();
bool askCheckout();
int dropOrders();
void checkout(int *money, int *priceSum);
string getCurrentDateTime();
void printReceipt(int *money, bool &dine);

#endif // ORDER_SYSTEM_H_INCLUDED
