# Core Workflows

## Order workflow

1. Register a customer or use customer ID `0` for a walk-in.
2. Select available food items by ID.
3. Enter quantities.
4. Enter a discount amount.
5. The system calculates tax and final total.
6. Enter the payment method.
7. The order is saved and receives a generated order ID.
8. Staff can update the order status as it moves through preparation and service.

## Inventory workflow

1. Add an ingredient with its quantity, unit, and reorder threshold.
2. View the inventory list.
3. Items at or below the reorder threshold are marked `LOW STOCK`.
4. Record a restock to increase the quantity.

## Reservation workflow

1. Enter customer name and phone number.
2. Enter table number, date/time, and guest count.
3. The reservation is stored with a generated ID.
4. View reservations from the reservation menu.

## Backup workflow

1. Select Create Backup to copy existing data files into `backups/`.
2. Select Restore Backup to copy available backup files back into `data/`.
3. Keep backups outside the public repository if they contain real customer information.
