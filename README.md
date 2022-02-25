# Association-Rules-Mining

This is CMPUT 690(data mining course) assignment. This assignment is about mining for association rules. We are asked to implement the `Apriori algorithm` as published in the paper:

Rakesh Agrawal, Ramakrishnan Srikant, Fast Algorithms for Mining Association Rules, (Proc. 20th Int. Conf. Very Large Data Bases, VLDB, 1994).
Link to the paper: [Click Here.](http://rakesh.agrawal-family.com/papers/vldb94apriori.pdf)

```sh
Ck: Candidate itemset of size k
Lk: Frequent itemset of size k

INPUT: database and min_support

L1 = {frequent items};
for (k = 1; Lk is not empty; k++) do begin
    Ck+1 = candidates generated from Lk;
    for each transaction t in database do
       increment the count of all candidates in
       Ck+1   that are contained in t
    Lk+1  = candidates in Ck+1 with min_support
    end
return all Lk;
```

### How to Run:

```sh
make
./output filename.txt min_s min_c [optional: r, f, a]

```

min_s = minimum support [in range 0-1]\n
min_c = minumum confidence [in range 0-1]

The forth parameter is either "r", "f", "a", or absent. When "r", then all strong association rules are displayed. When "f" then all frequent itemsets are displayed. When "a" then all frequent itemsets and all strong association rules are displayed. When absent, then only the number of frequent itemsets of different sizes and the number of strong rules are displayed.



