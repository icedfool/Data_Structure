#include <iostream>
#include <string>
#include <list>
#include <map>
#include <algorithm>

typedef std::map<std::string,int> COSTUME_SHOP_TYPE;
typedef std::map<std::string,std::string> PEOPLE_TYPE;
typedef std::map<std::string, std::list<std::string>> COSTUME_TYPE;

// prototypes for the helper functions
void addCostume(COSTUME_SHOP_TYPE &costume_shop,
                COSTUME_TYPE& costume_list);

void rentCostume(COSTUME_SHOP_TYPE& costume_shop, 
                 PEOPLE_TYPE& people,
                 COSTUME_TYPE& costume_list,
                 std::list<std::string>& name_list);

void lookup(COSTUME_SHOP_TYPE& costume_shop,
            COSTUME_TYPE& costume_list);

void printPeople(PEOPLE_TYPE& people,
                 std::list<std::string>& name_list);

int main() {
  // two data structures store all of the information for efficiency
  COSTUME_SHOP_TYPE costume_shop;
  PEOPLE_TYPE people;
  COSTUME_TYPE costume_list;
  std::list<std::string> name_list;
  char c;
  while (std::cin >> c) {
    if (c == 'a') {
      addCostume(costume_shop,costume_list);
    } else if (c == 'r') {
      rentCostume(costume_shop,people,costume_list,name_list);
    } else if (c == 'l') {
      lookup(costume_shop,costume_list);
    } else if (c == 'p') {
      printPeople(people,name_list);
    } else {
      std::cerr << "error unknown char " << c << std::endl;
      exit(0);
    }
  }
}

// the function take in two arguments,one is the costume shop 
// and another is a help list to store the costume name and its 
// borrower.
void addCostume(COSTUME_SHOP_TYPE& costume_shop,
                COSTUME_TYPE& costume_list) {
  std::string costume_name;
  std::list<std::string> temp_list;//just a help list, so a blank one is enough
  int num;
  std::cin >> costume_name >> num;
  //if it is the first time to add some costume to the shop, then use the insert
  //version, and for others we only need to edit the second value of that pair.
  if(costume_shop.find(costume_name) == costume_shop.end()){
    costume_shop.insert(std::pair<std::string,int>(costume_name,num));
    costume_list.insert(std::pair<std::string,std::list<std::string>>
    (costume_name,temp_list));
  }
  else{
    (*costume_shop.find(costume_name)).second += num;
  }
  //printing details
  if(num == 1){
    std::cout<<"Added "<<num<<" "<<costume_name<<" costume."<<std::endl;
  }
  else{
    std::cout<<"Added "<<num<<" "<<costume_name<<" costumes."<<std::endl;
  }
}


void rentCostume(COSTUME_SHOP_TYPE& costume_shop, 
                 PEOPLE_TYPE& people,
                 COSTUME_TYPE& costume_list,
                 std::list<std::string>& name_list) {
  std::string first_name;  
  std::string last_name;
  std::string costume_name;
  std::cin >> first_name >> last_name >> costume_name;
  std::string input_name = first_name+ " " + last_name; //notice space
  // the function will break only in two circum stances
  // 1. the shop has no such costume
  // 2. the amount of the costume is 0.
  // circumstance1
  if (costume_shop.find(costume_name) == costume_shop.end()){
    std::cout<<"The shop doesn't carry "<<costume_name<<" costumes."<<std::endl;
    if(people.find(input_name) == people.end()){
      people.insert(std::pair<std::string,std::string>(input_name,""));
    }
    //also push that name to the name_list and we are going to need that for
    //print function.
    if(find(name_list.begin(),name_list.end(),last_name+" "+first_name)
     == name_list.end()){
      name_list.push_front(last_name+" "+first_name);
    }
  }
  // circumstance2
  else if((*costume_shop.find(costume_name)).second == 0){
    std::cout<<"No "<<costume_name<<" costumes available."<<std::endl;
    if(people.find(input_name) == people.end()){
      people.insert(std::pair<std::string,std::string>(input_name,""));
    }
    //also push that name to the name_list and we are going to need that for
    //print function.
    if(find(name_list.begin(),name_list.end(),last_name+" "+first_name) 
      == name_list.end()){
        name_list.push_front(last_name+" "+first_name);
    }
  }
  // three circumstance for people to rent costume
  // 1.the customer hasn't got a costume yet
  // 2.the customer has a costume and want to rent a new one
  // 3.the customer has a costume and want to rent exactly the same one.
  else{
    // circumstance 1
    if(people.find(input_name) == people.end() ||
      (*people.find(input_name)).second == ""){
      // people.insert(std::pair<std::string,std::string>(input_name,costume_name));
      people[input_name] = costume_name;
      //decrease the costume's account by 1
      (*costume_shop.find(costume_name)).second -= 1;
      //add the name of the costomer to the costume list
      (*costume_list.find(costume_name)).second.push_back(input_name);
      //also push that name to the name_list and we are going to need that for
      //print function.
      if(find(name_list.begin(),name_list.end(),last_name+" "+first_name)
       == name_list.end()){
        name_list.push_front(last_name+" "+first_name);
      }
      std::cout<<input_name<<" rents a "<<costume_name<<" costume."<<std::endl;
    }
    // circumstance 2
    else if((*people.find(input_name)).second != costume_name){
      //increase the old costume's account by 1
      std::string oldCostume = (*people.find(input_name)).second;
      (*costume_shop.find(oldCostume)).second += 1;
      //remove the name of the old costomer rent the older costume
      (*costume_list.find(oldCostume)).second.remove(input_name);
      //insert the new one
      people.insert(std::pair<std::string,std::string>(input_name,costume_name));
      //decrease the costume's account by 1
      (*costume_shop.find(costume_name)).second -= 1;
      (*costume_list.find(costume_name)).second.push_back(input_name);
      (*people.find(input_name)).second = costume_name;
      //also push that name to the name_list and we are going to need that for
      //print function.
      if(find(name_list.begin(),name_list.end(),last_name+" "+first_name)
       == name_list.end()){
        name_list.push_front(last_name+" "+first_name);
      }
      std::cout<<input_name<<" returns a "<<oldCostume<<" costume before renting a "
      <<costume_name<<" costume."<<std::endl;
    }
    //circumstance 3
    else if ((*people.find(input_name)).second == costume_name){
      std::cout<<input_name<<" already has a "<<costume_name<<" costume."<<std::endl;
      if(find(name_list.begin(),name_list.end(),last_name+" "+first_name) == name_list.end()){
        name_list.push_front(last_name+" "+first_name);
      }
    }
    else{
      std::cerr<<"Wrong Circumstances.Please check."<<std::endl;
    }
  }
}

