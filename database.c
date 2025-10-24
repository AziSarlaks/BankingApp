#include "database.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static sqlite3 *db = NULL;

int init_database() {
    int rc = sqlite3_open("clients.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Не удалось открыть базу данных: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    printf("База данных инициализирована успешно\n");
    return 1;
}

int close_database() {
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
    return 1;
}

int addClient(ClientData data) {
    if (!db) {
        fprintf(stderr, "База данных не инициализирована\n");
        return 0;
    }

    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO client_data (phone, login, password, fullname, balance, banned) VALUES (?, ?, ?, ?, ?, ?);";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, data.phone_number, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, data.login, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, data.password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, data.fullname, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, data.balance);
    sqlite3_bind_int(stmt, 6, data.is_banned);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Ошибка вставки данных: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    printf("Клиент добавлен успешно\n");
    return 1;
}

int getBalance(int id) {
    if (!db) {
        fprintf(stderr, "База данных не инициализирована\n");
        return -1;
    }

    sqlite3_stmt *stmt;
    const char *sql = "SELECT balance FROM client_data WHERE id = ?;";
    int balance = -1;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, id);
    
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        balance = sqlite3_column_int(stmt, 0);
        printf("Баланс клиента %d: %d\n", id, balance);
    } else {
        fprintf(stderr, "Клиент с id %d не найден\n", id);
        balance = -1;
    }

    sqlite3_finalize(stmt);
    return balance;
}

int updateBalance(int id, int new_balance) {
    if (!db) {
        fprintf(stderr, "База данных не инициализирована\n");
        return 0;
    }

    sqlite3_stmt *stmt;
    const char *sql = "UPDATE client_data SET balance = ? WHERE id = ?;";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, new_balance);
    sqlite3_bind_int(stmt, 2, id);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Ошибка обновления баланса: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    int changes = sqlite3_changes(db);
    if (changes > 0) {
        printf("Баланс клиента %d обновлен на %d\n", id, new_balance);
        return 1;
    } else {
        printf("Клиент с id %d не найден\n", id);
        return 0;
    }
}

int Deposit(int id, int sum) {
    if (!db) {
        fprintf(stderr, "База данных не инициализирована\n");
        return 0;
    }

    int current_balance = getBalance(id);
    if (current_balance == -1) {
        return 0;
    }

    int new_balance = current_balance + sum;
    return updateBalance(id, new_balance);
}