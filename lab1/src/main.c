#include <stdio.h>
#include <stdlib.h>
#include "./config/menu.h"
#include "./feature/Auth/auth.h"
#include "./feature/Message/message.h"

int main() {
    int choice = 0;

    while (choice != 4) {
        menu();
        
        printf("Enter your choice(1-4): ");
        input(&choice, "int");
        
        switch (choice)
        {
        case 1:
            login();
            break;
        
        case 2: 
            postMessage();
            break;
        case 3: 
            logout();
            break;
        
        case 4: 
            logger("4", "", "+OK", "Exit successfully\n");
            exit(1);
            break;
        
        default:
            printf("Invalid choice. Please enter a valid option (1-4).\n");
            break;
        }
        choice = 0;
    }

    return 1;
};
