#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <stdexcept>

using namespace std;

class Mobile
{
public:
    string brand;
    string model;
    double price;
    int stock;
    bool emiAvailable;

    // Decrease the stock when a mobile phone is sold
    void decreaseStock()
    {
        if (stock > 0)
        {
            stock--;
        }
    }
};

class Buyer
{
public:
    string name;
    string mobileNumber;
    string address;
    string dateOfPurchase;
    string paymentType; // "emi" or "cash"
    double downPayment;
    int remainingMonths;
};

class Shop
{
private:
    static const int MAX_INVENTORY_SIZE = 100;
    static const int MAX_BUYERS_SIZE = 100;

    Mobile inventory[MAX_INVENTORY_SIZE];
    Buyer buyers[MAX_BUYERS_SIZE];
    int inventorySize = 0;
    int buyersSize = 0;
   
    int mobilePurchasedIndex[MAX_BUYERS_SIZE];


public:
    void loadInventoryFromFile()
    {
        ifstream file("mobile_inventory.txt");
        if (!file.is_open())
        {
            cout << "Inventory file not found. Please create the inventory file." << endl;
            return;
        }

        Mobile mobile;
        while (!file.eof())
        {
            string line;
            getline(file, line);

            if (line.find("Brand: ") != string::npos)
            {
                mobile.brand = line.substr(line.find("Brand: ") + 7);
                getline(file, line);
                mobile.model = line.substr(line.find("Model: ") + 7);
                getline(file, line);
                mobile.price = stod(line.substr(line.find("Price: ") + 7));
                getline(file, line);
                mobile.stock = stoi(line.substr(line.find("Stock: ") + 7));
                mobile.emiAvailable = (mobile.price > 15000);

                inventory[inventorySize++] = mobile;
            }
        }

        file.close();
    }

    // Function to add a mobile phone to the inventory
void addMobilePhone()
{
    if (inventorySize < MAX_INVENTORY_SIZE)
    {
        Mobile mobile;
        cout << "Enter brand: ";
        cin >> mobile.brand;
        cout << "Enter model: ";
        cin >> mobile.model;

        bool existsInInventory = false;

        // Check if the mobile already exists in the inventory
        for (int i = 0; i < inventorySize; i++)
        {
            if (inventory[i].brand == mobile.brand && inventory[i].model == mobile.model)
            {
                existsInInventory = true;
                cout << "Mobile phone already exists in inventory. Updating stock." << endl;
                cout << "Enter additional stock quantity: ";
                int additionalStock;
                cin >> additionalStock;
                inventory[i].stock += additionalStock;

                // Update the inventory file
                updateInventoryFile();

                cout << "Stock updated for " << mobile.brand << " " << mobile.model << "." << endl;
                cout << "------------------------------------------------------" << endl;
                return;
            }
        }

        if (!existsInInventory)
        {
            // The mobile is not in the inventory, so add a new entry
            cout << "Enter price: ";
            cin >> mobile.price;
            cout << "Enter stock quantity: ";
            cin >> mobile.stock;

            // Determine EMI availability based on the price
            mobile.emiAvailable = (mobile.price > 15000);

            // Update the inventory array
            inventory[inventorySize++] = mobile;

            // Update the inventory file
            ofstream file("mobile_inventory.txt", ios::app);
            if (file.is_open())
            {
                file << "Brand: " << mobile.brand << endl;
                file << "Model: " << mobile.model << endl;
                file << "Price: " << mobile.price << endl;
                file << "Stock: " << mobile.stock << endl;
                file << "EMI Available: " << (mobile.emiAvailable ? "Yes" : "No") << endl;
                file << endl; // Add a separator between entries
                file.close();

                cout << "Mobile phone added to inventory and updated in the file." << endl;
            }
            else
            {
                cout << "Error: Unable to update the inventory file." << endl;
            }

            cout << "------------------------------------------------------" << endl;
        }
    }
    else
    {
        cout << "Inventory is full. Cannot add more mobile phones." << endl;
        cout << "------------------------------------------------------" << endl;
    }
}


