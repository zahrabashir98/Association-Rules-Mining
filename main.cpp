#include <map>
#include <iostream>
#include <cassert>
#include <vector>
#include <set>
#include <string>
#include <tuple>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <unordered_map>
#include <chrono>
using namespace std;


vector<set<int> > frequents_vector; //it will save all the frequent sets.
map<set<int>, int> bigmap; //it will save all the frequent itemsets and their count.
map<set<int>, int> two_itemsets; //helper ds: two_itemsets is a map which saves all of the frequent 2-itemsets to be used for the search in function "should_be_searched".
map<int, int>  countmap; //helper ds: it will save number of frequent itemsets for each k for ease of printing. e.g: {1:2, 2:10, 3:7,...}
int strong_rules_num = 0;
int max_k; //to calculate the maximum k of the itemsets.
int num_of_transactions;
struct StrongAssociation{
	set<int> left;
	set<int> right;
	float confidence;
	float support;
};
vector<StrongAssociation> associations_vector; //this vector keeps all association rules

/*
This function reads the dataset and parses that. The data will be pushed back to a vector of sets.
input: filename
output: transactions
*/
vector<set<int> > read_inputs(string filename){
    ifstream fin;
    string str;
    vector<set<int> > transactions; 
    max_k = 0;
    fin.open(filename);
    if(!fin) {
            cout << "The file cannot be oppened.\n";
            exit(0);
        }
    
    while(!getline(fin, str).eof()){
        set<int> rows;
        int pre = 0;
        for(int i=0;i<str.size();i++){
            if(str[i] == ' ') {
                int num = atoi(str.substr(pre, i).c_str());
                rows.insert(num);
                // cout<<num<<"\n";
                pre = i+1;
            }
        }
        transactions.push_back(rows);
        if (rows.size()>max_k){
            max_k = rows.size();
        }
    }

    return transactions;
}

/*
This is a helper function for me to print any set. (For debugging and displaying purpose)
input: s(a set)
output: -
*/
void print_set(set<int> s){
 
    // I used register for speedup.
    for (register auto it = s.begin(); it !=s.end(); ++it){
        cout << *it << ' ';
    }

cout<<"\n Finished set printing\n";
}

/*
This is a helper function for me to print hashmaps. (For debugging and displaying purpose)
input: hashmap
output: -
*/
void print_hashmap(map<set<int>, int> *hashmap){
    
    map<set<int>, int>::iterator tt1 = hashmap->begin();
    for (; tt1 != hashmap->end(); tt1++)
    {
        set<int> s = tt1->first;
        int c = tt1->second;
        set<int>::iterator iter2 = s.begin();
		for(;iter2 != s.end();iter2++) {
			cout<<*iter2<<" ";
		}
        cout<<": "<< tt1->second  << endl;}

}

/*
This function prints frequent itemsets. It will be called after running the whole code, and writes the results into a file as well.
input: -
output: -
*/
void print_frequent_itemsets(){
    ofstream myfile("freqs.txt");
    cout<<"\n\nLIST OF ALL FREQUENT ITEM SETS"<<endl;
    for (register int i=0; i<frequents_vector.size(); i++){
        // print_set(frequents_vector[i]);
        for (register auto it = frequents_vector[i].begin(); it !=frequents_vector[i].end(); ++it){
        myfile<< *it << ' ';
        cout << *it << ' ';
        }
        myfile << "("<<(bigmap.find(frequents_vector[i])->second)/float(num_of_transactions)<<")"<<endl;
        cout<<"("<<(bigmap.find(frequents_vector[i])->second)/float(num_of_transactions)<<")"<<endl;
    }
    myfile.close();
}


