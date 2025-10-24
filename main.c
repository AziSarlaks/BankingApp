#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h> 
#include "database.h"

static void on_button_clicked(GtkWidget *widget, gpointer data) {
    
    ClientData new_client = {0};
    
    
    strcpy(new_client.login, "Azi");  
    strcpy(new_client.password, "123");
    strcpy(new_client.fullname, "Sarlaks");
    strcpy(new_client.phone_number, "(555) 353-4444"); 
    
    new_client.balance = 100000;
    new_client.is_banned = 0;
    
    addClient(new_client);
    
    getBalance(1);

    Deposit(1, 1000);
    g_print("Кнопка нажата!\n");
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *box;

    gtk_init(&argc, &argv);

    if (!init_database()) {
        fprintf(stderr, "Ошибка инициализации базы данных\n");
        return 1;
    }

    // Создание главного окна
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Банковское приложение");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Создание контейнера
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Создание кнопки
    button = gtk_button_new_with_label("Пополнить баланс");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    // Показать все элементы
    gtk_widget_show_all(window);

    // Главный цикл GTK
    gtk_main();

    // Закрытие базы данных при выходе
    close_database();

    return 0;
}