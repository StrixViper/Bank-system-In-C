#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

struct DateOfBirth
{
    int day;
    int month;
    int year;
};

struct Register_Details
{
    char fname[30];
    char lname[30];
    char address[40];
    int phoneNumber;
    char email[50];
    struct DateOfBirth dob;
    char password[30];
    char userName[20];
    struct tm lastLoginTime;
    double balance;
    char coinType[20];
};
struct Transaction {
    double amount;
    double balance_before;
    double balance_after;
    char type[20];
    char date[20];
};


struct Transfer_Details
{
    char senderUserName[20];
    char receiverUserName[20];
    double transferAmount;
};
// Function to log a transaction and save it to a file
void logTransaction(struct Register_Details *user, struct Transaction transaction) {
    FILE *file = fopen("transaction_history.txt", "a"); // Open file in append mode

    if (file != NULL) {
        // Get the current date and time
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        strftime(transaction.date, sizeof(transaction.date), "%Y-%m-%d %H:%M:%S", tm_info);

        // Write transaction details to the file
        fprintf(file, "Username: %s\n", user->userName);
        fprintf(file, "Transaction Amount: %.2f\n", transaction.amount);
        fprintf(file, "Transaction Type: %s\n", transaction.type);
        fprintf(file, "Balance Before: %.2f\n", transaction.balance_before);
        fprintf(file, "Balance After: %.2f\n", transaction.balance_after);
        fprintf(file, "Date: %s\n", transaction.date);
        fprintf(file, "\n");

        // Close the file
        fclose(file);
    } else {
        printf("Error opening file for transaction history.\n");
    }
}


//-----------------------------------------------------//

void logDeposit(struct Register_Details *user, double amount) {
    struct Transaction transaction;
    transaction.amount = amount;
    transaction.balance_before = user->balance;
    transaction.balance_after = user->balance + amount;
    strcpy(transaction.type, "Deposit");

    // Log the deposit transaction
    logTransaction(user, transaction);

    // Update the user's balance
    user->balance = transaction.balance_after;
}

void logWithdrawal(struct Register_Details *user, double amount) {
    struct Transaction transaction;
    transaction.amount = amount;
    transaction.balance_before = user->balance;
    transaction.balance_after = user->balance - amount;
    strcpy(transaction.type, "Withdrawal");

    // Log the withdrawal transaction
    logTransaction(user, transaction);

    // Update the user's balance
    user->balance = transaction.balance_after;
}

//------------------------------------------------------------//

void printUserTransactionHistory(const char* username) {
    FILE *file = fopen("transaction_history.txt", "r");

    if (file != NULL) {
        printf("\nTransaction History for %s:\n", username);

        char line[100];  // Adjust the buffer size as needed
        while (fgets(line, sizeof(line), file) != NULL) {
            // Check if the line contains the specified username
            if (strstr(line, "Username") != NULL && strstr(line, username) != NULL) {
                printf("%s", line);

                // Print the remaining lines of the transaction entry
                for (int i = 0; i < 4; ++i) {
                    fgets(line, sizeof(line), file);
                    printf("%s", line);
                }

                printf("\n");
            }
        }

        fclose(file);
    } else {
        printf("Error opening file for reading transaction history.\n");
    }
}

//------------------------------------------------//