/*
This function prints strong associations. It will be called after running the whole code, and writes the results into a file as well.
input: -
output: -
*/
void print_strong_associations(){
    cout<<"LIST OF ALL STRONG ASSOCIATION RULES"<<endl;
    ofstream myfile("strongs.txt");
    for (int i=0; i<associations_vector.size(); i++){
        set<int> left = associations_vector[i].left;
        set<int> right = associations_vector[i].right;
        int left_array[left.size()];
        copy(left.begin(),left.end(),left_array);
        int arrSize = sizeof(left_array)/sizeof(left_array[0]);
        for (register int i = 0; i<arrSize; i++) {
            if (i==arrSize-1){
              myfile<<left_array[i]<<"";
              cout << left_array[i]<<"";
            }
            else{
            myfile<<left_array[i]<<", ";
            cout << left_array[i]<<", ";
            }
        }
        
        myfile<<" -> ";
        cout << " -> ";

        int right_array[right.size()];
        copy(right.begin(),right.end(),right_array);
        int arrSize_r = sizeof(right_array)/sizeof(right_array[0]);
        for (register int i = 0; i<arrSize_r; i++) {
            if (i==arrSize_r-1){
              myfile<<right_array[i]<<" ";
            cout << right_array[i]<<" ";  
            }
            else{
            myfile<<right_array[i]<<", ";
            cout << right_array[i]<<", ";
            }
        }

        myfile<< "("<<associations_vector[i].support<< ","<<associations_vector[i].confidence <<")"<<"\n";
        cout<< "("<<associations_vector[i].support<< ","<<associations_vector[i].confidence <<")"<<"\n";

        }
    myfile.close();
}

/*
At first, it will write the program duration to a file. Then, it will print count of each k-frequent itemset.
It will also print the number of strong association rules. 
It will be called after running the whole code, and writes the results into a file as well.
input: duration(long)
output: -
*/
void print_count_freq(long duration){
    
    ofstream myfile("results.txt");
    myfile << duration/float(1000000)<<endl;
    cout<< "Run Time: "<<(duration/float(1000000))<<endl;
    map<int, int>::iterator it;
    for (it = countmap.begin(); it != countmap.end(); it++){
        myfile<<"Number of frequent itemsets_"<< it->first << ':' << it->second << endl;
        cout<<"Number of frequent itemsets_"<< it->first << ':' << it->second << endl;
        
    }
    cout<<"Number of strong association rules: "<<strong_rules_num<<endl;
    myfile<<"Number of strong association rules: "<<strong_rules_num<<endl;
    myfile.close();
}


