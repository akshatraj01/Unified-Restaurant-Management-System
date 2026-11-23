# Architecture

The application follows a simple modular architecture:

```text
User
 |
 v
src/main.c  (main menu and navigation)
 |
 +--> Authentication
 +--> Menu Management
 +--> Customer Management
 +--> Order and Kitchen Management
 +--> Inventory Management
 +--> Employee Management
 +--> Reservation Management
 +--> Reports
 +--> Backup and Restore
 |
 v
include/restaurant.h  (shared data structures and declarations)
 |
 v
data/*.dat and data/audit.log  (persistent local storage)
```

Each feature is implemented in a separate `.c` file. Shared structures and function declarations are kept in `include/restaurant.h`.
