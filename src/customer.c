#include "restaurant.h"

int find_customer_by_id(int id, Customer *result) {
    FILE *fp = fopen(CUSTOMER_FILE, "rb");
    Customer c;
    if (!fp) return 0;
    while (fread(&c, sizeof(c), 1, fp) == 1) {
        if (c.id == id) { if (result) *result = c; fclose(fp); return 1; }
    }
    fclose(fp);
    return 0;
}

static int next_customer_id(void) {
    FILE *fp = fopen(CUSTOMER_FILE, "rb");
    Customer c; int max_id = 0;
    if (!fp) return 1;
    while (fread(&c, sizeof(c), 1, fp) == 1) if (c.id > max_id) max_id = c.id;
    fclose(fp); return max_id + 1;
}

static void add_customer(void) {
    Customer c; FILE *fp;
    c.id = next_customer_id(); c.loyalty_points = 0;
    read_text("Customer name: ", c.name, sizeof(c.name));
    read_text("Phone: ", c.phone, sizeof(c.phone));
    fp = fopen(CUSTOMER_FILE, "ab");
    if (!fp) { puts("Unable to save customer."); return; }
    fwrite(&c, sizeof(c), 1, fp); fclose(fp);
    printf("Customer registered. ID: %d\n", c.id);
    log_action("staff", "registered customer");
}

static void list_customers(void) {
    FILE *fp = fopen(CUSTOMER_FILE, "rb"); Customer c;
    if (!fp) { puts("No customer records."); return; }
    printf("\n%-6s %-25s %-18s %-8s\n", "ID", "Name", "Phone", "Points");
    while (fread(&c, sizeof(c), 1, fp) == 1)
        printf("%-6d %-25s %-18s %-8d\n", c.id, c.name, c.phone, c.loyalty_points);
    fclose(fp);
}

void customer_management(void) {
    int choice;
    do {
        puts("\n--- CUSTOMER MANAGEMENT ---");
        puts("1. Register customer\n2. View customers\n0. Back");
        choice = read_int("Choice: ");
        if (choice == 1) add_customer();
        else if (choice == 2) list_customers();
        else if (choice != 0) puts("Invalid choice.");
        if (choice) pause_screen();
    } while (choice != 0);
}