void TermsOfUse() {
    printf("\n===== Welcome to the Terms of Use =====\n\n");

    printf("By accessing and using this service, you agree to comply with and be bound by the following terms and conditions.\n");
    printf("Please read these terms carefully before using the service.\n\n");

    printf("1. Acceptance of Terms\n");
    printf("By using this service, you signify your agreement to these terms. If you do not agree to these terms, please do not use the service.\n\n");

    printf("2. User Responsibilities\n");
    printf("You are responsible for maintaining the confidentiality of your account and password and for restricting access to your computer.\n");
    printf("You agree to accept responsibility for all activities that occur under your account or password.\n\n");

    printf("3. Intellectual Property\n");
    printf("All content included on this service, such as text, graphics, logos, images, and software, is the property of the service provider.\n");
    printf("You may not reproduce, distribute, display, or perform any part of this content without prior written permission from the service provider.\n\n");

    printf("4. Limitation of Liability\n");
    printf("The service provider shall not be liable for any direct, indirect, incidental, special, or consequential damages resulting from the use or inability to use the service.\n");
    printf("This includes but is not limited to damages for loss of profits, business interruption, or loss of data.\n\n");

    printf("5. Changes to Terms\n");
    printf("The service provider reserves the right to modify these terms at any time. It is your responsibility to review these terms periodically for changes.\n\n");

    printf("6. Adualt Access\n");
    printf("The service provider determine the you must be above 18 to use our services\n\n");

    printf("7. Account Permission Acception\n");
    printf("The system can decide when to delete or ban your account any time it want!\n\n");

    printf("8. Monthly charging\n");
    printf("The bank system can and will charge your account each month 5 dollars for each account to use our services\n\n");

    printf("Thank you for reviewing our Terms of Use. If you have any questions, please contact us.\n");

    printf("\n===== End of Terms of Use =====\n\n");
}

//----------------------------------------//

bool isValidEmail(const char *email) {

    if (email[0] == '\0') {
        return false;
    }


    char *atSymbol = strchr(email, '@');
    if (atSymbol == NULL) {
        return false;
    }


    char *dotSymbol = strchr(atSymbol, '.');
    if (dotSymbol == NULL) {
        return false;
    }


    if (email[strlen(email) - 1] == '.') {
        return false;
    }

    return true;
}

//-------------------------------------------//

int is_good_password(const char *password) {
    int length = 0;
    int has_digit = 0;
    int has_letter = 0;

    while (password[length] != '\0') {
        if (isdigit(password[length])) {
            has_digit = 1;
        } else if (isalpha(password[length])) {
            has_letter = 1;
        }

        length++;

        if (length > 30) {
            return 0;
        }
    }

    if (length < 8 || !has_digit || !has_letter) {
        return 0;
    }

    return 1;
}

//-------------------------------------------//

bool Is_Age_Adult(int birthYear) {
    time_t t = time(NULL);
    struct tm *current_time = localtime(&t);
    int currentYear = current_time->tm_year + 1900;

    return (currentYear - birthYear) > 18;
}

//-------------------------------------------//

void printDetails(const struct Register_Details *person, const struct tm *registrationDate);

//-----------------------------------------------///Register The User  and save his Details

void Register(struct Register_Details *users, int *numUsers) {
    struct Register_Details person;

    printf("Insert your first name: ");
    scanf("%s", person.fname);

    printf("Insert your last name: ");
    scanf("%s", person.lname);

    printf("Insert your address: ");
    scanf("%s", person.address);

    printf("Insert your phone number: ");
    scanf("%d", &person.phoneNumber);
//password input check if password is valid password
    do {
        printf("Insert your password (it must be between 8-30 characters and contain digits and letters): ");
        scanf("%s", person.password);
    } while (!is_good_password(person.password));

    do {
        printf("Insert your email address: ");
        scanf("%s", person.email);

        if (!isValidEmail(person.email)) {
            printf("Invalid email address. Please enter a valid email.\n");
        }
    } while (!isValidEmail(person.email));

    // Date of birth input
    do {
        printf("Insert the day of your birth: ");
        scanf("%d", &person.dob.day);

        if (person.dob.day < 1 || person.dob.day > 31) {
            printf("Invalid day! Please enter a valid day (1-31).\n");
        }
    } while (person.dob.day < 1 || person.dob.day > 31);

    do {
        printf("Insert the month of your birth: ");
        scanf("%d", &person.dob.month);

        if (person.dob.month < 1 || person.dob.month > 12) {
            printf("Invalid month! Please enter a valid month (1-12).\n");
        }
    } while (person.dob.month < 1 || person.dob.month > 12);

    do {
        printf("Insert the year of your birth: ");
        scanf("%d", &person.dob.year);

        if (!Is_Age_Adult(person.dob.year)) {
            printf("Invalid age, you MUST be above 18 to continue\n");
        }
    } while (!Is_Age_Adult(person.dob.year));

    printf("Insert your UserName:\n");
    scanf("%s",person.userName);

    users[*numUsers] = person;
    (*numUsers)++;

    time_t t = time(NULL);
    struct tm *current_time = localtime(&t);

    FILE *file = fopen("user_registration_details.csv", "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        return;
    }

    fprintf(file, "Name: %s %s\n", person.fname, person.lname);
    fprintf(file, "Address: %s\n", person.address);
    fprintf(file, "Phone Number: %d\n", person.phoneNumber);
    fprintf(file, "Email: %s\n", person.email);
    fprintf(file, "Date of Birth: %02d/%02d/%04d\n", person.dob.month, person.dob.day, person.dob.year);
    fprintf(file, "Registration Date: %02d/%02d/%04d\n", current_time->tm_mon + 1, current_time->tm_mday, current_time->tm_year + 1900);
    fprintf(file,"UserName: %20s\n",person.userName);

    fprintf(file, "\n");

    fclose(file);
    person.balance=0.0;
    printf("\nRegistration Details saved in system.\n");

    int show_details_option;
    printf("Would You like to See your Detailsc Now?\n0:NO\n1:YES\n");
    scanf("%d",&show_details_option);
    switch (show_details_option) {
        case 0:
            printf("Have A good day, thank you for your registration");
            break;
        case 1:
            printDetails(&person, current_time);
            break;
    }


}

