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

vector<set<size_t> > frequents_vector;
// vector<map<int, set<size_t> > > frequents_vector;
map<set<size_t>, int> bigmap;
map<set<size_t>, int> hashmap;
map<int, int>  countmap;
int max_k = 15;
int strong_rules_num = 0;

vector<set<int> > read_inputs(string filename){
    ifstream fin;

    fin.open(filename);
    if(!fin) {
            cout << "Input file could not be opened\n";
            exit(0);
        }
    string str;
    vector<set<int> > transactions; 
    while(!getline(fin, str).eof()){
        set<int> arr;
        int pre = 0;
        for(int i=0;i<str.size();i++){
            if(str[i] == ' ') {
                int num = atoi(str.substr(pre, i).c_str());
                arr.insert(num);
                // cout<<num<<"\n";
                pre = i+1;
            }
        }
        transactions.push_back(arr);
    }

    return transactions;
}


void print_hashmap(){
    
    map<set<size_t>, int>::iterator tt1 = hashmap.begin();
    for (; tt1 != hashmap.end(); tt1++)
    {
        set<size_t> s = tt1->first;
        int c = tt1->second;
        set<size_t>::iterator iter2 = s.begin();
		for(;iter2 != s.end();iter2++) {
			cout<<*iter2<<" ";
		}
        cout<<": "<< tt1->second  << endl;}

}
void print_cont_freq(){
    map<int, int>::iterator it;
    for (it = countmap.begin(); it != countmap.end(); it++){
    cout<<"Number of frequent itemsets_"<< it->first << ':' << it->second << endl;
    }
}
void pruning(map<set<size_t>, int> initmap, int min_s){
	map<set<size_t>, int>::iterator iter;

    iter = initmap.begin();
	for(;iter != initmap.end(); iter++){
		if(iter->second >= min_s){
			hashmap.insert(pair<set<size_t>, int>(iter->first, iter->second));
            bigmap.insert(pair<set<size_t>, int>(iter->first, iter->second));
            //append to all frequent itemsets vector
            frequents_vector.push_back(iter->first);

        }
    }
}




bool should_be_searched(int arr[], int k)
{
    int count = pow(2, k);
    // set<size_t> subset;
    // The outer for loop will run 2^n times to print all subset .
    // Here variable i will act as a binary counter
    for (register int i = 0; i < count; i++) {
        set<size_t> subset;
        for (register int j = 0; j < k; j++) {
            if ((i & (1 << j)) != 0){
                subset.insert(arr[j]);
            }
        }
        // cout<<subset.size();
        if (subset.size() == k-1){
            
            // if peyda nasshoddd
            if (hashmap.find(subset) == hashmap.end()){
                return false;
            }
        }
        cout << "\n";
    }
    return true;
}
struct StrongAssociation{
	set<size_t> left;// left side of the rule
	set<size_t> right;// right side o the rule
	float ratio;//confidence
	int count;//support
};

vector<StrongAssociation> associations_vector;// to keep all association rules
void print_set(set<size_t> s){
 
    // using begin() to print set
    for (register auto it = s.begin(); it !=s.end(); ++it)
        cout << *it << ' ';

// cout<<"\n Finished set printing\n";
}



void find_subsets(int arr[], set<size_t> s, int k,float min_c)
{
    // cout<<"SIZE  "<< k<<endl; 
    int count = pow(2, k);
    // set<size_t> subset;
    // The outer for loop will run 2^n times to print all subset .
    // Here variable i will act as a binary counter
    for (register int i = 0; i < count; i++) {
        set<size_t> subset;
        for (register int j = 0; j < k; j++) {
            if ((i & (1 << j)) != 0){
                subset.insert(arr[j]);
            }
        }
        
        // cout<<subset.size();
        if (subset.size() <k & subset.size()>0){
            set<size_t> diff;
            set_difference(s.begin(), s.end(), subset.begin(), subset.end(), inserter(diff, diff.end()));
            float c = float(bigmap.find(s)->second) / float(bigmap.find(subset)->second);
            // int s = bigmap.find(s)->second;
            // set_count(support) = bigmap.find(s)->second)
            // print_set(s);
            // print_set(subset);
            // print_set(diff);
            // cout<<"CONFIDENCE: "<<c<<endl;
            // in check lazeeme?
            if (bigmap.find(subset) != bigmap.end() & bigmap.find(diff) != bigmap.end()){
                set<size_t> left = subset;
                set<size_t> right = diff;
                if (c>=min_c){
                    // cout<<c<<" "<<"CONFIDENCE"<<endl;
                    StrongAssociation sa;
                    sa.ratio = c;
                    sa.count = bigmap.find(s)->second;
                    sa.left = left;
                    sa.right = right;
                    associations_vector.push_back(sa);
                    strong_rules_num +=1;
                 }
            }
        }
    }
    // cout<<endl;
}

