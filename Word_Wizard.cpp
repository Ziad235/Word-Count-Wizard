#include <iostream>
#include <string.h>
#include <stdio.h>
#include <iomanip>
#include <ctime>
#include <string>
#include <algorithm>
#include <fstream>
#include <cctype>
#include <math.h>

#include <list>

using namespace std;
using std::string;

long wordcount=0, uniquewords=0; 

class HashNode
{
	private:
		string key;
		int frequency;
	public:
		HashNode(string key, int frequency)
		{
			this->key = key;
			this->frequency = frequency;
		}
        HashNode();
		friend class HashTable;
}; 


class HashVector
{
	public:
		HashNode **nodes;				//pointer to int(array) to store elements
		int v_size;						//current size of vector (number of elements in vector)
		int v_capacity;					//capacity of vector
	public:
		HashVector();						//No argument constructor
		~HashVector();					//Destructor
		void push_back(HashNode *element);		//Add an element at the end of vector
		void insert(int index, HashNode *element);
		int size() const;				//Return current size of vector
		int capacity() const;			//Return capacity of vector
		bool empty() const; 			//Rturn true if the vector is empty, False otherwise
		void display();
        void sort();
};

HashVector::HashVector()
{
    v_capacity = 100;
    nodes = new HashNode*[v_capacity];
    v_size = 0;
}

HashVector::~HashVector()
{
    delete[] nodes;
}

int HashVector::size() const
{
    return (v_size);
}

int HashVector::capacity() const
{
    return (v_capacity);
}

bool HashVector::empty() const
{
    return (v_size==0);
}

void HashVector::sort()
{
    for (int i = 0; i<v_size; i++){
        HashNode *temp;
        for (int j=1; j<v_size; j++){
            if(nodes[i] > nodes[j])
            {
                temp = nodes[i];
                nodes[i] = nodes[j];
                nodes[j] = temp; 
            }
        }
    }
}


void HashVector::insert(int index, HashNode *element)
{
    if(v_size >= v_capacity)
    {
        int newcap = 2 * max(1,v_capacity);
        HashNode** temp = new HashNode*[newcap];
        //HashNode** temp[newcap];// = new HashNode[newcap];
        for (int j = 0; j<v_capacity; ++j)
            temp[j] = nodes[j];
        if (nodes != NULL) delete[] nodes;
        nodes = temp;
        v_capacity = newcap;
    }

    nodes[v_size] = element;
    v_size++;
}


void HashVector::push_back(HashNode *element)
{
    insert(v_size, element);
}


class HashTable
{
	private:
		HashVector *buckets;		// Array of lists of type Hash Nodes
		int size;					    //Current Size of HashTable
		int capacity;				    // Total Capacity of HashTable
		int collisions;				// Total Number of Collisions
	public:
        string hf;                  // string store the hashfunction name
		HashTable(int capacity);
		unsigned long hashCode1(const string key);
        unsigned long hashCode2(const string key);
        unsigned long hashCode3(const string key);
		unsigned long hashCode4(string key);

		void insert(const string key, string);
		string search(const string key);
		int getSize();
		int getCollisions();
        int frequency(string key);
};


HashTable::HashTable(int capacity)
{
	buckets = new HashVector[capacity];			//Create an array of size capacity. Each locaiton of Array is a list/chain
	this->capacity = capacity;
	this->size = 0;
	this->collisions = 0;
    this->hf = ""; 
}
HashTable myHashTable(100003);// Prime number bigger than > 19880*1.333;


//The first hashcode uses polynomial hashcode
unsigned long HashTable::hashCode1(const string key)
{
    unsigned long hashcode=0;
        int keysize = key.size();
        for(int i = 0; i<key[i]!='\0'; i++)
        {   
            hashcode += int(key[i])* long(pow(10,i)) ; 
        }
        return (hashcode%capacity);
}

//The second hashcode function uses cyclic-shift hashcode
unsigned long HashTable::hashCode2(const string key)
{
    unsigned long hashcode=0;
        int keysize = key.size();
        for(int i = 0; i<key[i]!='\0'; i++)
        {   hashcode = (hashcode<<5)|(hashcode>>27);
            hashcode += (unsigned int)(key[i]); 
        }
        return (hashcode%capacity);
}

//The third hash function uses summation hashcode
unsigned long HashTable::hashCode3(const string key)
{
    unsigned long hashcode=0;
        int keysize = key.size();
        for(int i = 0; i<key[i]!='\0'; i++)
        {   
            hashcode += (unsigned int)(key[i]); 
        }
        return (hashcode%capacity);
}

//The fourth hash function uses Robert Sedgwicks Algorithm
unsigned long HashTable::hashCode4(string key)
{
    unsigned long hashcode=0;
    unsigned long a = 378551;
    unsigned long b = 63689;

        for(int i = 0; i<key[i]!='\0'; i++)
        {   
            hashcode = hashcode * a + int(key[i]);
            a = a*b;
        }
        return (hashcode%capacity);
}


//================================================
void HashTable::insert(const string key, string hashf)
{
	long pos;
    
    if (hashf=="f1")
        pos = hashCode1(key);
    else if (hashf == "f2")
        pos = hashCode2(key);
    else if (hashf == "f3")
        pos = hashCode3(key);
    else if (hashf == "f4")
        pos = hashCode4(key);
      
    HashNode *newNode = new HashNode(key,1);

    if (buckets[pos].v_size ==0){
        buckets[pos].push_back(newNode);
        ++size;
        uniquewords++;
        return;
    }
    else{

        for(int i=0; i<buckets[pos].size(); i++){
            if (buckets[pos].nodes[i]->key == key ){
                buckets[pos].nodes[i]->frequency++;
                ++size;
                return;
            }            
        }
    }
            buckets[pos].push_back(newNode);
            ++size;
            uniquewords++;
            ++collisions;
            buckets[pos].sort();
            return;
}
//===================================================

