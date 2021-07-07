#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
using namespace std;
struct TreeNode
{
    char data=-1;
    int frequency=0;
    TreeNode* left = nullptr;
    TreeNode* right=nullptr;
    TreeNode(int frequency_)
    {
        frequency=frequency_;
    }
    TreeNode(char data_, int frequency_)
    {
        data=data_;
        frequency=frequency_;
    }
};
/**
 * Functor used to compare TreeNodes for the construction of a min heap.
 */
struct compareNodes
{
    bool operator() (const TreeNode* first, const TreeNode* second)
    {
        return first->frequency > second->frequency;
    }
};
class huffman_tree {
private:
    unordered_map<char, int> frequency_list;
    TreeNode *root = nullptr;
    unordered_map<char, string> codes;
    /**
     * Fills a map with key being a char in the tree and the value being the huffman encoding of that
     * char for this specific tree
     * @param node the current root of the subtree we are on
     * @param code string that holds the huffman encoding of the current node
     */
    void encodeTree(TreeNode *node, string code)
    {
        //If null, no more leaves to check in this branch
        if (node == nullptr)
            return;
        //If leaf, we know that it contains a char and must put in the appropriate huffman encoding
        if (node->left == nullptr && node->right == nullptr)
            codes[node->data] = code;
        //Goes to left subtree
        encodeTree(node->left, code + "0");
        //Goes to right subtree
        encodeTree(node->right, code + "1");
    }
public:
    /*
    Preconditions: input is a string of characters with ascii values 0-127
    Postconditions: Reads the characters of input and constructs a
    huffman tree based on the character frequencies of the file contents.
    Constructor taken from HuffmanTrees slides 5-7
    */
    huffman_tree(const string &input)
    {
        //Gets frequencies of all the unique chars in the string with ascii values 0-127
        for (unsigned int i = 0; i < input.length(); ++i)
        {
            if (frequency_list.find(input[i]) == frequency_list.end())
                frequency_list.insert({input[i], 1});
            else
                ++frequency_list[input[i]];
        }
        //Checks to see if the string was empty. If so, just return
        if(frequency_list.empty())
            return;
        priority_queue<TreeNode *, vector<TreeNode *>, compareNodes> pq;
        /*
         * Pushes in nodes with the char and their respective frequency into a min heap ordered by frequency
         */
        for (auto it = frequency_list.begin(); it != frequency_list.end(); it++)
        {
            TreeNode *node = new TreeNode(it->first, it->second);
            pq.push(node);
        }
        //Algorithm to construct the huffman tree. From
        while (pq.size() > 1)
        {
            TreeNode *root1 = pq.top();
            pq.pop();
            TreeNode *root2 = pq.top();
            pq.pop();
            TreeNode *temproot = new TreeNode(root1->frequency + root2->frequency);
            temproot->left = root1;
            temproot->right = root2;
            pq.push(temproot);
        }
        root = pq.top();

        //If only 1 unique character in string, make the code 0. Otherwise, store encodings of all unique characters.
        if(frequency_list.size()==1)
            codes[input[0]] = "0";
        else
            encodeTree(root, "");
    }
    /*
    Preconditions: input is a character with ascii value between 0-127
    Postconditions: Returns the Huffman code for character if character is in the tree
    and an empty string otherwise.
    */
    string get_character_code(char character) const
    {
        //Checks to see if character exists in the tree and if it is return the encoding. If not, return an empty string.
        if (codes.find(character) == codes.end())
            return "";
        else
            return codes.at(character);
    }
    /*
    Preconditions: input is a string of characters with ascii values 0-127
    Postconditions: Returns the Huffman encoding for the contents of file_name
    if file name exists and an empty string otherwise.
    If the file contains letters not present in the huffman_tree,
    return an empty string
    */
    string encode(const string &input) const
    {
        string encoded="";
        for(unsigned int i =0; i<input.size()&&!codes.empty();++i)
        {
            if(codes.find(input[i])==codes.end())
                return "";
            encoded+=codes.at(input[i]);
        }
        return encoded;
    }
    /*
    Preconditions: string_to_decode is a string containing Huffman-encoded text
    Postconditions: Returns the plaintext represented by the string if the string
    is a valid Huffman encoding and an empty string otherwise
    */
    string decode(const string &string_to_decode) const
    {
        string decoded="";
        TreeNode* current = root;
        if(frequency_list.size()==1)
        {
            for(unsigned int i =0; i<string_to_decode.length();++i)
            {
                if(string_to_decode[i]=='0')
                    decoded+=root->data;
                else
                    return "";
            }
            return decoded;
        }
        for(unsigned int i =0; i<string_to_decode.length()&&current!=nullptr;++i)
        {
            //Go to the left subtree
            if(string_to_decode[i]=='0')
                current=current->left;
            //Go to the right subtree
            else if(string_to_decode[i]=='1')
                current=current->right;
            //If not 0 or 1 return an empty string for an invalid input
            else
                return"";
            //If root node, add the char in the node to the string
            if(current->left==nullptr&&current->right== nullptr)
            {
                decoded+=current->data;
                current=root;
            }
        }
        //If we didn't end on a leaf node and set current back to root, we have an invalid input
        if(current!=root)
            return"";
        return decoded;
    }
};