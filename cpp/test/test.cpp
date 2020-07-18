#include <iostream>
#include <vector>

using namespace std;

struct Abstract {
    virtual void f() = 0; // pure virtual
    virtual void g() {}  // non-pure virtual
    ~Abstract() {
        g(); // OK: calls Abstract::g()
        // f(); // undefined behavior
        Abstract::f(); // OK: non-virtual call
    }
};
 
// definition of the pure virtual function
void Abstract::f() { std::cout << "A::f()\n"; }
 
struct Concrete : Abstract {
    void f() override {
        Abstract::f(); // OK: calls pure virtual function
    }
    void g() override {}
    ~Concrete() {
        g(); // OK: calls Concrete::g()
        f(); // OK: calls Concrete::f()
    }
};


int main() {
    cout << "hello" << endl;

    vector<Abstract*> moves;

    moves.push_back(new Concrete());

    return 0;
};