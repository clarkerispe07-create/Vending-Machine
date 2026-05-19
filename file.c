#include <stdio.h>

void displayMenu() {
    printf("=================================\n");
    printf("        VENDING MACHINE          \n");
    printf("=================================\n");
    printf("=================================\n");
    printf("        Liqour Store             \n");
    printf("=================================\n");
    printf("1. San Miguel Beer       - PHP 150.00\n");
    printf("2. Red Horse             - PHP 150.00\n");
    printf("3. Alfonso               - PHP 200.00\n");
    printf("4. The Bar               - PHP 250.00\n");
    printf("5. Tanduay Select        - PHP 120.00\n");
    printf("=================================\n");
}

float calculateTotal(int choice, int quantity) {
    float price = 0;

    switch(choice) {
        case 1:
            price = 150.00;
            break;
        case 2:
            price = 150.00;
            break;
        case 3:
            price = 200.00;
            break;
        case 4:
            price = 250.00;
            break;
        case 5:
            price = 120.00;
            break;
        default:
            printf("Invalid product choice!\n");
    }

    return price * quantity;
}

void getProductName(int choice, char product[]) {
    switch(choice) {
        case 1:
            sprintf(product, "San Miguel Beer");
            break;
        case 2:
            sprintf(product, "Red Horse");
            break;
        case 3:
            sprintf(product, "Alfonso");
            break;
        case 4:
            sprintf(product, "The Bar");
            break;
        case 5:
            sprintf(product, "Tanduay Select");
            break;
        default:
            sprintf(product, "Unknown");
    }
}

void printReceipt(int choice, int quantity, float total, float money, float change) {
    char product[20];

    getProductName(choice, product);

    printf("\n=========== RECEIPT ===========\n");
    printf("Product: %s\n", product);
    printf("Quantity: %d\n", quantity);
    printf("The Total Price: PHP %.2f\n", total);
    printf("Your Cash: PHP %.2f\n", money);
    printf("Your Change: PHP %.2f\n", change);
    printf("================================\n");
    printf("Thank you for your purchase!\n");
}

int main() {
    int choice, quantity;
    float total, money, change;

    displayMenu();

    printf("Enter the product number you want to buy: ");
    scanf("%d", &choice);
    
    if(choice >5 || choice<1){
        printf("Invalid Choice");
        return 1;
    }

    printf("How many would you like to buy? ");
    scanf("%d", &quantity);

    total = calculateTotal(choice, quantity);

    printf("\nTotal amount is PHP %.2f\n", total);
    printf("Enter your money: PHP ");
    scanf("%f", &money);

    if(money < total) {
        printf("\nInsufficient money!\n");
        printf("You need PHP %.2f more.\n", total - money);
    } else {
        
        change = money - total;

        printReceipt(choice, quantity, total, money, change);
    }

    return 0;
}