#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#define MAX_NAME 50


//For Agents end the email with "nwc.ja", for customers end the email with any other domain.//

//Struct for storing customer information
typedef struct{
    int cust_num;
    char fname[MAX_NAME];
    char lname[MAX_NAME];
    char email[MAX_NAME];
    char password[MAX_NAME];
    char income_class[MAX_NAME];
    char  pay_eligible[MAX_NAME]; 
    enum type{Customer, Agent}user_type; 
}INF;
//Struct for customer information controlled by agent. 
typedef struct{
    int cust_num;
    int premises_num;
    char service_fname[MAX_NAME];
    char service_lname[MAX_NAME];
    char meter_size[MAX_NAME];
    int first_meter_reading;
    int prev_reading;
    int current_reading;
    int archive_status;
}AGT; 
//function that assigns income class to the user. 
void income_class(INF *user){
    int incomeClass;
    do{
        printf("Choose your income class:\n");
        printf("\n1. Low\n2. low/Medium\n3. medium\n4.Medium/High\n5. High\n");
        printf("Enter your income class (1-5): ");
        scanf("%d", &incomeClass);
        //Validates input to ensure it is between 1 and 5.
        if(incomeClass<1 || incomeClass>5){
            printf("Invalid choice. Please try again.\n");
        }
    } while (incomeClass < 1 || incomeClass > 5);
    switch(incomeClass){
        case 1:
            strcpy(user->income_class, "Low");
            break;
        case 2:
            strcpy(user->income_class, "low/Medium");
            break;
        case 3:
            strcpy(user->income_class, "medium");
            break;
        case 4:
            strcpy(user->income_class, "Medium/High");
            break;
        case 5:
            strcpy(user->income_class, "High");
            break;
    }
}
//Function to hash the email and password for security.
void hash(char pass[MAX_NAME], char emal[MAX_NAME], int *hashedemail, int *hashedpass){
    *hashedemail = 0;
    *hashedpass = 0;
    //Basic hash function that multiplies the ASCII value of each character by its position in the string.
    for (int i = 0; i < strlen(emal); i++) {
        *hashedemail += (int)emal[i] * (i + 1);
    }
    //Basic hash function that multiplies the ASCII value of each character by its position in the string.
    for (int i = 0; i < strlen(pass); i++) {
        *hashedpass += (int)pass[i] * (i + 1);
    }
}
//Function that checks if the email already exists in the file.
int duplicateemail(char *email){
    int hashemail=0;
    //Hahs function to camppare the email to the one in the file.
    for(int i=0; i<strlen(email); i++){
        hashemail += (int)email[i]*(i+1);
    }
    FILE *Customerrec;
    Customerrec = fopen("Customerrec.txt", "r");
    if (Customerrec == NULL) {
        printf("Error opening file!\n");
        return 0;
    }
    char line[256]; //Stores each line read from the file.
    int savedHemail;
    //Reads the file line by line. 
    while (fgets(line, sizeof(line), Customerrec)) {
        //Checks if the line contains the email and extracts the hashed email.
        if (sscanf(line, "Email: %d", &savedHemail) == 1) {
            if (savedHemail == hashemail) {
                fclose(Customerrec);
                return 1; //Email exists.
            }
        }
    }
    fclose(Customerrec);
    return 0; //Email does not exist.
}
//Function that checks if the email is valid.
int validemail(char *email){
    //Checks if the email contains an '@' and a '.' character.  
    return(strchr(email, '@')!=NULL && strchr(email, '.')!=NULL);
}
//Function that hides the password input when the user types it in. 
void maskedInput(char pass[MAX_NAME]) {
    char ch;
    int i = 0;

    printf("Enter Password: ");
    while ((ch = getch()) != '\r') { 
        if (i < MAX_NAME - 1) {
            if (ch == '\b' && i > 0) { //Handles backspace.
                printf("\b \b");
                i--;
            } else if (ch != '\b') {//Adds character to password
                pass[i++] = ch;
                printf("*"); //Masks the input with '*' character.
            }
        }
    }
    pass[i] = '\0'; //Null-terminate the string.
    printf("\n");
}
//Funtcion that registers a new account for the user, depending on the email it could be a agent or a customer. 
void regestr(){
    int hashedemail=0, hashedpass=0, attempts=0; 
    INF user;
    srand(time(0));
    user.cust_num=rand()%10000000 + 1000000;//Genrates random customer ID. 
    FILE *Customerrec; 
    Customerrec = fopen("Customerrec.txt", "a+");
    if (Customerrec == NULL) {
        printf("Error opening file!\n");
        return;
    }
    printf("\n********************************\n");
    printf("Register Your Account.");
    printf("\n\nCustomer number: %d", user.cust_num);
    printf("\nEnter your first name: "); 
    scanf("%s", user.fname);
    printf("Enter your last name: ");
    scanf("%s", user.lname);
    //Loops until a valid and non-duplicate email is entered. 
    do{
        if(attempts>5){
            printf("Too many attempts. Please try again later.\n");
            return;
        }
        attempts++;
        printf("Enter your email: ");
        getchar();
        fgets(user.email, sizeof(user.email), stdin);//Reads email using fgets to allow spaces.
        user.email[strcspn(user.email, "\n")] = 0;//Removes newline character from the input.
        if(Type_user(user.email)){
            user.user_type=Agent;
        }else{
            user.user_type=Customer;
        }
        if(!validemail(user.email)){
            printf("Invalid email format. Please try again.\n");
        }else if(duplicateemail(user.email)){
            printf("Email already exists. Please try again.\n");
        }
    }while(!validemail(user.email) || duplicateemail(user.email));
    maskedInput(user.password);
    hash(user.password, user.email, &hashedemail, &hashedpass);
    //Writes customer data to the file.
    fprintf(Customerrec, "\n\nCustomer ID: %d", user.cust_num);
    fprintf(Customerrec, "\nFirst Name: %s", user.fname);
    fprintf(Customerrec, "\nLast Name: %s", user.lname);
    fprintf(Customerrec, "\nEmail: %d", hashedemail);
    fprintf(Customerrec, "\nPassword: %d", hashedpass);
    income_class(&user);
    fprintf(Customerrec, "\nIncome class: %s", user.income_class);
    fprintf(Customerrec, "\nUser Type(0-Customer, 1-Agent): %d", user.user_type);
    fclose(Customerrec);
    printf("\nRegistration Successful.\n");
    printf("*********************************\n");
}
//Function that checks if the customer number already exists in the file.
int check_cust_num(int cust_num){
    FILE *Customerrec=fopen("Customerrec.txt", "r");
    if(Customerrec ==  NULL){
        printf("Error opening file!\n");
        return 0;
    }
    char line[256];
    int found;
    //Reads the file line by line.
    while(fgets(line,sizeof(line), Customerrec)){
        if(sscanf(line, "Customer ID: %d", &found)==1){
            if(found==cust_num){
                fclose(Customerrec);
                return 1; //Customer number exists.
            }
        }
    }
    fclose(Customerrec);
    return 0;//Customer number does not exist.
}
//Function that checks if the password is correct by comparing the hashed password to the one in the file.
int credentials_validate(char *email, char *password){
    FILE *Customerrec;
    Customerrec = fopen("Customerrec.txt", "r");
    if (Customerrec == NULL) {
        printf("Error opening file!\n");
        return 0;
    }
    int hashemail=0, hashpass=0;
    hash(password, email, &hashemail, &hashpass);
    char line[256];//Buffer to store each line from the file. 
    int Hemail, Hpass;
    //Reads the file line by line.
    while (fgets(line, sizeof(line), Customerrec)){
        if(sscanf(line, "Email: %d", &Hemail) == 1){
            fgets(line, sizeof(line), Customerrec); 
            if (sscanf(line, "Password: %d", &Hpass)==1){
                if(Hemail==hashemail && Hpass==hashpass){
                    fclose(Customerrec);
                    return 1; //Valid credentials.
                }
            }
        }
    }
    fclose(Customerrec);
    return 0;//Invalid credentials.
}
//Function that determines if the user is a customer or an agent based on the email domain.
int Type_user(char *email){
    char *position=strchr(email, '@');
    if(position !=NULL){
       if(strncmp(position+1, "nwc.ja", 6)==0){
        return 1; //Agent.
       }
    }
    return 0;//Customer.
}
//Function that adds a new customer to the agent file.
void add_cust(AGT *agent){
    int choice=0;
    //Initialize customer details. 
    agent->prev_reading=0;
    agent->current_reading=0;
    agent->archive_status=0;
    FILE *Agentrec; 
    Agentrec = fopen("Agentrec.txt", "a+");
    if (Agentrec == NULL) {
        printf("Error opening file!\n");
        return;
    }
    printf("\n********************************\n");
    printf("Add Customer.\n\n");
    //Prompting agent for customer details.
    printf("Enter Customers numeber(7 digits): ");
    scanf("%d", &agent->cust_num);
    if(check_cust_num(agent->cust_num)) printf("Customer exists.\n");
    printf("Add Premises number(7 Digits): ");
    scanf("%d", &agent->premises_num); 
    printf("Add customer  first name: ");
    scanf("%s", agent->service_fname);
    printf("Add customer last name: ");
    scanf("%s", agent->service_lname);
    printf("Enter meter size installed:\n1.15mm\n2.30mm\n3.150mm\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    switch(choice){
        case 1: 
            strcpy(agent->meter_size, "15mm");
            break;
        case 2:
            strcpy(agent->meter_size, "30mm");
            break;
        case 3: 
            strcpy(agent->meter_size, "150mm");
            break;
        default:
            printf("Invalid choice. Defaulting to 15mm.\n");
            strcpy(agent->meter_size, "15mm");
    }
    printf("Enter first meter reading: ");
    scanf("%d", &agent->first_meter_reading);
    //Writing customer details to the agent file.
    fprintf(Agentrec, "Customer Number: %d", agent->cust_num);
    fprintf(Agentrec, "\nPremises Number: %d", agent->premises_num);
    fprintf(Agentrec, "\nCustomer First Name: %s", agent->service_fname);
    fprintf(Agentrec, "\nCustomer Last Name: %s", agent->service_lname);
    fprintf(Agentrec, "\nMeter Size: %s", agent->meter_size);
    fprintf(Agentrec, "\nFirst Meter Reading: %d", agent->first_meter_reading);
    fprintf(Agentrec, "\nPrevious Meter Reading: %d", agent->prev_reading);
    fprintf(Agentrec, "\nCurrent Meter Reading: %d", agent->current_reading);
    fprintf(Agentrec, "\nArchive status(0-Not Archived, 1-Archived): %d", agent->archive_status);
    fclose(Agentrec);//Close the file after writing.
    printf("\nCustomer added successfully.\n");
    printf("*********************************\n");
    printf("....Returning to agent menu....\n");
}
//Function to edit customer details in the agent file.
void edit_cust(AGT *agent){
    //Initialize variables 
    int cust_num, found=0, choice, fcust_num, fpremises_num, ffirst_meter_reading;
    char meter_size[MAX_NAME], t_fname[MAX_NAME], t_lname[MAX_NAME], t_meter_size[MAX_NAME];
    //Opening necessary files.
    FILE *Agentrec, *Temp;
    Agentrec = fopen("Agentrec.txt", "r");
    Temp= fopen("Temp.txt", "w");
    if(Agentrec == NULL || Temp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    printf("\n********************************\n");
    printf("Edit Customer.\n");
    printf("\nEnter the customer number to edit: ");
    scanf("%d", &cust_num);
    //Reading customer details from the agent file.
    while( fscanf(Agentrec,"\nCustomer Number: %d\nPremises Number: %d\nCustomer First name: %s\nCustomer Last Name: %s\nMeter Size: %s\nFirst Meter Reading: %d",&fcust_num, &fpremises_num, t_fname, t_lname, t_meter_size, &ffirst_meter_reading)==6){
        if(fcust_num == cust_num){
            found=1; 
            while (1){
                printf("\nChoose the field to edit: \n");
                printf("1. FIrst name\n2.Last name\n3. Meter size\n4. First meter reading\n5. Exit\n"); 
                printf("Enter your choice: ");
                scanf("%d", &choice);
                if(choice<1 || choice>5){
                    printf("Invalid choice. Please try again.\n");
                    continue;
                }
                switch(choice){
                    case 1: 
                        printf("Enter new first name: ");
                        scanf("%s", t_fname);
                        break;
                    case 2:
                        printf("Enter new last name: ");
                        scanf("%s", t_lname);
                        break;
                    case 3:
                        printf("Enter new meter size: ");
                        scanf("%s", t_meter_size);
                        break;
                    case 4:
                        printf("Enter new first meter reading: ");
                        scanf("%d", &ffirst_meter_reading);
                        break;
                    case 5:
                        printf("\n....Exiting edit mode....\n");
                        goto save_changes;
                }
            }
        }
        //Write updated customer details to the temporary file.
    save_changes:
        fprintf(Temp,"\nCustomer Number: %d\nPremises Number: %d\nCustomer First name: %s\nCustomer Last Name: %s\nMeter Size: %s\nFirst Meter Reading: %d", fcust_num,fpremises_num, t_fname, t_lname, t_meter_size, ffirst_meter_reading);
    }
    fclose(Agentrec);
    fclose(Temp);
    //Updating the original file with the changes made.
    if(found){
        remove("Agentrec.txt");
        rename("Temp.txt", "Agentrec.txt");
        printf("\nCustomer details updated successfully.\n");
    }else{
        remove("Temp.txt");
        printf("\nCustomer not found.\n");
    }
}
//Function to view customer details in the agent file.
void view_cust(AGT *agent){ 
    //Initialize variables and open necessary files.
    FILE *Agentrec;
    Agentrec= fopen("Agentrec.txt", "r");
    if(Agentrec == NULL) {
        printf("Error opening file!\n");
        return;
    }
    char line[256];
    int cust_num, found=0, fcust_num;
    printf("\n********************************\n");
    printf("View Customer.\n");
    printf("\nEnter the customer number to view: ");
    scanf("%d", &cust_num);
    //Searching for customer in file 
    while(fgets(line, sizeof(line), Agentrec)){
        if(sscanf(line, "Customer Number: %d", &fcust_num)==1){
            if(fcust_num==cust_num){
                found=1; 
                printf("\nFound\n");
                //Print all customer deatils until next customer number is found.
                while(fgets(line, sizeof(line), Agentrec)&& strncmp(line, "Customer Number:", 15)!=0){
                    printf("%s", line);
                }
                break;
            }
        }
    }
    //Close the file after reading.
    fclose(Agentrec);
    if(!found){
        printf("\nCustomer not found.\n");
    }
    printf("*********************************\n");
    printf("....Returning to agent menu....\n");
}
//Function to archive/delete customer records from the agent file. 
void archive_cust(AGT *agent){
    //Initialize variables and open necessary files.
    FILE *Agentrec, *Temp;
    Agentrec= fopen("Agentrec.txt", "r");
    Temp= fopen("Temp.txt", "w");
    if(Agentrec == NULL || Temp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    int cust_num, found=0, fcust_num;
    char line[256];
    printf("\n********************************\n");
    printf("Archive Customer.\n");
    printf("\nEnter the customer number to archive: ");
    scanf("%d", &cust_num);
    //Read through agent file line by line 
    while(fgets(line, sizeof(line), Agentrec)){
        if(sscanf(line, "Customer Number: %d", &fcust_num)==1){
            if(fcust_num==cust_num){
                found=1; 
                fprintf(Temp, "%s", line);//Write customer number line to temp file
                printf("\nCustomer found and archived.\n");
                //Process remaining customer details
                while(fgets(line, sizeof(line), Agentrec)&& strncmp(line, "Customer Number:", 15)!=0){
                    if(strstr(line, "Archive status:")!=NULL){
                        fprintf(Temp, "Archive status: 1\n");
                    }else{
                        //Write all other lines to temp file.
                        fprintf(Temp, "%s", line);
                    }
                }
            }else fprintf(Temp, "%s", line);//Write all other lines to temp file.
        }else fprintf(Temp, "%s", line);//Write all other lines to temp file.
    }
    //Close the files after reading.
    fclose(Agentrec);
    fclose(Temp);
    //If customer was found the replace the original file with the temp file.
    if(found){
        remove("Agentrec.txt");
        rename("Temp.txt", "Agentrec.txt");
    }else{
        remove("Temp.txt");
        printf("\nCustomer not found.\n");
    }
    printf("*********************************\n");
    printf("....Returning to agent menu....\n");
} 
//Function to get income class from the customer record file.
void get_incomeclass(int cust_num, char income_class[MAX_NAME]){
    //Initialize variables and open necessary files.
    FILE *Customerrec=fopen("Customerrec.txt", "r");
    if(Customerrec == NULL) {
        printf("Error opening file!\n");
        return;
    }
    char line[256];
    int fcust_num;
    char fincome_class[MAX_NAME];
    //Search through file for matching customer number 
    while(fgets(line, sizeof(line), Customerrec)){
        if(sscanf(line, "Customer ID: %d", &fcust_num)==1&&fcust_num==cust_num){
            //Once found, looks for income class line.
            while(fgets(line, sizeof(line), Customerrec)){
                if(sscanf(line, "Income class: %s", fincome_class)==1){
                    strcpy(income_class, fincome_class);
                    fclose(Customerrec);
                    return;
                }
            }
        }
    }
    //If customer number not found, close the file and set income class to "Low".
    fclose(Customerrec);
    printf("Customer not found.\n");
    strcpy(income_class, "Low");
}
//Function to generate a random number between min and max.
int random(int min, int max){
    return (rand() % (max - min + 1)) + min;
}
//Function to get the maximum usage based on income class.
int get_maxuse(char *income_class){
    if(strcmp(income_class, "Low")==0) return 125; 
    if(strcmp(income_class, "Low/Medium")==0)return 175; 
    if(strcmp(income_class, "Medium")==0)return 220;
    if(strcmp(income_class, "Medium/High")==0)return 250;
    if(strcmp(income_class, "High")==0)return 300;
    return 125; //Default to low income class if not found.
}
//Funciton to calculate monthly usage based on income class.
int monthly_usage(char *incone_class){
    int maxuse=get_maxuse(income_class);
    int total=0; 
    //Generate random monthly usage for 30 days.
    for(int i=0; i<30; i++){
        total+=random(0, maxuse);
    }
    return total; 
}
//Function to calculate service charge based on meter size.
double service_charge(char *meter_size){
    if(strcmp(meter_size, "15mm")==0) return 1155.92;
    if(strcmp(meter_size, "30mm")==0) return 6217.03;
    if(strcmp(meter_size, "150mm")==0) return 39354.59; 
    return 0.00;//Return 0 if meter size not found.
}
//Function to generate a bill for a customer.
void bill(AGT *agent){
    //Initialize variables and open necessary files.
    FILE *Agentrec;
    Agentrec= fopen("Agentrec.txt", "r");
    if(Agentrec == NULL) {
        printf("Error opening file!\n");
        return;
    }
    int cust_num, found=0, fcust_num, first_read=0, prev_read=0, curr_read=0, monthly_total, early_eligible;
    double water_rate, sewage_rate, early_disc=0.00, overdue=0.00;
    char line[256], meter_size[MAX_NAME]="", income_class[MAX_NAME]="";
    printf("\n********************************\n");
    printf("Generate Bill.\n");
    printf("\nEnter the customer number to get bill: ");
    scanf("%d", &cust_num);
    get_incomeclass(cust_num, income_class);
    //Search for customer in file 
    while(fgets(line, sizeof(line), Agentrec)){
        if(sscanf(line, "Customer Number: %d", &fcust_num)==1){
            if(fcust_num==cust_num){
                found=1; 
                //Read customer details until next customer number is found.
                while(fgets(line, sizeof(line), Agentrec)){
                    if(sscanf(line, "Meter Size: %s", meter_size)==1)continue;
                    if(sscanf(line, "First Meter Reading: %d", &first_read)==1)continue; 
                    if(sscanf(line, "Previous Meter Reading: %d", &prev_read)==1)continue; 
                    if(sscanf(line, "Current Meter Reading: %d", &curr_read)==1)break;
                }
                break;
            }
        }
    }
    fclose(Agentrec);
    //If customer not found, print message and return to agent menu.
    if(!found){
        printf("\nCustomer not found.\n");
        printf("*********************************\n");
        printf("....Returning to agent menu....\n");
        return;
    }
    //Calculate monthly usage and rates based monthly usage.
    monthly_total=monthly_usage(income_class);
    if(monthly_total>=41000){
        water_rate=494.87;
        sewage_rate=571.56;
    }else if(monthly_total>=27001){
        water_rate=290.10;
        sewage_rate=335.06;
    }else if(monthly_total>=14001){
        water_rate=266.15;
        sewage_rate=307.42;
    }else{
        water_rate=149.55;
        sewage_rate=172.72;
    }
    //Calculat bill charges.
    double servicecharge=service_charge(meter_size);
    int curr_consumption=curr_read-prev_read;
    double water_charge=curr_consumption*water_rate;
    double sewage_charge=curr_consumption*sewage_rate;
    double pam=(water_charge+sewage_charge+servicecharge)*0.0121;
    double xfactor=(water_charge+sewage_charge+servicecharge)*0.05;
    double kfactor=(water_charge+sewage_charge+servicecharge+pam-xfactor)*0.02;
    double curr_total=water_charge+sewage_charge+servicecharge+pam+kfactor-xfactor;
    //Determine early payment discount eligibility. 
    early_eligible=rand()%2;
    if(early_eligible){
        early_disc=-250; 
    }
    double total_due=curr_total+early_disc+overdue;
    //Print bill details.
    printf("\nCustomer number: %d", cust_num);
    printf("\nMeter size: %s", meter_size);
    printf("\nFirst meter reading: %d", first_read);
    printf("\nPrevious meter reading: %d", prev_read);
    printf("\nCurrent meter reading: %d", curr_read);
    printf("\nCurrent consumption: %d", curr_consumption);
    printf("\nWater charge: %.2f", water_charge);
    printf("\nSewage charge: %.2f", sewage_charge);
    printf("\nService charge: %.2f", servicecharge);
    printf("\nPrice Adjustment Mechanism: %.2f", pam);
    printf("\nX-Factor: %.2f", xfactor);
    printf("\nK-Factor: %.2f", kfactor);
    printf("\nTotal current charges: %.2f", curr_total);
    printf("\nOverdue charges: %.2f", overdue);
    if(early_eligible){
        printf("\nEarly payment discount: %.2f", early_disc);
    }else{
        printf("\nEarly payment discount: Not eligible.");
    }
    printf("\nTotal due: %.2f", total_due);
    printf("\n*********************************\n");
    printf("....Returning to agent menu....\n");
}
//Funtion to handle agent interface and menu options.
void agent_interface(){
    //Initialize variables.
    AGT *agent;
    int choice=0;
    while (choice!=6){
        //Display menu options.
        printf("\n********************************\n");
        printf("Welcome to the Agent Interface.\n");
        printf("\n1. Add Customer\n2. Edit Customer\n3. View Customer\n4. Delete/Archive Customer\n5. Genrate Bills\n6. Logout");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        //Validate user input 
        if(choice<1 || choice>6){
            printf("Invalid choice. Please try again.\n");
        }
        switch(choice){
            case 1:
                add_cust(agent);//Add new customer
                break;  
            case 2:
                edit_cust(agent);//Edit existing customer details
                break;
            case 3:
                view_cust(agent);//View customer details
                break;
            case 4:
                archive_cust(agent); //Archive customer records
                break;
            case 5: 
                bill(agent);//Generate bill for customer
                break;
            case 6: 
                printf("Logging out...\n");//Exit the loop and return to main menu.
                break;
        }
    }
}
//Function to hash the card number for security.
void hash_cardnum(char card_num[MAX_NAME], int *hash_card){
    *hash_card = 0;
    //Basic hash function that multiplies the ASCII value of each character by its position in the string.
    for (int i = 0; i < strlen(card_num); i++) {
        *hash_card += (int)card_num[i] * (i + 1);
    }
}
//Function to register a payment card for a customer.
void registr_card(){
    //Initialize variables and open necessary files.
    FILE *Customerrec=fopen("Customerrec.txt", "r");
    FILE *Paymentrec=fopen("Paymentrec.txt", "a+");
    FILE *Temp=fopen("Temp.txt", "w");
    if(Customerrec==NULL || Temp==NULL || Paymentrec==NULL){
        printf("Error opening file!\n");
        return;
    }
    char line[256]; 
    int cust_num, found=0, fcust_num, hash_card=0;
    char card_number[MAX_NAME], exp_date[MAX_NAME];
    printf("\n********************************\n");
    printf("Register Payment Card.\n");
    printf("\nEnter your customer number: ");
    scanf("%d", &cust_num);
    //Search for customer in file
    while(fgets(line, sizeof(line), Customerrec)){
        if(sscanf(line, "Customer ID: %d", &fcust_num)==1){
            if(fcust_num==cust_num){
                found=1;
                //Prompt for payment card details.
                printf("Enter your card number (16 digits): ");
                scanf("%s", card_number);
                while(strlen(card_number)!=16){
                    printf("Incvalid card number. Please try again: ");
                    scanf("%s", card_number);
                }
                printf("Enter expiration date (MM/YY): ");
                scanf("%s", exp_date);
                while(strlen(exp_date)!=5 || exp_date[2]!='/'){
                    printf("Invalid expiration date. Please try again: ");
                    scanf("%s", exp_date);
                }
                //Hash the card number for security and store it to payment file.
                hash_cardnum(card_number, &hash_card);
                dprintf(Paymentrec, "\nCustomer ID: %d", cust_num);
                fprintf(Paymentrec, "\nCard Number: %d", hash_card);
                fprintf(Paymentrec, "\nExpiration Date: %s", exp_date);
                printf("\nPayment card registered successfully.\n");
                fprintf(Temp, "%s", line);
                //Write customer details to temp file.
                while (fgets(line, sizeof(line), Customerrec)){
                    if(strncmp(line, "Payment Eligibility: ", 19)==0){
                        fprintf(Temp, "Payment Eligibility: Eligible\n");
                    }else{
                        fprintf(Temp, "%s", line);
                    }
                }
                break;
            }
        }
        fprintf(Temp, "%s", line);
    }
    //Close the files after reading.
    fclose(Customerrec);
    fclose(Paymentrec);
    fclose(Temp);
    //Update customer record file with changes made.
    if(found){
        remove("Customerrec.txt");
        rename("Temp.txt", "Customerrec.txt");
    }else{
        remove("Temp.txt");
        printf("\nCustomer not found.\n");
    }
    printf("*********************************\n");
    printf("....Returning to customer menu....\n");
}
//Function to print the receipt for the customer after payment.
void receipt(int cust_cum, int premesis_num, char fname[MAX_NAME], char lname[MAX_NAME], int prev_read, int curr_read, double curr_total,double bb_forward, double overdue, double total_due){
    printf("\n********************************\n");
    printf("Receipt.\n");
    printf("\nCustomer number: %d", cust_cum);
    printf("\nPremises number: %d", premesis_num);
    printf("\nCustomer first name: %s", fname);
    printf("\nCustomer last name: %s", lname);
    printf("\nPrevious meter reading: %d", prev_read);
    printf("\nCurrent meter reading: %d", curr_read);
    printf("\nBalance Brought Forward: %.2f", bb_forward);
    printf("\nCurrent total charges: %.2f", curr_total);
    printf("\nOverdue charges: %.2f", overdue);
    printf("\nTotal due: %.2f", total_due);
    printf("*********************************\n");
}
//Function to process bill payment for a customer.
void pay(int cust_num, int premises_num, char fname[MAX_NAME], char lname[MAX_NAME], int prev_read, int curr_read, double curr_total,double bb_forward, double overdue, double total_due){
    //Initialize variables and open necessary files.
    FILE *Paymentrec=fopen("Paymentrec.txt", "r");
    FILE *Agentrec=fopen("Agentrec.txt", "r");
    FILE *Temp=fopen("Temp.txt", "w");
    if(Paymentrec==NULL || Agentrec==NULL || Temp==NULL){
        printf("Error opening file!\n");
        return;
    }
    char line[256], card_num[MAX_NAME];
    int cust_num, fcust_num,found=0,c_found=0,fhash_card, hash_card, choice;
    double amount=0.00;
    //Search for customer's card in payment file
    while(fgets(line, sizeof(line), Paymentrec)){
        if(sscanf(line, "Customer ID: %d", &fcust_num)==1 && fcust_num==cust_num){
            if(fgets(line, sizeof(line), Paymentrec)){
                if(sscanf(line, "Hashed Card Number: %d", &fhash_card)==1){
                    c_found=1; 
                }
            }
        }
    }
    //If customer card not found, print message and return to agent menu.
    fclose(Paymentrec);
    if(!c_found){
        printf("Customer payment card not found.\n");
        printf("Registera payment card first.\n");
        fclose(Agentrec);
        fclose(Temp);
        return;
    }
    printf("\n********************************\n");
    printf("Pay your bill.\n");
    printf("\nEnter your card number: ");
    scanf("%s", card_num);
    hash_cardnum(card_num, &hash_card);//Hash the card number to verify 
    if(hash_card!=fhash_card){
        printf("Invalid card number. Please try again.\n");
        fclose(Agentrec);
        fclose(Temp);
        return;
    }
    printf("Enter the amount to pay: ");
    scanf("%lf", &amount);
    if(total_due<0){
        total_due=0.00;
    }
    bb_forward=total_due; 
    //Update customer record with new balance.
    while(fgets(line, sizeof(line), Agentrec)){
        if(sscanf(line, "Customer Number: %d", &fcust_num)==1 && fcust_num==cust_num){
            fprintf(Temp, "Customer Number: %d", fcust_num);
            fprintf(Temp, "\nPremises Number: %d", premises_num);
            fprintf(Temp, "\nCustomer First Name: %s", fname);
            fprintf(Temp, "\nCustomer Last Name: %s", lname);
            fprintf(Temp, "\nPrevious Meter Reading: %d", prev_read);
            fprintf(Temp, "\nCurrent Meter Reading: %d", curr_read);
            fprintf(Temp, "\nBalance Brought Forward: %.2f", bb_forward);
            continue; 
        }
        //Copy all other lines to temp file.
        fputs(line, Temp);
    }
    //Close the files after reading.
    fclose(Agentrec);
    fclose(Temp);
    //Update the original file with the changes made.
    remove("Agentrec.txt");
    rename("Temp.txt", "Agentrec.txt");
    //Print the receipt for the customer.
    receipt(cust_num, premises_num, fname, lname, prev_read, curr_read, curr_total, bb_forward, overdue, total_due);
    printf("\nPayment successful.\n");
}
//Function to view the bill for a customer and to optionally pay.
void view_bill(){
    //Initialize variables and open necessary files.
    FILE *Agentrec;
    Agentrec= fopen("Agentrec.txt", "r");
    if(Agentrec == NULL) {
        printf("Error opening file!\n");
        return;
    }
    int cust_num, found=0, fcust_num, first_read=0, prev_read=0, curr_read=0, monthly_total, early_eligible, premises_num, choice;
    double water_rate, sewage_rate, early_disc=0.00, overdue=0.00, servicecharge, curr_consumption, water_charge,bb_forward,  sewage_charge, pam, xfactor, kfactor, curr_total;
    char line[256], meter_size[MAX_NAME]="", income_class[MAX_NAME]="", fname[MAX_NAME]="", lname[MAX_NAME]="";
    printf("\n********************************\n");
    printf("View Bill.\n");
    printf("\nEnter your customer number: ");
    scanf("%d", &cust_num);
    get_incomeclass(cust_num, income_class);
    //Search for customer in file and read details.
    while(fgets(line, sizeof(line), Agentrec)){
        if(sscanf(line, "Customer Number: %d", &fcust_num)==1){
            if(fcust_num==cust_num){
                found=1; 
                while(fgets(line, sizeof(line), Agentrec)){
                    if(sscanf(line, "Premises Number: %d", &premises_num)==1)continue;
                    if(sscanf(line, "Customer First Name: %s", fname)==1)continue;
                    if(sscanf(line, "Customer Last Name: %s", lname)==1)continue;
                    if(sscanf(line, "Meter Size: %s", meter_size)==1)continue;
                    if(sscanf(line, "First Meter Reading: %d", &first_read)==1)continue; 
                    if(sscanf(line, "Previous Meter Reading: %d", &prev_read)==1)continue; 
                    if(sscanf(line, "Current Meter Reading: %d", &curr_read)==1)break;
                }
                break;
            }
        }
    }
    //Close the file after reading.
    fclose(Agentrec);
    //If customer not found, print message and return to agent menu.
    if(!found){
        printf("\nCustomer not found.\n");
        printf("*********************************\n");
        printf("....Returning to agent menu....\n");
        return;
    }
    //Calculate monthly usage and rates based on monthly usage.
    monthly_total=monthly_usage(income_class);
    if(monthly_total>=41000){
        water_rate=494.87;
        sewage_rate=571.56;
    }else if(monthly_total>=27001){
        water_rate=290.10;
        sewage_rate=335.06;
    }else if(monthly_total>=14001){
        water_rate=266.15;
        sewage_rate=307.42;
    }else{
        water_rate=149.55;
        sewage_rate=172.72;
    }
    //Calculate bill charges.   
    servicecharge=service_charge(meter_size);
    curr_consumption=curr_read-prev_read;
    water_charge=curr_consumption*water_rate;
    sewage_charge=curr_consumption*sewage_rate;
    pam=(water_charge+sewage_charge+servicecharge)*0.0121;
    xfactor=(water_charge+sewage_charge+servicecharge)*0.05;
    kfactor=(water_charge+sewage_charge+servicecharge+pam-xfactor)*0.02;
    curr_total=water_charge+sewage_charge+servicecharge+pam+kfactor-xfactor;
    early_eligible=rand()%2;
    if(early_eligible){
        early_disc=-250; 
    }
    double total_due=curr_total+early_disc+overdue;
    //Print bill details.
    printf("************************\n");
    printf("\nCustomer number %d's bill.", cust_num);
    printf("\nPrevious meter reading: %d", prev_read);
    printf("\nCurrent meter reading: %d", curr_read);
    printf("\nTotal current charges: %.2f", curr_total);
    printf("\nOverdue charges: %.2f", overdue);
    printf("\nTotal due: %.2f", total_due);
    printf("\n*********************************\n");
    //Prompt for payment option.
    printf("Would you ike to pay your bill? (1-Yes, 0-No): ");
    scanf("%d", &choice);
    if(choice){
        pay(cust_num, premises_num, fname, lname, prev_read, curr_read, curr_total,bb_forward, overdue, total_due);
    }else{
        printf("\n....Returning to customer menu....\n");
    }
    printf("*********************************\n");
}
//Function to handle customer interface and menu options.
void cust_interface(){
    int choice=0;
    printf("\n********************************\n");
    printf("Welcome to the Customer Interface.\n");
    while(choice!=3){
        //Display menu options.
        printf("1. Rgister Payment Card\n2. View Bill\n3. Logout\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        if(choice<1 || choice>5){
            printf("Invalid choice. Please try again.\n");
            continue;
        }
        switch(choice){
            case 1:
                registr_card();//Register payment card
                break;
            case 2:
                view_bill();//View bill and pay
                break;
            case 3:
                printf("Logging out...\n");//Exit the loop and return to main menu.
                break;
        }
    }
    printf("*********************************\n");
}
//Funtion to handle user sign in
void signin(){
    printf("\n********************************\n");
    printf("Sign in to your account.");
    //Initialize variables.
    char email[MAX_NAME], password[MAX_NAME];
    int attempts=0, trys=3;
    do{
        printf("\n\nEnter your email: ");
        scanf("%s", email);
        maskedInput(password);//Get password input without displaying it.
        //Validate email format.
        if(credentials_validate(email, password)){
            int result=credentials_validate(email, password);
            printf("%d", result);
            printf("\nSign in successful.\n");
            //Determine user type based on email domain.
            if(Type_user(email)==1){
                agent_interface(); //For Agents 
            }else if(Type_user(email)==0){
                printf("You are a customer");
                cust_interface();//For Customers
            }
            printf("*********************************\n");
            printf("....Returning to main menu....\n");
            return; 
        }else{
            printf("\nInvalid email or password. Please try again.\n");
            attempts++;
            printf("You have %d attempts left.\n", trys-attempts);//Allow 3 attempts.
        }
    }while(attempts<3);
}
//Main program entry point.
int main(){
    //Initialize variables and seed random number generator.
    int choice=0; 
    srand(time(0));
    while(choice!=3){
        //Display main menu options.
        printf("\n--------------------------------\n");
        printf("Welcome to the National Water Commission Utility Platform");
        printf("\n--------------------------------\n");
        printf("\n\n1. Register Account\n2. Sign in\n3.Exit"); 
        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);
        //Validate user input
        if(choice<1 || choice>3){
            printf("Invalid choice. Please try again.\n");
        }
        switch(choice){
            case 1:
                regestr();
                printf("\n....Returning to main menu...."); //Register new account
                break;
            case 2:
                signin();//Sign in to existing account
                break;
            case 3:
                printf("Exiting the program...\n");//Exit the program
                break;
        }
    }
    return 0;//Return 0 to indicate successful completion.
}