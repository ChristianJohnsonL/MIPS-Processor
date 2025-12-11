#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
using namespace std;


int memSize(ifstream& f, vector<string>& data);

void print(ifstream& f, vector<string>& data, vector<string>& memory, int size, int changed);

int hexCharToValue(char c);

bool isHex(const std::string& s);

std::string stripLeadingZeros(const std::string& s);

std::string hexToBinary(const std::string& hex);

// Helpers for working with 16‑bit signed memory values
int hexStringToSigned16(const std::string& hex);
std::string signed16ToHexString(int value);

//== Amari Start
void Add(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory);

void Sub(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory);

void And(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory);

void Addi(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory);

void Or(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory);

void Xor(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory);

void Lw(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory);

//== Amari Stop

void Sw(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory);

void Beq(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory);

void Bne(int registers[6], ifstream& f, int size, vector<string>& data, vector<string>& memory);

void mipsProcessor(string instruction, ifstream& f, int size, vector<string>& data, vector<string>& memory);