#pragma once
#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <algorithm>
#include <condition_variable>
#include <map>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

namespace after{
namespace Dispatcher{
    struct impl_KEY{
        size_t index = 0;
        impl_KEY &operator=(const size_t &other){
          index = other;
          return *this;
        }
        impl_KEY(const size_t &other) : index(other){}

        impl_KEY() = default;
        impl_KEY &operator=(const impl_KEY &other) = default;
        impl_KEY &operator=(impl_KEY &&other) = default;
        impl_KEY(const impl_KEY &other) = default;
        impl_KEY(impl_KEY&& other) = default;
        ~impl_KEY() = default;
    };

    template <typename inData_t, typename outData_t, typename Callable_t, typename Key_t> struct Dispatcher;

    template <typename inData_t, typename outData_t, typename Callable_t, typename Key_t> class impl_Request {
        Callable_t _func;
        Dispatcher<inData_t, outData_t, Callable_t, Key_t> *_dispatcher;

    public:
        std::pair<Key_t, inData_t> _data;
        impl_Request(Callable_t func, std::pair<Key_t, inData_t> v, Dispatcher<inData_t, outData_t, Callable_t, Key_t> *dispatcher) : _data(std::move(v)), _func(std::move(func)), _dispatcher(dispatcher) {}

        template <typename T1, typename T2>
        void process(std::pair<T1, T2> p) {
          if constexpr (std::is_same_v<T1, impl_KEY>) { //Note: cppcheck has a bug here. This is not a syntax error
            _dispatcher->threadSafe_push_outData({p.first, _func(std::move(p.second))});
          } else {
            _dispatcher->threadSafe_push_outData(_func(std::move(p)));
          }
        }
    };

