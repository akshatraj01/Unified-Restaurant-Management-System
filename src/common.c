#include "restaurant.h"
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif

void ensure_data_directory(void) {
#ifdef _WIN32
    _mkdir(DATA_DIR);
#else
    mkdir(DATA_DIR, 0755);
#endif
}

void log_action(const char *username, const char *action) {
    FILE *fp = fopen(AUDIT_FILE, "a");
    if (!fp) return;
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char stamp[32] = "unknown-time";
    if (tm_info) strftime(stamp, sizeof(stamp), "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(fp, "[%s] user=%s action=%s\n", stamp, username, action);
    fclose(fp);
}

int read_int(const char *prompt) {
    char line[128], *end;
    long value;
    for (;;) {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) return 0;
        value = strtol(line, &end, 10);
        while (*end == ' ' || *end == '\t' || *end == '\n') end++;
        if (end != line && *end == '\0') return (int)value;
        puts("Invalid input. Please enter a whole number.");
    }
}

double read_double(const char *prompt) {
    char line[128], *end;
    double value;
    for (;;) {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) return 0.0;
        value = strtod(line, &end);
        while (*end == ' ' || *end == '\t' || *end == '\n') end++;
        if (end != line && *end == '\0' && value >= 0.0) return value;
        puts("Invalid input. Enter a non-negative number.");
    }
}

void read_text(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (!fgets(buffer, (int)size, stdin)) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\r\n")] = '\0';
}

void pause_screen(void) {
    char line[8];
    printf("\nPress Enter to continue...");
    fgets(line, sizeof(line), stdin);
}
