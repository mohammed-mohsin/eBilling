#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef _WIN32
#define CLEAR system("cls")
#else
#define CLEAR printf("\033[2J\033[1;1H")
#endif

#define PAUSE                             \
    printf("Press Enter to continue..."); \
    getchar();                            \
    getchar()

// Function prototypes
void displayMenu();
void login();
void registerUser();
void home();
void loadUsers();
void showUsers();
void payBill();
void viewBill();
double calculateBill();

// Structures
typedef struct
{
    char name[20];
    char pass[20];
    char phone[20];
    char meter[20];
    double history[12];
    double prev;
} User;

// Global variables
#define MAX_USERS 100
int userCount = 0;
int i = 0;
User users[MAX_USERS];
User currentUser = {"", "", "", "", {0}, 0};

// Function definitions
void displayMenu()
{
    CLEAR;
    char *menu =
        "\n\n\n"
        "\t\t=====================================\n"
        "\t\t== Welcome to the E-Billing System ==\n"
        "\t\t=====================================\n"
        "\t\t|    1. Login                       |\n"
        "\t\t-------------------------------------\n"
        "\t\t|    2. Register                    |\n"
        "\t\t-------------------------------------\n"
        "\t\t|    3. Exit                        |\n"
        "\t\t=====================================\n"
        "\n\t\tPlease enter your choice: ";
    for (i = 0; i < strlen(menu); i++)
    {
        printf("%c", menu[i]);
        fflush(stdout);
        usleep(5000);
    }
    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        login();
        break;
    case 2:
        registerUser();
        break;
    case 3:
        exit(EXIT_SUCCESS);
        break;
    default:
        puts("\t\tInvalid choice. Please try again.");
        usleep(2000000);
        displayMenu();
    }
}

void login()
{
    loadUsers();
    int found = 0;
    CLEAR;
    printf("\n\n\n\t\tEnter your username: ");
    scanf("%s", currentUser.name);
    printf("\t\tEnter your password: ");
    scanf("%s", currentUser.pass);
    for (i = 0; i < userCount; i++)
    {
        if (strcmp(currentUser.name, users[i].name) == 0 && strcmp(currentUser.pass, users[i].pass) == 0)
        {
            found = 1;
            currentUser = users[i];
            break;
        }
    }

    if (found)
    {
        puts("\t\tLogin successful.");
        usleep(1000000);
        home();
    }
    else
    {
        puts("\t\tLogin failed.");
        usleep(2000000);
        puts("\n\n");
        puts("\t\t=============================");
        puts("\t\t Press Enter to try again.");
        puts("\t\t Press 0 to go back to menu.");
        puts("\t\t=============================");
        printf("\t\t> ");
        getchar();
        char choice = getchar();
        // scanf("%d", &choice);
        if (choice == '0')
        {
            displayMenu();
        }
        else
        {
            login();
        }
    }
}

void registerUser()
{
    // loadUsers();
    CLEAR;
    // check if user limit is reached
    if (userCount == MAX_USERS)
    {
        puts("\t\tUser limit reached. Please try again later.");
        usleep(2000000);
        displayMenu();
        return;
    }
    // open file for appending
    FILE *fp = fopen("users.csv", "a");
    FILE *fp2 = fopen("history.csv", "a");
    if (fp == NULL || fp2 == NULL)
    {
        puts("\t\tError opening file. Please try again.");
        usleep(2000000);
        displayMenu();
    }
    // input the user details in the registry
    printf("\n\n\n\t\tEnter your username: ");
    scanf("%s", currentUser.name);
    printf("\t\tEnter your password: ");
    scanf("%s", currentUser.pass);
    printf("\t\tEnter your phone number(11 digit): ");
    scanf("%s", currentUser.phone);
    printf("\t\tEnter your meter number(10 digit): ");
    scanf("%s", currentUser.meter);
    // check if user already exists
    int found = 0;
    for (i = 0; i < userCount; i++)
    {
        if (strcmp(currentUser.name, users[i].name) == 0)
        {
            found = 1;
            break;
        }
    }
    if (found)
    {
        puts("\t\tUser already exists. Please try again.");
        usleep(2000000);
        displayMenu();
        return;
    }
    // initialize the user history
    currentUser.prev = 0.0;
    for (i = 0; i < 12; i++)
    {
        currentUser.history[i] = 0.0;
    }
    // add user to the registry
    users[userCount] = currentUser;

    // write the user details to the file
    fprintf(fp, "%s,%s,%s,%s\n", users[userCount].name, users[userCount].pass, users[userCount].phone, users[userCount].meter);
    fclose(fp);

    // initialize the user's history
    fprintf(fp2, "%s,%020.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf\n", currentUser.meter, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

    fclose(fp2);

    puts("\t\tRegistration successful.");
    usleep(2000000);
    home();
}

void home()
{
    CLEAR;
    char *menu = "\n\n\n"
                 "\t\t=====================================\n"
                 "\t\t== Welcome to the E-Billing System ==\n"
                 "\t\t=====================================\n"
                 "\t\t|    1. Pay Bills                   |\n"
                 "\t\t-------------------------------------\n"
                 "\t\t|    2. View Bills                  |\n"
                 "\t\t-------------------------------------\n"
                 "\t\t|    3. Logout                      |\n"
                 "\t\t=====================================\n"
                 "\n\t\tPlease enter your choice: ";
    for (i = 0; i < strlen(menu); i++)
    {
        printf("%c", menu[i]);
        fflush(stdout);
        usleep(5000);
    }
    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        payBill();
        break;
    case 2:
        viewBill();
        break;
    case 3:
        // logout
        displayMenu();
        break;
    default:
        puts("\t\tInvalid choice. Please try again.");
        usleep(2000000);
        home();
    }
}

