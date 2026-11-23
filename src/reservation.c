#include "restaurant.h"

static int next_reservation_id(void) {
    FILE *fp = fopen(RESERVATION_FILE, "rb"); Reservation r; int max_id = 0;
    if (!fp) return 1;
    while (fread(&r, sizeof(r), 1, fp) == 1) if (r.id > max_id) max_id = r.id;
    fclose(fp); return max_id + 1;
}

static void add_reservation(void) {
    Reservation r; FILE *fp; r.id = next_reservation_id(); r.status = 0;
    read_text("Customer name: ", r.customer_name, sizeof(r.customer_name));
    read_text("Phone: ", r.phone, sizeof(r.phone));
    r.table_number = read_int("Table number: ");
    read_text("Reservation date/time (YYYY-MM-DD HH:MM): ", r.date_time, sizeof(r.date_time));
    r.guests = read_int("Number of guests: ");
    if (r.table_number <= 0 || r.guests <= 0) { puts("Invalid table or guest count."); return; }
    fp = fopen(RESERVATION_FILE, "ab");
    if (!fp) { puts("Unable to save reservation."); return; }
    fwrite(&r, sizeof(r), 1, fp); fclose(fp);
    printf("Reservation created. ID: %d\n", r.id);
}

static void list_reservations(void) {
    FILE *fp = fopen(RESERVATION_FILE, "rb"); Reservation r;
    if (!fp) { puts("No reservations."); return; }
    printf("\n%-5s %-22s %-8s %-20s %-8s %-8s\n", "ID", "Customer", "Table", "Date/time", "Guests", "Status");
    while (fread(&r, sizeof(r), 1, fp) == 1)
        printf("%-5d %-22s %-8d %-20s %-8d %-8d\n", r.id, r.customer_name, r.table_number,
               r.date_time, r.guests, r.status);
    fclose(fp);
}

void reservation_management(void) {
    int choice;
    do {
        puts("\n--- TABLE RESERVATIONS ---");
        puts("1. Create reservation\n2. View reservations\n0. Back");
        choice = read_int("Choice: ");
        if (choice == 1) add_reservation();
        else if (choice == 2) list_reservations();
        else if (choice != 0) puts("Invalid choice.");
        if (choice) pause_screen();
    } while (choice != 0);
}
