#include <cstring>
#include <stdexcept>
#include <utility>
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

class SmallString {
public:
  static constexpr size_t ssoThreshold = 15;

  SmallString() noexcept
    : size(0), capacity(ssoThreshold), isSmall(true) {
    data.small[0] = '\0';
  }

  SmallString(const char* s) {
    size_t len = strlen(s);
    if (len <= ssoThreshold) {
      isSmall = true;
      capacity = ssoThreshold;
      memcpy(data.small, s, len + 1);
    } else {
      isSmall = false;
      capacity = len;
      data.ptr = new char[capacity + 1];
      memcpy(data.ptr, s, len + 1);
    }
    size = len;
  }

  SmallString(const SmallString& other) {
    size = other.size;
    capacity = other.capacity;
    isSmall = other.isSmall;
    if (isSmall) {
      memcpy(data.small, other.data.small, size + 1);
    } else {
      data.ptr = new char[capacity + 1];
      memcpy(data.ptr, other.data.ptr, size + 1);
    }
  }

  SmallString(SmallString&& other) noexcept
    : size(other.size), capacity(other.capacity), isSmall(other.isSmall) {
    if (isSmall) {
      memcpy(data.small, other.data.small, size + 1);
    } else {
      data.ptr = other.data.ptr;
      other.data.ptr = nullptr;
    }
    other.size = 0;
    other.capacity = ssoThreshold;
    other.isSmall = true;
    other.data.small[0] = '\0';
  }

  ~SmallString() {
    if (!isSmall && data.ptr) delete[] data.ptr;
  }

  SmallString& operator=(SmallString other) noexcept {
    swap(other);
    return *this;
  }

  size_t getSize() const noexcept { return size; }
  size_t getCapacity() const noexcept { return capacity; }

  void reserve(size_t newCap) {
    if (newCap <= capacity) return;
    char* buf = new char[newCap + 1];
    memcpy(buf, rawData(), size + 1);
    if (!isSmall) delete[] data.ptr;
    data.ptr = buf;
    isSmall = false;
    capacity = newCap;
  }

  void pushBack(char c) {
    if (size + 1 > capacity) {
      reserve(capacity + capacity/2 + 1);
    }
    char* p = rawData();
    p[size++] = c;
    p[size] = '\0';
  }

  char& operator[](size_t idx) {
    assert(idx < size);
    return rawData()[idx];
  }

  const char* cStr() const noexcept {
    return rawData();
  }

private:
  size_t size;
  size_t capacity;
  bool   isSmall;
  union Data {
    char* ptr;
    char  small[ssoThreshold + 1];
    Data(): ptr(nullptr) {}
  } data;

  char* rawData() noexcept {
    return isSmall ? data.small : data.ptr;
  }
  const char* rawData() const noexcept {
    return isSmall ? data.small : data.ptr;
  }

  void swap(SmallString& other) noexcept {
    std::swap(size, other.size);
    std::swap(capacity, other.capacity);
    std::swap(isSmall, other.isSmall);
    Data tmp;
    memcpy(&tmp, &data, sizeof(Data));
    memcpy(&data, &other.data, sizeof(Data));
    memcpy(&other.data, &tmp, sizeof(Data));
  }
};

int main() {

  cout << "Enter your text (multiple words allowed):" << endl;
  cout << ">> ";

  string input;
  getline(cin, input);

  SmallString s(input.c_str());
  s.pushBack('!');

  cout << endl;
  cout << "You entered: " << s.cStr() << endl;
  cout << "(size=" << s.getSize() << ", capacity=" << s.getCapacity() << ")" << endl;

  return 0;
}
