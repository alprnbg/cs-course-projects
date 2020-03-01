#include "LinkedList.h"

Node* find_tail_helper(LinkedList& list){
    Node* current = list.head;
    while(current){
        if(!current->next)
            break;
        current = current->next;
    }
    return current;
}

LinkedList::LinkedList(){
    this->length = 0;
}

LinkedList::LinkedList(const LinkedList& list){
    this->length = list.length;
    this->head = new Node(*(list.head));
    this->tail = find_tail_helper(*this);
}

LinkedList& LinkedList::operator=(const LinkedList& list){
    delete this->head;
    this->head = new Node(*list.head);
    this->length = list.length;
    this->tail = find_tail_helper(*this);
    return *this;
}

LinkedList::LinkedList(LinkedList&& list){
    cout<<"move constructor"<<endl;
    this->head = move(list.head);
    this->length = move(list.length);
    this->tail = find_tail_helper(*this);
    list.head = nullptr;
    list.tail = nullptr;
    list.length = 0;
}

LinkedList& LinkedList::operator=(LinkedList&& list) {
    cout<<"move assignment"<<endl;
    delete this->head;
    this->head = move(list.head);
    this->length = move(list.length);
    this->tail = find_tail_helper(*this);
    list.head = nullptr;
    list.tail = nullptr;
    list.length = 0;
    return *this;
}

void LinkedList::pushTail(string _name, float _amount) {
    Node *newNode = new Node(_name, _amount);
    if (this->head == nullptr) {
        head = tail = newNode;
    } else {
        this->tail->next = newNode;
        this->tail = this->tail->next;
    }
    length++;
}

void LinkedList::updateNode(string _name, float _amount){
    Node* current = this->head;
    while(current){
        if(current->name == _name){
            current->amount = _amount;
            return;
        }
        current = current->next;
    }
}

LinkedList::~LinkedList(){
   delete head;
}