    template <typename inData_t, typename outData_t, typename Callable_t, typename Key_t> class impl_Worker {
        impl_Request<inData_t, outData_t, Callable_t, Key_t> *request = nullptr;
        bool running = true;
        Dispatcher<inData_t, outData_t, Callable_t, Key_t> *_dispatcher;

    public:
        explicit impl_Worker(Dispatcher<inData_t, outData_t, Callable_t, Key_t> *dispatcher) : _dispatcher(dispatcher) {}
        void run(){
          while (running) {
            if (_dispatcher->addWorker(this)) {
              request->process(std::move(request->_data));
            } else {
              running = false;
            }
          }
        }
        void stop() { running = false; }
        void receive_request(impl_Request<inData_t, outData_t, Callable_t, Key_t> *request) { this->request = request; }
    };







// inData_t: The type you want to pass to the lambda
// outData_t: The type which the return vector contains
// Callable_t: The type of lambda
    template <typename inData_t, typename outData_t, typename Callable_t, typename Key_t> struct Dispatcher {

        template <typename Input_t, typename Output_t>
        Dispatcher(Input_t input, Output_t &output, Callable_t lambda, const size_t& numThreads) noexcept :  numRequests(input.size()) {
          outData.clear();
          reserve_sizes(numRequests);
          set_concurrent_tasks(std::move(input), std::move(lambda));
          start_threads(numThreads);
          wait_for_completion();
          move_out(output);
        }


    private:
        // I need requests be shared and lightweight
        std::vector<impl_Request<inData_t, outData_t, Callable_t, Key_t> *> requests;
        std::vector<impl_Request<inData_t, outData_t, Callable_t, Key_t> *> requests_backup; // backup for deleting
        std::mutex mu_requests;
        std::vector<impl_Worker<inData_t, outData_t, Callable_t, Key_t> *> allWorkers;
        std::vector<std::thread> threads;
        std::mutex mu_mainTread;
        std::condition_variable cv_mainThread;
        std::vector<std::pair<Key_t, outData_t>> outData;
        std::mutex mu_outData;
        size_t numRequests;

        bool start_threads(const size_t& workers) {
          impl_Worker<inData_t, outData_t, Callable_t, Key_t> *w = nullptr;
          for (size_t i = 0; i < workers; ++i) {
            w = new impl_Worker<inData_t, outData_t, Callable_t, Key_t>(this);
            allWorkers.push_back(w);
            threads.emplace_back(&impl_Worker<inData_t, outData_t, Callable_t, Key_t>::run, w);
          }
          return true;
        }

        void stop() {
          for (auto &worker : allWorkers) worker->stop();
          for (auto &thread : threads) thread.join();
          for (auto &worker : allWorkers) delete worker;
          for (auto &r : requests_backup) delete r;
        }

        void wait_for_completion() {
          std::unique_lock lk(mu_mainTread);
          cv_mainThread.wait(lk, [&] { return requests.empty() && outData.size() == numRequests; });
          stop();
        }

        void reserve_sizes(const size_t &num) {
          requests.reserve(num);
          requests_backup.reserve(num);
          outData.reserve(num);
        }

        template <typename Input_t>
        void set_concurrent_tasks(Input_t inData, Callable_t func) {
          impl_Request<inData_t, outData_t, Callable_t, Key_t> *rq = nullptr;
          for (auto &ele : inData) {
            // Every request has its own copy of lambda
            rq = new impl_Request<inData_t, outData_t, Callable_t, Key_t>(func, std::move(ele), this);
            requests.push_back(rq);
            requests_backup.push_back(rq);
          }
        }

        bool addWorker(impl_Worker<inData_t, outData_t, Callable_t, Key_t> *worker) {
          bool get_request = false;
          std::scoped_lock lk(mu_requests);
          if (!requests.empty()) {
            auto *request = requests.back();
            worker->receive_request(request);
            requests.pop_back();
            get_request = true;
          }

          return get_request;
        }

        void threadSafe_push_outData(std::pair<Key_t, outData_t> data) {
          std::scoped_lock lk(mu_outData);
          outData.push_back(std::move(data));
          if (outData.size() == numRequests) {
            cv_mainThread.notify_one();
          }
        }

        template <typename Output_t>
        void move_out(Output_t& output){
          if constexpr (std::is_same_v<std::decay_t<Output_t>, std::map<Key_t, outData_t>>) {
            std::move(outData.begin(), outData.end(), std::inserter(output, output.begin()));
          }else{
            std::move(outData.begin(), outData.end(), std::back_inserter(output));
          }
        }

        friend void impl_Worker<inData_t, outData_t, Callable_t, Key_t>::run();
        template <typename T1, typename T2>
        friend void impl_Request<inData_t, outData_t, Callable_t, Key_t>::process(std::pair<T1,T2> p);
    };
}


// Use this API, if sorting is expensive
// or __the operation of `lambda` makes assumption of position__
// Input_t:     container of std::pair<Key_t, inData_t>
// Output_t:    container of std::pair<Key_t, outData_t> including std::map and others that supports push_back
// Callable_t:  Accept only one argument of type std::pair<Key_t, inData_t> and return value of type std::pair<Key_t, outData_t>
//              The callable object will be copy for each input-output pair
template <typename Input_t, typename Output_t, typename Callable_t> void run_concurrent_tasks(Input_t input, Output_t &output, const size_t & numThreads, Callable_t lambda) {
  using Key_t = std::decay_t<typename Input_t::value_type::first_type>;
  using inData_t = std::decay_t<typename Input_t::value_type::second_type>;
  using outData_t = std::decay_t<typename Output_t::value_type::second_type>;
  // Ensure the input and output key is of the same type to eliminate conversion overhead
  static_assert(std::is_same_v<Key_t, std::decay_t<typename Output_t::value_type::first_type>>);

  Dispatcher::Dispatcher<inData_t, outData_t, Callable_t, Key_t> dispatcher(std::move(input), output, std::move(lambda), numThreads);
}

// Use this API, if you simply what the order of the output coincide with the input
// And __make no assumption of position__ when executing the `lambda`
// Input_t:     vector, list, deque... As long as it is subscriptable by operator[] and has size() method
// Output_t:    vector, list, deque... As long as it has push_back() method
// Callable_t:  Accept only one argument of type Input_t::value_type and return value of type Output_t::value_type
//              The callable object will be copy for each input-output pair
template <typename Input_t, typename Output_t, typename Callable_t> void run_concurrent_tasks_ordered(Input_t input, Output_t &output, const size_t & numThreads, Callable_t lambda) {
  using inData_t = std::decay_t<typename Input_t::value_type>;
  using outData_t = std::decay_t<typename Output_t::value_type>;
  using Key_t = Dispatcher::impl_KEY;
  const auto size = input.size();
  std::vector<Key_t> keys(size);
  std::iota(keys.begin(), keys.end(), 0);
  std::vector<std::pair<Key_t, inData_t>> concurrent_input;
  concurrent_input.reserve(size);
  for (size_t i = 0; i < size; i++) {
    concurrent_input.emplace_back(keys[i], std::move(input[i]));
  }
  std::vector<std::pair<Key_t, outData_t>> concurrent_output;
  Dispatcher::Dispatcher<inData_t, outData_t, Callable_t, Key_t> dispatcher(std::move(concurrent_input), concurrent_output, std::move(lambda), numThreads);
  std::sort(concurrent_output.begin(), concurrent_output.end(), [](const auto& v1, const auto& v2) { return v1.first.index < v2.first.index; });
  output.clear();
  output.reserve(size);
  for (auto &ele : concurrent_output) {
    output.push_back(std::move(ele.second));
  }
}
}

#endif

