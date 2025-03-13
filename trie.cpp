#include "trie.h"

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    // Clean up the Trie (optional, but recommended)
    // Implement a recursive function to delete all nodes if needed.
}

void Trie::insert(const std::string& word) {
    TrieNode* current = root;
    for (char ch : word) {
        if (current->children.find(ch) == current->children.end()) {
            current->children[ch] = new TrieNode();
        }
        current = current->children[ch];
    }
    current->isEndOfWord = true;
}

void Trie::dfs(TrieNode* node, std::string prefix, std::vector<std::string>& suggestions) {
    if (node->isEndOfWord) {
        suggestions.push_back(prefix);
    }
    for (auto& child : node->children) {
        dfs(child.second, prefix + child.first, suggestions);
    }
}

std::vector<std::string> Trie::search(const std::string& prefix) {
    std::vector<std::string> suggestions;
    TrieNode* current = root;
    for (char ch : prefix) {
        if (current->children.find(ch) == current->children.end()) {
            return suggestions; // No matching words
        }
        current = current->children[ch];
    }
    dfs(current, prefix, suggestions);
    return suggestions;
}

TrieNode* Trie::getRoot() const {
    return root;
}