//-------------------------------------------//

void printDetails(const struct Register_Details *person, const struct tm *registrationDate) {
    printf("Name: %s %s\n", person->fname, person->lname);
    printf("Address: %s\n", person->address);
    printf("Phone Number: %d\n", person->phoneNumber);
    printf("Email: %s\n", person->email);
    printf("Date of Birth: %02d/%02d/%04d\n", person->dob.month, person->dob.day, person->dob.year);
    printf("Registration Date: %02d/%02d/%04d\n", registrationDate->tm_mon + 1, registrationDate->tm_mday, registrationDate->tm_year + 1900);
    printf("UserName: %s\n",person->userName);
    printf("Balance %lf\n",person->balance);

}

//-------------------------------------------//

int isValidLogin(const char *username, const char *password, const struct Register_Details *users, int numUsers) {
    for (int i = 0; i < numUsers; ++i) {
        if (strcmp(username, users[i].userName) == 0 && strcmp(password, users[i].password) == 0) {
            return 1; // Valid login
        }
    }
    return 0; // Invalid login
}

//-------------------------------------------//

void updateBalanceInFile(const struct Register_Details *user) {
    FILE *file = fopen("user_registration_details.csv", "r+");
    if (file != NULL) {
        char line[1000];
        while (fgets(line, sizeof(line), file) != NULL) {
            if (strstr(line, user->userName) != NULL) {
                fseek(file, ftell(file) + strlen("Balance: "), SEEK_SET);
                fprintf(file, "%.2f\n", user->balance);
                break;
            }
        }
        fclose(file);
    }
}

//-------------------------------------------//


void Deposit(struct Register_Details *user) {
    double amount;
    printf("Insert the amount you want to deposit: ");
    scanf("%lf", &amount);

    if (amount < 0) {
        printf("Invalid deposit amount. Please enter a non-negative value.\n");
        return;
    }

    user->balance += amount;
    printf("Deposit successful. New balance: %.2f\n", user->balance);

    // Log the deposit transaction
    logDeposit(user, amount);

    // Update the balance in the file
    updateBalanceInFile(user);
}

void withdraw(struct Register_Details *user) {
    double amount;
    printf("Enter the withdrawal amount: ");
    scanf("%lf", &amount);

    if (amount < 0) {
        printf("Invalid withdrawal amount. Please enter a non-negative value.\n");
        return;
    }

    if (amount > user->balance) {
        printf("Insufficient funds. Your current balance is %.2f.\n", user->balance);

        char choice[3];
        printf("Do you still want to proceed and have a negative balance? (yes/no): ");
        scanf("%2s", choice);

        if (strcasecmp(choice, "yes") != 0) {
            printf("Withdrawal canceled.\n");
            return;
        }
    }

    user->balance -= amount;
    printf("Withdrawal successful. New balance: %.2f\n", user->balance);

    // Log the withdrawal transaction
    logWithdrawal(user, amount);

    // Update the balance in the file
    updateBalanceInFile(user);
}

