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


vector<set<int> > frequents_vector;
map<int, int>  countmap;
map<set<int>, int> bigmap;
int max_k = 15;
int strong_rules_num = 0;

struct StrongAssociation{
	set<int> left;
	set<int> right;
	float ratio;
	int count;
};
vector<StrongAssociation> associations_vector;// to keep all association rules


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

void print_set(set<int> s){
 
    // using begin() to print set
    for (register auto it = s.begin(); it !=s.end(); ++it){
        cout << *it << ' ';
    }

cout<<"\n Finished set printing\n";
}


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

void print_frequent_itemsets(){
    ofstream MyFile("freqs.txt");
    cout<<"\n\nLIST OF ALL FREQUENT ITEM SETS"<<endl;
    for (register int i=0; i<frequents_vector.size(); i++){
        // print_set(frequents_vector[i]);
        for (register auto it = frequents_vector[i].begin(); it !=frequents_vector[i].end(); ++it){
        MyFile<< *it << ' ';
        cout << *it << ' ';
        }
        MyFile << "("<<bigmap.find(frequents_vector[i])->second<<")"<<endl;

        cout<<"("<<bigmap.find(frequents_vector[i])->second<<")"<<endl;
    }
    MyFile.close();

}

void print_strong_associations(){
    cout<<"LIST OF ALL STRONG ASSOCIATION RULES"<<endl;
    ofstream MyFile("strongs.txt");
    for (int i=0; i<associations_vector.size(); i++){
        set<int> left = associations_vector[i].left;
        set<int> right = associations_vector[i].right;
        int left_array[left.size()];
        copy(left.begin(),left.end(),left_array);
        int arrSize = sizeof(left_array)/sizeof(left_array[0]);
        for (register int i = 0; i<arrSize; i++) {
        MyFile<<left_array[i]<<" ";
        cout << left_array[i]<<" ";}
        
        MyFile<<" -> ";
        cout << " -> ";

        int right_array[right.size()];
        copy(right.begin(),right.end(),right_array);
        int arrSize_r = sizeof(right_array)/sizeof(right_array[0]);
        for (register int i = 0; i<arrSize_r; i++) {
        MyFile<<right_array[i]<<" ";
        cout << right_array[i]<<" ";}

        MyFile<< "("<<associations_vector[i].count<< " "<<associations_vector[i].ratio <<")"<<"\n";
        cout<< "("<<associations_vector[i].count<< " "<<associations_vector[i].ratio <<")"<<"\n";

        }
    MyFile.close();
}

void print_cont_freq(){
    ofstream MyFile("count_freq.txt");
    map<int, int>::iterator it;
    for (it = countmap.begin(); it != countmap.end(); it++){
        MyFile<<"Number of frequent itemsets_"<< it->first << ':' << it->second << endl;
        cout<<"Number of frequent itemsets_"<< it->first << ':' << it->second << endl;
        
    }
    cout<<strong_rules_num<<endl;
    MyFile<<endl<<strong_rules_num<<endl;
    MyFile.close();
}


