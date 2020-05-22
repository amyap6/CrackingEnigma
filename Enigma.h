/**
 * @file Enigma.h
 *
 * @brief Enigma machine components modelled in classes and functions
 *
 * @ingroup Modern
 *
 * @author Amanda Pryor
 * Contact: agy17huu@uea.ac.uk
 *
 */

#ifndef MODERN_ENIGMA_H
#define MODERN_ENIGMA_H

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include <sstream>

/** Class modelling an Enigma Scrambling wheel.
 *
 * The Scrambler's wirings are represented by a substitution cipher, and the rotation by the offset.
 * The Scrambler can encrypt a character in two directions.
 */
class Scrambler{
private:
    std::string name;
    std::string substitutionCipher;
    //assuming a ring setting of A for every scrambler, in practice this may be modifiable
    int ringSetting = 0;
    int offset = 0;
public:
    static std::string alphabet;

    /** Default constructor initialises a Scrambler object with the identity cipher as its substitution cipher
     */
    Scrambler();
    Scrambler(std::string substitutionCipher, std::string name);

    std::string getName();
    int getRingSetting();
    int getOffset();

    void setRingSetting(int i);
    void setOffset(int i);

    /** When rotate is called on a Scrambler, its offset increases by 1.
     * When the increased offset is greater than 25, it is set back to 0.
     */
    void rotate();
    void rotate(int rotations);

    /** This encrypts the character by finding its index in the alphabet, finding which character is at this index
     *      in the Scrambler's substitution cipher, and returning this character.
     *
     * @param c the character to be encrypted
     * @return the encrypted character
     */
    char encryptLeft(char c);

    /** This encrypts the character by finding its index in the Scrambler's substitution cipher, finding which character
     *      is at this index in the alphabet, and returning this character.
     *
     * @param c the character to be encrypted
     * @return the encrypted character
     */
    char encryptRight(char c);
};

/** Class modelling an Enigma reflector wheel
 *
 * The reflector contains physical wirings represented as a substitution cipher, and can encrypt a character.
 */
class Reflector{
private:
    std::string substitutionCipher;
public:
    explicit Reflector(std::string cipher);

    /** Encrypts a character by finding the index of the character in the alphabet, and returning the character at
     *      this index in the substitution cipher.
     *
     * @param c character to be encrypted
     * @returns the encrypted character
     */
    char encrypt(char c);
};

/** Class modelling a pair of characters that have been 'plugged' together on an Enigma plugboard
 *
 * When two characters are 'plugged' together, they can be encrypted as each other with the plugboard.
 */
class LetterPair{
private:
    char one;
    char two;
public:

    LetterPair();
    LetterPair(char c, char d);

    char getOne();
    char getTwo();

    /** @returns true if either this->one or this->two are equal to c
     *
     * @param c the character to be searched for
     */
    bool contains(char c);

    /** Encrypts a character with its pair, assumes that contains() has already been called for the character
     *
     * @param c the character to encrypt
     * @returns the encrypted character
     */
    char encrypt(char c);
};

/** Class modelling an Enigma plugboard, in which 6 pairs of characters are plugged together.
 *
 * The plugboard forms one step of the encryption process of an Enigma machine, in which a letter is encrypted as
 *      its pair if it is plugged. This happens both before and after the rotor encryptions.
 */
class Plugboard{
private:
    LetterPair pair1;
    LetterPair pair2;
    LetterPair pair3;
    LetterPair pair4;
    LetterPair pair5;
    LetterPair pair6;
public:

    /** This constructor initialises an "empty" plugboard, where all the LetterPairs contain "self-plugged" letters.
     *  This means that no letters are swapped by the plugboard step of the encryption.
     */
    Plugboard();

    /** This constructor assumes that each LetterPair param passed contains no repeated letters.
     *  i.e. if the user sets param 'one' to contain 'A', they should not set any other pair to contain 'A'
     */
    Plugboard(LetterPair one, LetterPair two, LetterPair three, LetterPair four,
              LetterPair five, LetterPair six);

