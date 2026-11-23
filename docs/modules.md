# Module Details

## Authentication
Provides demonstration login flows for admin, manager, and cashier roles. Successful and failed login attempts are written to the audit log.

## Food menu
Stores food IDs, names, categories, prices, and availability. Supports listing, insertion, updates, and availability changes.

## Customer management
Registers customers with generated IDs and initial loyalty-point balances, and displays saved customer records.

## Order and kitchen management
Creates orders containing multiple food lines, computes subtotal, discount, illustrative tax, and final total, stores payment method and timestamp, and supports order-status updates.

## Inventory
Stores ingredient quantities, units, and reorder thresholds. Displays low-stock warnings and supports manual restocking.

## Employee management
Stores employee information and salary, and tracks attendance-day counts.

## Reservations
Stores customer contact details, table number, reservation date/time, guest count, and reservation status.

## Reports
Calculates lifetime non-cancelled order revenue and aggregates item quantities from saved orders.

## Backup and restore
Copies known data files into the `backups/` directory and can restore those copies.
