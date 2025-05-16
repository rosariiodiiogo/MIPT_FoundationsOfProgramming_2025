#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T> class Deque{
private:
    struct  Node
    {
        T data;
        Node* prev;
        Node* next;
    
        Node(T value)
            :data(value),
            prev(nullptr),
            next(nullptr)
        {
        }
    };

    Node* front;
    Node* rear;
    int size;

public:
    Deque()
        :front(nullptr),
        rear(nullptr),
        size(0)
    {
    }

    ~Deque()
    {
        while (!is_empty())
        {
            pop_front();
        }
    }

    void push_front(T value)
    {
        Node* new_node = new Node(value);

        if (is_empty())
        {
           front = rear = new_node;
        }
        else
        {
            new_node -> next = front;
            front -> prev = new_node;
            front = new_node;
        }
        size++;
    }

    void push_back(T value)
    {
        Node* new_node = new Node(value);
        if (is_empty())
        {
            front = rear = new_node;
        }
        else
        {
            new_node -> prev = rear;
            rear -> next = new_node;
            rear = new_node;
        }
        size++;
    }

    void pop_front()
    {
        if (is_empty())
        {
            throw runtime_error("Deque is empty");
        }
        
        Node* temp = front;
        front = front -> next;

        if (front == nullptr)
        {
            rear = nullptr;
        }
        else
        {
            front -> prev = nullptr;
        }
        delete temp;
        size--;
    }

    void pop_back()
    {
        if(is_empty())
        {
            throw runtime_error("Deqe is empty");
        }

        Node* temp = rear;
        rear = rear -> prev;

        if (rear == nullptr)
        {
            front = nullptr;
        }
        else
        {
            rear -> next = nullptr;
        }
        delete temp;
        size--;
    }

    T get_front()
    {
        if(is_empty())
            throw runtime_error("Deque is empty");

        return front -> data;
    }

    T get_rear()
    {
        if (is_empty())
            throw runtime_error("Deque is empty");
        
        return rear -> data;
    }

    bool is_empty()
    {
        return size == 0;
    }

    int get_size()
    {
        return size;
    }

    void display()
    {
        Node* current = front;

        while (current != nullptr)
        {
            cout << current -> data << " ";
            current = current -> next;
        }
        cout << endl;
    }
};

template <typename T> class Stack: public Deque <T>
{
public: 

    using Deque<T>::push_back;
    using Deque<T>::pop_back;
    
    void push(const T& value)
    {
        push_back(value);
    }

    void pop()
    {
        pop_back();
    }

    T& top()
    {
        return this -> get_rear();
    }
};

int main()
{
    Stack <int> stk;

    stk.push(7);
    stk.push(8);

    cout << "Stack: ";
    stk.display();

    stk.pop();
    cout << "Stack: ";
    stk.display();
}