    LetterPair getPair1();
    LetterPair getPair2();
    LetterPair getPair3();
    LetterPair getPair4();
    LetterPair getPair5();
    LetterPair getPair6();

    void setPair1(LetterPair pair);
    void setPair2(LetterPair pair);
    void setPair3(LetterPair pair);
    void setPair4(LetterPair pair);
    void setPair5(LetterPair pair);
    void setPair6(LetterPair pair);

    /** Checks whether a character has been plugged to a different letter on the plugboard
     *
     * @param c the character to check
     * @returns true if the character is plugged
     */
    bool isPlugged(char c);

    /** Checks if the character is a member of a LetterPair object in this Plugboard
     *  If so, @returns the encrypted character, otherwise @returns the original character
     *
     * @param c the character to be encrypted
     * @returns the encrypted character
     */
    char encrypt(char c);
};

/** Class representing a Rotor Set in an Enigma machine
 *
 *  The Rotor Set contains 3 Scrambler slots, and a Reflector.
 *  The rotor set can encrypt a character.
 */
class RotorSet{
private:
    Scrambler left;
    Scrambler middle;
    Scrambler right;
public:
    RotorSet();

    /** This constructor assumes that the user does not try to place the same scrambler in two different slots
     * i.e if param 'one' is Scrambler II, 'two' or 'three' should not be II.
     */
    RotorSet(Scrambler one, Scrambler two, Scrambler three);

    Scrambler getLeft();
    Scrambler getMiddle();
    Scrambler getRight();

    /** Sets the offsets of each Scrambler
     *
     * @param offset1 the desired offset for the Scrambler in the left-most position
     * @param offset2 the desired offset for the Scrambler in the middle position
     * @param offset3 the desired offset for the Scrambler in the right-most position
     */
    void setScramblers(int offset1, int offset2, int offset3);

    /** Rotates the RotorSet unit.
     *
     *  The right-most scrambler rotates after every inputted character.
     *  The middle scrambler rotates when the right-most ring-setting is reached (every 26 letters)
     *  The left scrambler rotates when the middle ring-setting is reached (every 626 letters)
     */
    void rotate();
};

/** Class modelling an Enigma machine
 *
 * Each Enigma contains a RotorSet consisting of 3 Scramblers (both defined in Cyclometer.h), a Reflector,
 *      and a Plugboard.
 * The Enigma encrypts a string using the current state of its rotors and plugboard as the key.
 */
class Enigma{
public:

    static Scrambler I;
    static Scrambler II;
    static Scrambler III;

    static RotorSet orderOne;
    static RotorSet orderTwo;
    static RotorSet orderThree;
    static RotorSet orderFour;
    static RotorSet orderFive;
    static RotorSet orderSix;

    RotorSet rotors;
    Reflector reflector = Reflector("GEKPBTAUMOCNILJDXZYFHWVQSR");
    Plugboard plugboard;

    Enigma(RotorSet r, Plugboard p);

    void changeRotorOrder(RotorSet r);

    /** Sets the rotors to a random configuration, does not affect plugboard
     */
    void randomise();

    /** Encrypts a character with the plugboard, 3 Scramblers in the "inward" direction and the Reflector
     *
     * @param c the character to be encrypted
     * @returns the encrypted character after the Reflector.
     */
    char encryptLeft(char c);

    /** Encrypts a character with 3 Scramblers in the "outward" direction and the plugboard.
     *
     * @param c the character to be encrypted
     * @returns the encrypted character after the Scrambler in thr right-most slot.
     */
    char encryptRight(char c);

    /** Fully encrypts the string by calling encryptLeft() and encryptRight() on each character
     *
     * This method assumes that message is a string of the form "THISISANEXAMPLE"
     * Where every letter is capital, and there are no non-letter characters.
     *
     * @param message the string to be encrypted
     * @returns the encrypted string after the complete encryption process
     */
    std::string encrypt(std::string message);

    /**@returns the current setting of this Enigma as a string
     *  In the format I-II-III, 0-0-0, A-A B-B C-C D-D E-E F-F
     */
    std::string printSetting();
};


#endif //MODERN_ENIGMA_H
