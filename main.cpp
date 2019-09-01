#include <cstdlib>
#include <string>
#include <iostream>
#include <optional>
#include <filesystem>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weverything"
#define SOL_ALL_SAFETIES_ON 1
#include "./sol.hpp"

#pragma GCC diagnostic pop

struct Beta {
    int val = 0;
    sol::protected_function cb;
    Beta (int t_val) : val(t_val) {
        std::cout << "Beta" << std::endl;
    }
    ~Beta () {
        std::cout << "~Beta" << std::endl;
    }
    static void setupLua (sol::state& lua) {
        lua.new_usertype<Beta>(
            "Beta", sol::constructors<Beta(int)>(),
            "setCallback", &Beta::setCallback,
            "getValue", &Beta::getValue
        );
    }
    int getValue () {
        return val;
    }
    void setCallback (sol::protected_function t_cb) {
        cb = t_cb;
    }
    void runCallback () {
        cb();
    }
};
struct Alpha {
    std::vector<Beta> items;
    Alpha () {
        std::cout << "Alpha" << std::endl;
    }
    ~Alpha () {
        std::cout << "~Alpha" << std::endl;
    }

    void run () {
        for (Beta& item : items) {
            item.runCallback();
        }
    }
};
class Zeta {
    public:
    Alpha alp;
    sol::state lua;

    Zeta () {
        std::cout << "Zeta" << std::endl;
        setupLua();
    }
    ~Zeta () {
        std::cout << "~Zeta" << std::endl;
    }

    // Return a reference to the Beta instance in the vector. We want to keep it around.
    Beta& createBeta (int val) {
        alp.items.push_back(Beta(val));
        return alp.items.at(alp.items.size()-1);
    }

    void setupLua () {
        Beta::setupLua(lua);

        // Have a function we use to create Beta instances
        lua.set_function("createBeta", &Zeta::createBeta, this);

        lua.script_file("./file.lua");
    }

    void run () {
        alp.run();
    }
};
int main () {
    std::cout << "Starting\n";

    Zeta z;

    z.run();

    return 0;
}