// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void generateWordsandValidate(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict,
    std::string word,
    std::set<std::string>& words);

bool validateWord(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> words;


    if (in.empty()) return words;

    else if (in.find("-") == string::npos) words.insert(in);

    else {
        generateWordsandValidate(in, floating, dict,"", words);
    }

    return words;  
}

bool validateWord(
    std::string& word,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    //check first if all the floating characters (including multiplicity if there are duplicates) are present in the word
    for (int i = 0; i < floating.size(); i++) {
        if (word.find(floating[i]) == string::npos) return false;
        std::string float_clone = floating;
        float_clone.erase(i, 1);
        if (!float_clone.empty()) return validateWord(word, float_clone, dict); 
    }

    //check if word is in dictionary
    if (dict.find(word) == dict.end()) return false;

    return true;
}
// Define any helper functions here
void generateWordsandValidate(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict,
    std::string word,
    std::set<std::string>& words) 
{
        
    //first check if word length and input length are same as that means we have a full word with no blanks
    int dash_count = 0;
    for (int i = word.size(); i < in.size(); i++) {
        if (in[i] == '-') {
            dash_count++;
        }
    }

    //if number of chars in word = number of chars in input then add it to words set
    if (word.size() == in.size()) {
        if(validateWord(word, floating, dict)) {
            words.insert(word);
        }
        return;
    }

    //if given corresponding character in 'in' isn't a '-', simply add character to word and then recurse
    else if (in[word.size()] != '-') {
        word += in[word.size()]; 
        generateWordsandValidate(in, floating, dict, word, words);
    }
    
    //if there are as many floats as there are missing letters
    //instead of generating all possible combinations with all letters, only generate all possible combinations with floating letters
    //then recurse which leads to a handful of combinations and thus lower runtime.
    else if (floating.size() == dash_count) {
        for (int i = 0; i < floating.size(); i++) {
            std::string float_clone = floating;
            word += floating[i];
            float_clone.erase(i, 1);
            generateWordsandValidate(in, float_clone, dict, word, words);
        }
    }

    //generate every possible combo for non-floating-letter blanks 
    else {
        for (char letter = 'a'; letter <= 'z'; ++letter) {
            std::string float_clone = floating;
            if (float_clone.find(letter) != string::npos) {
                float_clone.erase(float_clone.find(letter), 1);
            }
            generateWordsandValidate(in, float_clone, dict, word + letter, words);
        }
    }
}
