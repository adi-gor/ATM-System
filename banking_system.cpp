#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<cstdlib>
#define MIN_BALANCE 500
using namespace std;

class InsufficientFunds{};

class Account {
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long NextAccountNumber;
    public:
        Account() {}
        Account(string fname, string lname, float balance);
        long getAccNo() {return accountNumber;}
        string getFirstName() {return firstName;}
        string getLastName() {return lastName;}
        float getBalance() {return balance;}

        void Deposit(float amount);
        void Withdraw(float amount);
        static void setLastAccountNumber(long accountNumber);
        static long getLastAccountNumber();

        friend ofstream & operator<<(ofstream &ofs, Account &acc);
        friend ifstream & operator>>(ifstream &ifs, Account &acc);
        friend ostream & operator<<(ostream &, Account &acc);
};

long Account::NextAccountNumber = 0;

class Bank {
    map<long, Account> accounts;
    public:
        Bank();
        Account OpenAccount(string fname, string lname, float balance);
        Account BalanceEnquiry(long accountNumber);
        Account Deposit(long accountNumber, float amount);
        Account Withdraw(long accountNumber, float amount);
        void CloseAccount(long accountNumber);
        void ShowAllAccounts();
        ~Bank();
};

int main() {
    Bank b;
    Account acc;
    int choice;
    string fname, lname;
    long accountNumber;
    float balance;
    float amount;
    cout << "Banking System";
    while (choice != 7) {
        cout << "\nSelect an option";
        cout << "\n1)Open an Account";
        cout << "\n2)Balance Enquiry";
        cout << "\n3)Deposit";
        cout << "\n4)Withdrawal";
        cout << "\n5)Close an Account";
        cout << "\n6)Show all Accounts";
        cout << "\n7)Quit";
        cout << "\nEnter your choice\t";
        cin >> choice;
        switch(choice) {
            case 1:
                cout << "\nEnter First Name: ";
                cin >> fname;
                cout << "\nEnter Last Name: ";
                cin >> lname;
                cout << "\nEnter Balance";
                cin >> balance;
                acc = b.OpenAccount(fname, lname, balance);
                cout << "\nCongratulations Account is created";
                cout << "\n" << acc;
                break;
            case 2:
                cout << "\nEnter Account Number: ";
                cin >> accountNumber;
                acc = b.BalanceEnquiry(accountNumber);
                cout << "\nYour Account Details";
                cout << "\n" << acc;
                break;
            case 3:
                cout << "\nEnter Account Number: ";
                cin >> accountNumber;
                cout << "\nEnter Balance: ";
                cin >> amount;
                acc = b.Deposit(accountNumber, amount);
                cout << "\nAmount is Deposited";
                cout << endl << acc;
            case 4:
                cout << "\nEnter Account Number: ";
                cin >> accountNumber;
                cout << "\nEnter Balance: ";
                cin >> amount;
                acc = b.Withdraw(accountNumber, amount);
                cout << "\nAmount Withdrawn";
                cout << endl << acc;
                break;
            case 5:
                cout << "\nEnter Account Number: ";
                cin >> accountNumber;
                b.CloseAccount(accountNumber);
                cout << endl << "Account is Closed" << endl;
                cout << acc;
            case 6:
                b.ShowAllAccounts();
                break;
            case 7:
                break;
            default:
                cout << "\nEnter correct choice";
                exit(0);
        }
    }
    return 0;
}

Account::Account(string fname, string lname, float balance) {
    NextAccountNumber++;
    accountNumber = NextAccountNumber;
    firstName = fname;
    lastName = lname;
    this -> balance = balance;
}

void Account::Deposit(float amount) {
    balance += amount;
}

void Account::Withdraw(float amount) {
        if(balance - amount < MIN_BALANCE)
            throw InsufficientFunds();
        balance -= amount;
}

void Account::setLastAccountNumber(long accountNumber) {
    NextAccountNumber = accountNumber;
}

long Account::getLastAccountNumber() {
    return NextAccountNumber;
}

ofstream & operator<<(ofstream &ofs, Account &acc) {
    ofs << acc.accountNumber << endl;
    ofs << acc.firstName << endl;
    ofs << acc.lastName << endl;
    ofs << acc.balance << endl;
    return ofs;
}

ifstream & operator>>(ifstream &ifs, Account &acc) {
    ifs >> acc.accountNumber;
    ifs >> acc.firstName;
    ifs >> acc.lastName;
    ifs >> acc.balance;
    return ifs;
}

ostream & operator<<(ostream &os, Account &acc) {
    os << "\nFirst Name: " << acc.getFirstName();
    os << "\nLast Name: " << acc.getLastName();
    os << "\nAccount Number: " << acc.getAccNo();
    os << "\nBalance: " << acc.getBalance();
    return os;
}

Bank::Bank() {
    Account account;
    ifstream infile;
    infile.open("Bank.data");
    if(!infile) {
        cout << "File not found!" << endl;
        return;
    }
    while(!infile.eof()) {
        infile >> account;
        accounts.insert(pair<long,Account>(account.getAccNo(),account));
    }
    Account::setLastAccountNumber(account.getAccNo());
    infile.close();
}

Account Bank::OpenAccount(string fname, string lname, float balance) {
    ofstream outfile;
    Account account(fname, lname, balance);
    accounts.insert(pair<long, Account>(account.getAccNo(), account));
    outfile.open("Bank.data", ios::trunc);
    map<long, Account>::iterator itr;
    for(itr = accounts.begin(); itr != accounts.end(); itr++) {
        outfile << itr -> second;
    }
    outfile.close();
    return account;
}

Account Bank::BalanceEnquiry(long accountNumber) {
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    return itr -> second;
}

Account Bank::Deposit(long accountNumber, float amount) {
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    itr -> second.Deposit(amount);
    return itr -> second;
}

Account Bank::Withdraw(long accountNumber, float amount) {
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    itr -> second.Withdraw(amount);
    return itr -> second;
}

void Bank::CloseAccount(long accountNumber) {
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    cout << "\nAccount Deleted" << itr -> second;
    accounts.erase(accountNumber);
}

void Bank::ShowAllAccounts() {
    map<long, Account>::iterator itr;
    for(itr = accounts.begin(); itr != accounts.end(); itr++) {
        cout << "Account " << itr -> first << endl << itr -> second << endl;
    }
}

Bank::~Bank() {
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);
    map<long, Account>::iterator itr;
    for(itr = accounts.begin(); itr != accounts.end(); itr++) {
        outfile << itr -> second;
    }
    outfile.close();
}