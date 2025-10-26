#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

typedef struct ClientData {
    int id;
    char login[101];
    char password[101];
    char fullname[101];
    int is_banned;
    int balance;
    char phone_number[15];
} ClientData;


int init_database();
int close_database();
// Creates a new client record in the database with the provided client data
int addClient(ClientData data);

// Retrieves the current account balance for the specified client ID
int getBalance(int id);

// Deposits the specified amount into the client's account and updates the balance
int Deposit(int id, int sum);

// Directly updates the client's balance to the specified new amount
int updateBalance(int id, int new_balance);

// Searches for and returns complete client information based on the client ID
ClientData findUserbyID(int id);

// Withdraws the specified amount from the client's account if sufficient funds are available
int Withdraw(int id, int sum);

int ChangeStatus(int id);

#endif