    // Function to sell a mobile phone
// Function to sell a mobile phone
// Function to sell a mobile phone
void sellMobilePhone()
{
    if (inventorySize == 0)
    {
        cout << "Inventory is empty. Cannot sell mobile phones." << endl;
        return;
    }

    string sellBrand, sellModel;
    bool mobileFound = false;

    cout << "Enter brand of the mobile: ";
    cin >> sellBrand;
    cout << "Enter the model to sell: ";
    cin >> sellModel;

    for (int i = 0; i < inventorySize; i++)
    {
        if (inventory[i].brand == sellBrand && inventory[i].model == sellModel)
        {
            if (inventory[i].stock > 0)
            {
                Buyer buyer;
                cout << "Enter buyer's name: ";
                cin >> buyer.name;
                cout << "Enter buyer's mobile number: ";
                cin >> buyer.mobileNumber;
                cout << "Enter buyer's address: ";
                cin >> buyer.address;
                cout << "Enter date of purchase: ";
                cin >> buyer.dateOfPurchase;

                if (inventory[i].price <= 15000)
                {
                    // Direct cash payment for phones with price <= 15000
                    buyer.paymentType = "cash";
                    buyer.downPayment = inventory[i].price;
                    buyer.remainingMonths = 0;
                }
                else
                {
                    cout << "Select payment type (emi/cash): ";
                    cin >> buyer.paymentType;

                    if (buyer.paymentType == "emi" && inventory[i].emiAvailable)
                    {
                        buyer.downPayment = 0.3 * inventory[i].price;
                        cout << "Down payment will be " << buyer.downPayment << " INR"
                            << " \nDo you want to pay the remaining amount in less than 8 months? (yes/no): ";
                        string choice;
                        cin >> choice;

                        if (choice == "yes")
                        {
                            cout << "Enter the number of months to pay off the remaining amount (max 8 months): ";
                            cin >> buyer.remainingMonths;

                            if (buyer.remainingMonths > 8)
                            {
                                buyer.remainingMonths = 8;
                            }
                        }
                        else
                        {
                            buyer.remainingMonths = 8; // Default to 8 months if not paying in less than 8 months.
                        }

                        double monthlyPayment = (inventory[i].price - buyer.downPayment) / buyer.remainingMonths;

                        cout << "\nDown Payment: " << buyer.downPayment << " INR" << endl;
                        cout << "Amount Payable per Month: " << monthlyPayment << " INR" << endl;
                        cout << "Number of Months: " << buyer.remainingMonths << " months" << endl;
                        cout << endl;
                    }
                    else if (buyer.paymentType == "cash")
                    {
                        buyer.downPayment = inventory[i].price;
                        buyer.remainingMonths = 0;
                        cout << "\nDown Payment: " << buyer.downPayment << " INR" << endl;
                        cout << "Amount Payable per Month: N/A (Cash payment)" << endl;
                        cout << "Number of Months: N/A (Cash payment)" << endl;
                        cout << endl;
                    }
                    else
                    {
                        throw runtime_error("Invalid payment type or EMI not available for this mobile.");
                    }
                }

                // Save buyer details to "buyer.txt" file
                ofstream buyerFile("buyer.txt", ios::app);
                if (buyerFile.is_open())
                {
                    buyerFile << "Buyer Name: " << buyer.name << endl;
                    buyerFile << "Buyer Mobile Number: " << buyer.mobileNumber << endl;
                    buyerFile << "Buyer Address: " << buyer.address << endl;
                    buyerFile << "Date of Purchase: " << buyer.dateOfPurchase << endl;
                    buyerFile << "Brand: " << inventory[i].brand << endl;
                    buyerFile << "Model: " << inventory[i].model << endl;
                    buyerFile << "Price (INR): " << inventory[i].price << endl;
                    buyerFile << "Payment Type: " << buyer.paymentType << endl;
                    buyerFile << "Down Payment (INR): " << buyer.downPayment << endl;

                    if (buyer.paymentType == "emi")
                    {
                        buyerFile << "Amount Payable per Month (INR): " << (inventory[i].price - buyer.downPayment) / buyer.remainingMonths << endl;
                        buyerFile << "Number of Months: " << buyer.remainingMonths << " months" << endl;
                    }
                    else
                    {
                        buyerFile << "Amount Payable per Month: N/A (Cash payment)" << endl;
                        buyerFile << "Number of Months: N/A (Cash payment)" << endl;
                    }

                    buyerFile << "******************************************" << endl;
                    buyerFile << endl;
                    buyerFile.close();
                }
                else
                {
                    cout << "Error: Unable to update the buyer file." << endl;
                }

                // Update the inventory and buyer arrays
                buyers[buyersSize] = buyer;
                mobilePurchasedIndex[buyersSize] = i; // Store the index of the purchased mobile phone
                buyersSize++;
                inventory[i].decreaseStock();

                // Update the inventory file after selling
                updateInventoryFile();

                mobileFound = true;
                generateReceipt(buyer, inventory[i]); // Generate and display the receipt
                break;
            }
            else
            {
                cout << "Out of stock!" << endl;
                mobileFound = true;
            }
        }
    }

    if (!mobileFound)
    {
        cout << "Mobile phone not found in inventory. Please try again." << endl;
    }
}


// Function to update the inventory file after selling a mobile
void updateInventoryFile()
{
    ofstream file("mobile_inventory.txt");
    if (file.is_open())
    {
        for (int i = 0; i < inventorySize; i++)
        {
            file << "Brand: " << inventory[i].brand << endl;
            file << "Model: " << inventory[i].model << endl;
            file << "Price: " << inventory[i].price << endl;
            file << "Stock: " << inventory[i].stock << endl;
            file << "EMI Available: " << (inventory[i].emiAvailable ? "Yes" : "No") << endl;
            file << endl; // Add a separator between entries
        }
        file.close();
    }
    else
    {
        cout << "Error: Unable to update the inventory file." << endl;
    }
}
     // Function to display mobile phones in inventory
    // Function to display mobile phones in inventory
void displayMobilePhones()
{
    ifstream file("mobile_inventory.txt");
    if (!file.is_open())
    {
        cout << "Inventory file not found. Please create the inventory file." << endl;
        return;
    }

    cout << "\n***************************************************************************************************************************" << endl;
    cout << "                                          Mobile Phones in Inventory" << endl;
    cout << "***************************************************************************************************************************" << endl;
    cout << left << setw(15) << "Brand" << setw(15) << "Model" << setw(15) << "Price (INR)" << setw(15) << "Stock" << setw(15) << "EMI Available" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;

    while (!file.eof())
    {
        string line;
        getline(file, line);

        if (line.find("Brand: ") != string::npos)
        {
            Mobile mobile;
            mobile.brand = line.substr(line.find("Brand: ") + 7);
            getline(file, line);
            mobile.model = line.substr(line.find("Model: ") + 7);
            getline(file, line);
            mobile.price = stod(line.substr(line.find("Price: ") + 7));
            getline(file, line);
            mobile.stock = stoi(line.substr(line.find("Stock: ") + 7));
            mobile.emiAvailable = (mobile.price > 15000);

            cout << left << setw(15) << mobile.brand << setw(15) << mobile.model
                 << setw(15) << mobile.price << setw(15) << mobile.stock
                 << setw(15) << (mobile.emiAvailable ? "Yes" : "No") << endl;
        }
    }

    file.close();

    cout << "***************************************************************************************************************************" << endl;
    cout << endl;
    cout << "----------------------------------------------------" << endl;
}