/*
This function is designed to check something 
Consider we want to combine two itemsets {1,3} and {2,3}-> when we want to generate 3-itemsets, we will combine them and the 
resulting set would be {1,2,3}. But it should not be added to our map becuase {1,2} is not in our frequents list. So, before searching
{1,2,3} in database, we have to make sure if it should be searched or not.
The difference is only including two elements ({1,2} here), so we will search it in 2-itemsets map.
input: diff(set)
output: True/False
*/
bool should_be_searched(set<int> diff){
    if (two_itemsets.find(diff) != two_itemsets.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
This function prunes the initmap and removes the items with a count of less than minimum count.(updates *hashmap)
input: initmap, *hashmap, min_s
output: - 
*/
void pruning(map<set<int>, int> initmap, map<set<int>, int> *hashmap, float min_s){
	map<set<int>, int>::iterator iter;

    iter = initmap.begin();
	for(;iter != initmap.end(); iter++){
		if(iter->second >= min_s){
			hashmap->insert(pair<set<int>, int>(iter->first, iter->second));
            bigmap.insert(pair<set<int>, int>(iter->first, iter->second));
            //append to all frequent itemsets vector
            frequents_vector.push_back(iter->first);
        }
    }
}




/*
Learned how to find the subsets without recursion the masking method from this website: https://slaystudy.com/find-all-subsets-of-an-array-c/
This function will find all the subsets of an array to find the strong association rules.
input: arr[], s, k, min_c, num_of_transactions
ouput: -
*/
void find_subsets(int arr[], set<int> s, int k, float min_c){ 
    int count = pow(2, k);
    for (register int i = 0; i < count; i++) {
        set<int> subset;
        for (register int j = 0; j < k; j++) {
            if ((i & (1 << j)) != 0){
                subset.insert(arr[j]); 
            }
        }
        
        // cout<<subset.size();
        if (subset.size() <k & subset.size()>0){ //check if the subset is not empty, or not the same size.
            set<int> diff;
            // find the difference between the subset and the original set. (for example: original:{1,2,3}, subset:{1,2} => diff:{3})
            set_difference(s.begin(), s.end(), subset.begin(), subset.end(), inserter(diff, diff.end()));
            float c = float(bigmap.find(s)->second) / float(bigmap.find(subset)->second); //calculate the confidence
            c = round( c * 1000.0 ) / 1000.0;

            if (bigmap.find(subset) != bigmap.end() & bigmap.find(diff) != bigmap.end()){
                set<int> left = subset;
                set<int> right = diff;
                // check if the confidence is more than the treshold, then create a strong association object.
                if (c>=min_c){
                    StrongAssociation sa;
                    sa.confidence = c;
                    sa.support = round(float(bigmap.find(s)->second) / float(num_of_transactions) *1000.0)/1000.0;
                    sa.left = left;
                    sa.right = right;
                    associations_vector.push_back(sa);
                    strong_rules_num +=1;
                 }
            }
        }
    }
}


/*
This function generates the hashmap with 1-itemsets. In other words, it creates a map  
(key= all distinct items in dataset value = frequency) (it will also check the minumum support treshold)
It does not return anything, just assigns values to the hashmap.
After calling this function, *hashmap will contain frequent 1-itemsets.
input: *hashmap, transactions, min_s
output: -
*/
void generate_1_itemset_map(map<set<int>, int> *hashmap, vector<set<int> > transactions, float min_s){

    map<set<int>, int> initmap;
    map<set<int>, int>::iterator iter;

    for (register int index=0; index!=transactions.size(); index++){
        int array[transactions[index].size()];
        copy(transactions[index].begin(),transactions[index].end(),array);
        int array_size  = transactions[index].size();
        // cout<<array_size;
        for (register int i =0; i< array_size; i++){
            set<int> t_set;
			t_set.insert(array[i]);

            if (initmap.find(t_set) != initmap.end()){
                initmap.find(t_set)->second +=1;
            }
            else{
            initmap.insert(pair<set<int>, int>(t_set, 1));}
        }
    }
    map<set<int>, int>::iterator it;
    pruning(initmap, hashmap, min_s);
    countmap.insert(pair<int, int>(1, hashmap->size()));
    // print_hashmap(hashmap);

}


/*
This function generates the hashmap with k-itemsets. In other words, it creates a map  
(key= all distinct items in dataset value = frequency)
it does not return anything, just assigns values to the hashmap.
input: *hashmap, transactions, k
output: -
*/
map<set<int>, int> create_k_frequent(map<set<int>, int> *hashmap, int k){
    set<int> combined;
    map<set<int>, int>::iterator it1 = hashmap->begin();
    map<set<int>, int> tmpmap;

    for (; it1 != hashmap->end();it1++)
    {
    map<set<int>, int>::iterator it2 = hashmap->begin();
        for (it2 = it1; it2 != hashmap->end();it2++)
        {
            // combined.insert(it1->first.begin(), it1->first.end());
            // combined.insert(it2->first.begin(), it2->first.end());
            if (it1 == it2){
                continue;
            }
            set<int> combined;
            //I used union function for combining the items, and then I check if the size of the combination is the size that we want, otherwise, continue;
            set_union(it1->first.begin(), it1->first.end(), it2->first.begin(), it2->first.end(), inserter(combined, combined.end()));
            if (combined.size()!=k){
                continue;
            }
            else{
                if (k<=2){
                tmpmap[combined] = 0;
                // cout<<"k is less than 2"<<endl;
                }
                else{
                    // this part checks if all the subsets of a set are in the frequent set.(explained in "should_be_searched" function) 
                    set<int> diff;
                    set<int> uni;
                    set<int> inter;
                    set_union(it1->first.begin(), it1->first.end(), it2->first.begin(), it2->first.end(), inserter(uni, uni.end()));
                    set_intersection(it1->first.begin(), it1->first.end(), it2->first.begin(), it2->first.end(), inserter(inter, inter.end()));
                    set_difference(uni.begin(), uni.end(), inter.begin(), inter.end(), inserter(diff, diff.end()));
                    
                    if (should_be_searched(diff)){
                        // cout<<"it should be searched"<<endl;
                        tmpmap[combined] = 0;
                    }
                    else{
                        // print_set(diff);
                        // cout<<"the subset does not exist, next!"<<endl;
                        continue;
                    }
                }
            }
        }
    }
    return tmpmap;
}

/*
This function will search one set in the whole dataset. (the most time-consuming part!)
So, for instnce, it will search {1,2} in all transactions and returns the count to the function that called it.
After checking the count, it will check if the num is more than the treshold, and if it was, it will be added to the hashmap.
[Here, the two maps that I have(tmpmap, and *hashmap) somehow represent the C and L maps in the slides.]
input: tmpmap, transactions, *hashmap, min_s, k
output: - 
*/
void search_db(map<set<int>, int> tmpmap, vector<set<int> > transactions, map<set<int>, int> *hashmap, float min_s, int k){
    map<set<int>, int>::iterator it1 = tmpmap.begin();
    // map<set<int>, int>  *new_hashmap = hashmap; 

    for (; it1 != tmpmap.end();it1++){
    int count = 0;
        set<int> combined = it1->first;
        for (register int index=0; index!=transactions.size(); index++){
            
            if(includes(transactions[index].begin(), transactions[index].end(),combined.begin(), combined.end())){
                count +=1;}

        }
    if (count>=min_s){
        hashmap->insert(pair<set<int>, int>(combined, count));
        bigmap.insert(pair<set<int>, int>(combined, count));
        frequents_vector.push_back(combined);

        }
    }
    if (k==2){
        two_itemsets = *hashmap;
    }
    if (hashmap->size()!=0){
    countmap.insert(pair<int, int> (k, hashmap->size()));}
}


/*
How to run: 
~ make
After running make, then:
input example for 100 transactions: ./output filename.txt 0.1 0.8 (optional{r,a,f})
input example for 1k transactions: ./output filename.txt 0.01 0.8
input example for 10k transactions: ./output filename.txt 0.001 0.8
*/
int main(int argc, char **argv){

    if (argc<4 || argc>5){
        cout<<"Incorrect input format\n";
        return 0;
    }
    string to_do = "absent";
    if (argc == 5){
        to_do = string(argv[4]);
    }

    string filename = string(argv[1]);
    float min_c = atof(argv[3]);
    vector<string> valid_list;
    valid_list.push_back("r");
    valid_list.push_back("a");
    valid_list.push_back("f");
    bool found = (std::find(valid_list.begin(), valid_list.end(), to_do) != valid_list.end());
    
    if (found ==0 ){
        if (argc !=4){
            cout<<"Wrong Input!"<<endl;
            return 0;
        }
    }

    vector<set<int> > transactions;
    map<set<int>, int> *hashmap = new map<set<int>, int>();
    map<set<int>, int> tmpmap;

    transactions = read_inputs(filename);
    num_of_transactions = transactions.size();
    float min_s = float(atof(argv[2])*num_of_transactions);

    auto start = std::chrono::high_resolution_clock::now();
    generate_1_itemset_map(hashmap, transactions, min_s);    
    int k = 2;
    bool flag = true;
    // find frequent itemsets

    while(flag){
        tmpmap = create_k_frequent(hashmap, k);
        hashmap->clear(); //clear the hashmap to fill it with the new items
        search_db(tmpmap, transactions, hashmap, min_s, k);
        // print_hashmap(hashmap);

        if (hashmap->size()<=1){
            flag = false;}

        if (k>max_k){
            flag = false;}

        k++;
    }

    // find association rules
    for (vector<set<int> >::iterator it = frequents_vector.begin() ; it != frequents_vector.end(); ++it){
        int array[it->size()];
        copy(it->begin(),it->end(),array);

        if (it->size()>1){ //only generate association rules from frequent itemsets with lenghth more than 1.
            find_subsets(array, *it, it->size(), min_c);
        }
    }

    // time
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    // cout << "DURATION:  "<<duration.count()<<" micro-s"<< endl;
    
    // print_frequent_itemsets();
    // print_strong_associations();
    print_count_freq(duration.count());

    if (to_do =="r"){
        print_strong_associations();
    }
    else if (to_do =="f"){
        print_frequent_itemsets();
    }
    else if (to_do =="a"){
        print_strong_associations();
        cout<<endl;
        print_frequent_itemsets();
    }
    else if (to_do=="absent"){
        print_count_freq(duration.count());
    }

  return 0;
}