//-------------------------------------------//

void checkBalance(const struct Register_Details *user) {
    printf("Current Balance: %.2f\n", user->balance);
}

void generateRandomCardNumber(char *cardNumber) {
    // Seed the random number generator with the current time
    srand(time(NULL));

    // Generate a random 16-digit card number
    for (int i = 0; i < 16; i++) {
        cardNumber[i] = '0' + rand() % 10;
    }

    // Null-terminate the string
    cardNumber[16] = '\0';
}

void printCreditCard(const char *name) {
    // Open the file for writing, using the desired file name
    FILE *file = fopen("creditcarddetails.csv", "w");

    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int expiryYear = tm.tm_year + 1900 + 3;
    char expiryDate[12];
    sprintf(expiryDate, "%02d/%04d", tm.tm_mon + 1, expiryYear);

    char cardNumber[17];
    generateRandomCardNumber(cardNumber);

    fprintf(file, "==============================================\n");
    fprintf(file, "|               CREDIT CARD                  |\n");
    fprintf(file, "|                                            |\n");
    fprintf(file, "| Cardholder: %-30s |\n", name);
    fprintf(file, "| Card Number: %-26s    |\n", cardNumber);
    fprintf(file, "| Expiry Date: %-24s      |\n", expiryDate);
    fprintf(file, "|                                            |\n");
    fprintf(file, "==============================================\n");

    // Close the file
    fclose(file);
}

void myAccount(const struct Register_Details *user)
{
    printf("\n===== My Account Details =====\n");
    printDetails(user, &user->lastLoginTime);
    printf("==============================\n");
}

//-------------------------------------------//

void deleteAccount(struct Register_Details *users, int *numUsers, const char *username)
{
    int i;
    int found = 0;

    // Search for the user with the specified username
    for (i = 0; i < *numUsers; ++i)
    {
        if (strcmp(users[i].userName, username) == 0)
        {
            found = 1;
            break;
        }
    }

    if (found)
    {
        // Open the original file in read mode
        FILE *originalFile = fopen("user_registration_details.csv", "r");
        if (originalFile == NULL)
        {
            fprintf(stderr, "Error opening original file for reading.\n");
            return;
        }

        // Open a temporary file in write mode
        FILE *tempFile = fopen("temp_user_registration_details.csv", "w");
        if (tempFile == NULL)
        {
            fprintf(stderr, "Error opening temporary file for writing.\n");
            fclose(originalFile);
            return;
        }

        // Copy all non-deleted users to the temporary file
        for (int k = 0; k < *numUsers; ++k)
        {
            if (k != i)
            {
                fprintf(tempFile, "Name: %s %s\n", users[k].fname, users[k].lname);
                // ... (other fields and details)
                fprintf(tempFile, "Last Login Time: %02d/%02d/%04d %02d:%02d:%02d\n",
                        users[k].lastLoginTime.tm_mon + 1, users[k].lastLoginTime.tm_mday,
                        users[k].lastLoginTime.tm_year + 1900, users[k].lastLoginTime.tm_hour,
                        users[k].lastLoginTime.tm_min, users[k].lastLoginTime.tm_sec);
                fprintf(tempFile, "UserName: %s\n", users[k].userName);
                fprintf(tempFile, "Balance: %.2f\n", users[k].balance);
                fprintf(tempFile, "\n");
            }
        }

        // Close both files
        fclose(originalFile);
        fclose(tempFile);

        // Replace the original file with the temporary file
        if (rename("temp_user_registration_details.csv", "user_registration_details.csv") != 0)
        {
            fprintf(stderr, "Error renaming temporary file.\n");
            return;
        }

        (*numUsers)--;
        printf("Account deleted successfully.\n");
    }
    else
    {
        printf("Account not found. Deletion failed.\n");
    }
}


//----------------------------------------------------//

