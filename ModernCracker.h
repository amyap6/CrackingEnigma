/**
 * @file ModernCracker.h
 *
 * @brief A program to crack individual Enigma messages using brute force and hill-climbing processes, combined
 * with linguistic metrics such as Index of Coincidence and bigram scoring.
 *
 * @ingroup Modern
 *
 * @author Amanda Pryor
 * Contact: agy17huu@uea.ac.uk
 *
 */

#ifndef MODERN_MODERNCRACKER_H
#define MODERN_MODERNCRACKER_H

#include <string>
#include "Enigma.h"

/** Finds the index of coincidence of an inputted message. The index of coincidence (IC) is a linguistic metric which
 * describes the letter distribution of the text. A text containing completely random letters will yield an IC of
 * roughly 0.036, whereas a text containing standard English sentences will yield an IC of roughly 0.0667.
 * This can be used to determine whether deciphered messages "look like" English text or not.
 * formula obtained from http://practicalcryptography.com/cryptanalysis/text-characterisation/index-coincidence/
 *
 * @param message the message to study
 * @returns the index of coincidence of the message
 */
double getIndexOfCoincidence(std::string message);

/** Prints a vector of RotorSet objects
 *  In the format I-II-III, A-A-A
 *
 * @param settings set of possible rotorSet settings
 */
static void print(std::vector<RotorSet> possibleKeys);

/** Tests one RotorSet setting, by deciphering the message and finding its IC.
 * If the decrypted IC is greater than 0.05, the setting is added to a vector of potential correct settings
 *
 * @param possibleKeys the current set of possible correct RotorSet settings
 * @param enigma an Enigma object containing the RotorSet settings to test, and an empty plugboard
 * @param message the message being cracked
 * @returns the updated list of possible keys
 */
std::vector<RotorSet> testSetting(std::vector<RotorSet> possibleKeys, Enigma enigma, std::string message);

/** Finds which possible key yeilds the highest IC, and returns this as the most likely key.
 * This can speed up cracking but is not reliable, so has not been used in this program.
 * However, the user may wish to experiment with or use this method to speed up cracking, so the method has been left in
 *
 * @param enigma Enigma object used to decrypt messages
 * @param message the message being cracked
 * @param keys the vector of possible correct keys found
 * @returns a RotorSet object representing the most likely setting
 */
RotorSet findBestSetting(Enigma enigma, std::string message, std::vector<RotorSet> possibleKeys);

/** Tests all possible Scrambler offsets for the current RotorSet order, by calling testSetting on each combination
 *
 * @param possibleKeys the current set of possible correct RotorSet settings
 * @param enigma Enigma object used to decrypt messages
 * @param message the message being cracked
 * @returns the updated list of possible keys
 */
std::vector<RotorSet> testAllOffsets(std::vector<RotorSet> possibleKeys, Enigma enigma, std::string message);

/** Tests all possible RotorSet settings by calling testAllOffsets for each rotor order.
 *
 * @param enigma Enigma object used to decrypt messages
 * @param message the message being cracked
 * @returns a vector containing possible correct RotorSet settings
 */
std::vector<RotorSet> testAllSettings(Enigma enigma, std::string message);

/** Searches a file containing either bigrams or trigrams to find the associated score. More common ngrams are given
 * higher scores, i.e. 'ING' would be associated with a higher number than 'SZX'.
 * These scores can be used to assess how much a piece of text "looks like" English language.
 *
 * @param filePath the file path to be searched
 * @param ngram the ngram to be searched for
 * @returns the score associated with this ngram
 */
double searchFile(std::string filePath, std::string ngram);

/** Assigns a new LetterPair swap to the Enigma object and deciphers the message using the Enigma object.
 * The new LetterPair object is always set to pair6 to avoid overwriting previously determined correct letter swaps.
 *
 * @param enigma Enigma object used for decrypting messages
 * @param message the message being cracked
 * @param letter1 the first letter being tested as a swap
 * @param letter2 the second letter being tested as a swap
 * @returns the decrypted message using these settings
 */
std::string trySwap(Enigma enigma, std::string message, char letter1, char letter2);

/** Finds one plugboard pair based on either bigram or trigram scoring.
 * In this program as it is, only bigram scoring is used, but the option of using trigrams is available to the user.
 * The method tries plugboard swaps in order of most frequent letters in English language, since this will make the
 * most difference to the scores. The message is then decrypted using the potential swap, and it is analysed to find
 * the total of its bigram scores. If this is better than the current best swap, it replaces it. The method continues
 * optimising the bigram score, and returns the swap that generated the highest score.
 *
 * @param enigma Enigma object used for decrypting messages
 * @param message the message being cracked
 * @param filePath the path of file to be searched
 * @param typeOfScoring the type of scoring being used represented as either a 2 or 3
 * @returns the swap which generated the highest ngram score.
 */
LetterPair findPairNgram(Enigma enigma, std::string message, std::string filePath, int typeOfScoring);

/** Finds all correct plugboard settings by calling findPairNgram for each swap
 *
 * @param enigma Enigma object used for decrypting messages, with correct RotorSet settings already found
 * @param message the message being cracked
 * @param filePathBi the path of the file containing a map of bigram scores
 * @param filePathTri the path of the file containing a map of trigram scores
 * @returns the complete Enigma setting used to generate this message
 */
Enigma findPlugboardSettings(Enigma enigma, std::string message, std::string bigramFilePath, std::string trigramFilePath);

/** Finds whether two messages were likely to have been encrypted using the same key, by analysing how many letters
 * match each other in the messages. If both were encrypted with different settings, the match rate will be roughly
 * 1/26, but if they were encrypted with the same setting, the match rate will be roughly 1/13.
 * This method compares as many letters as the shorter messages length
 *
 * @param message1 the first message being examined
 * @param message2 the second message being examined
 * @returns true if both messages were encrypted using the same key
 */
bool sameKey(std::string message1, std::string message2);

/** Cracks a single message encrypted with an Enigma machine, by assessing the messages suitability, finding the
 * set of possible correct RotorSet settings, and finding the correct plugboard swaps for each of these RotorSet objects
 *
 * @param enigma Enigma object used to decrypt messages
 * @param message the message being cracked
 * @param bigramFilePath the path of the file containing a map of bigram scores
 * @param trigramFilePath the path of the file containing a map of trigram scores
 * @returns the set of possible Enigma settings used to encrypt this message
 */
std::vector<Enigma> crackMessage(Enigma enigma, std::string message, std::string bigramFilePath, std::string trigramFilePath);

/** Cracks two messages simultaneously if they pass the sameKey test, by appending them together and calling
 * crackMessage
 *
 * @param enigma Enigma object used to decrypt messages
 * @param message the first message being cracked
 * @param message2 the second message being cracked
 * @param bigramFilePath the path of the file containing a map of bigram scores
 * @param trigramFilePath the path of a file containing a map of trigram scores
 * @returns the set of possible Enigma settings used to encrypt these messages
 */
std::vector<Enigma> crackMessages(Enigma enigma, std::string message, std::string message2, std::string bigramFilePath, std::string trigramFilePath);

#endif //MODERN_MODERNCRACKER_H
