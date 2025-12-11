#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cctype>
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

int getRegisterCount(const vector<string>& data)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        // Check for empty string or whitespace-only string
        bool isEmpty = true;
        for (char c : data[i])
        {
            if (!std::isspace(static_cast<unsigned char>(c)))
            {
                isEmpty = false;
                break;
            }
        }
        if (isEmpty)
            return static_cast<int>(i);
    }
    return static_cast<int>(data.size()); // No empty line found, return size
}


void print(ifstream& f, vector<string>& data, vector<string>& memory, int size, int changed)
{
    // Reload data from file first
    data.clear();
    memory.clear();
    f.clear();
    f.seekg(0);
    string line;
    while (getline(f, line))
    {
        data.push_back(line);
    }
    


    
    int registerCount = getRegisterCount(data);
    int memoryStart = registerCount;
    
    if (memoryStart < static_cast<int>(data.size()) && data[memoryStart] == "")
    {
        memoryStart += 1;
    }

    // Print registers
    cout << "Registers: " << endl;
    for (int i = 0; i < registerCount && i < static_cast<int>(data.size()); i++)
    {
        cout << "$" << i << "  " << data[i];
        if (changed >= 0 && i == changed)
            cout << " <- modified";
        cout << endl;
    }

    cout << endl;

    // Print memory
    for (int i = memoryStart; i < static_cast<int>(data.size()); i++)
    {
        int memIndex = i - memoryStart;
        cout << "Memory[$" << memIndex << "]:  " << data[i];
        if (changed >= 0 && i == changed)
            cout << " <-modified";
        cout << endl;
    }






    // // Print memory with indication of what changed
    // for (size_t i = 0; i < data.size(); i++)
    // {
    //     cout << "$" << i <<"  " << data[i];
    //     if (changed >= 0 && static_cast<int>(i) == changed)
    //         cout << " <- modified";
    //     cout << endl;
    // }

    // for (size_t i = 0; i < memory.size(); i++)
    // {
    //             cout << "$" << i <<"  " << memory[i];
    //         if (changed >= 0 && static_cast<int>(i) == changed)
    //         {
    //             cout << " <-modified";
    //             cout << endl;
    //         }
    // }

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

// Helper function to convert 4-digit hex string to signed 16-bit integer
int hexStringToSigned16(const std::string& hex)
{
    if (hex.length() != 4) return 0;
    
    int value = 0;
    for (int i = 0; i < 4; i++)
    {
        int digit = hexCharToValue(hex[i]);
        if (digit == -1) return 0;
        value = (value << 4) | digit;
    }
    
    // Convert to signed 16-bit (two's complement)
    if (value & 0x8000)
    {
        value = value - 0x10000;
    }
    
    return value;
}

// Helper function to convert signed 16-bit integer to 4-digit hex string
std::string signed16ToHexString(int value)
{
    // Convert to unsigned 16-bit
    unsigned int uvalue = value & 0xFFFF;
    
    std::string hex = "";
    for (int i = 0; i < 4; i++)
    {
        int digit = (uvalue >> (12 - i * 4)) & 0xF;
        if (digit < 10)
            hex += ('0' + digit);
        else
            hex += ('A' + digit - 10);
    }
    
    return hex;
}

// Helper function to write memory back to file
void writeMemoryToFile(vector<string>& data, const std::string& filename)
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        for (size_t i = 0; i < data.size(); i++)
        {
            file << data[i];
            if (i < data.size() - 1)
                file << "\n";
        }
        file.close();
    }
}

//== Amari Start
void Add(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory)
{
    int rsIndex = registers[1];
    int rtIndex = registers[2];
    int rdIndex = registers[3];
    
    // Check bounds
    int registerCount = getRegisterCount(data);
    if (rsIndex < 0 || rsIndex >= registerCount ||
        rtIndex < 0 || rtIndex >= registerCount ||
        rdIndex < 0 || rdIndex >= registerCount)
    {
        std::cout << "ADD: Register index out of bounds!" << std::endl;
        return;
    }
    
    // Read memory values (signed 16-bit)
    int rsVal = hexStringToSigned16(data[rsIndex]);
    int rtVal = hexStringToSigned16(data[rtIndex]);
    int result = rsVal + rtVal;
    
    // Write result back to memory (as signed 16-bit, wrapped)
    data[rdIndex] = signed16ToHexString(result);
    writeMemoryToFile(data, "Memory.h");

    std::cout << "Add $"<< rdIndex << " $" << rsIndex << " + $" << rtIndex << "-> $" << rdIndex << " = $" << rsIndex << " + $" << rtIndex << std::endl << std::endl; 
    print(f, data, memory,size, rdIndex);
}

