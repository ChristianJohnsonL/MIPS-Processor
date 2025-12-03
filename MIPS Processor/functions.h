#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
using namespace std;


int memSize(ifstream& f, vector<string>& data);

void print(ifstream& f, vector<string>& data, int size, int changed);

int hexCharToValue(char c);

bool isHex(const std::string& s);

std::string stripLeadingZeros(const std::string& s);

std::string hexToBinary(const std::string& hex);

//== Amari Start
void Add(int registers[6], ifstream& f, int size);

void Sub(int registers[6], ifstream& f, int size);

void And(int registers[6], ifstream& f, int size);

void Addi(int registers[6], ifstream& f, int size);

void Or(int registers[6], ifstream& f, int size);

void Xor(int registers[6], ifstream& f, int size);

void Lw(int registers[6], ifstream& f, int size);

//== Amari Stop

void Sw(int registers[6], ifstream& f, int size);

void Beq(int registers[6], ifstream& f, int size);

void Bne(int registers[6], ifstream& f, int size);

void mipsProcessor(string instruction, ifstream& f, int size);