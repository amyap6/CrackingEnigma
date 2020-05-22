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

#include <string>
#include <algorithm>
#include "Enigma.h"
#include "ModernCracker.h"

double getIndexOfCoincidence(std::string message){
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    double sum = 0;
    for (char i : alphabet){
        int count = std::count(message.begin(), message.end(), i);
        sum = sum + (count*(count - 1));
    }
    double indexCoincidence = sum / (message.length()*(message.length() - 1));
    return indexCoincidence;
}

static void print(std::vector<RotorSet> possibleKeys){
    std::stringstream ss;
    for (RotorSet r : possibleKeys){
        ss << r.getLeft().getName() << "-" << r.getMiddle().getName() << "-" << r.getRight().getName() << ", "
           << r.getLeft().getOffset() << "-" << r.getMiddle().getOffset() << "-" << r.getRight().getOffset() << "\n";
    }
    std::cout << ss.str();
}

std::vector<RotorSet> testSetting(std::vector<RotorSet> possibleKeys, Enigma enigma, std::string message){
    RotorSet settings = enigma.rotors;
    std::string decrypted = enigma.encrypt(std::move(message));
    double IC = getIndexOfCoincidence(decrypted);
    if (IC >= 0.05)
    {
        possibleKeys.push_back(settings);
    }
    return possibleKeys;
}

RotorSet findBestSetting(Enigma enigma, std::string message, std::vector<RotorSet> possibleKeys){
    RotorSet highestIC;
    double currentIC = 0.0;

    for (RotorSet r : possibleKeys) {
        enigma.rotors = r;

        std::string decrypted = enigma.encrypt(message);
        double IC = getIndexOfCoincidence(decrypted);

        if (IC > currentIC){
            highestIC = r;
            currentIC = IC;
        }
    }
    return highestIC;
}

std::vector<RotorSet> testAllOffsets(std::vector<RotorSet> possibleKeys, Enigma enigma, std::string message){
    for (int offset1 = 0; offset1 < 26; offset1++){
        for (int offset2 = 0; offset2 < 26; offset2++){
            for (int offset3 = 0; offset3 < 26; offset3++){
                enigma.rotors.setScramblers(offset1, offset2, offset3);
                possibleKeys = testSetting(possibleKeys, enigma, message);
            }
        }
    }
    return possibleKeys;
}

std::vector<RotorSet> testAllSettings(Enigma enigma, std::string message){

    std::vector<RotorSet> possibleKeys;

    enigma.rotors = Enigma::orderOne;
    possibleKeys = testAllOffsets(possibleKeys, enigma, message);

    enigma.rotors = Enigma::orderTwo;
    possibleKeys = testAllOffsets(possibleKeys, enigma, message);

    enigma.rotors = Enigma::orderThree;
    possibleKeys = testAllOffsets(possibleKeys, enigma, message);

    enigma.rotors = Enigma::orderFour;
    possibleKeys = testAllOffsets(possibleKeys, enigma, message);

    enigma.rotors = Enigma::orderFive;
    possibleKeys = testAllOffsets(possibleKeys, enigma, message);

    enigma.rotors = Enigma::orderSix;
    possibleKeys = testAllOffsets(possibleKeys, enigma, message);

    return possibleKeys;
}

double searchFile(std::string filePath, std::string ngram){
    double ngramScore = 0.0;

    std::fstream ngramScores;
    ngramScores.open(filePath);

    std::string line;

    while (getline(ngramScores, line)){
        std::stringstream ss(line);

        std::string ngramVal;
        getline(ss, ngramVal, ',');

        if (ngramVal == ngram){
            std::string score;
            getline(ss, score);

            int count = std::stoi(score);
            ngramScore = (double) count;
        }
    }

    return ngramScore;
}

std::string trySwap(Enigma enigma, std::string message, char letter1, char letter2){

    LetterPair test = LetterPair(letter1, letter2);

    enigma.plugboard.setPair6(test);

    std::string decrypted = enigma.encrypt(std::move(message));

    return decrypted;
}

