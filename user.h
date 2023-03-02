#pragma once

#include <ctime>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class User {
public:
    void add_experiment(const std::string& name, size_t count,
                        std::function<int(uint64_t, uint64_t, uint64_t&)>&& func,
                        uint64_t n, uint64_t mod);

    void exec_experiment(const std::string& name);

    const std::vector<double>& get_time_results(const std::string& name);
    const std::vector<int>& get_return_codes(const std::string& name);
    const std::vector<uint64_t>& get_results(const std::string& name);
private:
    class Experiment {
    public:
        Experiment() = default;
        Experiment(const Experiment& other) = default;
        Experiment(Experiment&& other) = default;
        Experiment& operator=(const Experiment& other) = default;
        Experiment& operator=(Experiment&& other) = default;
        Experiment(size_t count, std::function<int(uint64_t&)>&& func);

        void exec_experiments();

        size_t count_ = 0;
        std::function<int(uint64_t&)> function_;
        std::vector<double> time_results_;
        std::vector<int> return_codes_;
        std::vector<uint64_t> results_;
    };

    std::function<int(uint64_t&)> set_args(std::function<int(uint64_t, uint64_t, uint64_t&)>&& func,
                                           uint64_t n, uint64_t mod);

    std::unordered_map<std::string, Experiment> experiments_;
};