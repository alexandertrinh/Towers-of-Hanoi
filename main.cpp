#include <iostream>
#include <sstream>
#include "disk.h"
#include "board.h"

/*
 * Ran out of time. Queue not working. Worked by myself. Alexander Trinh.
 */

using namespace std;

bool is_Int(string str);
bool is_ValidNum(string str);

int main()
{
    unsigned int validInput = 1;
    char c = '\0';
    int n;
    string str;

    while (validInput) {
        cout << "Would you like to use STACKS or QUEUE? (S/Q):  ";
        cin >> c;
        if (c == 's' || c == 'S' || c == 'q' || c == 'Q') {
            cout << "How many disks would you like?:    ";
            cin.ignore();
            getline(cin, str);
            if (is_Int(str) && is_ValidNum(str))
                validInput = 0;
        }

        if (validInput != 0)
            cout << "Please enter a valid input" << endl;
    } cout << endl;

    stringstream ss(str);
    ss >> n;
    Board *towersOfHanoi = new Board(n , c); //n disks, c stack or queue;

    cout << endl;

    return 0;
}

bool is_Int(string str) {
    for (int i = 0; i < str.size(); i++) {
        int uppercaseChar = toupper(str[i]);
        if (uppercaseChar >= 'A' && uppercaseChar <= 'Z') //If character is not A-Z
            return false;
    }
    return true;
}

bool is_ValidNum(string str) {
    int num;
    stringstream ss(str);
    ss >> num;

    if (num > 2)
        return true;
    else
        return false;
}