void Sub(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory)
{
    int rsIndex = registers[1];
    int rtIndex = registers[2];
    int rdIndex = registers[3];
    
    // Check bounds
    int registerCount = getRegisterCount(data);
    if (rsIndex < 0 || rsIndex >= registerCount ||
        rtIndex < 0 || rtIndex >= registerCount ||
        rdIndex < 0 || rdIndex >= registerCount)
    {
        std::cout << "SUB: Register index out of bounds!" << std::endl;
        return;
    }
    
    // Read actual memory values (signed 16-bit)
    int rsVal = hexStringToSigned16(data[rsIndex]);
    int rtVal = hexStringToSigned16(data[rtIndex]);
    int result = rsVal - rtVal;
    
    // Write result back to memory
    data[rdIndex] = signed16ToHexString(result);
    writeMemoryToFile(data, "Memory.h");

    std::cout << "Sub $"<< rdIndex << " $" << rsIndex << " - $" << rtIndex << "-> $" << rdIndex << " = $" << rsIndex << " - $" << rtIndex << std::endl << std::endl; 
    print(f, data, memory, size, rdIndex);
}

void And(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory)
{
    int rsIndex = registers[1];
    int rtIndex = registers[2];
    int rdIndex = registers[3];
    
    // Check bounds
    int registerCount = getRegisterCount(data);
    if (rsIndex < 0 || rsIndex >= registerCount ||
        rtIndex < 0 || rtIndex >= registerCount ||
        rdIndex < 0 || rdIndex >= registerCount)
    {
        std::cout << "AND: Register index out of bounds!" << std::endl;
        return;
    }
    
    // Read actual memory values (signed 16-bit)
    int rsVal = hexStringToSigned16(data[rsIndex]);
    int rtVal = hexStringToSigned16(data[rtIndex]);
    int result = rsVal & rtVal;
    
    // Write result back to memory
    data[rdIndex] = signed16ToHexString(result);
    writeMemoryToFile(data, "Memory.h");

    std::cout << "And $"<< rdIndex << " $" << rsIndex << " & $" << rtIndex << "-> $" << rdIndex << " = $" << rsIndex << " & $" << rtIndex << std::endl << std::endl; 
    print(f, data, memory, size, rdIndex);
}

void Addi(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory)
{
    int rsIndex = registers[1];
    int rtIndex = registers[2];
    int imm = registers[4];  // Already sign-extended in mipsProcessor
    
    // Check bounds
    int registerCount = getRegisterCount(data);
    if (rsIndex < 0 || rsIndex >= registerCount ||
        rtIndex < 0 || rtIndex >= registerCount)
    {
        std::cout << "ADDI: Register index out of bounds!" << std::endl;
        return;
    }
    
    // Read actual memory value (signed 16-bit)
    int rsVal = hexStringToSigned16(data[rsIndex]);
    int result = rsVal + imm;
    
    // Write result back to memory
    data[rtIndex] = signed16ToHexString(result);
    writeMemoryToFile(data, "Memory.h");

    std::cout << "Addi $"<< rtIndex << " $" << rsIndex << " + " << imm << "-> $" << rtIndex << " = $" << rsIndex << " + " << imm << std::endl << std::endl; 
    print(f, data, memory, size, rtIndex);
}

void Or(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory)
{
    int rsIndex = registers[1];
    int rtIndex = registers[2];
    int rdIndex = registers[3];
    
    // Check bounds
    int registerCount = getRegisterCount(data);
    if (rsIndex < 0 || rsIndex >= registerCount ||
        rtIndex < 0 || rtIndex >= registerCount ||
        rdIndex < 0 || rdIndex >= registerCount)
    {
        std::cout << "OR: Register index out of bounds!" << std::endl;
        return;
    }
    
    // Read actual memory values (signed 16-bit)
    int rsVal = hexStringToSigned16(data[rsIndex]);
    int rtVal = hexStringToSigned16(data[rtIndex]);
    int result = rsVal | rtVal;
    
    // Write result back to memory
    data[rdIndex] = signed16ToHexString(result);
    writeMemoryToFile(data, "Memory.h");    

    std::cout << "Or $"<< rdIndex << " $" << rsIndex << " || $" << rtIndex << "-> $" << rdIndex << " = $" << rsIndex << " || $" << rtIndex << std::endl << std::endl; 
    print(f, data, memory, size, rdIndex);
}

