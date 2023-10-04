// JEIKO SANTOS 824389729
// NATHNAEL ATLAW 822655984
#include "readlines.h"
#include "shared.h"
#include <fstream>
#include <iostream>
#include <pthread.h>

// Global queue and mutexes used across threads
extern std::queue<std::string> lineQueue;  // Queue to hold lines read from the input file
extern pthread_mutex_t mutex;  // Mutex for synchronizing access to shared resources
extern pthread_mutex_t printMutex;  // Mutex for synchronizing print operations to avoid interleaving

void* readlines(void* arg) {
    // Cast the void pointer back to SharedData pointer
    SharedData* data = (SharedData*) arg;

    // Open the test file for reading
    std::ifstream testFile(data->fileName[1]);
    // Check if the file is open, if not, print an error message and return nullptr
    if (!testFile.is_open()) {
        pthread_mutex_lock(&printMutex);  // Lock printMutex to avoid interleaved printing
        std::cerr << "Display Unable to open <<missing_file.txt>>" << std::endl;  // Print error message
        pthread_mutex_unlock(&printMutex);  // Unlock printMutex
        return nullptr;  // Return nullptr indicating the thread is finished
    }

    std::string line;  // Variable to hold a line read from the file
    unsigned int processedLines = 0;  // Counter for the number of processed lines
    // Read lines from the file until EOF
    while (std::getline(testFile, line)) {
        pthread_mutex_lock(&mutex);  // Lock mutex to avoid concurrent access to lineQueue
        lineQueue.push(line);  // Push the read line into the queue
        pthread_mutex_unlock(&mutex);  // Unlock mutex

        processedLines++;  // Increment the counter of processed lines

        // Update progress bar
        // Check if hashmarkInterval is not zero and if processedLines is a multiple of hashmarkInterval
        if (data->hashmarkInterval != 0 && processedLines % data->hashmarkInterval == 0) {
            pthread_mutex_lock(&printMutex);  // Lock printMutex to avoid interleaved printing
            std::cout << "#";  // Print a hashmark
            std::cout.flush();  // Flush the output stream to ensure immediate printing
            pthread_mutex_unlock(&printMutex);  // Unlock printMutex
        }
    }
    testFile.close();  // Close the test file
    
    // Print newline and total line count after progress bar
    pthread_mutex_lock(&printMutex);  // Lock printMutex to avoid interleaved printing
    std::cout << std::endl << processedLines << std::endl;  // Print newline and total line count
    pthread_mutex_unlock(&printMutex);  // Unlock printMutex

    return nullptr;  // Return nullptr indicating the thread is finished
}