LetterPair findPairNgram(Enigma enigma, std::string message, std::string filePath, int typeOfScoring){

    std::string freqAlphabet = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
    RotorSet startSetting = enigma.rotors;

    LetterPair pair = LetterPair('A', 'A');
    double bestScore = 0.0;

    double currentScore;

    for (int i = 0; i < freqAlphabet.length(); i++) {
        for (int j = i + 1; j < freqAlphabet.length(); j++) {
            currentScore = 0.0;

            char letter1 = freqAlphabet[i];
            char letter2 = freqAlphabet[j];

            if (letter1 != letter2 && !enigma.plugboard.isPlugged(letter1) && !enigma.plugboard.isPlugged(letter2)) {

                enigma.rotors = startSetting;
                std::string swapped = trySwap(enigma, message, letter1, letter2);

                for (int k = 0; k < swapped.length() - (typeOfScoring - 1); k++) {
                    std::string substr = swapped.substr(k, typeOfScoring);
                    double score = searchFile(filePath, substr);
                    currentScore += score;
                }
                if (currentScore > bestScore) {
                    pair = LetterPair(letter1, letter2);
                    bestScore = currentScore;
                }
            }
        }
    }
    return pair;
}

Enigma findPlugboardSettings(Enigma enigma, std::string message, std::string bigramFilePath, std::string trigramFilePath){

    LetterPair firstPair = findPairNgram(enigma, message, bigramFilePath, 2);
    enigma.plugboard.setPair1(firstPair);

    LetterPair secondPair = findPairNgram(enigma, message, bigramFilePath, 2);
    enigma.plugboard.setPair2(secondPair);

    LetterPair thirdPair = findPairNgram(enigma, message, bigramFilePath, 2);
    enigma.plugboard.setPair3(thirdPair);

    LetterPair fourthPair = findPairNgram(enigma, message, bigramFilePath, 2);
    enigma.plugboard.setPair4(fourthPair);

    LetterPair fifthPair = findPairNgram(enigma, message, bigramFilePath, 2);
    enigma.plugboard.setPair5(fifthPair);

    LetterPair sixthPair = findPairNgram(enigma, message, bigramFilePath, 2);
    enigma.plugboard.setPair6(sixthPair);

    return enigma;
}

bool sameKey(std::string message1, std::string message2){
    bool sameKey;
    int matches = 0;
    double matchRate;
    if (message1.length() < message2.length()){
        for (int i = 0; i < message1.length(); i++){
            char one = message1[i];
            char two = message2[i];
            if (one == two){
                matches++;
            }
        }
        matchRate = matches/message1.length();
    }
    else {
        for (int i = 0; i < message2.length(); i++){
            char one = message1[i];
            char two = message2[i];
            if (one == two){
                matches++;
            }
        }
        matchRate = (double) matches/message2.length();
    }
    sameKey = matchRate > 0.05;
    return sameKey;
}

std::vector<Enigma> crackMessage(Enigma enigma, std::string message, std::string bigramFilePath, std::string trigramFilePath){
    std::vector<Enigma> enigmas;

    if (message.length() < 94){
        std::cout << "Message too short to be cracked. Try a longer message";
        return enigmas;
    }

    std::vector<RotorSet> keys = testAllSettings(enigma, message);

    Plugboard empty = Plugboard(LetterPair('A', 'A'), LetterPair('B', 'B'),
                                    LetterPair('C', 'C'), LetterPair('D', 'D'), LetterPair('E', 'E'),
                                    LetterPair('F', 'F'));
    for (RotorSet r : keys){
        enigma.rotors = r;
        enigma.plugboard = empty;
        Enigma potentialKey = findPlugboardSettings(enigma, message, bigramFilePath, trigramFilePath);
        enigmas.push_back(potentialKey);
    }

    return enigmas;

}

std::vector<Enigma> crackMessages(Enigma enigma, std::string message, std::string message2, std::string bigramFilePath, std::string trigramFilePath){
    std::vector<Enigma> enigmas;

    bool same = sameKey(message, message2);

    std::string msg;

    if (same){
        msg = message.append(message2);
        enigmas = crackMessage(std::move(enigma), msg, std::move(bigramFilePath), std::move(trigramFilePath));
        return enigmas;
    }
    else {
        std::cout << "Messages were not encrypted using the same setting. Please crack them seperately using "
                     "crackMessage(). \n";
        return enigmas;
    }
}