    // Function to check units available per brand and model
    void checkUnitsAvailable()
    {
        string brand, model;
        cout << "\nEnter brand name: ";
        cin >> brand;
        cout << "Enter model: ";
        cin >> model;

        int unitsAvailable = 0;
        for (int i = 0; i < inventorySize; i++)
        {
            if (inventory[i].brand == brand && inventory[i].model == model)
            {
                unitsAvailable = inventory[i].stock;
                break;
            }
        }

        cout << "Units available for " << brand << " " << model << ": " << unitsAvailable << endl;
                    cout<<"------------------------------------------------------"<<endl;
    }

    // Function to display buyer details
// Function to display buyer details, including the purchased mobile
// Modify the displayBuyerDetails function
void displayBuyerDetails()
{
    cout << "\n*********************************************************************************************************************************************************************************************************" << endl;
    cout << "                                                                    Buyer Details" << endl;
    cout << "\n*********************************************************************************************************************************************************************************************************" << endl;
    cout << left << setw(15) << "Name" << setw(15) << "Mobile Number" << setw(15) << "Address" << setw(15) << "Date of Purchase" << setw(15) << "Payment Type" << setw(20) << "Down Payment (INR)" << setw(25) << "Amount Payable per Month (INR)" << setw(20) << "Number of Months" << setw(15) << "Brand" << setw(15) << "Model" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < buyersSize; i++)
    {
        int mobileIndex = mobilePurchasedIndex[i]; // Get the index of the purchased mobile
        cout << left << setw(15) << buyers[i].name << setw(15) << buyers[i].mobileNumber
             << setw(15) << buyers[i].address << setw(15) << buyers[i].dateOfPurchase
             << setw(15) << buyers[i].paymentType << setw(20) << buyers[i].downPayment;

        if (buyers[i].paymentType == "emi")
        {
            cout << setw(25) << (buyers[i].remainingMonths > 0 ? (inventory[mobileIndex].price - buyers[i].downPayment) / buyers[i].remainingMonths : 0);
            cout << setw(20) << buyers[i].remainingMonths;
        }
        else
        {
            cout << setw(25) << "N/A (Cash payment)";
            cout << setw(20) << "N/A";
        }

        cout << setw(15) << inventory[mobileIndex].brand << setw(15) << inventory[mobileIndex].model << endl;
    }
    cout << "\n*********************************************************************************************************************************************************************************************************" << endl;