int HashTable::frequency(string key)
{
    long pos;
    
    if (hf=="f1")
        pos = hashCode1(key);
    else if (hf == "f2")
        pos = hashCode2(key);
    else if (hf == "f2")
        pos = hashCode3(key);
    else if (hf == "f2")
        pos = hashCode4(key);

    if (buckets[pos].v_size ==0){
        return 0;
    }
    else {

        int end = buckets[pos].v_size;
        int start = 0;
        int mid; 

        while(end>=start)
        {
            mid= (end+start)/2;
            if (buckets[pos].nodes[mid]->key == key )
                return (buckets[pos].nodes[mid]->frequency);
            
            if (key < buckets[pos].nodes[mid]->key)
                end = mid - 1;
            else 
                start = mid + 1;
        }
    

        for(int i=0; i<buckets[pos].size(); i++){
        if (buckets[pos].nodes[i]->key == key ){
            return buckets[pos].nodes[i]->frequency;
        }            
        }
    }

    return 0;

}
//====================================================

int HashTable::getSize()
{
	return this->size;
}
//======================================================
int HashTable::getCollisions()
{
	return this->collisions;
}
//======================================================
void readFile(ifstream& file, string hashf)
{
    string rawword;

    while(file >> rawword)
    {
        if (rawword=="\n")
            continue;

        string word = "";
        

        for(int i = 0; i < rawword.length(); ++i)
        {
             if (isalpha(rawword[i]))
                word += rawword[i];       
        }

        if ((word == "")||(word=="f")||(word=="e")||(word==" "))        
            continue;   
        

        transform(word.begin(), word.end(), word.begin(), ::tolower);
        // cout << word << " ";
        ++wordcount;
        myHashTable.insert(word, hashf);
    }
}

void hashfunctionmenu()
{
	cout<<"List of available hashfunctions:"<<endl
		<<"1. Polynomial Hash Function                  : enter f1"<<endl
		<<"2. Cyclic Hash Function  [default]           : enter f2"<<endl
		<<"3. Summation Hash Function                   : enter f3"<<endl
		<<"4. RS Hash Function                          : enter f4"<<endl        
		<<"exit/quit                   "<<endl;
}

void menufunctions()
{
	cout<<"List of available functions:"<<endl
		<<"collisons                   : Display the number of collisions by a hash function"<<endl
		<<"unique                      : Display the number of unique words in the text"<<endl
		<<"wordcount                   : Display the total number of words in the text"<<endl
		<<"frequency <word>            : Search for a word and return its frequency"<<endl   
        <<"help                        : Display the menu" << endl   
		<<"exit/quit                   "<<endl;
}

void mainstart()
{   
    string filename, user_input;
    cout << " Enter the full path of a file: " << endl;
    cout<<">";
	getline(cin,user_input);
	filename = user_input.substr(0,user_input.find(" "));

        string user_input2, command2, parameter;

    ifstream file;

    file.open(filename, ios::in);

    while(!file)
    {
        cout << " Can't open file at specified path!" << endl;
        cout << " Please re-enter the full path of the file! " << endl;
        cout <<">";
        getline(cin,user_input);
        filename = user_input.substr(0,user_input.find(" "));

        file.open(filename, ios::in);
    }

     
    string command;
    do
	{
        hashfunctionmenu();

        do{
        cout<<">";
		getline(cin,user_input);

		command = user_input.substr(0,user_input.find(" "));

        if (command == "")
            command = "f2";

			if((command=="f1")||(command=="f2")||(command=="f3")||(command=="f4"))				
                {
                    readFile(file,command);
                    break;
                    }
			else if(command == "exit")			return;
			else 							{cout<<"Invalid Commad!"<<endl;
                                            cout << "Please enter a valid choice!"<<endl;}
        } while(command!="exit" and command!="quit");



        myHashTable.hf = command;
        menufunctions();

        do{

        cout<<">";
		getline(cin,user_input2);

		command2 = user_input2.substr(0,user_input2.find(" "));
        parameter = user_input2.substr(user_input2.find(" ")+1);

			if(command2=="collisons")              cout << " Collisions = " << myHashTable.getCollisions() << endl;
            else if (command2 == "wordcount")      cout << " Word Count = " << wordcount << endl;
            else if (command2 == "unique")         cout << " Unique words = " << uniquewords << endl;
			else if(command2 == "exit")			   return;
            else if(command2 == "help")            menufunctions();
            else if(command2 == "frequency")       cout << "Frequency = " << myHashTable.frequency(parameter) << endl;
			else 							{cout<<"Invalid Commad!"<<endl;
                                            cout << "Please enter a valid choice!"<<endl;}
        
        cout<<"==================================================="<<endl;
        cout <<"word count = " << wordcount << endl;
        cout<<"Hash Table size = "<< myHashTable.getSize()<<endl;
        cout<<"Avg. Number of Collisions Per Entry = "<< float(myHashTable.getCollisions())/myHashTable.getSize()<<endl;
        cout<<"==================================================="<<endl;
        
        } while(command2!="exit" and command2!="quit");

	} while(command!="exit" and command!="quit");

}

int main()
{

    mainstart();
    return(0);
}