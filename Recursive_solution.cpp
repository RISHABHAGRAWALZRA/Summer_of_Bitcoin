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
unordered_map<string, int> mapping; 
unordered_map<int, string> revmapping;
unordered_map<int, set<int>> trxn_parents;
unordered_map<int, pair<int, int>> trxn_info;

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

void processData(string trxn_id, int &i) // To process the input and convert it into desired form which is sorting according to parents and map it with an integer value
{
    for (auto parent : input_trxnList[trxn_id]->parents)
    {
        if (mapping.find(parent) == mapping.end())
        {
            processData(parent, i);
        }
    }
    mapping[trxn_id] = i;
    revmapping[i] = trxn_id;
    set<int> par;
    for (auto parent : input_trxnList[trxn_id]->parents)
    {
        par.insert(mapping[parent]);
    }
    trxn_parents[i] = par;
    trxn_info[i] = make_pair(input_trxnList[trxn_id]->fee, input_trxnList[trxn_id]->weight);
    i++;
}

pair<int, unordered_set<int>> findListOfTrxn(int start, int end, int currentWeight, unordered_set<int> &selectedListOfTrx) // finding solution with recurrsive approach
{   
    if ((start > end) || (currentWeight == 0))
    {
        unordered_set<int> ans = selectedListOfTrx;
        return make_pair(0, ans);
    }
    else if (currentWeight < trxn_info[start].second)
    {
        return findListOfTrxn(start + 1, end, currentWeight, selectedListOfTrx);
    }
    else
    {   
        int flag = 1;
        for (auto ele : trxn_parents[start])
        {
            if (selectedListOfTrx.find(ele) == selectedListOfTrx.end())
            {
                flag = 0;
                break;
            }
        }

        int inFee = 0;
        unordered_set<int> inList;
        if (flag)
        {
            selectedListOfTrx.insert(start);
            int nextWeight = currentWeight - trxn_info[start].second;
            pair<int, unordered_set<int>> in = findListOfTrxn(start + 1, end, nextWeight , selectedListOfTrx);
            inFee = in.first + trxn_info[start].first;
            inList = in.second;
            selectedListOfTrx.erase(start);
        }

        

        pair<int, unordered_set<int>> out = findListOfTrxn(start + 1, end, currentWeight, selectedListOfTrx);
        int outFee = out.first;

        pair<int, unordered_set<int>> ans;
        if (inFee > outFee)
        {
            ans = make_pair(inFee, inList);
        }
        else
        {
            ans = make_pair(outFee, out.second);
        }

        return ans;
    }
}

int main()
{
    readInputData("mempool.csv");
    int i = 0;
    for (auto ele : input_trxnList)
    {
        if (mapping.find(ele.first) == mapping.end())
        {
            processData(ele.first, i);
        }
    }

    cout << "Started\n"; // Tag for ease
    unordered_set<int> temp;
    int totalTrxn = 5214; // Total nummber of transaction in mempool file(INPUT DATA)
    pair<int, unordered_set<int>> ans = findListOfTrxn(0, totalTrxn-1, BLOCK_MAX_WEIGHT_LIMIT, temp);
    cout<<"Ended\n";
    //Trying to print the answer
    /* cout << ans.first << "\n";
    for (auto ele : ans.second)
    {
        cout << ele << "\n";
    } */

    return 0;
}