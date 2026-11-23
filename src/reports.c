#include "restaurant.h"

static void sales_summary(void) {
    FILE *fp = fopen(ORDER_FILE, "rb"); RestaurantOrder o;
    double revenue = 0.0; int count = 0, cancelled = 0;
    if (!fp) { puts("No sales data yet."); return; }
    while (fread(&o, sizeof(o), 1, fp) == 1) {
        if (o.status == 4) cancelled++;
        else { revenue += o.total; count++; }
    }
    fclose(fp);
    puts("\n========== SALES SUMMARY ==========");
    printf("Completed/non-cancelled orders: %d\n", count);
    printf("Cancelled orders: %d\n", cancelled);
    printf("Recorded revenue: Rs. %.2f\n", revenue);
    puts("Note: This is a lifetime summary of saved orders, not a date-filtered report.");
}

static void popular_items(void) {
    FILE *fp = fopen(ORDER_FILE, "rb"); RestaurantOrder o;
    int ids[MAX_ITEMS] = {0}, quantities[MAX_ITEMS] = {0}, n = 0, i, j;
    if (!fp) { puts("No order history."); return; }
    while (fread(&o, sizeof(o), 1, fp) == 1) {
        if (o.status == 4) continue;
        for (i = 0; i < o.line_count; i++) {
            for (j = 0; j < n && ids[j] != o.lines[i].food_id; j++) {}
            if (j == n && n < MAX_ITEMS) ids[n++] = o.lines[i].food_id;
            if (j < MAX_ITEMS) quantities[j] += o.lines[i].quantity;
        }
    }
    fclose(fp);
    puts("\n========== ITEM SALES ==========");
    for (i = 0; i < n; i++) {
        FoodItem f;
        if (find_food_by_id(ids[i], &f)) printf("%-24s %d sold\n", f.name, quantities[i]);
        else printf("Food ID %d: %d sold (item no longer in menu)\n", ids[i], quantities[i]);
    }
}

void reports_menu(void) {
    int choice;
    do {
        puts("\n--- ANALYTICS & REPORTS ---");
        puts("1. Sales summary\n2. Food item sales\n0. Back");
        choice = read_int("Choice: ");
        if (choice == 1) sales_summary();
        else if (choice == 2) popular_items();
        else if (choice != 0) puts("Invalid choice.");
        if (choice) pause_screen();
    } while (choice != 0);
}
