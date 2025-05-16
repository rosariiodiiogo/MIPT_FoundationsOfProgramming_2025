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

int main()
{
    Deque <int> deque;

    deque.push_front(2);
    deque.push_back(4);
    deque.push_front(1);
    deque.push_back(6);

    cout << "Deque after pushes: ";
    deque.display();

    cout << "Front element: " << deque.get_front() << endl;
    cout << "Rear element: " << deque.get_rear() << endl;

    deque.pop_front();
    deque.pop_back();

    cout << "Deque after pops: ";
    deque.display();

    cout << "Size of deque: " << deque.get_size() << endl;
}