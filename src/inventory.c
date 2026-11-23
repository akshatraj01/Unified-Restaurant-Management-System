#include "restaurant.h"

static int next_ingredient_id(void) {
    FILE *fp = fopen(INVENTORY_FILE, "rb"); Ingredient x; int max_id = 0;
    if (!fp) return 1;
    while (fread(&x, sizeof(x), 1, fp) == 1) if (x.id > max_id) max_id = x.id;
    fclose(fp); return max_id + 1;
}

static void add_ingredient(void) {
    Ingredient x; FILE *fp;
    x.id = next_ingredient_id();
    read_text("Ingredient name: ", x.name, sizeof(x.name));
    x.quantity = read_double("Current quantity: ");
    read_text("Unit (kg/litre/pcs): ", x.unit, sizeof(x.unit));
    x.reorder_level = read_double("Low-stock threshold: ");
    fp = fopen(INVENTORY_FILE, "ab");
    if (!fp) { puts("Unable to save ingredient."); return; }
    fwrite(&x, sizeof(x), 1, fp); fclose(fp);
    log_action("manager", "added inventory ingredient");
    puts("Ingredient saved.");
}

static void list_inventory(void) {
    FILE *fp = fopen(INVENTORY_FILE, "rb"); Ingredient x;
    if (!fp) { puts("No inventory records."); return; }
    printf("\n%-5s %-22s %-12s %-12s %-12s\n", "ID", "Ingredient", "Quantity", "Unit", "Alert");
    while (fread(&x, sizeof(x), 1, fp) == 1)
        printf("%-5d %-22s %-12.2f %-12s %-12s\n", x.id, x.name, x.quantity, x.unit,
               x.quantity <= x.reorder_level ? "LOW STOCK" : "OK");
    fclose(fp);
}

static void restock(void) {
    FILE *fp = fopen(INVENTORY_FILE, "rb+"); Ingredient x; int id, found = 0; double amount;
    if (!fp) { puts("No inventory file."); return; }
    id = read_int("Ingredient ID: "); amount = read_double("Quantity to add: ");
    while (fread(&x, sizeof(x), 1, fp) == 1) if (x.id == id) {
        x.quantity += amount; fseek(fp, -(long)sizeof(x), SEEK_CUR); fwrite(&x, sizeof(x), 1, fp);
        found = 1; break;
    }
    fclose(fp); puts(found ? "Stock updated." : "Ingredient not found.");
}

void inventory_management(void) {
    int choice;
    do {
        puts("\n--- INVENTORY MANAGEMENT ---");
        puts("1. Add ingredient\n2. View stock and low-stock alerts\n3. Restock ingredient\n0. Back");
        choice = read_int("Choice: ");
        if (choice == 1) add_ingredient();
        else if (choice == 2) list_inventory();
        else if (choice == 3) restock();
        else if (choice != 0) puts("Invalid choice.");
        if (choice) pause_screen();
    } while (choice != 0);
}
