#include <stdio.h>
#include <stdlib.h>

char *softDrinks[5] = {
    "Coke", "Sprite", "Royal", "Pepsi", "Mountain Dew"
};
float softPrices[5] = {25, 25, 25, 25, 25};

char *junkFoods[5] = {
    "Chips", "Cheetos", "Piattos", "Nova", "Pringles"
};
float junkPrices[5] = {15, 20, 18, 22, 35};

char *waterStore[5] = {
    "Bottled Water", "Mineral Water", "Distilled Water", "Sparkling Water", "Purified Water"
};
float waterPrices[5] = {10, 12, 15, 20, 18};

char *liquorStore[5] = {
    "San Miguel Beer", "Red Horse", "Alfonso", "The Bar", "Tanduay Select"
};
float liquorPrices[5] = {150, 150, 200, 250, 120};

int softStock[5] = {50, 100, 75, 80, 30};
int junkStock[5] = {56, 100, 70, 44, 200};
int waterStock[5] = {20, 15, 10, 80, 12};
int liquorStock[5] = {30, 50, 20, 50, 40};

void displayStores() {
    printf("\n==========================================\n");
    printf("               SELECT STORE            \n");
    printf("===========================================\n");
    printf("1. Soft Drinks\n");
    printf("2. Junk Foods\n");
    printf("3. Water\n");
    printf("4. Liquor Store\n");
    printf("5. Exit\n");
    printf("===========================================\n");
}

void saveToFile(char *product, int quantity, float total, float money, float change) {
    FILE *file = fopen("sales.txt", "a");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

        fprintf(file, "\n=========== RECEIPT ===========\n");
        fprintf(file, "Product : %s\n", product);
        fprintf(file, "Quantity: %d\n", quantity);
        fprintf(file, "Total   : PHP %.2f\n", total);
        fprintf(file, "Cash    : PHP %.2f\n", money);
        fprintf(file, "Change  : PHP %.2f\n", change);
        fprintf(file, "================================\n");

        fclose(file);
}

void printReceipt(char *product, int quantity, float total, float money, float change) {
    printf("\n=========== RECEIPT ===========\n");
    printf("Product : %s\n", product);
    printf("Quantity: %d\n", quantity);
    printf("Total   : PHP %.2f\n", total);
    printf("Cash    : PHP %.2f\n", money);
    printf("Change  : PHP %.2f\n", change);
    printf("================================\n");
    printf("Thank you for your purchase!\n\n");
    saveToFile(product, quantity, total, money, change);
}

void saveStock() {
    FILE *file = fopen("stock.txt", "w");

    if (file == NULL) {
        printf("Error saving stock!\n");
        return;
    }

    int *stocks[] = { softStock, junkStock, waterStock, liquorStock };
    char *names[] = { "Soft", "Junk", "Water", "Liquor" };

    for (int i = 0; i < 4; i++) {
        fprintf(file, "%s: ", names[i]);

        for (int j = 0; j < 5; j++) {
            fprintf(file, "%d ", stocks[i][j]);
        }

        fprintf(file, "\n");
    }

    fclose(file);
}

int main() {

    char **items;
    float *prices;
    int *stock;

    int store, choice, quantity;
    float total, change;
    float pocketMoney = 500.00;
    char *product;
    char buyAgain;

storeMenu:

    while (1) {

        displayStores();

       printf("Enter store number: ");

        if (scanf("%d", &store) != 1) {
             printf("\nInvalid input! Numbers only.\n");

            while (getchar() != '\n');

        continue;
        }

        if (store < 1 || store > 4) {
            printf("Exiting program...\n");
            break;
        }

        if (store == 1) {
            items = softDrinks;
            prices = softPrices;
            stock = softStock;
        }
        else if (store == 2) {
            items = junkFoods;
            prices = junkPrices;
            stock = junkStock;
        }
        else if (store == 3) {
            items = waterStore;
            prices = waterPrices;
            stock = waterStock;
        }
        else {
            items = liquorStore;
            prices = liquorPrices;
            stock = liquorStock;
        }

productMenu:

        while (1) {

            printf("\nYour Balance: PHP %.2f\n", pocketMoney);

            printf("\n===========================================\n");
            printf("            AVAILABLE PRODUCTS       \n");
            printf("===========================================\n");

            for (int i = 0; i < 5; i++) {
                if (stock[i] == 0) {
                    printf("%d. %s - OUT OF STOCK\n", i + 1, items[i]);
                } else {
                    printf("%d. %s - PHP %.2f | Stock: %d\n",
                           i + 1, items[i], prices[i], stock[i]);
                }
            }
            printf("\n===========================================\n");

            printf("0. Back to Store Menu\n");

            printf("\nEnter product number: ");
            scanf("%d", &choice);

            if (choice == 0) goto storeMenu;

            if (choice < 1 || choice > 5) {
                printf("Invalid product!\n");
                continue;
            }


                if (stock[choice - 1] == 0) {
                    int option;

                        printf("\nTHE PRODUCT IS OUT OF STOCK!\n");
                        printf("0. Cancel\n");
                        printf("1. Choose another product\n");
                        printf("2. Back to Store Menu\n");
                        printf("Enter choice: ");
                        scanf("%d", &option);

                        if (option == 1) {
                                goto productMenu;
                        } else if(option == 2){
                                goto storeMenu;
                    } else {
                         return 0;   
                    }
                }   

quantityMenu:

            while (1) {

                printf("Enter quantity (0 = back): ");
                scanf("%d", &quantity);

                if (quantity == 0) goto productMenu;

                if (quantity < 0) {
                    printf("Invalid quantity!\n");
                    continue;
                }

                if (quantity > stock[choice - 1]) {
                    printf("Only %d available!\n", stock[choice - 1]);
                    continue;
                }

                total = prices[choice - 1] * quantity;

                if (pocketMoney < total) {
                    printf("\nInsufficient money! Need PHP %.2f more\n", total - pocketMoney);
                    printf("Choose again!\n\n");
                   goto storeMenu;
                }

                float oldMoney = pocketMoney;

                product = items[choice - 1];

                printf("\n===========================================");
                printf("\n       Previous Money: PHP %.2f\n", oldMoney);
                printf("   You Buy %s (%d pcs, PHP %.2f)\n", product, quantity, total);
                printf("===========================================");


                pocketMoney -= total;
                stock[choice - 1] -= quantity;
                saveStock();

                float change = oldMoney - total;

                printf("\n       Remaining Money: PHP %.2f\n", pocketMoney);
                printf("   Remaining Stock: %s = %d pcs\n",product, stock[choice - 1]);
                printf("===========================================\n");
                printReceipt(product, quantity, total, oldMoney, change);

               printf("\nDo you want to buy again? (Y/N):  ");
                scanf(" %c", &buyAgain);

                    if (buyAgain == 'Y' || buyAgain == 'y') {
                                goto storeMenu;
                    }
                        else {
                            printf("\nThank you for using the vending machine!\n");
                                return 0;
                }
            }
        }
    }

    return 0;
}