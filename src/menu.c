#include "restaurant.h"

int find_food_by_id(int id, FoodItem *result) {
    FILE *fp = fopen(MENU_FILE, "rb");
    FoodItem item;
    if (!fp) return 0;
    while (fread(&item, sizeof(item), 1, fp) == 1) {
        if (item.id == id) { if (result) *result = item; fclose(fp); return 1; }
    }
    fclose(fp);
    return 0;
}

static int next_food_id(void) {
    FILE *fp = fopen(MENU_FILE, "rb");
    FoodItem item; int max_id = 0;
    if (!fp) return 1;
    while (fread(&item, sizeof(item), 1, fp) == 1) if (item.id > max_id) max_id = item.id;
    fclose(fp);
    return max_id + 1;
}

static void list_food(void) {
    FILE *fp = fopen(MENU_FILE, "rb");
    FoodItem item;
    if (!fp) { puts("No menu records yet."); return; }
    printf("\n%-5s %-24s %-16s %-10s %-10s\n", "ID", "Food", "Category", "Price", "Status");
    puts("----------------------------------------------------------------");
    while (fread(&item, sizeof(item), 1, fp) == 1)
        printf("%-5d %-24s %-16s Rs.%-7.2f %-10s\n", item.id, item.name, item.category, item.price,
               item.available ? "Available" : "Unavailable");
    fclose(fp);
}

static void add_food(void) {
    FoodItem item;
    FILE *fp;
    item.id = next_food_id();
    read_text("Food name: ", item.name, sizeof(item.name));
    read_text("Category: ", item.category, sizeof(item.category));
    item.price = read_double("Price (Rs.): ");
    item.available = 1;
    fp = fopen(MENU_FILE, "ab");
    if (!fp) { puts("Could not open menu file."); return; }
    fwrite(&item, sizeof(item), 1, fp);
    fclose(fp);
    log_action("admin", "added food item");
    printf("Food item added with ID %d.\n", item.id);
}

static void update_food(void) {
    FILE *fp = fopen(MENU_FILE, "rb+");
    FoodItem item; int id, found = 0;
    if (!fp) { puts("Menu file not found."); return; }
    id = read_int("Food ID to update: ");
    while (fread(&item, sizeof(item), 1, fp) == 1) {
        if (item.id == id) {
            read_text("New food name: ", item.name, sizeof(item.name));
            read_text("New category: ", item.category, sizeof(item.category));
            item.price = read_double("New price: ");
            item.available = read_int("Available? (1=yes, 0=no): ") != 0;
            fseek(fp, -(long)sizeof(item), SEEK_CUR);
            fwrite(&item, sizeof(item), 1, fp);
            found = 1; break;
        }
    }
    fclose(fp);
    puts(found ? "Food item updated." : "Food ID not found.");
}

static void toggle_food(void) {
    FILE *fp = fopen(MENU_FILE, "rb+");
    FoodItem item; int id, found = 0;
    if (!fp) { puts("Menu file not found."); return; }
    id = read_int("Food ID: ");
    while (fread(&item, sizeof(item), 1, fp) == 1) {
        if (item.id == id) {
            item.available = !item.available;
            fseek(fp, -(long)sizeof(item), SEEK_CUR);
            fwrite(&item, sizeof(item), 1, fp);
            found = 1; break;
        }
    }
    fclose(fp);
    puts(found ? "Availability changed." : "Food ID not found.");
}

void menu_management(void) {
    int choice;
    do {
        puts("\n--- FOOD MENU MANAGEMENT ---");
        puts("1. View menu\n2. Add food item\n3. Update food item\n4. Toggle availability\n0. Back");
        choice = read_int("Choice: ");
        switch (choice) {
            case 1: list_food(); break;
            case 2: add_food(); break;
            case 3: update_food(); break;
            case 4: toggle_food(); break;
            case 0: break;
            default: puts("Invalid choice.");
        }
        if (choice) pause_screen();
    } while (choice != 0);
}