void print_frequent_itemsets(){
    cout<<"\n\nLIST OF ALL FREQUENT ITEM SETS"<<endl;
    for (register int i=0; i<frequents_vector.size(); i++){
        print_set(frequents_vector[i]);
        cout<<"("<<bigmap.find(frequents_vector[i])->second<<")"<<endl;
    }
}

void print_strong_associations(){
    cout<<"LIST OF ALL STRONG ASSOCIATION RULES"<<endl;
    for (int i=0; i<associations_vector.size(); i++){
        set<size_t> left = associations_vector[i].left;
        set<size_t> right = associations_vector[i].right;
        int left_array[left.size()];
        copy(left.begin(),left.end(),left_array);
        int arrSize = sizeof(left_array)/sizeof(left_array[0]);
        for (register int i = 0; i<arrSize; i++) {
        cout << left_array[i]<<" ";}

        cout << " -> ";

        int right_array[right.size()];
        copy(right.begin(),right.end(),right_array);
        int arrSize_r = sizeof(right_array)/sizeof(right_array[0]);
        for (register int i = 0; i<arrSize_r; i++) {
        cout << right_array[i]<<" ";}
        cout<< "("<<associations_vector[i].count<< " "<<associations_vector[i].ratio <<")"<<"\n";

        }
}
/*
This function will search one set in the whole dataset.
So, for instnce, it will search {1,2} in all transactions and returns the count to the function that called it.
*/
int search_db(set<size_t> combined, vector<set<int> > transactions, int k){
    vector<size_t> intersected;
    set<size_t>::iterator it = combined.begin();
    // Iterate till the end of set
    int count = 0;
    for (register int index=0; index!=transactions.size(); index++){
        
        set<size_t> intersected;
        set_intersection(transactions[index].begin(), transactions[index].end(), combined.begin(), combined.end(), inserter(intersected, intersected.end()));
        if (intersected.size() == k){   
            // This means that the entry is found ! So, let's count.
            count +=1;}
    }
    return count;
}


void create_k_frequent( vector<set<int> > transactions, int min_s){
    // advance function: it will advance the iterator by N elements (we cannot use ++ here.)
    int k = 2;
    int advance_value = 1;
    map<set<size_t>, int> tmpmap;
    bool flag = true;
    while(flag){
    cout <<"K: "<< k<<"\n";
    advance_value = 1;
    int count = 0;
    //check kon too in ziadi nare, khosoosal ba should be searched
    for (map<set<size_t>, int>::iterator first_itr = hashmap.begin(); first_itr!=hashmap.end(); first_itr++){        
        map<set<size_t>, int>::iterator sec_iter = hashmap.begin();
        advance(sec_iter,advance_value);
        for (; sec_iter!=hashmap.end(); sec_iter++){
            set<size_t> combined;
            combined.insert(first_itr->first.begin(), first_itr->first.end());
            combined.insert(sec_iter->first.begin(), sec_iter->first.end());            
            // cout<< combined.size();
            //ghablan: > k
            // print_set(combined);
            // cout<<endl;
            if(k>2){
            if (combined.size() != k){
                // cout<<"Cannot be combined\n";
                continue;
            }}
            int count = search_db(combined, transactions, k);
            //pruning is done here.
            if (count >= min_s){
                tmpmap.insert(pair<set<size_t>, int>(combined, count));
            }
            
        }
        advance_value +=1;
    }
    // clear the hashmap from k-1 itemsets, and add k-itemsets to that
    hashmap.clear();
    //save size k;
    countmap.insert(pair<int, int> (k, tmpmap.size()));
    map<set<size_t>, int>::iterator iter= tmpmap.begin();;
	for(;iter != tmpmap.end(); iter++){
        hashmap.insert(pair<set<size_t>, int>(iter->first, iter->second));
        bigmap.insert(pair<set<size_t>, int>(iter->first, iter->second));
        //append to all frequent itemsets vector
        frequents_vector.push_back(iter->first);
    }
    print_hashmap();
    // if (tmpmap.size()==1){
    if (tmpmap.size()<=1){
        cout<<"END OF WHILE\n";
        flag = false;
    }
    if (k>max_k){
        flag = false;
    }
    tmpmap.clear();
    k++;
    cout<<"end of k \n";

}
}

