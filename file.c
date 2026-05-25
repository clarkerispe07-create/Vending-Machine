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

void displayStores() {
    printf("=================================\n");
    printf("        SELECT STORE            \n");
    printf("=================================\n");
    printf("1. Soft Drinks\n");
    printf("2. Junk Foods\n");
    printf("3. Water\n");
    printf("4. Liquor Store\n");
    printf("=================================\n");
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
    printf("Thank you for your purchase!\n");
    saveToFile(product, quantity, total, money, change);
}


int main() {
    int store, choice, quantity;
    float total, money, change;
    char *product;
    
    storeMenu:

    while(1){
        int backToStoreMenu = 0;
   
    displayStores();

    printf("Enter store number: ");
    scanf("%d", &store);

    char **items;
    float *prices;

    if (store < 1 || store > 4) {
        printf("Exiting the program. Goodbye!\n");
        break;
    }

    if (store == 1) {
        items = softDrinks;
        prices = softPrices;
    } 
    else if (store == 2) {
        items = junkFoods;
        prices = junkPrices;
    } 
    else if (store == 3) {
        items = waterStore;
        prices = waterPrices;
    } 
    else if (store == 4) {
        items = liquorStore;
        prices = liquorPrices;
    }
    else {
        printf("Invalid store!\n");
        return 0;
    }

    productMenu:

    while (1){
    
    printf("\n=================================\n");
    printf("        AVAILABLE PRODUCTS       \n");
    printf("=================================\n");

    for (int i = 0; i < 5; i++) {
        printf("%d. %s - PHP %.2f\n", i + 1, items[i], prices[i]);
    }
    printf("=================================\n");
    printf("0. Back to Store Menu\n\n");

    
    printf("Enter product number: ");
    scanf("%d", &choice);

    if (choice == 0) {
      goto storeMenu;
    }
     
    if (choice < 1 || choice > 5) {
        printf("Invalid product!\n");
        continue;
    }
    quantityMenu:
    while(1){

    printf("\nEnter quantity (0 = Back to Products Menu): ");
    scanf("%d", &quantity);

    if (quantity == 0) {
         goto productMenu;
    }

    if (quantity < 0) {
        printf("Invalid quantity!\n");
        continue;
    }

    total = prices[choice - 1] * quantity;

    while(1){

    printf("\nTotal amount is PHP %.2f\n", total);
    printf("Enter your money (0 = Back to Quantity Menu): PHP ");
    scanf("%f", &money);

    if (money == 0) {
        goto quantityMenu;
    }
    
    if (money < total) {
        printf("\nInsufficient money!\n");
        printf("You need PHP %.2f more.\n", total - money);
        continue;
    }
    
    change = money - total;
    product = items[choice - 1];

    printReceipt(product, quantity, total, money, change);
    backToStoreMenu = 1;
   return 0;
    } 
    if (backToStoreMenu){        
            continue;
    }
    break;
    }  
    if (backToStoreMenu) {
    break;
    }
    if (backToStoreMenu) {
    break;
    }
        break;
        }
        break;
     }
    
    return 0;
    
}