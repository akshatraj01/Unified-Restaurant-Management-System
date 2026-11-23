#include "restaurant.h"

static int next_employee_id(void) {
    FILE *fp = fopen(EMPLOYEE_FILE, "rb"); Employee e; int max_id = 0;
    if (!fp) return 1;
    while (fread(&e, sizeof(e), 1, fp) == 1) if (e.id > max_id) max_id = e.id;
    fclose(fp); return max_id + 1;
}

static void add_employee(void) {
    Employee e; FILE *fp; e.id = next_employee_id(); e.present_days = 0;
    read_text("Employee name: ", e.name, sizeof(e.name));
    read_text("Role: ", e.role, sizeof(e.role));
    e.salary = read_double("Monthly salary: ");
    fp = fopen(EMPLOYEE_FILE, "ab");
    if (!fp) { puts("Unable to save employee."); return; }
    fwrite(&e, sizeof(e), 1, fp); fclose(fp);
    printf("Employee added. ID: %d\n", e.id);
}

static void list_employees(void) {
    FILE *fp = fopen(EMPLOYEE_FILE, "rb"); Employee e;
    if (!fp) { puts("No employee records."); return; }
    printf("\n%-6s %-24s %-18s %-12s %-8s\n", "ID", "Name", "Role", "Salary", "Days");
    while (fread(&e, sizeof(e), 1, fp) == 1)
        printf("%-6d %-24s %-18s Rs.%-8.2f %-8d\n", e.id, e.name, e.role, e.salary, e.present_days);
    fclose(fp);
}

static void mark_attendance(void) {
    FILE *fp = fopen(EMPLOYEE_FILE, "rb+"); Employee e; int id, found = 0;
    if (!fp) { puts("No employee file."); return; }
    id = read_int("Employee ID: ");
    while (fread(&e, sizeof(e), 1, fp) == 1) if (e.id == id) {
        e.present_days++; fseek(fp, -(long)sizeof(e), SEEK_CUR); fwrite(&e, sizeof(e), 1, fp);
        found = 1; break;
    }
    fclose(fp); puts(found ? "Attendance marked." : "Employee not found.");
}

void employee_management(void) {
    int choice;
    do {
        puts("\n--- EMPLOYEE MANAGEMENT ---");
        puts("1. Add employee\n2. View employees\n3. Mark attendance\n0. Back");
        choice = read_int("Choice: ");
        if (choice == 1) add_employee();
        else if (choice == 2) list_employees();
        else if (choice == 3) mark_attendance();
        else if (choice != 0) puts("Invalid choice.");
        if (choice) pause_screen();
    } while (choice != 0);
}