// take in two arguments, one is the costume shop, and another is
// the costume list. this function will print the current value
// of costume the shop still has and the number and name of costomers 
// that rent this costume. when rent_number or  available_number is 0,
// it will print nothing.
void lookup(COSTUME_SHOP_TYPE& costume_shop,
            COSTUME_TYPE& costume_list) {
  std::string costume_name;
  std::cin >> costume_name;
  //get the number that still in the shop
  if(costume_shop.find(costume_name) == costume_shop.end()){
    std::cout<<"The shop doesn't carry "<<costume_name<<" costumes."<<std::endl;
  }
  else{
    int num_available = (*costume_shop.find(costume_name)).second;
    int num_rented = (*costume_list.find(costume_name)).second.size();
    //create a list iterator to point at the begining of the list that costume
    //points at.
    std::list<std::string>::iterator iter = (*costume_list.find(costume_name)).
    second.begin();
    std::cout<<"Shop info for "<<costume_name<<" costumes:"<<std::endl;
    if(num_available == 1){
      std::cout<<"  "<<num_available<<" copy available"<<std::endl;
    }
    else if(num_available > 1){
      std::cout<<"  "<<num_available<<" copies available"<<std::endl;
    }
    //when num_available = 0, just pass.
    if(num_rented == 1){
      std::cout<<"  "<<num_rented<<" copy rented by:"<<std::endl;
      std::cout<<"  "<<"  "<<(*iter)<<std::endl;
    }
    if (num_rented > 1){
      std::cout<<"  "<<num_rented<<" copies rented by:"<<std::endl;
      for(;iter != (*costume_list.find(costume_name)).second.end(); iter++){
        std::cout<<"  "<<"  "<<(*iter)<<std::endl;
      }
    }
    //when num_available = 0, just pass.
  }
}

// the funciton take in one argument, the people map.
// and this function will print all the people that attempted to
// rent costume, even though they failed to rent one.
void printPeople(PEOPLE_TYPE& people,std::list<std::string>& name_list) {
  int size = people.size();
  if(size == 1){
    std::cout<<"Costume info for the "<<size<<" party attendee:"<<std::endl;
  }
  else{
    std::cout<<"Costume info for the "<<size<<" party attendees:"<<std::endl;
  }
  name_list.sort();
  PEOPLE_TYPE::iterator iter = people.begin();
  std::list<std::string>::iterator iter_list = name_list.begin();
  for(;iter_list != name_list.end();iter_list++){
    std::size_t pos = (*iter_list).find(" ");
    int size = (*iter_list).size();
    std::string first_name = (*iter_list).substr(pos+1);
    std::string last_name = (*iter_list).substr(0,size-first_name.size()-1);
    iter = people.find(first_name+" "+last_name);
    if((*iter).second == ""){
      std::cout<<"  "<<first_name<<" "<<last_name<<" does not have a costume."
      <<std::endl;
    }
    else{
      std::cout<<"  "<<first_name<<" "<<last_name<<" is wearing a "<<(*iter).
      second<<" costume."<<std::endl;
    }
  }
}