/*
This function generates the hashmap with 1-itemsets. In other words, it creates a map  
(key= all distinct items in dataset value = frequency)
it does not return anything, just assigns values to the hashmap.
*/
void generate_1_itemset_map( vector<set<int> > transactions, int min_s){

    map<set<size_t>, int> initmap;
    map<set<size_t>, int>::iterator iter;

    for (register int index=0; index!=transactions.size(); index++){
        int array[transactions[index].size()];
        copy(transactions[index].begin(),transactions[index].end(),array);
        int array_size  = transactions[index].size();
        // cout<<array_size;
        for (register int i =0; i< array_size; i++){
            set<size_t> t_set;
			t_set.insert(array[i]);
            // cout<<array[i]<<"\n";
            if (initmap.find(t_set) != initmap.end()){
                initmap.find(t_set)->second +=1;
            }
            else{
            initmap.insert(pair<set<size_t>, int>(t_set, 1));}
        }
    }

    map<set<size_t>, int>::iterator it;
    cout<<"After limiting by min support:"<<"\n";
    pruning(initmap, min_s);
    countmap.insert(pair<int, int>(1, hashmap.size()));
    print_hashmap();
    cout<<"FINISH"<<"\n";
}

int main(int argc, char **argv){
    // if (argc !=6){
    //     cout<<"Wrong number of inputs\n";
    //     return 0;
    // }
    // string filename = string(argv[1]);
    // int min_support = int(argv[2]*transaction_count);
    // int min_s = int(argv[2]);
    // float min_c = float(argv[3]);
    // string to_do = string(argv[4]);
    string filename = "10000.txt";
    int min_s = 2;
    float min_c = 0.8;
    char to_do = 'a';
    vector<set<int> > transactions;
    cout  <<"\n";

    // auto start = std::chrono::high_resolution_clock::now();

    transactions = read_inputs(filename);
    cout  <<"read the dataset\n";
    // map<set<size_t>, int> *hashmap = new map<set<size_t>, int>();

    generate_1_itemset_map( transactions, min_s);
    create_k_frequent( transactions, min_s);

    for (vector<set<size_t> >::iterator it = frequents_vector.begin() ; it != frequents_vector.end(); ++it){
        int array[it->size()];
        copy(it->begin(),it->end(),array);
        if (it->size()>1){
            // cout << ' ' << *it;
            find_subsets(array, *it, it->size(), min_c);
        }
    }

    
    // if (to_do =='r'){
    //     print_strong_associations();
    // }
    // else if (to_do =='f'){
    //     print_frequent_itemsets();
    // }
    // else if (to_do =='a'){
    //     print_strong_associations();
    //     cout<<endl;
    //     print_frequent_itemsets();
    // }
    // else if (to_do =='o'){
        // print_cont_freq();
    // }

    // print_strong_associations();

    // auto stop = std::chrono::high_resolution_clock::now();
    // auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    // cout << "DURATION:  "<<duration.count()<<" micro-s"<< endl;
    
  return 0;
}


    // cout<<"Value of the intial map:"<<"\n";
    // for (it = initmap.begin(); it != initmap.end(); it++)
    // {
    //     set<int>::iterator itr;
    //     cout << it->second  << endl;
    // }