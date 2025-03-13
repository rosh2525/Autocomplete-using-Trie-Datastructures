#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <vector>
#include <string>

class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}
};

class Trie {
private:
    TrieNode* root;

    void dfs(TrieNode* node, std::string prefix, std::vector<std::string>& suggestions);

public:
    Trie();
    void insert(const std::string& word);
    std::vector<std::string> search(const std::string& prefix);
};

#endif // TRIE_H