void Xor(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory)
{
    int rsIndex = registers[1];
    int rtIndex = registers[2];
    int rdIndex = registers[3];
    
    // Check bounds
    int registerCount = getRegisterCount(data);
    if (rsIndex < 0 || rsIndex >= registerCount ||
        rtIndex < 0 || rtIndex >= registerCount ||
        rdIndex < 0 || rdIndex >= registerCount)
    {
        std::cout << "XOR: Register index out of bounds!" << std::endl;
        return;
    }
    
    // Read actual memory values (signed 16-bit)
    int rsVal = hexStringToSigned16(data[rsIndex]);
    int rtVal = hexStringToSigned16(data[rtIndex]);
    int result = rsVal ^ rtVal;
    
    // Write result back to memory
    data[rdIndex] = signed16ToHexString(result);
    writeMemoryToFile(data, "Memory.h");

    std::cout << "Xor $"<< rdIndex << " $" << rsIndex << " ^ $" << rtIndex << "-> $" << rdIndex << " = $" << rsIndex << " ^ $" << rtIndex << std::endl << std::endl; 
    print(f, data, memory, size, rdIndex);
}

void Lw(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory)
{
    int baseIndex = registers[1];
    int rtIndex = registers[2];
    int offset = registers[4];  
    
    // Check bounds for registers
    int registerCount = getRegisterCount(data);
    if (baseIndex < 0 || baseIndex >= registerCount ||
        rtIndex < 0 || rtIndex >= registerCount)
    {
        std::cout << "LW: Register index out of bounds!" << std::endl;
        return;
    }
    
    // Read base address from register (signed 16-bit)
    int baseAddr = hexStringToSigned16(data[baseIndex]);
    int address = baseAddr + offset;
    
    // Memory starts at index 17 in data vector (after 16 registers + 1 empty line)
    const int MEMORY_START_INDEX = 17;
    int memoryIndex = MEMORY_START_INDEX + address;
    
    // Expand memory if address is out of bounds
    if (address < 0)
    {
        std::cout << "LW: Address " << address << " is negative!" << std::endl;
        return;
    }
    
    // Expand data vector if memory address is out of bounds
    if (memoryIndex >= static_cast<int>(data.size()))
    {
        int oldSize = data.size();
        data.resize(memoryIndex + 1);

        // Initialize new memory locations with "0000"
        for (int i = oldSize; i <= memoryIndex; i++)
        {
            data[i] = "0000";
        }
        std::cout << "LW: Expanded memory to " << (address + 1) << " locations" << std::endl;
    }
    
    // Load word from memory address to rt register
    data[rtIndex] = data[memoryIndex];
    writeMemoryToFile(data, "Memory.h");

    std::cout << "Lw $"<< rtIndex << " Memory[$" << baseIndex << " + " << offset << "] -> $" << rtIndex << " = Memory[$" << baseIndex << " + " << offset << "]" << std::endl << std::endl; 
    print(f, data, memory, size, rtIndex);
}

void Sw(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory)
{
    int baseIndex = registers[1];
    int rtIndex = registers[2];
    int offset = registers[4];  // Already sign-extended in mipsProcessor
    
    // Check bounds for base and rt registers
    int registerCount = getRegisterCount(data);
    if (baseIndex < 0 || baseIndex >= registerCount ||
        rtIndex < 0 || rtIndex >= registerCount)
    {
        std::cout << "SW: Register index out of bounds!" << std::endl;
        return;
    }
    
    // Read base address from register (signed 16-bit)
    int baseAddr = hexStringToSigned16(data[baseIndex]);
    int address = baseAddr + offset;
    
    // Memory starts at index 17 in data vector (after 16 registers + 1 empty line)
    const int MEMORY_START_INDEX = 17;
    int memoryIndex = MEMORY_START_INDEX + address;
    
    // Expand memory if address is out of bounds
    if (address < 0)
    {
        std::cout << "SW: Address " << address << " is negative!" << std::endl;
        return;
    }
    
    // Expand data vector if memory address is out of bounds
    if (memoryIndex >= static_cast<int>(data.size()))
    {
        // Expand data to include the new memory address
        int oldSize = data.size();
        data.resize(memoryIndex + 1);
        // Initialize new memory locations with "0000"
        for (int i = oldSize; i <= memoryIndex; i++)
        {
            data[i] = "0000";
        }
        std::cout << "SW: Expanded memory to " << (address + 1) << " locations" << std::endl;
    }
    
    // Store word from rt register to memory address
    data[memoryIndex] = data[rtIndex];
    writeMemoryToFile(data, "Memory.h");

    std::cout << "Sw Memory[$" << baseIndex << " + " << offset << "] $" << rtIndex << " -> Memory[$" << baseIndex << " + " << offset << "] = $" << rtIndex << std::endl << std::endl; 
    print(f, data, memory, size, memoryIndex);
}

