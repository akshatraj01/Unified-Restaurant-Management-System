CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -Iinclude
SOURCES = src/main.c src/common.c src/auth.c src/menu.c src/customer.c src/order.c src/inventory.c src/employee.c src/reservation.c src/reports.c src/backup.c
TARGET = urms

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all clean
