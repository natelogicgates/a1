// JEIKO SANTOS 824389729
// NATHNAEL ATLAW 822655984

#include <bits/stdc++.h>
using namespace std;

// Class representing a character node in the Trie
class charNode {
public:
    charNode *links[31];  // Pointers to the next character nodes (includes alphabets and special characters)
    bool flag;  // Flag to mark the end of a word
    int wordCount;  // Field to hold the exact word count for this node

    // Constructor to initialize the node
    charNode() : flag(false), wordCount(0) {
        memset(links, 0, sizeof(links));  // Initialize the links to NULL
    }
};

// Main Trie class
class Tree {
private:
    charNode *root;  // Root node of the Trie

public:
    // Constructor to initialize the Trie
    Tree() {
        root = new charNode();
    }

    // Function to convert character to index for Trie
    int charToIndex(char c) {
            if (c == ' ')
               return 26;
          else if (c == '\'')
               return 27;
          else if (c == '-')
               return 28;
          else if (c == '_')
               return 29;
          else if (c == '\0')
               return 30;
          else if (isalpha(c) && islower(c))
               return c - 'a';
          else if (isalpha(c) && isupper(c))
               return c - 'A';
          else
               return -1;
    }

    // Function to insert a word into Trie
    void insert(string word) {
        charNode *node = root;
        for (char c : word) {
            int index = charToIndex(c);
            if (index == -1) continue;  // Skip if character is not supported
            if (node->links[index] == nullptr) {
                node->links[index] = new charNode();  // Create new node if it doesn't exist
            }
            node = node->links[index];
        }
        node->flag = true;  // Mark the end of the word
        node->wordCount++;  // Increment the word count
    }

    // Function to count the number of substrings in the Trie that can be made from the given word
    int countSubstrings(string word) {
        int count = 0;
        for (string::size_type i = 0; i < word.size(); i++) {
            charNode *node = root;
            for (string::size_type j = i; j < word.size(); j++) {
                int index = charToIndex(word[j]);
                if (index == -1)
                    continue;  // Skip if character is not supported

                if (node->links[index] == NULL)
                    break;  // Stop searching if the node doesn't exist
                
                node = node->links[index];

                if (node->flag)
                    count++;  // Increment count if a word is found
            }
        }
        return count;  // Return the total count
    }
};
