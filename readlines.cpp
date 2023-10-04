// JEIKO SANTOS 824389729
// NATHNAEL ATLAW 822655984
#include "readlines.h"
#include "shared.h"
#include <fstream>
#include <iostream>
#include <pthread.h>

extern std::queue<std::string> lineQueue;
extern pthread_mutex_t mutex;
extern pthread_mutex_t printMutex;  // Ensure printMutex is available

void* readlines(void* arg) {
    SharedData* data = (SharedData*) arg;
    std::ifstream testFile(data->fileName[1]);
    if (!testFile.is_open()) {
        pthread_mutex_lock(&printMutex);
        std::cerr << "Display Unable to open <<missing_file.txt>>" << std::endl;
        pthread_mutex_unlock(&printMutex);
        return nullptr;
    }

    std::string line;
    unsigned int processedLines = 0;
    while (std::getline(testFile, line)) {
        pthread_mutex_lock(&mutex);
        lineQueue.push(line);
        pthread_mutex_unlock(&mutex);
       
        processedLines++;

       // Update progress bar
        if (data->hashmarkInterval != 0 && processedLines % data->hashmarkInterval == 0) {
            pthread_mutex_lock(&printMutex);
            std::cout << "#";
            std::cout.flush();
            pthread_mutex_unlock(&printMutex);
        }
    }
    testFile.close();
    
    // Print newline and line count after progress bar
    pthread_mutex_lock(&printMutex);
    std::cout << std::endl << processedLines << std::endl;
    pthread_mutex_unlock(&printMutex);

    return nullptr;
}
