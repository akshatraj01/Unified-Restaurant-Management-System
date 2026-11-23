#include "restaurant.h"

static void show_banner(void) {
    puts("\n==============================================================");
    puts("          UNIFIED RESTAURANT MANAGEMENT SYSTEM");
    puts("==============================================================");
    puts("  Integrated restaurant operations | Pure C | File Handling");
    puts("--------------------------------------------------------------");
}

int main(void) {
    int choice;
    ensure_data_directory();
    do {
        show_banner();
        puts("1. Authentication / Role Login");
        puts("2. Food Menu Management");
        puts("3. Customer Management");
        puts("4. Order & Kitchen Management");
        puts("5. Inventory Management");
        puts("6. Employee Management");
        puts("7. Table Reservations");
        puts("8. Analytics & Reports");
        puts("9. Backup & Restore");
        puts("0. Exit");
        choice = read_int("\nEnter your choice: ");
        switch (choice) {
            case 1: authentication_menu(); break;
            case 2: menu_management(); break;
            case 3: customer_management(); break;
            case 4: order_management(); break;
            case 5: inventory_management(); break;
            case 6: employee_management(); break;
            case 7: reservation_management(); break;
            case 8: reports_menu(); break;
            case 9: backup_restore_menu(); break;
            case 0: puts("Thank you for using URMS."); break;
            default: puts("Invalid choice. Please try again.");
        }
    } while (choice != 0);
    return 0;
}
