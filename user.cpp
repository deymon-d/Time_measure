#include "user.h"

User::Experiment::Experiment(size_t count, std::function<int(uint64_t&)>&& func) {
    count_ = count;
    function_ = std::move(func);
    results_.resize(count_);
    time_results_.resize(count_);
    return_codes_.resize(count_);
}

std::function<int(uint64_t &)>
User::set_args(std::function<int(uint64_t, uint64_t, uint64_t&)>&& func, uint64_t n, uint64_t mod) {
    return [n, mod, func](uint64_t& result) {
        return func(n, mod, result);
    };
}

void User::Experiment::exec_experiments() {
    for (size_t i = 0; i < count_; ++i) {
        time_t start = clock();
        return_codes_[i] = function_(results_[i]);
        time_t end = clock();
        time_results_[i] = ((double)(end - start)) / CLOCKS_PER_SEC;
    }
}

void User::add_experiment(const std::string& name, size_t count,
                          std::function<int(uint64_t, uint64_t, uint64_t &)>&& func,
                          uint64_t n, uint64_t mod) {
    experiments_[name] = Experiment(count, set_args(std::move(func), n, mod));
}

void User::exec_experiment(const std::string &name) {
    experiments_[name].exec_experiments();
}

const std::vector<uint64_t>& User::get_results(const std::string& name) {
    return experiments_[name].results_;
}

const std::vector<int>& User::get_return_codes(const std::string& name) {
    return experiments_[name].return_codes_;
}

const std::vector<double>& User::get_time_results(const std::string& name) {
    return experiments_[name].time_results_;
}