void Beq(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory)
{
    int rsIndex = registers[1];
    int rtIndex = registers[2];
    int offset = registers[4];  // Already sign-extended in mipsProcessor
    
    // Check bounds
    int registerCount = getRegisterCount(data);
    if (rsIndex < 0 || rsIndex >= registerCount ||
        rtIndex < 0 || rtIndex >= registerCount)
    {
        std::cout << "BEQ: Register index out of bounds!" << std::endl;
        return;
    }
    
    // Read actual memory values (signed 16-bit)
    int rsVal = hexStringToSigned16(data[rsIndex]);
    int rtVal = hexStringToSigned16(data[rtIndex]);
    
    bool branchTaken = (rsVal == rtVal);

    std::cout << "Beq $"<< rsIndex << " == $" << rtIndex << " -> " << (branchTaken ? "TAKEN" : "NOT TAKEN") << std::endl << std::endl; 
    print(f, data, memory, size, -1);  // No memory change for branches
}

void Bne(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory)
{
    int rsIndex = registers[1];
    int rtIndex = registers[2];
    int offset = registers[4];  // Already sign-extended in mipsProcessor
    
    // Check bounds
    int registerCount = getRegisterCount(data);
    if (rsIndex < 0 || rsIndex >= registerCount ||
        rtIndex < 0 || rtIndex >= registerCount)
    {
        std::cout << "BNE: Register index out of bounds!" << std::endl;
        return;
    }
    
    // Read actual memory values (signed 16-bit)
    int rsVal = hexStringToSigned16(data[rsIndex]);
    int rtVal = hexStringToSigned16(data[rtIndex]);
    
    bool branchTaken = (rsVal != rtVal);

    std::cout << "Bne $"<< rsIndex << " != $" << rtIndex << " -> " << (branchTaken ? "TAKEN" : "NOT TAKEN") << std::endl << std::endl; 
    print(f, data, memory, size, -1);  // No memory change for branches so -1
}
//== Amari Stop


void mipsProcessor(string instruction, ifstream& f, int size, vector<string>& data, vector<string> &memory)
{
    // Reload memory data from file to ensure we have latest state
    data.clear();
    f.clear();
    f.seekg(0);
    string line;
    while (getline(f, line))
    {
        data.push_back(line);
    }
    
    int operation = 0;
    int index = 0;
    bool rFormat = true;
    std::string binary;
    int registers[6];
    binary = hexToBinary(instruction);
    int result = 0;
    cout << "Binary value: " << binary << endl;

    for (int i = 0; i < 6; i++)
    {
        if (binary[i] != '0')
        {
            rFormat = false;
            cout << "Format: I-format" << endl;
            break;
        }
        if (i == 5)
        {
            cout << "Format: R-fomat" << endl;
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
        cout << "Op Code = " << registers[0] << endl;

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
            switch (i)
            {
                case 1:
                cout << "Rs = $" << registers[i] << endl;
                break;

                case 2:
                cout << "Rt = $" << registers[i] << endl;
                break;

                case 3:
                cout << "Rd = $" << registers[i] << endl;
                break;

                case 4:
                break;

                default:
                cout << "BINARY INDEXING ERROR";
                break;
            }
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
        cout << "Funct = " << registers[5] << endl;

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
        // Sign-extend the 16-bit immediate value
        if (result & 0x8000)  // If sign bit is set
        {
            result = result - 0x10000;  // Make it negative
        }
        registers[4] = result;
        result = 0;
        cout << registers[4] << " (sign-extended)" << endl;
        operation = registers[0];
    }

    switch(operation)
    {
        case 32:
        cout << "Operation is ADD" << endl;
        Add(registers, f, size, data, memory);
        break;

        case 34:
        cout << "Operation is SUB" << endl;
        Sub(registers, f, size, data, memory);
        break;

        case 36:
        cout << "Operation is AND" << endl;
        And(registers, f, size, data, memory);
        break;

        case 37:
        cout << "Operation is OR" << endl;
        Or(registers, f, size, data, memory);
        break;

        case 38:
        cout << "Operation is XOR" << endl;
        Xor(registers, f, size, data, memory);
        break;

        case 8:
        cout << "Operation is ADDI" << endl;
        Addi(registers, f, size, data, memory);
        break;

        case 35:
        cout << "Operation is LW" << endl;
        Lw(registers, f, size, data, memory);
        break;

        case 43:
        cout << "Operation is SW" << endl;
        Sw(registers, f, size, data, memory);
        break;

        case 5:
        cout << "Operation is BNE" << endl;
        Bne(registers, f, size, data, memory);
        break;

        case 4:
        cout << "Operation is BEQ" << endl;
        Beq(registers, f, size, data, memory);
        break;

        default:
        cout << operation << " is not in scope." << endl;

    }
}
