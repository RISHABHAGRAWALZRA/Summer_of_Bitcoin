# Summer_of_Bitcoin
Code Challenge Solution of "Summer of Bitcoin"


### Analysis of Problem statement ([Problem statement](https://github.com/RISHABHAGRAWALZRA/Summer_of_Bitcoin/blob/main/sb_README.pdf))
___

#### Input data
Every transaction have some id,fee, and weight and can have some parent transaction. (Input is given in mempool.csv file : [mempool.csv](https://github.com/RISHABHAGRAWALZRA/Summer_of_Bitcoin/blob/main/mempool.csv))

#### Condition
Given Block weight limit : 4000000 (40 lac) (Given in problem statement file)

#### Task
We have to find the list of transactions whose weight's sum follows the condition and have the maximum sum of fees



### Solution Approach
___
As we can see this is similar as knapsack problem means greedy approach won't provide us the maximum at any case. So, only recurrsive or DP approach will work here.

#### Recursive Approach
In this approach we have to create every possible combination under the condition stated
So, here we have given 5000 approx transactions in input.
Means at worst case it will create 2^5000 approx level of call which is not possible for normal computer. Generally 1000 call is the limit. It depends on the size of stack.

#### DP Approach
Unable to find DP solution because of there is the dependent condition in between the transactions.
1. Two child can have same parents.
2. Parents can also have parents.

#### Greedy Approach
This solution is not the best but provides the maximum at some extent under the stated condition
Here deciding factor is the ratio between fee and weight (fee/weight) and also taking care of that its parents are also in the selected transaction list.








