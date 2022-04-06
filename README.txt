This program is a modern solution to crack the Enigma cipher, using language analysis and brute-force style algorithms. This work was done as part of my third year dissertation in my Computer Science undergraduate degree.

Dependencies to be able to use this program:
1. Make sure you have a C/C++ compiler installed on your machine. 
	The Visual Studio compiler can be downloaded here: https://visualstudio.microsoft.com/vs/features/cplusplus/
2. Make sure you have an IDE on your machine. 
	The CLion IDE can be downloaded here: https://www.jetbrains.com/clion/download/#section=windows

To use this program:
1. Open Modern in your favourite IDE.
2. Configure the plugboard swaps by changing the LetterPair characters on lines 38 to 40 in main.cpp.
	A letter must not be a member of more than one LetterPair object.
3. Configure the order of the rotors by changing "orderOne" on line 41 in main.cpp to an order between One and Six.
	Rotor orders are specified on lines 290 to 295 in Enigma.cpp
4. Configure the rotation/offsets of the rotors by changing the numbers on line 42 in main.cpp.
	The numbers represent indices in the alphabet, i.e. 0 = A, 25 = Z. 
5. Choose a message to encrypt and crack. You can either choose a message from the "messages" collection, or type your own.
	5a. If you would like to crack a message from the collection, change the message index on lines 64, 66 and 68 to this message. 
	5b. If you would like to crack your own message, go to line 58 and replace the text in between the double quotes with your message. Please ensure your message contains only capital letters, with no lower case letters, whitespace, punctuation or numbers. Make sure you change lines 64, 66, and 68 so that they apply to your message.
6. Run main.cpp in the IDE.
7. The possible correct settings found and the potential decrypts will be printed to the console. Please note that this program has a runtime of between 25 and 70 minutes. 
