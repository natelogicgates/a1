// JEIKO SANTOS 824389729
// NATHNAEL ATLAW 822655984
#pragma once
#include <string>
#include <queue>
#include <set>

extern std::queue<std::string> lineQueue;
extern pthread_mutex_t mutex;
extern pthread_mutex_t printMutex;  // Mutex for safe printing

struct SharedData {
    unsigned int numOfProgressMarks;
    unsigned int hashmarkInterval;
    unsigned int minNumOfVocabStringsContainedForPrinting;
    const char *fileName[2];
    unsigned long totalNumOfCharsInVocabFile;
    std::set<std::string> vocabulary;
};
