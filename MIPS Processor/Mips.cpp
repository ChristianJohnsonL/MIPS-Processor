#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <cmath>
using namespace std;

int memSize(ifstream& f, vector<string>& data)
{
    string line;
    int size = 0;
    while (getline(f, line))
    {
        data.push_back(line);
        size++;
    }

    f.clear();
    f.seekg(0);
    return size;
}

void print(ifstream& f, vector<string>& data, int size)
{
    string line;
    while (getline(f, line))
    {
        cout << line << endl;
    }

    f.clear();
    f.seekg(0);
}

int hexCharToValue(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    c = std::toupper(static_cast<unsigned char>(c));
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return -1;
}

bool isHex(const std::string& s)
{
    if (s.empty()) return false;
    for (char c : s)
    {
        if (hexCharToValue(c) == -1) return false;
    }
    return true;
}


std::string stripLeadingZeros(const std::string& s)
{
    std::size_t i = 0;
    while (i < s.size() && s[i] == '0') ++i;
    if (i == s.size()) return "0";
    return s.substr(i);
}

std::string hexToBinary(const std::string& hex)
{
    static const std::string lookup[16] = {
        "0000","0001","0010","0011",
        "0100","0101","0110","0111",
        "1000","1001","1010","1011",
        "1100","1101","1110","1111"
    };

    std::string result;
    for (char c : hex)
    {
        int val = hexCharToValue(c);
        result += lookup[val];
    }
    return stripLeadingZeros(result);
}


void mipsProcessor(string instruction)
{
    int index = 0;
    bool rFormat;
    std::string binary;
    int registers[4];
    binary = hexToBinary(instruction);
    int result;

    for (int i = 0; i < 6; i++)
    {
        if (binary[i] != 0)
        {
            rFormat = false;
        }
    }


    if (rFormat)
    {
        for (int i = 5; i >= 0; i--)
        {
            if(binary[i] == 1)
            {
                result += pow(2, abs(i - 5));
            }
        }
        registers[0] = result;
        index+=6;
        result = 0;


        for (int i = 0; i < 4; i++)
        {
            for (int i = index + 5; i > index; i--)
            {
                
            }
            registers[i] = result;
            index+=5;
            result = 0;
        }
        for (int i = index; i < index + 6; i++)
        {

        }


    }
    else
    {

    }


}


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


    while(running)
    {
        cout << "What do you want to do? \nEnter 1 to enter an instruction. \nEnter 2 to print memory.\nPlease enter something: ";
        cin >> selection;

        switch(selection)
        {
            case 1:
            cout<<"Enter an instruction: ";
            cin >> instruction;
            mipsProcessor(instruction);
            break;

            case 2: 
            print(f, data, memSize(f, data));
            break;

            default:
            cin.clear();
            cin.ignore(100000, '\n');
            cout << "\nI have no idea why you selected " << selection << ". It was not on this list. I'm not even sure if this is an int. I mean, I could write code to figure it out, but please do better Dr. Dennis. ";
            break;
        }
    }
}