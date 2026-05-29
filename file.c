#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #define CLEAR_SCREEN() system("cls")
#else
    #define CLEAR_SCREEN() system("clear")
#endif

#define MAX_ITEMS_PER_STORE 100
#define BUFFER_SIZE 256

typedef struct {
    char name[128];
    float price;
    int stock;
} Product;

static Product active_products[MAX_ITEMS_PER_STORE];
static int active_product_count = 0;

static float loadMoney(void);
static void saveMoney(float pocketMoney);
static void saveInventory(const char *product, int quantity);
static void showInventory(void);
static void displayStores(void);
static void saveToFile(char *product, int quantity, float total, float money, float change);
static void printReceipt(char *product, int quantity, float total, float money, float change);
static void clearInputBuffer(void);

static void getStoreFileName(int storeNum, char *dest);
static void loadStoreProducts(const char *filename);
static void saveStoreProducts(const char *filename);

static void trim_newline(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) {
        s[n - 1] = '\0';
        n--;
    }
}

static void trim_whitespace(char *s) {
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;

    if (start != s) memmove(s, start, strlen(start) + 1);

    size_t n = strlen(s);
    while (n > 0 && isspace((unsigned char)s[n - 1])) {
        s[n - 1] = '\0';
        n--;
    }
}

static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void getStoreFileName(int storeNum, char *dest) {
    switch (storeNum) {
        case 1: strcpy(dest, "store1_soft_drinks.txt"); break;
        case 2: strcpy(dest, "store2_junk_foods.txt"); break;
        case 3: strcpy(dest, "store3_water.txt"); break;
        case 4: strcpy(dest, "store4_liquor.txt"); break;
        default: strcpy(dest, ""); break;
    }
}

static void loadStoreProducts(const char *filename) {
    active_product_count = 0;
    memset(active_products, 0, sizeof(active_products));

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        file = fopen(filename, "w");
        if (file != NULL) {
            if (strcmp(filename, "store1_soft_drinks.txt") == 0) {
                fprintf(file, "ColdBrew,45.00,12\nMatchaLatte,55.00,10\nIcedAmericano,40.00,15\nSodaCranberry,30.00,20\nMineralWater,15.00,30\n");
            } else if (strcmp(filename, "store2_junk_foods.txt") == 0) {
                fprintf(file, "TruffleChips,35.00,15\nLoadedNachos,60.00,8\nMozzSticks,50.00,10\nSaltedPretzel,25.00,12\nGarlicBread,20.00,15\n");
            } else if (strcmp(filename, "store3_water.txt") == 0) {
                fprintf(file, "PurifiedO2,15.00,25\nAlkalineWater,25.00,15\nDistilledH2O,12.00,30\nSparklingWater,30.00,10\nVitaminWater,35.00,12\n");
            } else if (strcmp(filename, "store4_liquor.txt") == 0) {
                fprintf(file, "CraftBeer,75.00,12\nRedWineGlass,120.00,6\nWhiskeyShot,90.00,15\nClassicGin,80.00,10\nVodkaTonic,85.00,8\n");
            }
            fclose(file);
            file = fopen(filename, "r");
        }
        if (file == NULL) return;
    }

    while (active_product_count < MAX_ITEMS_PER_STORE) {
        char line[256];
        if (!fgets(line, sizeof(line), file)) break;

        trim_newline(line);
        if (line[0] == '\0') continue;

        char name[128] = {0};
        float price = 0.0f;
        int stock = 0;

        int matched = sscanf(line, " %127[^,] , %f , %d", name, &price, &stock);
        if (matched != 3) continue;

        trim_whitespace(name);

        strncpy(active_products[active_product_count].name, name, sizeof(active_products[active_product_count].name) - 1);
        active_products[active_product_count].name[sizeof(active_products[active_product_count].name) - 1] = '\0';
        active_products[active_product_count].price = price;
        active_products[active_product_count].stock = stock;
        active_product_count++;
    }
    fclose(file);
}

static void saveStoreProducts(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf(" ✖ Error saving store file: %s!\n", filename);
        return;
    }
    for (int i = 0; i < active_product_count; i++) {
        fprintf(file, "%s,%.2f,%d\n", active_products[i].name, active_products[i].price, active_products[i].stock);
    }
    fclose(file);
}

static float loadMoney(void) {
    FILE *file = fopen("money.txt", "r");
    if (file == NULL) {
        float defaultMoney = 500.00f;
        file = fopen("money.txt", "w");
        if (file != NULL) {
            fprintf(file, "%.2f\n", defaultMoney);
            fclose(file);
        }
        return defaultMoney;
    }
    float pocketMoney = 0.0f;
    if (fscanf(file, "%f", &pocketMoney) != 1) pocketMoney = 500.00f;
    fclose(file);
    return pocketMoney;
}

