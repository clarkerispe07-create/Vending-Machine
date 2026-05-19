#include <stdio.h>

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

void printReceipt(char *product, int quantity, float total, float money, float change) {
    printf("\n=========== RECEIPT ===========\n");
    printf("Product : %s\n", product);
    printf("Quantity: %d\n", quantity);
    printf("Total   : PHP %.2f\n", total);
    printf("Cash    : PHP %.2f\n", money);
    printf("Change  : PHP %.2f\n", change);
    printf("================================\n");
    printf("Thank you for your purchase!\n");
}

int main() {
    int store, choice, quantity;
    float total, money, change;
    char *product;

    displayStores();

    printf("Enter store number: ");
    scanf("%d", &store);

    char **items;
    float *prices;

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

    printf("\n=================================\n");
    printf("        AVAILABLE PRODUCTS       \n");
    printf("=================================\n");

    for (int i = 0; i < 5; i++) {
        printf("%d. %s - PHP %.2f\n", i + 1, items[i], prices[i]);
    }

    printf("=================================\n");

    printf("Enter product number: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > 5) {
        printf("Invalid product!\n");
        return 0;
    }

    printf("Enter quantity: ");
    scanf("%d", &quantity);

    total = prices[choice - 1] * quantity;

    printf("\nTotal amount is PHP %.2f\n", total);

    printf("Enter your money: PHP ");
    scanf("%f", &money);

    if (money < total) {
        printf("\nInsufficient money!\n");
        printf("You need PHP %.2f more.\n", total - money);
        return 0;
    }

    change = money - total;

    product = items[choice - 1];

    printReceipt(product, quantity, total, money, change);

    return 0;
}