void coinType(struct Register_Details *user) {
    int coinTypeOption;

    printf("Select your coin type:\n");
    printf("1: Dollar\n2: ILS\n3: Euro\n");
    scanf("%d", &coinTypeOption);

    switch (coinTypeOption) {
        case 1:
            strcpy(user->coinType, "Dollar");
            user->balance *=3.8;// Convert balance to Dollar
            break;
        case 2:
            strcpy(user->coinType, "ILS");
            break;
        case 3:
            strcpy(user->coinType, "Euro");
            user->balance *=4;    // Convert balance to Euro
            break;
        default:
            printf("Invalid option. Defaulting to ILS.\n");
            strcpy(user->coinType, "ILS");
    }

    printf("Coin type updated to: %s\n", user->coinType);
}

// Example conversion rates (adjust these as needed)
#define DOLLAR_TO_ILS 3.2
#define EURO_TO_ILS 3.8

//----------------------------------------------------//

void performTransfer(struct Register_Details *sender, struct Register_Details *receiver, double amount)
{
    // Check if the sender has sufficient balance
    if (sender->balance < amount)
    {
        printf("Insufficient funds for the transfer.\n");
        return;
    }

    // Update balances
    sender->balance -= amount;
    receiver->balance += amount;

    // Log the transactions for both sender and receiver
    struct Transaction senderTransaction = {.amount = amount, .balance_before = sender->balance + amount, .balance_after = sender->balance, .type = "Transfer (Sent)"};
    struct Transaction receiverTransaction = {.amount = amount, .balance_before = receiver->balance - amount, .balance_after = receiver->balance, .type = "Transfer (Received)"};

    logTransaction(sender, senderTransaction);
    logTransaction(receiver, receiverTransaction);

    // Update the balances in the file
    updateBalanceInFile(sender);
    updateBalanceInFile(receiver);

    printf("Transfer successful. New balance: %.2f\n", sender->balance);
}

void transfer(struct Register_Details *users, int numUsers, const char *senderUserName)
{
    char receiverUserName[20];
    double amount;

    printf("Enter the username of the recipient: ");
    scanf("%s", receiverUserName);

    // Check if the sender exists
    int senderIndex = -1;
    for (int i = 0; i < numUsers; ++i)
    {
        if (strcmp(users[i].userName, senderUserName) == 0)
        {
            senderIndex = i;
            break;
        }
    }

    if (senderIndex == -1)
    {
        printf("Sender not found.\n");
        return;
    }

    // Check if the receiver exists
    int receiverIndex = -1;
    for (int i = 0; i < numUsers; ++i)
    {
        if (strcmp(users[i].userName, receiverUserName) == 0)
        {
            receiverIndex = i;
            break;
        }
    }

    if (receiverIndex == -1)
    {
        printf("Receiver not found.\n");
        return;
    }

    // Input transfer amount
    printf("Enter the amount to transfer: ");
    scanf("%lf", &amount);

    // Perform the transfer
    performTransfer(&users[senderIndex], &users[receiverIndex], amount);
}

///------------------------///
void colorMode()
{
    int coloroption;
    printf("1:dark mode: \n2:light mode: \n");
    scanf("%d",&coloroption);

    if(coloroption==1)
    {
        system("color f0");
    }
    else if(coloroption==2)
    {
        system("color 7");
    }
    else
    { printf("insert a valid option");}

}

//---------------------------------------------------//
void Settings(struct Register_Details *user, int *numUsers)
{
    int settingUserOption;
    printf("1: my account\n2: delete my account\n3: coin type\n4: show my credit card\n");
    scanf("%d", &settingUserOption);

    switch (settingUserOption)
    {
        case 1:
            myAccount(user);
            break;
        case 2:
            deleteAccount(user, numUsers, user->userName);
            break;
        case 3:
            coinType(user);
            break;
        case 4:
            printCreditCard(user->fname);
            break;
        case 5:
            colorMode();
            break;

        default:
            printf("Invalid option.\n");
    }
}


//---------------------------------------//
void ContactUs()
{
    printf("OUR CONTACTING WAY'S\n");
    printf("phone: 0503334444\n");
    printf("email:nyduo.buisness@gmail.com\n");
    printf("our adress:hayarkon 179\n");
    printf("join our discord server for buisness:N.Y DUO\n");
}

//------------------------------------------------//

