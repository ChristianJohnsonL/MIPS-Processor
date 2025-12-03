#include <iostream>
#include <vector>
#include <fstream>
#include <String.h>
#include "functions.h"
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

void print(ifstream& f, int size, int changed)
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
    return result;
}

//== Amari Start
void Add(int registers[6], ifstream& f, int size)
{
    int changed = 0;

    print(f, size, changed);
}

void Sub(int registers[6], ifstream& f, int size )
{
    int changed = 0;

    print(f, size, changed);
}

void And(int registers[6], ifstream& f, int size )
{
    int changed = 0;

    print(f, size, changed);
}

void Addi(int registers[6], ifstream& f, int size )
{
    int changed = 0;

    print(f, size, changed);
}

void Or(int registers[6], ifstream& f, int size )
{
    int changed = 0;

    print(f, size, changed);
}

void Xor(int registers[6], ifstream& f, int size )
{
    int changed = 0;

    print(f, size, changed);
}

//== Amari Stop

void Lw(int registers[6], ifstream& f, int size )
{
    int changed = 0;

    print(f, size, changed);
}

void Sw(int registers[6], ifstream& f, int size )
{
    int changed = 0;

    print(f, size, changed);
}

void Beq(int registers[6], ifstream& f, int size )
{
    int changed = 0;

    print(f, size, changed);
}

void Bne(int registers[6], ifstream& f, int size )
{
    int changed = 0;

    print(f, size, changed);
}


void mipsProcessor(string instruction, ifstream& f, int size)
{
    int operation = 0;
    int index = 0;
    bool rFormat = true;
    std::string binary;
    int registers[6];
    binary = hexToBinary(instruction);
    int result = 0;
    cout << binary << endl;

    for (int i = 0; i < 6; i++)
    {
        if (binary[i] != '0')
        {
            rFormat = false;
            cout << "I format" << endl;
            break;
        }
    }


    if (rFormat)
    {
        result = 0;
        for (int i = 0; i < 6; i++)
        {
            if(binary[i] == '1')
            {
                int bitPos = 5 - i;
                result += (1 << bitPos);
            }
        }
        registers[0] = result;           
        cout << registers[0] << endl;

        index = 6;                       
        result = 0;


        for (int i = 1; i < 5; i++)
        {
            result = 0;
            for (int j = index; j < index + 5; j++)
            {
                if (binary[j] == '1')
                {
                    int bitPos = (index + 4) - j;
                    result += (1 << bitPos);
                }
            }
            registers[i] = result;
            cout << registers[i] << endl;
            index+=5;
        }

        result = 0;

        for (int i = index; i < index + 6; i++)
        {
            if (binary[i] == '1')
            {
                int bitPos = (index + 5) - i;
                result += (1 << bitPos);
            }
        }
        registers[5] = result;
        result = 0;
        cout << registers[5] << endl;

        operation = registers[5];
    }
    else
    {
        result = 0;
        for (int i = 0; i < 6; i++)
        {
            if(binary[i] == '1')
            {
                int bitPos = 5 - i;
                result += (1 << bitPos);
            }
        }
        registers[0] = result;           
        cout << registers[0] << endl;

        index = 6;                       
        result = 0;


        for (int i = 1; i <= 2; i++)
        {
            result = 0;
            for (int j = index; j < index + 5; j++)
            {
                if (binary[j] == '1')
                {
                    int bitPos = (index + 4) - j;
                    result += (1 << bitPos);
                }
            }
            registers[i] = result;
            cout << registers[i] << endl;
            index+=5;
        }

        result = 0;

        for (int i = index; i < index + 16; i++)
        {
            if (binary[i] == '1')
            {
                int bitPos = (index + 15) - i;
                result += (1 << bitPos);
            }
        }
        registers[4] = result;
        result = 0;
        cout << registers[4] << endl;
        operation = registers[0];
    }

    switch(operation)
    {
        case 32:
        cout << "Operation is ADD" << endl;
        Add(registers, f, size);
        break;

        case 34:
        cout << "Operation is SUB" << endl;
        Sub(registers, f, size);
        break;

        case 36:
        cout << "Operation is AND" << endl;
        And(registers, f, size);
        break;

        case 37:
        cout << "Operation is OR" << endl;
        Or(registers, f, size);
        break;

        case 38:
        cout << "Operation is XOR" << endl;
        Xor(registers, f, size);
        break;

        case 8:
        cout << "Operation is ADDI" << endl;
        Addi(registers, f, size);
        break;

        case 35:
        cout << "Operation is LW" << endl;
        Lw(registers, f, size);
        break;

        case 43:
        cout << "Operation is SW" << endl;
        Sw(registers, f, size);
        break;

        case 5:
        cout << "Operation is BNE" << endl;
        Bne(registers, f, size);
        break;

        case 4:
        cout << "Operation is BEQ" << endl;
        Beq(registers, f, size);
        break;

        default:
        cout << operation << " is not in scope." << endl;

    }
}
