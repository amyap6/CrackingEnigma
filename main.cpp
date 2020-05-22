#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include <chrono>
#include "Enigma.h"
#include "ModernCracker.h"

using namespace std::chrono;

/** Returns the string in a formatted way to improve readability and conform with radio operations standards on
 * which Enigma messages were sent. Returns a string with a space between every 5 letters
 *
 * @param s the string to be formatted
 * @returns the formatted string
 */
std::string formatOutput(std::string s){
    std::string formatted;
    int counter = 1;

    for (char c : s){
        if (counter < 5){
            formatted.push_back(c);
        }
        else if (counter == 5){
            counter = 0;
            formatted.push_back(c);
            formatted.push_back(' ');
        }
        counter++;
    }
    return formatted;
}

int main() {

    Plugboard plugboard = Plugboard(LetterPair('G', 'P'), LetterPair('M', 'X'),
            LetterPair('U', 'D'), LetterPair('N', 'K'), LetterPair('V', 'J'),
            LetterPair('P', 'L'));
    Enigma enigma = Enigma(Enigma::orderThree, plugboard);
    enigma.rotors.setScramblers(13, 0, 16);

    std::fstream input;
    input.open("collection1.csv");

    std::vector<std::string> messages;
    std::string currentMessage;

    while(getline(input, currentMessage)){
        //skip first six letters -> the message key
        //this is to ensure that message key letters do not skew the letter distribution for the text
        currentMessage.erase(0, 6);

        messages.push_back(currentMessage);
    }

    std::string message = "BEFOREWRITINGTHISHISTORYIHAVENOTHADTHEUNPLEASANTTASKOFREADINGVOLUMINOUSRECORDSANDSCANNINGINNUMERABLEDOCUMENTS1WEHAVENEVERBEENENTHUSIASTICKEEPERSOFDIARIESANDLOGBOOKSANDHAVEHABITUALLYDESTROYEDRECORDSWHENTHEIRPERIODOFUTILITYWASOVERANDITISTHEMERESTCHANCETHATHASPRESERVEDAFEWDOCUMENTSOFINTERESTHARDLYANYOFTHESEAREDATEDSINCEMARCHTHEWEEKLYREPORTTOTHEDIRECTORPROVIDESAVALUABLERECORDOFOURACTIVITIESBUTITISNATURALLYTHISMORERECENTPERIODWHICHHUMANMEMORYMOSTEASILYRECALLSANDITISTHELACKOFDOCUMENTARYEVIDENCEABOUTEARLYDAYSWHICHISTHEMOSTSERIOUS";

    std::cout << "index of coincidence before = " << getIndexOfCoincidence(messages[87]) << "\n";

    std::string encrypted = enigma.encrypt(messages[87]);

    std::cout << "index of coincidence after = " << getIndexOfCoincidence(encrypted) << "\n";

    std::cout << "original message= " << formatOutput(messages[87]) << "\n";

    std::cout << "encrypted message= " << formatOutput(encrypted) << "\n";

    auto start = high_resolution_clock::now();

    std::vector<Enigma> potentialEnigmas = crackMessage(enigma, encrypted, "english_bigrams.csv", "english_trigrams.csv");

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<minutes>(stop - start);

    std::vector<std::string> potentialDecrypts;

    for (Enigma correct : potentialEnigmas) {
        std::string cracked = correct.encrypt(encrypted);
        potentialDecrypts.push_back(cracked);
    }

    for (Enigma correct : potentialEnigmas) {
        std::cout << "setting found = " << correct.printSetting();
    }

    for (std::string cracked : potentialDecrypts) {
        std::cout << "potential cracked message = " << formatOutput(cracked) << "\n";
    }

    std::cout << "time taken to crack = " << duration.count() << " minutes\n";

    return 0;
}
