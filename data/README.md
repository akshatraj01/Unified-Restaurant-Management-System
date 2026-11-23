# Runtime data directory

The application creates binary data files here when records are added. Binary files are intentionally not committed because they are generated at runtime and may depend on compiler/platform structure layout.

The application creates:
- menu.dat
- customers.dat
- orders.dat
- inventory.dat
- employees.dat
- reservations.dat
- audit.log