void AboutUs()
{
    printf("Welcome to Noam Bank!\n");
    printf("At Noam Bank, we are committed to providing you with reliable and secure financial services.\n");
    printf("With a legacy of trust and customer satisfaction, we strive to meet your banking needs efficiently.\n");
    printf("Our dedicated team of professionals works tirelessly to ensure the safety and confidentiality of your transactions.\n");
    printf("Noam Bank offers a wide range of services, including savings accounts, loans, and investment options tailored to your financial goals.\n");
    printf("We embrace innovation, using cutting-edge technology to enhance the banking experience for our customers.\n");
    printf("Your financial well-being is our priority, and we take pride in being a partner in your journey towards financial success.\n");
    printf("Whether you are a new customer or a longtime client, we value your trust and are committed to earning it every day.\n");
    printf("At Noam Bank, we believe in transparency and open communication, fostering a relationship built on mutual respect.\n");
    printf("Thank you for choosing Noam Bank as your financial partner. We look forward to serving you and helping you achieve your financial dreams.\n");
    printf("\n");
    printf("Noam Bank - Your Bridge to Financial Prosperity!\n");
}


//------------------------------------------------//
void displayMenu(struct Register_Details *users, int numUsers, int currentUserIndex) {
    int menuOption;

    printf("\nWelcome, %s %s!\n", users[currentUserIndex].fname, users[currentUserIndex].lname);

    do {
        printf("\n===== Menu =====\n");
        printf("1: Deposit\n");
        printf("2: Withdrawal\n");
        printf("3: Transfer\n");
        printf("4: Check Balance\n");
        printf("5: Generate Credit Card\n");
        printf("6: History\n");
        printf("7: Settings\n");
        printf("8: Contact us\n");
        printf("9: About Us\n");
        printf("0: Logout\n");
        printf("Choose an option: ");
        scanf("%d", &menuOption);

        switch (menuOption) {
            case 1:
                Deposit(&users[currentUserIndex]);
                break;
            case 2:
                withdraw(&users[currentUserIndex]);
                break;
            case 3:
                // Call transfer function
                transfer(users, numUsers, users[currentUserIndex].userName);
                break;
            case 4:
                checkBalance(&users[currentUserIndex]);
                break;
            case 5:
                // Call generate credit card function
                printCreditCard(users[currentUserIndex].fname);
                break;
            case 6:
                printUserTransactionHistory(users[currentUserIndex].userName);
                break;
            case 7:
                // Call settings function
                Settings(&users[currentUserIndex], &numUsers);
                break;
            case 8:
                ContactUs();
                break;
            case 9:
                AboutUs();
                break;
            case 0:
                printf("Logged out.\n");
                break;
            default:
                printf("Invalid option. Please choose a valid option.\n");
        }
    } while (menuOption != 0);
}


//-------------------------------------------//

void Login(struct Register_Details *users, int numUsers) {
    char enteredUsername[20];
    char enteredPassword[31];

    printf("Enter your username: ");
    scanf("%s", enteredUsername);

    printf("Enter your password: ");
    scanf("%s", enteredPassword);

    int userIndex = -1; // To store the index of the found user

    for (int i = 0; i < numUsers; ++i) {
        if (strcmp(enteredUsername, users[i].userName) == 0 && strcmp(enteredPassword, users[i].password) == 0) {
            userIndex = i;
            break; // User found, exit the loop
        }
    }

    if (userIndex != -1) {
        // Update last login time for the user
        time_t t = time(NULL);
        struct tm *current_time = localtime(&t);
        users[userIndex].lastLoginTime = *current_time;

        FILE *file = fopen("user_registration_details.csv", "a");
        if (file != NULL) {
            for (int i = 0; i < numUsers; ++i) {
                fprintf(file, "Name: %s %s\n", users[i].fname, users[i].lname);
                // ... (other fields and details)
                fprintf(file, "Last Login Time: %02d/%02d/%04d %02d:%02d:%02d\n",
                        current_time->tm_mon + 1, current_time->tm_mday, current_time->tm_year + 1900,
                        current_time->tm_hour, current_time->tm_min, current_time->tm_sec);
                fprintf(file, "\n");
            }
            fclose(file);
        } else {
            fprintf(stderr, "Error opening file for writing.\n");
        }

        printf("Login successful!\n");
        displayMenu(users, numUsers, userIndex);
    } else {
        printf("Invalid username or password. Please try again.\n");
        int after_login_fail_option;
        printf("Dont Have An Account?\n 1:Try Login Again: \n2:Register New Account");
        scanf("%d",&after_login_fail_option);
        switch (after_login_fail_option)
        {
            case 1:
                printf("run the system again for login in");
                break;

            case 2:
                Register(users,&numUsers);
            default:
                printf("Invalid Option - exit in 2 seconds");
                sleep(2);
                system("taskkill /F /commandline.exe");

        }


    }
}

