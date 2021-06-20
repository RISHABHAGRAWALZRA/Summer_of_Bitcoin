/*
Unable to find DP solution
Problems facing in DP solution are
1) Two child can have same parents
2) Parent can also have parents ( more than two levels)
These two points create two many dependent condition which act as hurdles in DP approach
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <set>
using namespace std;

class Transaction // Created Transaction Class for storing the data
{

public:
    string trxn_id; // transaction Id
    int fee;
    int weight;
    set<string> parents;
};

int BLOCK_MAX_WEIGHT_LIMIT = 4000000; // Limit of a block (Given in problem)
unordered_map<string, Transaction *> input_trxnList;
unordered_set<string> processedtrxn;
unordered_set<string> parentList;
unordered_map<string, unordered_set<string>> dp_trxnList;

void addTransactionInfo(vector<string> &trxn_info)
{

    Transaction *trxn = new Transaction();
    trxn->trxn_id = trxn_info[0];
    trxn->fee = stoi(trxn_info[1]);
    trxn->weight = stoi(trxn_info[2]);

    set<string> parents;
    if (trxn_info.size() > 3) // Finding parents for the trxn_id and add it into parents set
    {

        string parent;
        stringstream ss(trxn_info[3]);
        while (getline(ss, parent, ';'))
        {
            parentList.insert(parent);
            parents.insert(parent);
            parent.clear();
        }
    }

    trxn->parents = parents;
    input_trxnList[trxn_info[0]] = trxn;
}

void readInputData(string fileName)
{
    fstream myFile(fileName);
    if (!myFile.is_open())
    {
        cout << "ERROR: File is not Open" << '\n'; //If file is somehow can't open
    }
    else
    {
        string line;
        while (getline(myFile, line))
        {
            stringstream ss(line);
            vector<string> trxn_info;
            string word;
            while (getline(ss, word, ','))
            {
                trxn_info.push_back(word);
                word.clear();
            }
            if (trxn_info[0] != "tx_id")
            {
                addTransactionInfo(trxn_info);
            }
            line.clear();
        }
    }
    myFile.close();
}

void createProcessedList(string trxn_id) // To process the input and convert it into desired form which is sorting according to parents and map it with an integer value
{
    unordered_set<string> parents;
    for (auto parent : input_trxnList[trxn_id]->parents)
    {
        if (processedtrxn.find(parent) != processedtrxn.end())
        {
            cout << "Problem " << parent << "\n"; // This is showing the id's who are create the give below hurdles
        }
        else
        {
            processedtrxn.insert(parent);
        }
        parents.insert(parent);
    }
    processedtrxn.insert(trxn_id);
    dp_trxnList[trxn_id] = parents;
}

int main()
{
    readInputData("mempool.csv");
    for (auto ele : input_trxnList)
    {
        if (parentList.find(ele.first) == parentList.end())
        {
            createProcessedList(ele.first);
        }
    }

    // Unable to find DP solution
    // Problems facing in DP solution are
    // 1) Two child can have same parents
    // 2) Parent can also have parents ( more than two levels)
    // These two points create two many dependent condition which act as hurdles in DP approach

    return 0;
}