void pruning(map<set<int>, int> initmap, map<set<int>, int> *hashmap, int min_s){
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



bool should_be_searched(int arr[], int k, map<set<int>, int> *hashmap)
{
    int count = pow(2, k);
    // set<int> subset;
    // The outer for loop will run 2^n times to print all subset .
    // Here variable i will act as a binary counter
    for (register int i = 0; i < count; i++) {
        set<int> subset;
        for (register int j = 0; j < k; j++) {
            if ((i & (1 << j)) != 0){
                subset.insert(arr[j]);
            }
        }
        // cout<<subset.size();
        if (subset.size() == k-1){
            
            // if peyda nasshoddd
            if (hashmap->find(subset) == hashmap->end()){
                return false;
            }
        }
        cout << "\n";
    }
    return true;
}



void find_subsets(int arr[], set<int> s, int k,float min_c)
{
    // cout<<"SIZE  "<< k<<endl; 
    int count = pow(2, k);
    // set<int> subset;
    // The outer for loop will run 2^n times to print all subset .
    // Here variable i will act as a binary counter
    for (register int i = 0; i < count; i++) {
        set<int> subset;
        for (register int j = 0; j < k; j++) {
            if ((i & (1 << j)) != 0){
                subset.insert(arr[j]);
            }
        }
        
        // cout<<subset.size();
        if (subset.size() <k & subset.size()>0){
            set<int> diff;
            set_difference(s.begin(), s.end(), subset.begin(), subset.end(), inserter(diff, diff.end()));
            float c = float(bigmap.find(s)->second) / float(bigmap.find(subset)->second);
            c = round( c * 1000.0 ) / 1000.0;
            // int s = bigmap.find(s)->second;
            // set_count(support) = bigmap.find(s)->second)
            // print_set(s);
            // print_set(subset);
            // print_set(diff);
            // cout<<"CONFIDENCE: "<<c<<endl;
            // in check lazeeme?
            if (bigmap.find(subset) != bigmap.end() & bigmap.find(diff) != bigmap.end()){
                set<int> left = subset;
                set<int> right = diff;
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


/*
This function will search one set in the whole dataset.
So, for instnce, it will search {1,2} in all transactions and returns the count to the function that called it.
*/




/*
This function generates the hashmap with 1-itemsets. In other words, it creates a map  
(key= all distinct items in dataset value = frequency)
it does not return anything, just assigns values to the hashmap.
*/
void generate_1_itemset_map(map<set<int>, int> *hashmap, vector<set<int> > transactions, int min_s){

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
            // cout<<array[i]<<"\n";
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
map<set<int>, int> create_k_frequent(map<set<int>, int> *hashmap){
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
            set_union(it1->first.begin(), it1->first.end(), it2->first.begin(), it2->first.end(), inserter(combined, combined.end()));
            tmpmap[combined] = 0;
        }
    }
    return tmpmap;
   
}

void search_db(map<set<int>, int> tmpmap, vector<set<int> > transactions, map<set<int>, int> *hashmap, int min_s, int k){
    map<set<int>, int>::iterator it1 = tmpmap.begin();
    for (; it1 != tmpmap.end();it1++){
    int count = 0;
        set<int> combined = it1->first;
        for (register int index=0; index!=transactions.size(); index++){
            if(includes(transactions[index].begin(), transactions[index].end(),
                    combined.begin(), combined.end())){
                        count +=1;}

        }
    if (count>=min_s){
        hashmap->insert(pair<set<int>, int>(combined, count));
        bigmap.insert(pair<set<int>, int>(combined, count));
        frequents_vector.push_back(combined);

        }
    }
    countmap.insert(pair<int, int> (k, hashmap->size()));
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
    int min_s = 10;
    float min_c = 0.8;
    char to_do = 'a';
    vector<set<int> > transactions;
    map<set<int>, int> *hashmap = new map<set<int>, int>();
    map<set<int>, int> tmpmap;


    transactions = read_inputs(filename);
    cout  <<"read the dataset\n";


    auto start = std::chrono::high_resolution_clock::now();
    generate_1_itemset_map(hashmap, transactions, min_s);    
    
    int k = 2;
    bool flag = true;
    // find frequent itemsets
    while(flag){
        cout <<"K: "<< k<<"\n";

        tmpmap = create_k_frequent(hashmap);
        hashmap->clear();
        search_db(tmpmap, transactions, hashmap, min_s, k);
        // print_hashmap(hashmap);

        if (hashmap->size()<=1){
            cout<<"END OF WHILE\n";
            flag = false;}

        if (k>max_k){
            flag = false;}

        cout<<"end of K \n";
        k++;
    }

    // find associatopns
    for (vector<set<int> >::iterator it = frequents_vector.begin() ; it != frequents_vector.end(); ++it){
        int array[it->size()];
        copy(it->begin(),it->end(),array);
        if (it->size()>1){
            // cout << ' ' << *it;
            find_subsets(array, *it, it->size(), min_c);
        }
    }

    // time
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    cout << "DURATION:  "<<duration.count()<<" micro-s"<< endl;
    
    print_strong_associations();
    print_strong_associations();
    print_cont_freq();


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
    //     print_cont_freq();
    // }
    // else{

    // }


  return 0;
}