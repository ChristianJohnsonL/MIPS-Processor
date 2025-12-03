#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <cmath>
#include "functions.h"
using namespace std;


int main()
{
    int selection;
    bool running = true;
    string instruction;

    ifstream f("Memory.h");
    vector<string> data;

    if (!f)
    {
        std::ofstream file("Memory.h");
        data = {"012A", "012A", "A3F2", "7B19", "C0DE", "491E", "0F72", "9A44", "55EE", "D137", "6C80", "02FA", "FE21", "3D9B", "B4C7", "88F0", "1745", "E2AB", "4096", "F7CD"};
        for (int i = 0; i < 20; i++)
        {
            file << data[i] << "\n";
        }
        file.close();
    }

    int size = memSize(f, data);


    while(running)
    {
        int changed = size + 1;
        cout << "What do you want to do? \nEnter 1 to enter an instruction. \nEnter 2 to print memory.\nPlease enter something: ";
        cin >> selection;

        switch(selection)
        {
            case 1:
            cout<<"Enter an instruction: ";
            cin >> instruction;
            mipsProcessor(instruction, f, size);
            break;

            case 2: 
            print(f, data, memSize(f, data), changed);
            break;

            default:
            cin.clear();
            cin.ignore(100000, '\n');
            cout << "\nI have no idea why you selected " << selection << ". It was not on this list. I'm not even sure if this is an int. I mean, I could write code to figure it out, but please do better Dr. Dennis. ";
            break;
        }
    }
}