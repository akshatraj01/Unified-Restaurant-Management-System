#include "restaurant.h"

static int next_order_id(void) {
    FILE *fp = fopen(ORDER_FILE, "rb");
    RestaurantOrder o; int max_id = 0;
    if (!fp) return 1;
    while (fread(&o, sizeof(o), 1, fp) == 1) if (o.id > max_id) max_id = o.id;
    fclose(fp); return max_id + 1;
}

static void print_order(const RestaurantOrder *o) {
    int i; FoodItem food;
    printf("\nOrder #%d | Customer ID: %d | Status: %d\n", o->id, o->customer_id, o->status);
    puts("------------------------------------------------");
    for (i = 0; i < o->line_count; i++) {
        if (find_food_by_id(o->lines[i].food_id, &food))
            printf("%-22s x %-3d Rs. %.2f\n", food.name, o->lines[i].quantity,
                   o->lines[i].unit_price * o->lines[i].quantity);
    }
    printf("Subtotal: Rs. %.2f\nDiscount: Rs. %.2f\nTax: Rs. %.2f\nTOTAL: Rs. %.2f\n",
           o->subtotal, o->discount, o->tax, o->total);
}

static void place_order(void) {
    RestaurantOrder o; FoodItem food; FILE *fp;
    int id, qty, i;
    memset(&o, 0, sizeof(o));
    o.id = next_order_id();
    o.customer_id = read_int("Customer ID (0 for walk-in): ");
    puts("\nAvailable food items:");
    {
        FILE *mf = fopen(MENU_FILE, "rb");
        if (!mf) { puts("Add food items before placing an order."); return; }
        while (fread(&food, sizeof(food), 1, mf) == 1)
            if (food.available) printf("%d. %s - Rs. %.2f\n", food.id, food.name, food.price);
        fclose(mf);
    }
    while (o.line_count < MAX_ITEMS) {
        id = read_int("Food ID (0 to finish): ");
        if (id == 0) break;
        if (!find_food_by_id(id, &food) || !food.available) { puts("Invalid or unavailable item."); continue; }
        qty = read_int("Quantity: ");
        if (qty <= 0) { puts("Quantity must be positive."); continue; }
        for (i = 0; i < o.line_count; i++) if (o.lines[i].food_id == id) break;
        if (i < o.line_count) o.lines[i].quantity += qty;
        else { o.lines[o.line_count].food_id = id; o.lines[o.line_count].quantity = qty;
               o.lines[o.line_count].unit_price = food.price; o.line_count++; }
        o.subtotal += food.price * qty;
    }
    if (o.line_count == 0) { puts("Order not created: no items selected."); return; }
    o.discount = read_double("Discount amount (Rs.): ");
    if (o.discount > o.subtotal) o.discount = o.subtotal;
    o.tax = (o.subtotal - o.discount) * 0.05; /* Example tax rate; configure for local rules. */
    o.total = o.subtotal - o.discount + o.tax;
    o.status = 0;
    read_text("Payment method (Cash/UPI/Card): ", o.payment, sizeof(o.payment));
    time_t now = time(NULL); struct tm *tm_info = localtime(&now);
    if (tm_info) strftime(o.created_at, sizeof(o.created_at), "%Y-%m-%d %H:%M:%S", tm_info);
    fp = fopen(ORDER_FILE, "ab");
    if (!fp) { puts("Unable to save order."); return; }
    fwrite(&o, sizeof(o), 1, fp); fclose(fp);
    print_order(&o);
    printf("Order saved successfully. Order ID: %d\n", o.id);
    log_action("cashier", "created order");
}

static void list_orders(void) {
    FILE *fp = fopen(ORDER_FILE, "rb"); RestaurantOrder o;
    if (!fp) { puts("No orders found."); return; }
    while (fread(&o, sizeof(o), 1, fp) == 1) print_order(&o);
    fclose(fp);
}

static void update_status(void) {
    FILE *fp = fopen(ORDER_FILE, "rb+"); RestaurantOrder o; int id, status, found = 0;
    if (!fp) { puts("No order file found."); return; }
    id = read_int("Order ID: ");
    status = read_int("Status (0 Pending, 1 Preparing, 2 Ready, 3 Served, 4 Cancelled): ");
    if (status < 0 || status > 4) { puts("Invalid status."); fclose(fp); return; }
    while (fread(&o, sizeof(o), 1, fp) == 1) {
        if (o.id == id) {
            o.status = status;
            fseek(fp, -(long)sizeof(o), SEEK_CUR); fwrite(&o, sizeof(o), 1, fp);
            found = 1; break;
        }
    }
    fclose(fp); puts(found ? "Order status updated." : "Order not found.");
}

void order_management(void) {
    int choice;
    do {
        puts("\n--- ORDER & KITCHEN MANAGEMENT ---");
        puts("1. Place order\n2. View order history\n3. Update kitchen/order status\n0. Back");
        choice = read_int("Choice: ");
        if (choice == 1) place_order();
        else if (choice == 2) list_orders();
        else if (choice == 3) update_status();
        else if (choice != 0) puts("Invalid choice.");
        if (choice) pause_screen();
    } while (choice != 0);
}
