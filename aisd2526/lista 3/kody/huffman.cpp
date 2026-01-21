#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

// ============== BINARNE ==============

struct Node {
    char symbol;
    int freq;
    Node *left, *right;

    Node(char s, int f) : symbol(s), freq(f), left(nullptr), right(nullptr) {}
    Node(Node* l, Node* r) : symbol('\0'), freq(l->freq + r->freq), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

Node* buildHuffman(unordered_map<char, int>& freq) {
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (auto& p : freq) {
        pq.push(new Node(p.first, p.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        pq.push(new Node(left, right));
    }

    return pq.top();
}

void getCodes(Node* root, string code, unordered_map<char, string>& codes) {
    if (!root) return;

    if (!root->left && !root->right) {
        codes[root->symbol] = code;
    }

    getCodes(root->left, code + "0", codes);
    getCodes(root->right, code + "1", codes);
}

void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// ============== TERNARNE ==============

struct TernaryNode {
    char symbol;
    int freq;
    TernaryNode *child0, *child1, *child2;

    TernaryNode(char s, int f) : symbol(s), freq(f), child0(nullptr), child1(nullptr), child2(nullptr) {}
    TernaryNode(TernaryNode* c0, TernaryNode* c1, TernaryNode* c2)
        : symbol('\0'), freq(c0->freq + c1->freq + c2->freq), child0(c0), child1(c1), child2(c2) {}
};

struct CompareTernary {
    bool operator()(TernaryNode* a, TernaryNode* b) {
        return a->freq > b->freq;
    }
};

TernaryNode* buildHuffmanTernary(unordered_map<char, int>& freq) {
    priority_queue<TernaryNode*, vector<TernaryNode*>, CompareTernary> pq;

    for (auto& p : freq) {
        pq.push(new TernaryNode(p.first, p.second));
    }

    while ((pq.size() - 1) % 2 != 0) {
        pq.push(new TernaryNode('\0', 0));
    }

    while (pq.size() > 1) {
        TernaryNode* c0 = pq.top(); pq.pop();
        TernaryNode* c1 = pq.top(); pq.pop();
        TernaryNode* c2 = pq.top(); pq.pop();
        pq.push(new TernaryNode(c0, c1, c2));
    }

    return pq.top();
}

void getTernaryCodes(TernaryNode* root, string code, unordered_map<char, string>& codes) {
    if (!root) return;

    if (!root->child0 && !root->child1 && !root->child2 && root->symbol != '\0') {
        codes[root->symbol] = code;
    }

    getTernaryCodes(root->child0, code + "0", codes);
    getTernaryCodes(root->child1, code + "1", codes);
    getTernaryCodes(root->child2, code + "2", codes);
}

void deleteTernaryTree(TernaryNode* root) {
    if (!root) return;
    deleteTernaryTree(root->child0);
    deleteTernaryTree(root->child1);
    deleteTernaryTree(root->child2);
    delete root;
}

// ============== MAIN ==============

int main() {
    string text = "this is an example of a huffman tree";

    // Zlicz częstości
    unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }

    cout << "Tekst: " << text << endl << endl;

    // Binarne
    Node* rootBin = buildHuffman(freq);
    unordered_map<char, string> codesBin;
    getCodes(rootBin, "", codesBin);

    cout << "KODY BINARNE" << endl;
    for (auto& p : codesBin) {
        cout << "'" << p.first << "' : " << p.second << endl;
    }

    deleteTree(rootBin);

    // Ternarne
    TernaryNode* rootTer = buildHuffmanTernary(freq);
    unordered_map<char, string> codesTer;
    getTernaryCodes(rootTer, "", codesTer);

    cout << "KODY TERNARNE" << endl;
    for (auto& p : codesTer) {
        cout << "'" << p.first << "' : " << p.second << endl;
    }

    deleteTernaryTree(rootTer);

    return 0;
}