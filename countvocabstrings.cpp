// JEIKO SANTOS 824389729
// NATHNAEL ATLAW 822655984
#include "countvocabstrings.h"
#include "shared.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <pthread.h>
#include <queue>

// External declarations of a queue and mutexes used in the program
extern std::queue<std::string> lineQueue;  // Queue to hold lines read from the input file
extern pthread_mutex_t mutex;  // Mutex for synchronizing access to shared resources
extern pthread_mutex_t printMutex;  // Mutex for synchronizing print operations to avoid interleaving

void* countvocabstrings(void* arg) {
    // Cast the input argument to the correct type (SharedData)
    SharedData* data = (SharedData*) arg;

    // Open an output file stream to write the results
    std::ofstream outputFile("countNumOfContainedVocab.txt");
    
    // Check if the output file is open, if not, print an error message and return
    if (!outputFile.is_open()) {
        pthread_mutex_lock(&printMutex);  // Lock printMutex to avoid interleaving prints
        std::cerr << "Error opening output file." << std::endl;
        pthread_mutex_unlock(&printMutex);  // Unlock printMutex after printing the error message
        return nullptr;
    }

    unsigned int processedLines = 0;  // Counter for the number of processed lines

    // Infinite loop to keep processing lines until the queue is empty
    while (true) {
        pthread_mutex_lock(&mutex);  // Lock mutex to safely access the shared lineQueue
        if (lineQueue.empty()) {  // Check if the queue is empty
            pthread_mutex_unlock(&mutex);  // Unlock mutex before breaking the loop
            break;  // Exit the loop if the queue is empty
        }
        std::string line = lineQueue.front();  // Get the front line from the queue
        lineQueue.pop();  // Remove the front line from the queue
        pthread_mutex_unlock(&mutex);  // Unlock mutex after modifying the queue

        std::istringstream iss(line);  // Create a string stream from the line
        std::string word;  // Variable to hold individual words from the line
        unsigned int count = 0;  // Counter for the number of vocabulary strings in the line
        
        // Loop to extract words from the line and check if they are in the vocabulary
        while (iss >> word) {
            if (data->vocabulary.find(word) != data->vocabulary.end()) {  // Check if the word is in the vocabulary
                count++;  // Increment the count if the word is in the vocabulary
            }
        }
       
        // Update progress bar
        processedLines++;  // Increment the number of processed lines
        // Check if it's time to print a hash mark for progress
        if (data->hashmarkInterval != 0 && processedLines % data->hashmarkInterval == 0) {
            pthread_mutex_lock(&printMutex);  // Lock printMutex to avoid interleaving prints
            std::cout << "#";  // Print a hash mark
            std::cout.flush();  // Ensure the hash mark is printed immediately
            pthread_mutex_unlock(&printMutex);  // Unlock printMutex after printing
        }
        // Check if the count is above the minimum threshold for printing
        if (count >= data->minNumOfVocabStringsContainedForPrinting) {
            outputFile << count << std::endl;  // Write the count to the output file
        }
    }
    outputFile.close();  // Close the output file stream

    // Print newline and total line count after progress bar
    pthread_mutex_lock(&printMutex);  // Lock printMutex to avoid interleaving prints
    std::cout << std::endl << processedLines << std::endl;  // Print the total number of processed lines
    pthread_mutex_unlock(&printMutex);  // Unlock printMutex after printing

    return nullptr;  // Return nullptr indicating the function has completed its execution
}
