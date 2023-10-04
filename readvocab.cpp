#include "readvocab.h"
#include "shared.h"
#include <fstream>
#include <iostream>
#include <pthread.h>

// Global mutex used across threads for synchronized printing
extern pthread_mutex_t printMutex;

void* readvocab(void* arg) {
    // Cast the void pointer back to SharedData pointer
    SharedData* data = (SharedData*) arg;

    // Open the vocabulary file for reading
    std::ifstream vocabFile(data->fileName[0]);
    // Check if the file is open, if not, print an error message and return nullptr
    if (!vocabFile.is_open()) {
        pthread_mutex_lock(&printMutex);  // Lock printMutex to avoid interleaved printing
        std::cerr << "Display Unable to open <<missing_file.txt>>" << std::endl;  // Print error message
        pthread_mutex_unlock(&printMutex);  // Unlock printMutex
        return nullptr;  // Return nullptr indicating the thread is finished
    }

    unsigned int processedWords = 0;  // Counter for the number of processed words
    std::string word;  // Variable to hold a word read from the file
    // Read words from the file until EOF
    while (vocabFile >> word) {
        data->vocabulary.insert(word);  // Insert the read word into the vocabulary set
        processedWords++;  // Increment the counter of processed words

        // Update progress bar
        // Check if hashmarkInterval is not zero and if processedWords is a multiple of hashmarkInterval
        if (data->hashmarkInterval != 0 && processedWords % data->hashmarkInterval == 0) {
            pthread_mutex_lock(&printMutex);  // Lock printMutex to avoid interleaved printing
            std::cout << "#";  // Print a hashmark
            std::cout.flush();  // Flush the output stream to ensure immediate printing
            pthread_mutex_unlock(&printMutex);  // Unlock printMutex
        }
    }

    // Print newline and total word count after progress bar
    pthread_mutex_lock(&printMutex);  // Lock printMutex to avoid interleaved printing
    std::cout << std::endl << processedWords << std::endl;  // Print newline and total word count
    pthread_mutex_unlock(&printMutex);  // Unlock printMutex

    vocabFile.close();  // Close the vocabulary file
    return nullptr;  // Return nullptr indicating the thread is finished
}
