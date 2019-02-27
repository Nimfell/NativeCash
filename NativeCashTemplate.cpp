#include <stdio.h>
#include <string.h>
#include <list>
using namespace std;

class NativeCash
{
    protected:
      int size;
      int items_score;
      int max_items_score;
      list<char*>* slots;
      list<int>* values; 
      list<int>* hits;	  
  
    public:
      NativeCash(int sz)
      {      
        size = sz;
        items_score = 0;
        max_items_score = size * 5;      
        slots  = new list<char*>[size];	// keys
        values = new list<int>[size];	// vals
        hits   = new list<int>[size];	// hits
      }
      //----------------------------
      void put(char* key, int value) 
      {	
        int index = hashFun(key);
        if (index == -1) 
          return;
        int number;
        if (items_score == max_items_score)
        {
          number = find_unclaimed(index);
          delete_item(index, number);
        }  
        int char_lengh = strlen(key); 	  // make new char*
        char* _key = new char[char_lengh];		
        strcpy (_key, key);

        slots[index].push_back(_key);
        values[index].push_back(value);
        hits[index].push_back(0);         // "0" for work - "rand()%9" for test
        items_score ++;
      }

      bool is_key(char* key) 
      {     
        int index = hashFun(key);	      
        list<char*>* List = &slots[index];
        list<int>*   Hits = &hits [index];
        list<int>::iterator   _hits = Hits->begin();
        list<char*>::iterator _list;
        for (_list = List->begin(); _list != List->end(); ++_list) 
        {			
          char* val = *_list;            
          if ( strcmp(val, key) == 0)     
          { 
            *_hits ++; 
            return true; 
          }
          ++_hits;
        }   
        return false;  
      }

      int get(char* key)          						// get value by key
      { 
        int index = hashFun(key);						// index in the table
        if (index == -1)          						// if NULL
          return -1;
        int number = find(key);   						// index in the list
        if (number == -1)         						// if not found
          return -1;
        list<int>* Values = &values[index]; 			// get this list of values
        list<int>::iterator _values = Values->begin();
        while (number-- != 0)     						// go to the searching item in the list by number  
          _values++;      
        int get_val =*_values;    						// get value
        return get_val;
      }

      int get_max_items()
      {
        return max_items_score;
      }

    private:
      int find_unclaimed (int index)
      { 
        int number = 0;
        int min_val = -1;
        int number_val;
        list<int>* Hits = &hits[index];
        list<int>::iterator _hits;

        for (_hits = Hits->begin(); _hits != Hits->end(); ++_hits)
        {	
          if (min_val > *_hits || min_val == -1)
          {
            min_val = *_hits;
            number_val = number;
          }
          if (min_val == 0) 
            return number_val;
          number ++;        
        }
        return number_val;
      }

      void delete_item (int index, int item_number)
      {
        int number = 0;
        list<char*>* List = &slots [index];
        list<int>*   Hits = &hits  [index];
        list<int>*   Val  = &values[index];

        list<int>::iterator   _hits = Hits->begin();
        list<int>::iterator   _val  = Val->begin();
        list<char*>::iterator _list;

        for (_list = List->begin(); _list != List->end(); ++_list)
        {	
          if (number == item_number) 
          {
             List->erase(_list);
             Hits->erase(_hits);
             Val->erase(_val);
             items_score--;
             return;
          }
          number ++;
          _hits ++;
          _val ++;
        }  
      }

      int hashFun(char* key)
      {    
        if (key == NULL)
          return -1;
        int index = 0;	 
        index = (int)key[0];
        for (int i = 1; key[i] != 0; i++)  // adder of sumbols    
          index += (int)key[i];    
        if (index < 0) 
          index *= -1;  // number modulus 
        int p = 37; 	// Prime number
        int a = 19;  	// 1 ... ğ-1
        int b = 27;  	// 0 ... ğ-1
        index = ( (a*index + b) % p ) % size;
        return index;
      }

      int find(char* key) 					// get index in the list
      { 
        int number = 0;
        int index = hashFun(key);			//get index
        list<char*>* List = &slots[index];
        list<int>*   Hits = &hits [index];
        list<int>::iterator   _hits = Hits->begin();
        list<char*>::iterator _list;

        for (_list = List->begin(); _list != List->end(); ++_list)
        {			
          char* _val = *_list;
          if ( strcmp(key, _val) == 0)	      // compare	
          {	         
            list<int>::iterator _hits_prev = _hits;
            *_hits = *_hits + 1;  
            return number;
          }
          number ++;
          _hits ++;
        }
        return -1;
      }
};

//==================== TEST ===================
int test_Cash (NativeCash* Table)
{
  int test = 0;
  int overflow = 5;
  char string[] = "key 0";
  for (int i = 49; i <= (Table->get_max_items() + 49 + overflow); i++) // fill the entire array 
  { 
    string[4] = (char)i;    
    Table->put( string, i);
  }
  Table->is_key("key 1");
  if ( Table->get("key 9") != 57 ) test++;
  if ( Table->get("key 8") != -1 ) test++;   
  Table->put(NULL, 666);                  // do nothing
  if ( Table->get(NULL) != -1 ) test++;
  return test;
}