void loadUsers()
{
    userCount = 0;
    // open file for reading
    FILE *fp = fopen("users.csv", "r");
    FILE *fp2 = fopen("history.csv", "r");
    if (fp == NULL || fp2 == NULL)
    {
        puts("\t\tError opening file. Please try again.");
        usleep(2000000);
        displayMenu();
    }
    char line1[100], line2[150];
    char *token;
    while (fgets(line1, 100, fp) != NULL && fgets(line2, 150, fp2) != NULL)
    {
        // get the username
        token = strtok(line1, ",");
        strcpy(users[userCount].name, token);
        // get the password
        token = strtok(NULL, ",");
        strcpy(users[userCount].pass, token);
        // get the phone number
        token = strtok(NULL, ",");
        strcpy(users[userCount].phone, token);
        // get the meter number
        token = strtok(NULL, ",");
        // remove the newline character
        token[strlen(token) - 1] = '\0';
        strcpy(users[userCount].meter, token);
        // get the history
        token = strtok(line2, ",");
        token = strtok(NULL, ",");
        users[userCount].prev = atof(token);
        for (i = 0; i < 12; i++)
        {
            token = strtok(NULL, ",");
            users[userCount].history[i] = atof(token);
        }
        userCount++;
    }
    fclose(fp);
    // showUsers();
}

void payBill()
{
    CLEAR;
    puts("\n\n\n");
    puts("\t\t=====================================");
    puts("\t\t== Welcome to the E-Billing System ==");
    puts("\t\t=====================================");
    // get the user's history
    FILE *fp = fopen("history.csv", "r+");
    if (fp == NULL)
    {
        puts("\t\tError opening file. Please try again.");
        usleep(2000000);
        exit(EXIT_FAILURE);
    }
    char line[100];
    int bytes = 0;
    char *token;
    while (fgets(line, 100, fp) != NULL)
    {
        int len = strlen(line);
        token = strtok(line, ",");
        if (strcmp(token, currentUser.meter) == 0)
        {
            break;
        }
        bytes += len;
    }
    // get the current reading
    double curr;
    printf("\t\tEnter your current reading: ");
    scanf("%lf", &curr);
    // if the current reading is less than the previous reading
    if (curr < currentUser.prev)
    {
        puts("\t\tInvalid reading. Please try again.");
        usleep(2000000);
        payBill();
    }
    // calculate the bill
    double bill = calculateBill(curr - currentUser.prev);
    currentUser.prev = curr;
    // shift the history
    double tmp;
    for (i = 0; i < 12; i++)
    {
        tmp = currentUser.history[i];
        currentUser.history[i] = bill;
        bill = tmp;
    }
    bill = currentUser.history[0];
    // update the user's history
    fseek(fp, bytes, SEEK_SET);
    fprintf(fp, "%s,%020.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf,%07.2lf", currentUser.meter, currentUser.prev, currentUser.history[0], currentUser.history[1], currentUser.history[2], currentUser.history[3], currentUser.history[4], currentUser.history[5], currentUser.history[6], currentUser.history[7], currentUser.history[8], currentUser.history[9], currentUser.history[10], currentUser.history[11]);
    fclose(fp);

    // display the bill
    printf("\t\tYour bill is: ");
    int factor = bill / currentUser.history[1];
    printf("Tk. %.2lf\n", bill);

    printf("\t\tPress any key to continue...");
    getchar();
    getchar();
    home();
}

void viewBill()
{
    CLEAR;
    puts("\n\n\n");
    puts("\t\t=====================================");
    puts("\t\t== Welcome to the E-Billing System ==");
    puts("\t\t=====~~~~~~~~~~~~~~~~~~~~~~~~~~~=====");
    puts("\t\t= Your Bill History (last 12 month) =");
    puts("\t\t=====================================");
    puts("\t\t|      Month      |       Bill      |");
    for (i = 0; i < 12; i++)
    {
        printf("\t\t|=================|=================|\n");
        printf("\t\t|\t%2d        |    %8.2lf     |\n", i + 1, currentUser.history[i]);
    }
    puts("\t\t=====================================");
    printf("\t\tPress any key to continue...");
    getchar();
    getchar();
    home();
}

double calculateBill(double units)
{
    double bill = 0.0;
    if (units <= 100)
    {
        bill = units * 1.5;
    }
    else if (units <= 200)
    {
        bill = 100 * 1.5 + (units - 100) * 2.0;
    }
    else if (units <= 300)
    {
        bill = 100 * 1.5 + 100 * 2.0 + (units - 200) * 3.0;
    }
    else if (units <= 400)
    {
        bill = 100 * 1.5 + 100 * 2.0 + 100 * 3.0 + (units - 300) * 4.0;
    }
    else if (units <= 500)
    {
        bill = 100 * 1.5 + 100 * 2.0 + 100 * 3.0 + 100 * 4.0 + (units - 400) * 5.0;
    }
    else
    {
        bill = 100 * 1.5 + 100 * 2.0 + 100 * 3.0 + 100 * 4.0 + 100 * 5.0 + (units - 500) * 6.0;
    }
    return bill;
}

void showUsers()
{
    // print all user data
    for (i = 0; i < userCount; i++)
    {
        User user = users[i];
        puts("=====================================");
        puts(user.name);
        puts(user.pass);
        puts(user.meter);
        puts(user.phone);
        printf("%lf\n", user.prev);
        for (i = 0; i < 12; i++)
        {
            printf("%lf ", user.history[i]);
        }

        puts("=====================================");
    }
    usleep(2000000);
}

#endif