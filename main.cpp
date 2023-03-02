#include <iostream>

#include "tests.h"
#include "user.h"

int main() {
    User user;
    user.add_experiment("first", 10, factorial_by_mod, 10, 10);
    user.add_experiment("second", 10,  factorial_by_mod, 10, 100);
    user.add_experiment("third", 10, factorial_by_mod, 1e8, 1e9+7);
    user.add_experiment("fourth", 10, factorial_by_mod, 5e8, 1e9+7);
    user.add_experiment("bad", 10, factorial_by_mod, 1e12, 0);
    for (auto exp : {"first", "second", "third", "fourth", "bad"}) {
        std::cout << "Experiment: " << exp << "\n";
        user.exec_experiment(exp);
        std::cout << "returns with code number: " << user.get_return_codes(exp)[0] << "\n";
        std::cout << "returns result: " << user.get_results(exp)[0] << "\n";
        std::cout << "with time:\n";
        for (auto& time : user.get_time_results(exp)) {
            std::cout << time << "s\n";
        }
        std::cout <<"\n\n";
    }
    return 0;
}
