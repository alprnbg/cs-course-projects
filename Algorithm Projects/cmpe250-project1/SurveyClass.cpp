#include "SurveyClass.h"


//Looks for an element in a linked list and returns true if the linked list contains that element.
bool search_node(LinkedList* list, string _name){
    if(list->length==0)
        return false;
    Node* current = list->head;
    while(current){
        if(current->name == _name)
            return true;
        current = current->next;
    }
    return false;
}

SurveyClass::SurveyClass() {
    this->members = new LinkedList();
}

SurveyClass::SurveyClass(const SurveyClass& other){
    this->members = new LinkedList(*other.members);
}


SurveyClass& SurveyClass::operator=(const SurveyClass& list){
    delete this->members;
    this->members = new LinkedList(*list.members);
    return *this;
}


SurveyClass::SurveyClass(SurveyClass&& other){
    this->members = move(other.members);
    other.members = nullptr;
}


SurveyClass& SurveyClass::operator=(SurveyClass&& list){
    delete this->members;
    this->members = move(list.members);
    list.members = nullptr;
    return *this;
}


SurveyClass::~SurveyClass(){
    delete members;
}

// Adds a new Node to the linked list or updates the corresponding Node in the linked list
void SurveyClass::handleNewRecord(string _name, float _amount){
    if(search_node(this->members, _name))
        members->updateNode(_name, _amount);
    else
        members->pushTail(_name, _amount);
}


// Calculates and returns the minimum amount of expense.
// The minimum amount can have up to two decimal points.
float SurveyClass::calculateMinimumExpense(){
    if(members->length==0){
        return 0;
    }
    float min = members->head->amount;
    Node* current = members->head;
    while(current){
        if(current->amount<min)
            min = current->amount;
        current = current->next;
    }
    min = int(min*100) / 100.0;
    return min;
}


// Calculates and returns the maximum amount of expense.
// The maximum amount can have up to two decimal points.
float SurveyClass::calculateMaximumExpense(){
    if(members->length==0){
        return 0;
    }
    float max = members->head->amount;
    Node* current = members->head;
    while(current){
        if(current->amount>max)
            max = current->amount;
        current = current->next;
    }
    max = int(max*100) / 100.0;
    return max;
}


// Calculates and returns the average amount of expense.
// The average amount can have up to two decimal points.
float SurveyClass::calculateAverageExpense(){
    if(members->length==0){
        return 0;
    }
    float result,sum = 0;
    int N = 0;
    Node* current = members->head;
    while(current){
        sum += current->amount;
        current = current->next;
        N++;
    }
    result = int(sum/N*100) / 100.0;
    return result;
}


