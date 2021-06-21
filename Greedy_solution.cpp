#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>
using namespace std;

class Transaction // Created Transaction Class for storing the data
{

public:
    string trxn_id; // transaction Id
    int fee;
    int weight;
    unordered_set<string> parents;
};

int BLOCK_MAX_WEIGHT_LIMIT = 4000000; // Limit of a block (Given in problem)
unordered_map<string, Transaction *> input_trxnList;
multimap<double, string, greater<double> > sortedTrxnGreedyly;
vector<string> ans;

void addTransactionInfo(vector<string> &trxn_info)
{

    Transaction *trxn = new Transaction();
    trxn->trxn_id = trxn_info[0];
    trxn->fee = stoi(trxn_info[1]);
    trxn->weight = stoi(trxn_info[2]);

    unordered_set<string> parents;
    if (trxn_info.size() > 3) // Finding parents for the trxn_id and add it into parents set
    {

        string parent;
        stringstream ss(trxn_info[3]);
        while (getline(ss, parent, ';'))
        {
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

void createSortedTrxnListGreedyly()
{
    for (auto ele : input_trxnList)
    {
        double fee = (double)ele.second->fee;
        double weight = (double)ele.second->weight;
        double factor = fee / weight;
        sortedTrxnGreedyly.insert(pair<double, string>(factor, ele.first));
    }
}

bool canAdd(string tx_id, unordered_set<string> &selectedTrxn)
{
    for (auto ele : input_trxnList[tx_id]->parents)
    {
        if (selectedTrxn.find(ele) == selectedTrxn.end())
        {
            return false;
        }
    }
    return true;
}

void selectTrxnWisely()
{
    int currentBlockLimit = BLOCK_MAX_WEIGHT_LIMIT;
    int trxnFeeCollected = 0;

    unordered_set<string> selectedTrxn;
    while (currentBlockLimit > 0 && sortedTrxnGreedyly.size() > 0)
    {
        int flag=1;
        for (auto itr = sortedTrxnGreedyly.begin(); itr != sortedTrxnGreedyly.end(); itr++)
        {
            string tx_id = (*itr).second;
            if (canAdd(tx_id, selectedTrxn) && currentBlockLimit >= input_trxnList[tx_id]->weight)
            {
                trxnFeeCollected += input_trxnList[tx_id]->fee;
                currentBlockLimit -= input_trxnList[tx_id]->weight;
                ans.push_back(tx_id);
                selectedTrxn.insert(tx_id);
                sortedTrxnGreedyly.erase(itr);
                flag=0;
                break;
            }
        }
        if(flag){
            break;
        }
    }

    cout<<"Fee colleted from trxns "<<trxnFeeCollected<<"\n";
    cout<<"Filled Weight of Block "<<BLOCK_MAX_WEIGHT_LIMIT - currentBlockLimit<<"\n";
    cout<<"Selected Trxn Count "<<ans.size()<<" from "<<input_trxnList.size()<<"\n";
    
}

void generateOutput(string fileName)
{
    ofstream myfile(fileName);
    for(auto id : ans){
        myfile<<id<<"\n";
    }
    myfile.close();
}

int main()
{
    readInputData("mempool.csv");   // Reading Input Data from given file
    createSortedTrxnListGreedyly(); // fraction knapsack approach
    selectTrxnWisely();             // Greedy apprach
    generateOutput("block.txt");    // Save output in block.txt
    return 0;
}