static void saveMoney(float pocketMoney) {
    FILE *file = fopen("money.txt", "w");
    if (file == NULL) {
        printf(" ✖ Error saving money.txt!\n");
        return;
    }
    fprintf(file, "%.2f\n", pocketMoney);
    fclose(file);
}

/* ====================================================================
   REVISED: ORGANIZED INVENTORY LOGGER (Combines matching items)
==================================================================== */
static void saveInventory(const char *product, int quantity) {
    char items[200][128];
    int counts[200] = {0};
    int totalItems = 0;
    int found = 0;

    FILE *file = fopen("inventory.txt", "r");
    if (file != NULL) {
        while (totalItems < 200 && fscanf(file, " %127[^|] | %d\n", items[totalItems], &counts[totalItems]) == 2) {
            trim_whitespace(items[totalItems]);
            if (strcmp(items[totalItems], product) == 0) {
                counts[totalItems] += quantity;
                found = 1;
            }
            totalItems++;
        }
        fclose(file);
    }

    if (!found && totalItems < 200) {
        strncpy(items[totalItems], product, sizeof(items[totalItems]) - 1);
        items[totalItems][sizeof(items[totalItems]) - 1] = '\0';
        counts[totalItems] = quantity;
        totalItems++;
    }

    file = fopen("inventory.txt", "w");
    if (file == NULL) return;

    for (int i = 0; i < totalItems; i++) {
        fprintf(file, "%s|%d\n", items[i], counts[i]);
    }
    fclose(file);
}

/* ====================================================================
   REVISED: CLEAN GRID-ALIGNED INVENTORY VIEWER
==================================================================== */
static void showInventory(void) {
    float currentBalance = loadMoney();
    FILE *file = fopen("inventory.txt", "r");

    CLEAR_SCREEN(); 
    printf(" ════════════════ USER UTILITY CONSOLE ════════════════\n");
    printf(" 💳 CURRENT REMAINING BALANCE: PHP %.2f\n", currentBalance);
    printf(" ──────────────────────────────────────────────────────\n");
    printf(" 📥 SECURE OWNED INVENTORY LOG:\n");
    printf(" ┌──────────────────────────────┬──────────────────────┐\n");
    printf(" │ PRODUCT NAME                 │ QUANTITY OWNED       │\n");
    printf(" ├──────────────────────────────┼──────────────────────┤\n");

    if (file == NULL) {
        printf(" │ %-28s │ %-20s │\n", "No assets acquired yet.", "0 pcs");
    } else {
        char name[128];
        int qty;
        int rows = 0;

        while (fscanf(file, " %127[^|] | %d\n", name, &qty) == 2) {
            trim_whitespace(name);
            char qtyStr[32];
            sprintf(qtyStr, "%dx", qty);
            printf(" │ %-28s │ %-20s │\n", name, qtyStr);
            rows++;
        }
        fclose(file);

        if (rows == 0) {
            printf(" │ %-28s │ %-20s │\n", "No assets acquired yet.", "0 pcs");
        }
    }

    printf(" └──────────────────────────────┴──────────────────────┘\n");
    printf(" ══════════════════════════════════════════════════════\n");
    printf(" Input [0] to close core viewer panel: ");
    int back; 
    if (scanf("%d", &back) != 1) clearInputBuffer();
    clearInputBuffer();
}

void displayStores(void) {
    printf(" ═════════════════════════════════════════════════════\n");
    printf("                  SELECT STORE                        \n");
    printf(" ═════════════════════════════════════════════════════\n");
    printf("   📂 [1] Soft Drinks Store\n");
    printf("   📂 [2] Junk Foods Store\n");
    printf("   📂 [3] Water Store\n");
    printf("   📂 [4] Liquor Store\n");
    printf("   📂 [5] Show Inventory\n");
    printf("   📂 [6] Exit\n");
    printf(" ─────────────────────────────────────────────────────\n");
}

void saveToFile(char *product, int quantity, float total, float money, float change) {
    FILE *file = fopen("sales.txt", "a");
    if (file == NULL) return;
    fprintf(file, "\n=========== RECEIPT ===========\nProduct : %s\nQuantity: %d\nTotal   : PHP %.2f\nCash    : PHP %.2f\nChange  : PHP %.2f\n================================\n", product, quantity, total, money, change);
    fclose(file);
}

void printReceipt(char *product, int quantity, float total, float money, float change) {
    printf("\n  ✔ TRANSACTION ACCEPTED\n");
    printf(" ─────────────────────────────────────────────────────\n");
    printf("  Purchased Asset: %s (x%d)\n", product, quantity);
    printf("  Total Fees     : PHP %.2f\n", total);
    printf("  Cash Tendered  : PHP %.2f\n", money);
    printf("  Updated Change : PHP %.2f\n", change);
    printf(" ─────────────────────────────────────────────────────\n\n");
    saveToFile(product, quantity, total, money, change);
}

