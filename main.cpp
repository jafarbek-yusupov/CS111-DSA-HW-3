#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <map>
#include <fstream>
using namespace std;
typedef string str;
typedef long long ll;
typedef unsigned int ui;
#define elif else if
#define pb push_back
#define pop_back pop
#define vs vector<str>
#define vl vector<ll>
#define vi vector<int>

static int N = 5003;
static vi aVals = {33, 37, 39, 41};

class HashTable {
private:
    vector<list<str>> htable;
    int tableSize;
    int a;
    bool useMAD;
    int madB;
    int madP;
    int collisionCnt;
    
    ll polyHash(const str& key){
        ll hc=0; for(char c : key){ hc = hc*a + static_cast<int>(c);}
        return hc;
    }
    
    int compress(ll hc){
      ll absHash = (hc<0) ? -hc : hc;
      if(useMAD){
        absHash = absHash % madP;
        ll tt = ((static_cast<ll>(madB) * absHash + madB) % madP) % tableSize;
        return static_cast<int>(tt);
      } else{ return static_cast<int>(absHash % tableSize);}
    }
    
public:
    HashTable(int size, int aVal, bool useMad = false) {
        tableSize=size;
        this->a=aVal;
        useMAD=useMad;
        collisionCnt=0;
        htable.resize(tableSize);
        madB=17;
        madP=999983;
    }
    
    void insert(const str& key){
        ll hc = polyHash(key);
        int hval = compress(hc);
        collisionCnt += !htable[hval].empty() ? 1 : 0;
        htable[hval].pb(key);
    }
    
    bool find(const str& key){
        ll hc = polyHash(key);
        int hval = compress(hc);
        for(const str& k : htable[hval]){ if(k == key){ return true;}}
        return false;
    }
    
    void findMaxCollisions(){
        int maxCollsCnt = 0, maxHashVal = 0;
        for(int i=0; i<tableSize; i++){
            if(htable[i].size()>maxCollsCnt){
                maxCollsCnt = htable[i].size(); 
                maxHashVal = i;
            }
        }
        
        cout << "\n--- maximum collisions analysis ---";
        cout << "\nhash value with max collisions: " << maxHashVal;
        cout << "\nnumber of keys at this hash: " << maxCollsCnt;
        cout << "\nkeys: ";
        bool first = true;
        for(const str& key : htable[maxHashVal]) {
            if (!first) cout << ", ";
            cout << key;
            first = false;
        }
        cout << endl;
    }
    
    int getCollisionCount() const { return collisionCnt;}
    void getStats(int& totalKeys, int& usedBuckets, double& avgChainLen) {
        totalKeys = 0, usedBuckets = 0;
        for(int i=0; i<tableSize; i++){
            if(!htable[i].empty()){ usedBuckets++; totalKeys += htable[i].size();}
        }
        avgChainLen = usedBuckets>0 ? static_cast<double>(totalKeys) / usedBuckets : 0.0;
    }
};

vector<str> loadStringsFromFile(const str& filename) {
    vector<str> strs;
    ifstream file(filename);
    if(!file.is_open()){ cerr << "err -- couldnt open this file :: " << filename <<"\n"; return strs;}
    str line;
    while(getline(file, line)){
        if(!line.empty() && line.back()=='\r'){ line.pop_back();}
        if(!line.empty()){ strs.pb(line);}
    }
    file.close();
    return strs;
}

void task1(const vector<str>& keys) {
    cout << "\n========================================";
    cout << "\n------ t1 :: hash table with a=37 ------";
    cout << "\n========================================\n";
    
    HashTable ht(N, 37);
    cout << "inserting " << keys.size() << " keys";
    for(const str& k : keys){ ht.insert(k);}
  
    cout << "\ntesting find function ::: ";
    cout << "\nfind(\"Cullen Loeffler\") --- " << (ht.find("Cullen Loeffler") ? "found" : "not found");
    cout << "\nfind(\"SomeRandDneName\") --- " << (ht.find("SomeRandDneName") ? "found\n" : "not found\n");
    ht.findMaxCollisions();
    int totalKeys, usedBuckets;
    double avgChainLen;
    ht.getStats(totalKeys, usedBuckets, avgChainLen);
    cout << "\nstats :::\n";
    cout << "\ntotal keys -- " << totalKeys;
    cout << "\nused buckets -- " << usedBuckets;
    cout << "\naverage chain len -- " << fixed << setprecision(2) << avgChainLen;
    cout << "\ntotal collisions count -- " << ht.getCollisionCount();
}

