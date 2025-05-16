#include <iostream>
#include <cstddef>
#include <utility>
#include <type_traits>
#include <memory>
#include <functional>

using namespace std;

namespace my {

template<typename T, typename Deleter = std::default_delete<T>>
class shared_ptr {
private:
    struct ControlBlock {
        T* ptr;
        std::size_t count;
        Deleter deleter;

        ControlBlock(T* p, Deleter d)
            : ptr(p), count(1), deleter(std::move(d)) {}
    };

    ControlBlock* cb;

    void release() noexcept {
        if (!cb) 
            return;
        if (--cb->count == 0) {
            if (cb->ptr) cb->deleter(cb->ptr);
            delete cb;
        }
        cb = nullptr;
    }

public:
    shared_ptr() noexcept : cb(nullptr) {}

    explicit shared_ptr(T* p) : cb(nullptr) {
        if (p) 
            cb = new ControlBlock(p, Deleter{});
    }

    shared_ptr(T* p, Deleter d) : cb(nullptr) {
        if (p) 
            cb = new ControlBlock(p, std::move(d));
    }

    shared_ptr(const shared_ptr& other) noexcept : cb(other.cb) {
        if (cb)
            ++cb->count;
    }

    shared_ptr(shared_ptr&& other) noexcept : cb(other.cb) {
        other.cb = nullptr;
    }

    shared_ptr& operator=(const shared_ptr& other) noexcept {
        if (this != &other) {
            release();
            cb = other.cb;
            if (cb)
                ++cb->count;
        }
        return *this;
    }

    shared_ptr& operator=(shared_ptr&& other) noexcept {
        if (this != &other) {
            release();
            cb = other.cb;
            other.cb = nullptr;
        }
        return *this;
    }

    ~shared_ptr() {
        release();
    }

    T* get() const noexcept { 
        return cb ? cb->ptr : nullptr; 
    }

    T& operator*() const noexcept { 
        return *get(); 
    }

    T* operator->() const noexcept { 
        return get(); 
    }

    explicit operator bool() const noexcept { 
        return get() != nullptr; 
    }

    void reset() noexcept { release(); }

    void reset(T* p) {
        release();
        if (p) cb = new ControlBlock(p, Deleter{});
    }

    void reset(T* p, Deleter d) {
        release();
        if (p) cb = new ControlBlock(p, std::move(d));
    }
};

} // namespace my

struct Test {
    
    int id;
    Test(int id) : id(id) {
         cout << "Test " << id << " created\n"; 
    }

    ~Test() { 
        cout << "Test " << id << " destroyed (default delete)\n"; 
    }
};

void custom_deleter(Test* p) {
    cout << "Custom deleter for Test " << p->id << "\n";
    delete p;
}

struct StatefulDeleter {
    string message;
    StatefulDeleter(const string& msg) : message(msg) {}
    void operator()(Test* p) const {
        cout << message << " Test " << p->id << "\n";
        delete p;
    }
};

int main() {
    cout << "--- Test 1: Default deleter ---\n";
    {
        my::shared_ptr<Test> p1(new Test(1));
        auto p2 = p1;
    }
    cout << "\n";

    cout << "--- Test 2: C-style function deleter ---\n";
    {
        my::shared_ptr<Test, void(*)(Test*)> p3(new Test(2), custom_deleter);
        auto p4 = p3;
    }
    cout << "\n";

    cout << "--- Test 3: Lambda deleter ---\n";
    {
        auto lambda = [](Test* p) {
            cout << "Lambda deleter for Test " << p->id << "\n";
            delete p;
        };
        my::shared_ptr<Test, decltype(lambda)> p5(new Test(3), lambda);
    }
    cout << "\n";

    cout << "--- Test 4: Stateful functor deleter ---\n";
    {
        StatefulDeleter sd("Stateful deleter for");
        my::shared_ptr<Test, StatefulDeleter> p6(new Test(4), sd);
    }
    cout << "\n";

    cout << "--- Test 5: std::function deleter ---\n";
    {
        function<void(Test*)> func = [](Test* p) {
            cout << "std::function deleter for Test " << p->id << "\n";
            delete p;
        };
        my::shared_ptr<Test, function<void(Test*)>> p7(new Test(5), func);
    }
    cout << "\n";

    return 0;
}