    cout << "\n1. Main Menu" << endl;
    cout << "2. Exit" << endl;
    int choice;
    cout << "Enter your choice: ";
    cin >> choice;
    cout << "------------------------------------------------------" << endl;
    if (choice == 1)
    {
        // Continue to the main menu
    }
    else if (choice == 2)
    {
        cout << "THANK YOU" << endl;
        exit(0);
    }
    else
    {
        cout << "Invalid choice. Returning to the main menu." << endl;
    }
}
void generateReceipt(const Buyer &buyer, const Mobile &mobile)
{
    cout << "******************************************" << endl;
    cout << "Receipt" << endl;
    cout << "******************************************" << endl;
    cout << "Buyer Name: " << buyer.name << endl;
    cout << "Buyer Mobile Number: " << buyer.mobileNumber << endl;
    cout << "Buyer Address: " << buyer.address << endl;
    cout << "Date of Purchase: " << buyer.dateOfPurchase << endl;
    cout << "Brand: " << mobile.brand << endl;
    cout << "Model: " << mobile.model << endl;
    cout << "Price (INR): " << mobile.price << endl;
    cout << "Payment Type: " << buyer.paymentType << endl;
    cout << "Down Payment (INR): " << buyer.downPayment << endl;

    if (buyer.paymentType == "emi")
    {
        cout << "Amount Payable per Month (INR): " << (mobile.price - buyer.downPayment) / buyer.remainingMonths << endl;
        cout << "Number of Months: " << buyer.remainingMonths << " months" << endl;
    }
    else
    {
        cout << "Amount Payable per Month: N/A (Cash payment)" << endl;
        cout << "Number of Months: N/A (Cash payment)" << endl;

    }
    cout<<endl;
    cout << "******************************************" << endl;
    cout<<endl;
    cout<<"-------------------------------------------------"<<endl;
}


    // Function to display the main menu
    void displayMainMenu()
    {

        cout<<"Enter the operation you want to perform"<<endl;
        cout<<"----------------------------------------------------"<<endl;

        cout << "1. Add Mobile Phone" << endl;
        cout << "2. Sell Mobile Phone" << endl;
        cout << "3. Display Mobile Phones" << endl;
        cout << "4. Check Units Available per Brand and Model" << endl;
        cout << "5. Display Buyer Details" << endl;
        cout << "6. Exit" << endl;
    }
};
int main()
{
    Shop shop;
    shop.loadInventoryFromFile(); // Load inventory from the file at the beginning
    cout << " ____________________________________________" << endl;
    cout << " |      Mobile Shop Management System     | " << endl;
    cout << " |________________________________________|" << endl;

    while (true)
    {
        shop.displayMainMenu();
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            shop.addMobilePhone();
            break;

        case 2:
            shop.sellMobilePhone();
            break;

        case 3:
            shop.displayMobilePhones();
            break;

        case 4:
            shop.checkUnitsAvailable();
            break;

        case 5:
            shop.displayBuyerDetails();
            break;

        case 6:
            cout << "THANK YOU" << endl;
            return 0;

        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
