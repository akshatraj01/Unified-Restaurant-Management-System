#include "restaurant.h"
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif

static int copy_file(const char *source, const char *destination) {
    FILE *in = fopen(source, "rb"), *out;
    char buffer[4096]; size_t bytes;
    if (!in) return 0;
    out = fopen(destination, "wb");
    if (!out) { fclose(in); return 0; }
    while ((bytes = fread(buffer, 1, sizeof(buffer), in)) > 0)
        if (fwrite(buffer, 1, bytes, out) != bytes) { fclose(in); fclose(out); return 0; }
    fclose(in); fclose(out); return 1;
}

void backup_restore_menu(void) {
    int choice, ok = 1;
    const char *source[] = {MENU_FILE, CUSTOMER_FILE, ORDER_FILE, INVENTORY_FILE, EMPLOYEE_FILE, RESERVATION_FILE};
    const char *backup[] = {"backups/menu.dat", "backups/customers.dat", "backups/orders.dat",
                            "backups/inventory.dat", "backups/employees.dat", "backups/reservations.dat"};
    do {
        puts("\n--- BACKUP & RESTORE ---");
        puts("1. Create backup\n2. Restore backup\n0. Back");
        choice = read_int("Choice: ");
        if (choice == 1 || choice == 2) {
            ok = 1;
#ifdef _WIN32
            _mkdir("backups");
#else
            mkdir("backups", 0755);
#endif
            for (int i = 0; i < 6; i++) {
                const char *from = choice == 1 ? source[i] : backup[i];
                const char *to = choice == 1 ? backup[i] : source[i];
                FILE *probe = fopen(from, "rb");
                if (!probe) continue;
                fclose(probe);
                if (!copy_file(from, to)) ok = 0;
            }
            puts(ok ? (choice == 1 ? "Backup operation completed." : "Restore operation completed.")
                    : "Some files could not be copied.");
            log_action("admin", choice == 1 ? "created backup" : "restored backup");
        } else if (choice != 0) puts("Invalid choice.");
        if (choice) pause_screen();
    } while (choice != 0);
}