//-------------------------------------------//

void loadUsersFromFile(struct Register_Details *users, int *numUsers) {
    FILE *file = fopen("user_registration_details.csv", "r");
    if (file != NULL) {
        while (fscanf(file, "Name: %s %s\n", users[*numUsers].fname, users[*numUsers].lname) != EOF) {
            fscanf(file, "Address: %s\n", users[*numUsers].address);
            fscanf(file, "Phone Number: %d\n", &users[*numUsers].phoneNumber);
            fscanf(file, "Email: %s\n", users[*numUsers].email);
            fscanf(file, "Date of Birth: %02d/%02d/%04d\n",
                   &users[*numUsers].dob.month, &users[*numUsers].dob.day, &users[*numUsers].dob.year);

            fscanf(file, "Last Login Time: %02d/%02d/%04d %02d:%02d:%02d\n",
                   &users[*numUsers].lastLoginTime.tm_mon, &users[*numUsers].lastLoginTime.tm_mday,
                   &users[*numUsers].lastLoginTime.tm_year, &users[*numUsers].lastLoginTime.tm_hour,
                   &users[*numUsers].lastLoginTime.tm_min, &users[*numUsers].lastLoginTime.tm_sec);

            fscanf(file, "UserName: %s\n", users[*numUsers].userName);

            fscanf(file, "Balance: %lf\n", &users[*numUsers].balance);

            fscanf(file, "\n");

            (*numUsers)++;
        }
        fclose(file);
    } else {
        fprintf(stderr, "Error opening file for reading.\n");
        exit(EXIT_FAILURE);
    }
}

//----------------------------------------------------------------/


//------------------------------------------------//

int main() {
    struct Register_Details users[1000];
    int numUsers = 0;

    loadUsersFromFile(users, &numUsers);


    int option;
    printf("\nWelcome To NK Bank System:\n");
    printf("0:Register\n1:Sign-In\n2:Terms Of Use\n");
    //3:Withdraw
    //4:transform
    //5:Deposit
    //6:Balance
    //7:credit card
    //8:Settings(coin type,colored mode,delete account,my account)
    scanf("%d", &option);
    switch (option) {


        case 0: {
            Register(users, &numUsers);

            break;
        }
        case 1: {
            Login(users, numUsers);

            break;
        }
        case 2: {
            TermsOfUse();

            break;
        }

        default:
            printf("Invalid Option Try Again");


            return 0;
    }

}
void red() {
    printf("\033[1;31m");
}

void yellow() {
    printf("\033[1;33m");
}
void black() {
    printf("\033[0;30m");
}

void green() {
    printf("\033[0;32m");
}
void blue() {
    printf("\033[0;34m");
}

void purple() {
    printf("\033[0;35m");
}
void cyan() {
    printf("\033[0;36m");
}

void white() {
    printf("\033[0;37m");
}


void reset() {
    printf("\033[0m");
}

void printColoredText() {
    printf("Choose a color:\n");
    printf("1: Red\n2: Yellow\n3: Black\n4: Green\n5: Blue\n6: Purple\n7: Cyan\n8: White\n");

    int colorOption;
    scanf("%d", &colorOption);

    switch (colorOption) {
        case 1:
            red();
            break;
        case 2:
            yellow();
            break;
        case 3:
            black();
            break;
        case 4:
            green();
            break;
        case 5:
            blue();
            break;
        case 6:
            purple();
            break;
        case 7:
            cyan();
            break;
        case 8:
            white();
            break;
        default:
            printf("Invalid color option.\n");
            return;
    }

    printf("Colored Text Example\n");
    reset(); // Reset the color to default
}