int main() {
    int store, choice, quantity;
    float total, change;
    char storeFileName[64];

    storeMenu:
    while (1) {
        CLEAR_SCREEN(); 
        displayStores();

        printf(" Enter store number: ");
        if (scanf("%d", &store) != 1) {
            printf("\n ✖ Invalid input! Numbers only.\n");
            clearInputBuffer(); getchar(); continue;
        }
        clearInputBuffer();

        if (store == 5) {
            showInventory();
            continue; 
        }
        if (store < 1 || store > 6) {
             goto storeMenu;
        }
        if (store == 6) {
            printf(" Exiting program...\n"); break;
        }

        getStoreFileName(store, storeFileName);
        int insideProductMenu = 1;

        while (insideProductMenu) {
            loadStoreProducts(storeFileName);
            float pocketMoney = loadMoney();

            CLEAR_SCREEN(); 
            printf(" 📑 ACTIVE MENU: ");
            switch(store) {
                case 1: printf("Soft Drinks Store\n"); break;
                case 2: printf("Junk Foods Store\n"); break;
                case 3: printf("Water Store\n"); break;
                case 4: printf("Liquor Store\n"); break;
            }
            
            printf(" 💳 Your Balance: PHP %.2f\n", pocketMoney);
            printf(" ┌─────┬──────────────────────┬─────────────┬──────────────┐\n");
            printf(" │ ID  │ AVAILABLE PRODUCTS   │ PRICE       │ STOCK STATUS │\n");
            printf(" ├─────┼──────────────────────┼─────────────┼──────────────┤\n");

            for (int i = 0; i < active_product_count; i++) {
                Product *p = &active_products[i];
                if (p->stock == 0) {
                    printf(" │ %-3d │ %-20s │ --          │ OUT OF STOCK │\n", i + 1, p->name);
                } else {
                    printf(" │ %-3d │ %-20s │ PHP %-7.2f │ %02d pcs      │\n", i + 1, p->name, p->price, p->stock);
                }
            }
            printf(" └─────┴──────────────────────┴─────────────┴──────────────┘\n");
            printf("  ↩️  [0] Back to Store Menu\n\n");

            printf(" Enter product number: ");
            if (scanf("%d", &choice) != 1) {
                clearInputBuffer();
                printf(" ✖ Selection must be an item number.\n");
                getchar(); continue;
            }
            clearInputBuffer();

            if (choice == 0) {
                insideProductMenu = 0;
                break;
            }
            if (choice < 1 || choice > active_product_count) {
                printf(" Invalid product!\n");
                getchar(); continue;
            }

            Product *selected = &active_products[choice - 1];

            if (selected->stock == 0) {
                int option;
                printf("\n THE PRODUCT IS OUT OF STOCK!\n[0] Cancel\n[1] Choose another product\n[2] Back to Store Menu\nEnter choice: ");
                if (scanf("%d", &option) != 1) clearInputBuffer();
                clearInputBuffer();

                if (option == 1) continue; 
                else if (option == 2) { insideProductMenu = 0; break; }
                else return 0;
            }

            int insideQuantityMenu = 1;
            while (insideQuantityMenu) {
                printf(" Enter quantity (0 = back): ");
                if (scanf("%d", &quantity) != 1) {
                    printf("\n ✖ Invalid input! Numbers only.\n");
                    clearInputBuffer(); continue;
                }
                clearInputBuffer();

                if (quantity == 0) { insideQuantityMenu = 0; break; }
                if (quantity < 0) { printf(" Invalid quantity!\n"); continue; }
                if (quantity > selected->stock) { printf(" Only %d available!\n", selected->stock); continue; }

                total = selected->price * quantity;

                if (pocketMoney < total) {
                    printf("\n Insufficient money! Need PHP %.2f more\n Choose again!\n\n Press Enter...", total - pocketMoney); 
                    getchar(); insideQuantityMenu = 0; insideProductMenu = 0; break;
                }

                float oldMoney = pocketMoney;
                pocketMoney -= total;
                selected->stock -= quantity;

                saveInventory(selected->name, quantity);
                saveMoney(pocketMoney);
                saveStoreProducts(storeFileName);

                change = oldMoney - total;
                
                printf("\n===========================================");
                printf("\n       Previous Money: PHP %.2f\n", oldMoney);
                printf("   You Buy %s (%d pcs, PHP %.2f)\n", selected->name, quantity, total);
                printf("===========================================\n");
                printf("\n       Remaining Money: PHP %.2f\n", pocketMoney);
                printf("   Remaining Stock: %s = %d pcs\n", selected->name, selected->stock);
                printf("===========================================\n");
                
                printReceipt(selected->name, quantity, total, oldMoney, change);
                goto storeMenu;
            }
        }
    }
    return 0;
}