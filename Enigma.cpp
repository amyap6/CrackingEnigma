/**
 * @file Enigma.cpp
 *
 * @brief Enigma machine components modelled in classes and functions
 *
 * @ingroup Modern
 *
 * @author Amanda Pryor
 * Contact: agy17huu@uea.ac.uk
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include <sstream>
#include "Enigma.h"

std::string Scrambler::alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

Scrambler::Scrambler(){
    this->substitutionCipher = alphabet;
}

Scrambler::Scrambler(std::string substitutionCipher, std::string name){
    this->substitutionCipher = std::move(substitutionCipher);
    this->name = std::move(name);
}

std::string Scrambler::getName(){
    return this->name;
}

int Scrambler::getRingSetting(){
    return this->ringSetting;
}

void Scrambler::setRingSetting(int i) {
    if (i < 0 || i > 25){
        throw std::out_of_range("Integer out of range. Enter a value between 0 and 25");
    }
    else {
        this->offset = i;
    }
}

int Scrambler::getOffset(){
    return this->offset;
}

void Scrambler::setOffset(int i) {
    if (i < 0 || i > 25){
        throw std::out_of_range("Integer out of range. Enter a value between 0 and 25");}
    else {
        this->offset = i;
    }
}

void Scrambler::rotate(){
    if (this->offset < 25) {
        this->offset++;
    }
    else {
        this->offset = this->offset - 25;
    }
}

void Scrambler::rotate(int rotations){
    if (this->offset + rotations < 25){
        this->offset += rotations;
    }
    else {
        this->offset = this->offset + rotations - 25;
    }
}

char Scrambler::encryptLeft(char c){
    char newc;
    int index = c - 65;
    if (index + this->offset < 26){
        newc = this->substitutionCipher.at(index + this->offset);
    }
    else {
        newc = this->substitutionCipher.at(index + this->offset - 26);
    }
    return newc;
}

char Scrambler::encryptRight(char c){
    int index = this->substitutionCipher.find(c);
    if (index - this->offset < 0){
        index = index - this->offset + 26;
    }
    else {
        index = index - this->offset;
    }
    char newc = alphabet.at(index);
    return newc;
}

Reflector::Reflector(std::string cipher){
    this->substitutionCipher = std::move(cipher);
}

char Reflector::encrypt(char c){
    int index = c - 65;
    char newc = this->substitutionCipher.at(index);
    return newc;
}

LetterPair::LetterPair(){
    this->one = 'A';
    this->two = 'A';
}

LetterPair::LetterPair(char c, char d){
    this->one = c;
    this->two = d;
}

char LetterPair::getOne(){
    return this->one;
}

char LetterPair::getTwo(){
    return this->two;
}

bool LetterPair::contains(char c){
    return c == this->one || c == this->two;
}

char LetterPair::encrypt(char c){
    if (c == this->one){
        return this->two;
    }
    else {
        return this->one;
    }
}

Plugboard::Plugboard(){
    pair1 = LetterPair();
    pair2 = LetterPair();
    pair3 = LetterPair();
    pair4 = LetterPair();
    pair5 = LetterPair();
    pair6 = LetterPair();
}

Plugboard::Plugboard(LetterPair one, LetterPair two, LetterPair three, LetterPair four,
        LetterPair five, LetterPair six){
    this->pair1 = one;
    this->pair2 = two;
    this->pair3 = three;
    this->pair4 = four;
    this->pair5 = five;
    this->pair6 = six;
}

LetterPair Plugboard::getPair1(){
    return this->pair1;
}

LetterPair Plugboard::getPair2(){
    return this->pair2;
}

LetterPair Plugboard::getPair3(){
    return this->pair3;
}

LetterPair Plugboard::getPair4(){
    return this->pair4;
}

LetterPair Plugboard::getPair5(){
    return this->pair5;
}

LetterPair Plugboard::getPair6(){
    return this->pair6;
}

void Plugboard::setPair1(LetterPair pair){
    this->pair1 = pair;
}

void Plugboard::setPair2(LetterPair pair){
    this->pair2 = pair;
}

void Plugboard::setPair3(LetterPair pair){
    this->pair3 = pair;
}

void Plugboard::setPair4(LetterPair pair){
    this->pair4 = pair;
}

void Plugboard::setPair5(LetterPair pair){
    this->pair5 = pair;
}

void Plugboard::setPair6(LetterPair pair){
    this->pair6 = pair;
}

bool Plugboard::isPlugged(char c){
    return (this->pair1.contains(c) && this->pair1.getOne() != this->pair1.getTwo())
           || (this->pair2.contains(c) && this->pair2.getOne() != this->pair2.getTwo())
           || (this->pair3.contains(c) && this->pair3.getOne() != this->pair3.getTwo())
           || (this->pair4.contains(c) && this->pair4.getOne() != this->pair4.getTwo())
           || (this->pair5.contains(c) && this->pair5.getOne() != this->pair5.getTwo())
           || (this->pair6.contains(c) && this->pair6.getOne() != this->pair6.getTwo());
}

char Plugboard::encrypt(char c){
    if (pair1.contains(c)){
        return pair1.encrypt(c);
    }
    else if (pair2.contains(c)){
        return pair2.encrypt(c);
    }
    else if (pair3.contains(c)){
        return pair3.encrypt(c);
    }
    else if (pair4.contains(c)){
        return pair4.encrypt(c);
    }
    else if (pair5.contains(c)){
        return pair5.encrypt(c);
    }
    else if (pair6.contains(c)){
        return pair6.encrypt(c);
    }
    else {
        return c;
    }
}

RotorSet::RotorSet(){
    this->left = Scrambler();
    this->middle = Scrambler();
    this->right = Scrambler();
}

RotorSet::RotorSet(Scrambler one, Scrambler two, Scrambler three){
    this->left = std::move(one);
    this->middle = std::move(two);
    this->right = std::move(three);
}

Scrambler RotorSet::getLeft(){
    return this->left;
}

Scrambler RotorSet::getMiddle(){
    return this->middle;
}

Scrambler RotorSet::getRight(){
    return this->right;
}

void RotorSet::setScramblers(int offset1, int offset2, int offset3){
    this->left.setOffset(offset1);
    this->middle.setOffset(offset2);
    this->right.setOffset(offset3);
}

void RotorSet::rotate(){
    bool middleRotated = false;
    this->right.rotate();
    if (this->right.getOffset() == this->right.getRingSetting()){
        this->middle.rotate();
        middleRotated = true;
    }
    if (this->middle.getOffset() == this->middle.getRingSetting() && middleRotated){
        this->left.rotate();
    }
}

Scrambler Enigma::I = Scrambler("KPTYUELOCVGRFQDANJMBSWHZXI", "I");
Scrambler Enigma::II = Scrambler("UPHZLWEQMTDJXCAKSOIGVBYFNR", "II");
Scrambler Enigma::III = Scrambler("QUDLYRFEKONVZAXWHMGPJBSICT", "III");

RotorSet Enigma::orderOne = RotorSet(I, II, III);
RotorSet Enigma::orderTwo = RotorSet(I, III, II);
RotorSet Enigma::orderThree = RotorSet(II, I, III);
RotorSet Enigma::orderFour = RotorSet(II, III, I);
RotorSet Enigma::orderFive = RotorSet(III, I, II);
RotorSet Enigma::orderSix = RotorSet(III, II, I);

Enigma::Enigma(RotorSet r, Plugboard p){
    this->rotors = std::move(r);
    this->plugboard = p;
}

void Enigma::changeRotorOrder(RotorSet r){
    this->rotors = std::move(r);
}

void Enigma::randomise(){
    std::vector<Scrambler> scramblers = {I, II, III};
    std::random_shuffle(scramblers.begin(), scramblers.end());
    RotorSet r = {scramblers.at(0), scramblers.at(1), scramblers.at(2)};
    this->changeRotorOrder(r);

    int one = rand() % 26;
    int two = rand() % 26;
    int three = rand() % 26;
    this->rotors.setScramblers(one, two, three);
}

std::string Enigma::printSetting(){
    std::stringstream setting;

    setting << this->rotors.getLeft().getName() << "-" << this->rotors.getMiddle().getName() << "-"
            << this->rotors.getRight().getName() << ", ";
    setting << this->rotors.getLeft().getOffset() << "-" << this->rotors.getMiddle().getOffset() << "-"
            << this->rotors.getRight().getOffset() << ", ";
    setting << this->plugboard.getPair1().getOne() << "-" << this->plugboard.getPair1().getTwo() << "   ";
    setting << this->plugboard.getPair2().getOne() << "-" << this->plugboard.getPair2().getTwo() << "   ";
    setting << this->plugboard.getPair3().getOne() << "-" << this->plugboard.getPair3().getTwo() << "   ";
    setting << this->plugboard.getPair4().getOne() << "-" << this->plugboard.getPair4().getTwo() << "   ";
    setting << this->plugboard.getPair5().getOne() << "-" << this->plugboard.getPair5().getTwo() << "   ";
    setting << this->plugboard.getPair6().getOne() << "-" << this->plugboard.getPair6().getTwo() << "   " << "\n";

    return setting.str();
}

char Enigma::encryptLeft(char c){
    char newc = plugboard.encrypt(c);
    newc = rotors.getRight().encryptLeft(newc);
    newc = rotors.getMiddle().encryptLeft(newc);
    newc = rotors.getLeft().encryptLeft(newc);
    newc = reflector.encrypt(newc);
    return newc;
}

char Enigma::encryptRight(char c){
    char newc = rotors.getLeft().encryptRight(c);
    newc = rotors.getMiddle().encryptRight(newc);
    newc = rotors.getRight().encryptRight(newc);
    newc = plugboard.encrypt(newc);
    return newc;
}

std::string Enigma::encrypt(const std::string message) {
    std::string messageUpper;
    char cUpper;
    for (char c : message) {
        cUpper = toupper(c);
        messageUpper.push_back(cUpper);
    }
    messageUpper.erase(std::remove_if(messageUpper.begin(), messageUpper.end(), isspace), messageUpper.end());
    messageUpper.erase(std::remove_if(messageUpper.begin(), messageUpper.end(), ispunct), messageUpper.end());

    std::string encrypted;

    for (char c : messageUpper) {
        this->rotors.rotate();
        char newc = encryptRight(encryptLeft(c));
        encrypted.push_back(newc);
    }
    return encrypted;
}