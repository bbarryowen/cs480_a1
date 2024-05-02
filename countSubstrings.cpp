/*
 Project Name: assignment 1
 Description: takes 2 files, a vocab file and a file of words
              then writes each word into a trie and searches for
              each vocab word counting all the substrings in each
              word in the text file that match a vocab word

  Author:        Brendan Barry-Owen
  RedId: 825730146
  last edited:  9/6/23
 */

#include <fstream>
#include <vector>
#include <cstring>

// type for Node
struct Node
{
  char data;
  Node *nextList[31];
  Node(char letter) : data(letter)
  {
    for (int i = 0; i < 31; i++)
    {
      nextList[i] = nullptr;
    }
  }
};

// searches for a given vocab word in a given trie, returns true if found and false if not
bool findVocab(Node *trie, char *vocab)
{
  int loc;
  Node *currNode = trie;
  char letter;
  for (int i = 0; i < strlen(vocab) + 1; i++)
  {
    letter = vocab[i];
    if (tolower(letter) >= 97 && tolower(letter) <= 122)
    {
      loc = int(tolower(letter)) - 97;
    }
    else if (letter == '\0')
    {
      return true;
    }
    else if (letter == '\'')
    {
      loc = 26;
    }
    else if (letter == '-')
    {
      loc = 27;
    }
    else if (letter == '_')
    {
      loc = 28;
    }
    else if (letter == 32)
    {
      loc = 29;
    }
    else
    {
      loc = -1;
    }

    // checks if nextList where the letter would be is not null and moves to that node, else the vocab word is not in that line
    if (loc != -1)
    {
      if (currNode->nextList[loc] != nullptr)
      {
        currNode = currNode->nextList[loc];
      }
      else
      {
        return false;
      }
    }
  }
  return true;
}

// returns a slice from a given start value to the end of a given array as a char*
char *slice(char *array, int start)
{
  int arrayLen = strlen(array) + 1;
  char *outArray = (char *)malloc(arrayLen - start);
  for (int i = start; i < arrayLen; i++)
  {
    outArray[i - start] = array[i];
  }
  return outArray;
}

// inserts a word and all suffixes of that word into a given trie and returns true or false depending on if the word was inserted
bool insertWord(char *word, Node *nodes)
{
  Node *currNode;
  char *slicedWord;

  // outer for loop iterates over each index of the word and sets sliced word to the suffix
  for (int i = 0; i < strlen(word) + 1; i++)
  {
    slicedWord = slice(word, i);
    currNode = nodes;
    // iterates over each index of the sliced word and adds it to the trie
    for (int j = 0; j < strlen(slicedWord); j++)
    {
      int loc;
      char letter = slicedWord[j];

      if (tolower(letter) >= 97 && tolower(letter) <= 122)
      {
        loc = int(tolower(letter)) - 97;
      }
      else if (letter == '\0')
      {
        currNode->nextList[30] = new Node('\0');
        return true;
      }
      else if (letter == '\'')
      {
        loc = 26;
      }
      else if (letter == '-')
      {
        loc = 27;
      }
      else if (letter == '_')
      {
        loc = 28;
      }
      else if (letter == 32)
      {
        loc = 29;
      }
      else
      {
        loc = -1;
      }

      // if a node doesnt exist at the location in nextList then a node with the letter is created, else currNode is set to that node
      if (loc != -1)
      {  
        if (currNode->nextList[loc] == nullptr)
        {
          currNode->nextList[loc] = new Node(tolower(letter));
        }
        currNode = currNode->nextList[loc];
      }
    }
  }
  return false;
}

// checks to make sure arguments follow specifications and prints error message if not, returns true or false depending on if arguments are to specifications
int checkArgs(int numArgs, char *argsList[])
{
  // checks if number of arguments is correct
  if (numArgs != 3)
  {
    printf("Error: expected 2 arguments but got %d\n", numArgs - 1);
    return 1;
  }

  // loads in files
  const std::string argument1 = argsList[1];
  std::ifstream file1(argument1);

  const std::string argument2 = argsList[2];
  std::ifstream file2(argument2);
  int check = 0;

  // checks if file1 exists
  if (!file1)
  {
    printf("Error: file does not exist: %s\n", argument1.c_str());
    check = 1;
  }
  // checks if file2 exists
  if (!file2)
  {
    printf("Error: file does not exist: %s\n", argument2.c_str());
    check = 1;
  }
  // returns 1 if either of the files do not exists, else returns 0
  if (check != 0)
  {
    return 1;
  }
  return 0;
}

int main(int argc, char *argv[])
{
  int argsCode = checkArgs(argc, argv);
  // ends program with return code 1 if arguments are not to specifications
  if (argsCode != 0)
  {
    return 1;
  }

  // loading in arguments
  std::ifstream vocab(argv[1]);
  std::ifstream textFile(argv[2]);

  // reading each line from vocab to a dynamic array
  std::vector<char *> vocabWords;
  std::string line;
  while (getline(vocab, line))
  {
    char *lineCharArray = new char[line.length() + 1];
    strcpy(lineCharArray, line.c_str());
    vocabWords.push_back(lineCharArray);
  }

  // reading each line from given text to dynamic array
  std::vector<char *> textLines;
  while (getline(textFile, line))
  {
    char *lineCharArray = new char[line.length() + 1];
    strcpy(lineCharArray, line.c_str());
    textLines.push_back(lineCharArray);
  }

  // for each word in text file the word and all suffixes are inserted into trie
  for (int i = 0; i < textLines.size(); i++)
  {
    Node *wordsTrie = new Node('\0');
    insertWord(textLines[i], wordsTrie);
    int count = 0;
    bool isFound;
    // searches the created trie for each vocab word
    for (int j = 0; j < vocabWords.size(); j++)
    {
      isFound = findVocab(wordsTrie, vocabWords[j]);
      if (isFound)
      {
        count += 1;
      }
    }
    printf("%d\n", count);
  }
}
