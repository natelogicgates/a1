#include "readvocab.h"
#include "shared.h"
#include <fstream>
#include <iostream>
#include <pthread.h>

extern pthread_mutex_t printMutex;

void* readvocab(void* arg) {
    SharedData* data = (SharedData*) arg;
    std::ifstream vocabFile(data->fileName[0]);
    if (!vocabFile.is_open()) {
        std::cerr << "Display Unable to open <<missing_file.txt>>" << std::endl;
        return nullptr;
    }

    unsigned int processedWords = 0;
    std::string word;
    while (vocabFile  >> word) {
        data->vocabulary.insert(word);
        processedWords++;

        // Update progress bar
        if (data->hashmarkInterval != 0 && processedWords % data->hashmarkInterval == 0) {
            pthread_mutex_lock(&printMutex);
            std::cout << "#";
            std::cout.flush();
            pthread_mutex_unlock(&printMutex);
        }
    }

    // Print newline and word count after progress bar
    pthread_mutex_lock(&printMutex);
    std::cout << std::endl << processedWords << std::endl;
    pthread_mutex_unlock(&printMutex);

    vocabFile.close();
    return nullptr;
}
