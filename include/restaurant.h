#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DATA_DIR "data"
#define MENU_FILE DATA_DIR "/menu.dat"
#define CUSTOMER_FILE DATA_DIR "/customers.dat"
#define ORDER_FILE DATA_DIR "/orders.dat"
#define INVENTORY_FILE DATA_DIR "/inventory.dat"
#define EMPLOYEE_FILE DATA_DIR "/employees.dat"
#define RESERVATION_FILE DATA_DIR "/reservations.dat"
#define AUDIT_FILE DATA_DIR "/audit.log"
#define MAX_ITEMS 100
#define NAME_LEN 64

typedef struct {
    int id;
    char name[NAME_LEN];
    char category[32];
    double price;
    int available;
} FoodItem;

typedef struct {
    int id;
    char name[NAME_LEN];
    char phone[20];
    int loyalty_points;
} Customer;

typedef struct {
    int food_id;
    int quantity;
    double unit_price;
} OrderLine;

typedef struct {
    int id;
    int customer_id;
    int line_count;
    OrderLine lines[MAX_ITEMS];
    double subtotal;
    double discount;
    double tax;
    double total;
    int status; /* 0 pending, 1 preparing, 2 ready, 3 served, 4 cancelled */
    char payment[16];
    char created_at[32];
} RestaurantOrder;

typedef struct {
    int id;
    char name[NAME_LEN];
    double quantity;
    char unit[16];
    double reorder_level;
} Ingredient;

typedef struct {
    int id;
    char name[NAME_LEN];
    char role[24];
    double salary;
    int present_days;
} Employee;

typedef struct {
    int id;
    char customer_name[NAME_LEN];
    char phone[20];
    int table_number;
    char date_time[32];
    int guests;
    int status; /* 0 booked, 1 seated, 2 completed, 3 cancelled */
} Reservation;

void ensure_data_directory(void);
void log_action(const char *username, const char *action);
int read_int(const char *prompt);
double read_double(const char *prompt);
void read_text(const char *prompt, char *buffer, size_t size);
void pause_screen(void);

void authentication_menu(void);
void menu_management(void);
void customer_management(void);
void order_management(void);
void inventory_management(void);
void employee_management(void);
void reservation_management(void);
void reports_menu(void);
void backup_restore_menu(void);

int find_food_by_id(int id, FoodItem *result);
int find_customer_by_id(int id, Customer *result);

#endif