void task2(const vector<str>& keys) {
    cout << "\n===================================================";
    cout << "\n---- collision comparison for diff values of a ----";
    cout << "\n===================================================\n";
    cout << "\n" << setw(10) << "a value" << setw(20) << "total collisions" << setw(20) << "avg chain len\n";
    cout << str(50, '-') << endl;
    for (int a : aVals){
        HashTable ht(N,a);
        for(const str& k : keys){ ht.insert(k);}
        int totalKeys, usedBuckets;
        double avgChainLen;
        ht.getStats(totalKeys, usedBuckets, avgChainLen);
        cout << setw(10) << a << setw(20) << ht.getCollisionCount()
             << setw(20) << fixed << setprecision(3) << avgChainLen << endl;
    }
}

void task3(const vector<str>& keys) {
    cout << "\n=========================================";
    cout << "\n------------ mad compression ------------";
    cout << "\n=========================================\n";
    cout << "\n" << setw(10) << "a value" << setw(20) << "total collisions" 
         << setw(20) << "avg chain length\n";
    cout << str(50, '-') << endl;
    for(int a : aVals){
        HashTable ht(N, a, true); 
        for(const str& k : keys){ ht.insert(k);}
        int totalKeys, usedBuckets;
        double avgChainLen;
        ht.getStats(totalKeys, usedBuckets, avgChainLen);
        cout << setw(10) << a << setw(20) << ht.getCollisionCount()
             << setw(20) << fixed << setprecision(3) << avgChainLen << endl;
    }
}

void generateComparisonTable(const vector<str>& keys) {
    cout << "\n============================";
    cout << "\n----- comparison table -----";
    cout << "\n============================\n";
    
    cout << "\n" << setw(15) << "compression" << setw(10) << "a value" 
         << setw(18) << "collisions" << setw(18) << "avg chain len" 
         << setw(15) << "load factor\n" << str(76, '=') << endl;
    for(int a : aVals){
        HashTable ht(N, a, false); 
        for(const str& k : keys){ ht.insert(k);}
        int totalKeys, usedBuckets;
        double avgChainLen;
        ht.getStats(totalKeys, usedBuckets, avgChainLen);
        double loadFactor = static_cast<double>(totalKeys) / N;
        
        cout << setw(15) << "modulo" 
             << setw(10) << a 
             << setw(18) << ht.getCollisionCount()
             << setw(18) << fixed << setprecision(3) << avgChainLen
             << setw(15) << fixed << setprecision(3) << loadFactor << endl;
    }
    
    cout << str(76, '-') << endl;
    for(int a : aVals){
        HashTable ht(N, a, true); 
        for(const str& k : keys){ ht.insert(k);}
        int totalKeys, usedBuckets;
        double avgChainLen;
        ht.getStats(totalKeys, usedBuckets, avgChainLen);
        double loadFactor = static_cast<double>(totalKeys) / N;
        cout << setw(15) << "mad" 
             << setw(10) << a 
             << setw(18) << ht.getCollisionCount()
             << setw(18) << fixed << setprecision(3) << avgChainLen
             << setw(15) << fixed << setprecision(3) << loadFactor << endl;
    }
}

int main() {
    cout << "=============================\n";
    cout << "------- htable assign -------\n";
    cout << "=============================\n";
    
    str filename = "C:/TMPDIR4DSAHW3/Strings_collections.txt";
    vector<str> keys = loadStringsFromFile(filename);
    if(keys.empty()){ cerr << "err :: no keys loaded from file\n"; return 1;}
    cout << "loaded " << keys.size() << " keys from file\n";
    task1(keys);
    task2(keys);
    task3(keys);
    generateComparisonTable(keys);
    cout << "\n=============================";
    cout << "\n---- program run is over ----";
    cout << "\n=============================";
    
    return 0;
}
