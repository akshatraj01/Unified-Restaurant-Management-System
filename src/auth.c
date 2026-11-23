#include "restaurant.h"

static int authenticate(const char *role) {
    char username[64], password[64];
    read_text("Username: ", username, sizeof(username));
    read_text("Password: ", password, sizeof(password));

    /* Demo credentials are intentionally documented for academic demonstration.
       Replace this with salted password hashing before real-world deployment. */
    if ((!strcmp(role, "admin") && !strcmp(username, "admin") && !strcmp(password, "admin123")) ||
        (!strcmp(role, "manager") && !strcmp(username, "manager") && !strcmp(password, "manage123")) ||
        (!strcmp(role, "cashier") && !strcmp(username, "cashier") && !strcmp(password, "cash123"))) {
        log_action(username, "successful login");
        printf("\nWelcome, %s!\n", username);
        return 1;
    }
    puts("Invalid credentials.");
    log_action(username, "failed login");
    return 0;
}

void authentication_menu(void) {
    int choice;
    do {
        puts("\n--- ROLE LOGIN ---");
        puts("1. Admin");
        puts("2. Manager");
        puts("3. Cashier");
        puts("0. Back");
        choice = read_int("Select role: ");
        if (choice == 0) return;
        if (choice < 1 || choice > 3) { puts("Invalid choice."); continue; }
        const char *role = choice == 1 ? "admin" : choice == 2 ? "manager" : "cashier";
        if (authenticate(role)) {
            if (choice == 1) {
                puts("Admin access: all management modules are available from the main menu.");
            } else if (choice == 2) {
                puts("Manager access: operations, inventory, employees and reports.");
            } else {
                puts("Cashier access: customers, orders and billing.");
            }
            pause_screen();
        }
    } while (choice != 0);
}
