// JEIKO SANTOS 824389729
// NATHNAEL ATLAW 822655984
#include <iostream>
#include <pthread.h>
#include <queue>
#include <string>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <set>

#include "shared.h"
#include "readvocab.h"
#include "readlines.h"
#include "countvocabstrings.h"

#define MIN_NUMOF_MARKS 10
#define DEFAULT_HASHMARKINTERVAL 10
#define DEFAULT_MINNUM_OFVOCABSTRINGS_CONTAINED 0
#define NUMOFFILES 2

// Global queue and mutexes used across threads
std::queue<std::string> lineQueue;  // Queue to hold lines read from the input file
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex for synchronizing access to shared resources
pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex for synchronizing print operations to avoid interleaving

// Function: main
// Purpose:  The main function of the program. It initializes shared data, creates threads, and manages their execution.
//           It also handles command-line arguments and ensures synchronized access to shared resources using mutexes.
// Input:    argc - The number of command-line arguments.
//           argv - An array of command-line arguments.
// Output:   int - Returns 0 on successful execution, and 1 on error or invalid input.
int main(int argc, char* argv[]) {
    SharedData data;  // Data structure to hold shared data across threads
    int opt;  // Variable to hold the option character during command-line argument parsing

    // Parse command-line options and set corresponding parameters in the shared data
    while ((opt = getopt(argc, argv, "p:m:v:")) != -1) {
        switch (opt) {
            case 'p':
                data.numOfProgressMarks = atoi(optarg);  // Set the number of progress marks
                break;
            case 'm':
                data.hashmarkInterval = atoi(optarg);  // Set the hashmark interval
                break;
            case 'v':
                data.minNumOfVocabStringsContainedForPrinting = atoi(optarg);  // Set the minimum number of vocabulary strings for printing
                break;
            default:
                std::cerr << "Invalid arguments provided." << std::endl;  // Print error message for invalid arguments
                return 1;  // Return 1 indicating an error
        }
    }

    // Check if the correct number of non-option arguments (file paths) are provided
    if (argc - optind != 2) {
        std::cerr << "Please provide the paths for vocabulary and test files." << std::endl;  // Print error message
        return 1;  // Return 1 indicating an error
    }
    // Store the file paths in the shared data
    data.fileName[0] = argv[optind];
    data.fileName[1] = argv[optind + 1];

    // Thread identifiers
    pthread_t readvocabThread, readlinesThread, countvocabstringsThread;

    // Create threads and pass the shared data as an argument to them
    pthread_create(&readvocabThread, nullptr, readvocab, &data);
    pthread_create(&readlinesThread, nullptr, readlines, &data);
    pthread_create(&countvocabstringsThread, nullptr, countvocabstrings, &data);

    // Wait for the threads to finish their execution
    pthread_join(readvocabThread, nullptr);
    pthread_join(readlinesThread, nullptr);
    pthread_join(countvocabstringsThread, nullptr);

    // Destroy the mutexes to free resources
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&printMutex);

    return 0;  // Return 0 indicating successful execution
}
