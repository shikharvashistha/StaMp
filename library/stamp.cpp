#include "stamp.h"

namespace stamp
{
    struct thread_pool
    {
        int numThreads;                                                  // number of threads
        std::vector<pthread_t> thread_id;                                // thread id
        std::vector<std::function<void()>> lambda_void_array;            // lambda function array for void
        std::vector<std::function<void(int)>> lambda_int_array;          // lambda function array for int
        std::vector<std::function<void(int, int)>> lambda_int_int_array; // lambda function array for int, int
        int low1, high1, stride1, low2, high2, stride2;             // for parallel_for

        std::pair<int, int> partition1, partition2, partition3, partition4; // for parallel_for
        /// @brief
        /// @param numThreads
        /// @param type
        thread_pool(int numThreads, char type) // constructor for thread_pool
        {
            this->numThreads = numThreads;
            thread_id.resize(numThreads);
            if (type == 'i') // i for int
                lambda_int_array.resize(numThreads);
            else if (type == 'v') // v for void
                lambda_void_array.resize(numThreads);
            else if (type == 'k') // k for int, int
                lambda_int_int_array.resize(numThreads);
        }
        /// @brief
        /// @param lambda
        void set_lambda(std::function<void()> &&lambda) // set lambda function for void
        {
            for (int i = 0; i < numThreads; i++)
            {
                lambda_void_array[i] = lambda;
            }
        }

        /// @brief
        /// @param lambda
        void set_lambda(std::function<void(int)> &&lambda) // set lambda function for int
        {
            for (int i = 0; i < numThreads; i++)
            {
                lambda_int_array[i] = lambda;
            }
        }

        /// @brief
        /// @param lambda
        void set_lambda(std::function<void(int, int)> &&lambda) // set lambda function for int, int
        {
            for (int i = 0; i < numThreads; i++)
            {
                lambda_int_int_array[i] = lambda;
            }
        }
        /// @brief
        /// @param thread_id
        /// @param thread_stub
        /// @param arg
        void create_thread(pthread_t &thread_id, void *(*thread_stub)(void *), void *arg) // create thread
        {
            int status;
            status = pthread_create(&thread_id, NULL, thread_stub, arg);
            if (status != 0)
            {
                printf("Error creating thread\n");
            }
        }

        void join_thread(pthread_t &thread_id) // join thread
        {
            pthread_join(thread_id, NULL);
        }
    };

    /// @brief
    /// @param arg
    /// @return
    void *thread_stub_et(void *arg) // thread stub for execute_tuple
    {
        std::function<void()> *lambda = static_cast<std::function<void()> *>(arg);
        (*lambda)();
        return NULL;
    }

    /// @brief
    /// @param arg
    /// @return
    void *thread_stub_pf_1(void *arg) // thread stub for parallel_for_1
    {
        thread_pool *tp = static_cast<thread_pool *>(arg);
        std::pair<int, int> partition=tp->partition1;
        std::function<void(int)> *lambda = tp->lambda_int_array.data();
        for (int i = partition.first; i < partition.second; i++)
        {
            (*lambda)(i);
        }
        return NULL;
    }

    /// @brief
    /// @param arg
    /// @return
    void *thread_stub_pf_2(void *arg) // thread stub for parallel_for_2
    {
        thread_pool *tp = static_cast<thread_pool *>(arg);
        std::pair<int, int> partition=tp->partition1;
        std::function<void(int)> *lambda = tp->lambda_int_array.data();
        for (int i = 0; i < partition.second; i++)
        {
            (*lambda)(i);
        }
        return NULL;
    }

    /// @brief
    /// @param arg
    /// @return
    void *thread_stub_pf_3(void *arg) // thread stub for parallel_for_3
    {
        thread_pool *tp = static_cast<thread_pool *>(arg);
        std::pair<int, int> partition1=tp->partition1, partition2=tp->partition2;
        std::function<void(int, int)> *lambda = tp->lambda_int_int_array.data();
        for (int i = partition1.first; i < partition1.second; i += tp->stride1)
        {
            for (int j = partition2.first; j < partition2.second; j += tp->stride2)
            {
                (*lambda)(i, j);
            }
        }
        return NULL;
    }

    /// @brief
    /// @param arg
    /// @return
    void *thread_stub_pf_4(void *arg) // thread stub for parallel_for_4
    {
        thread_pool *tp = static_cast<thread_pool *>(arg);
        std::pair<int, int> partition1=tp->partition1, partition2=tp->partition2;
        std::function<void(int, int)> *lambda = tp->lambda_int_int_array.data();
        for (int i = 0; i < partition1.second; i += tp->stride1)
        {
            for (int j = 0; j < partition2.second; j += tp->stride2)
            {
                (*lambda)(i, j);
            }
        }
        return NULL;
    }

    /// @brief
    /// @param lambda1
    /// @param lambda2
    void execute_tuple(std::function<void()> &&lambda1, std::function<void()> &&lambda2)
    {
        // accepts two C++11 lambda functions and runs the them in parallel

        clock_t start = clock();

        thread_pool thread_pool(2, 'v');

        thread_pool.set_lambda(std::move(lambda1));
        thread_pool.set_lambda(std::move(lambda2));

        thread_pool.create_thread(thread_pool.thread_id[0], thread_stub_et, static_cast<void *>(&thread_pool.lambda_void_array[0]));
        thread_pool.create_thread(thread_pool.thread_id[1], thread_stub_et, static_cast<void *>(&thread_pool.lambda_void_array[1]));

        thread_pool.join_thread(thread_pool.thread_id[0]);
        thread_pool.join_thread(thread_pool.thread_id[1]);

        printf("StaMp Statistics: Threads = %d, Parallel execution time = %f seconds\n", thread_pool.numThreads, (double)(clock() - start) / CLOCKS_PER_SEC);
    }

    /// @brief
    /// @param low
    /// @param high
    /// @param stride
    /// @param lambda
    /// @param numThreads
    void parallel_for(int low, int high, int stride, std::function<void(int)> &&lambda, int numThreads)
    {
        // parallel_for accepts a C++11 lambda function and runs the loop body (lambda) in
        // parallel by using ‘numThreads’ number of Pthreads created inside StaMp

        clock_t start = clock();

        thread_pool thread_pool(numThreads, 'i');

        thread_pool.low1 = low;
        thread_pool.high1 = high;
        thread_pool.stride1 = stride;

        for (int i = 0; i < numThreads; i++)
        {
            thread_pool.set_lambda(std::move(lambda));
        }

        std::vector<std::pair<int, int>> partition;
        int partition_size = (high - low) / numThreads;
        for (int i = 0; i < numThreads; i++)
        {
            partition.push_back(std::make_pair(i * partition_size, (i + 1) * partition_size));
        }

        for (int i = 0; i < numThreads; i++)
        {
            thread_pool.partition1 = partition[i];
            thread_pool.create_thread(thread_pool.thread_id[i], *thread_stub_pf_1, static_cast<void *>(&thread_pool));
        }

        for (int i = 0; i < numThreads; i++)
        {
            thread_pool.join_thread(thread_pool.thread_id[i]);
        }

        printf("StaMp Statistics: Threads = %d, Parallel execution time = %f seconds\n", numThreads, (double)(clock() - start) / CLOCKS_PER_SEC);
    }

    /// @brief
    /// @param high
    /// @param lambda
    /// @param numThreads
    void parallel_for(int high, std::function<void(int)> &&lambda, int numThreads)
    {
        // Shorthand for using parallel_for when lowbound is zero and stride is one.

        clock_t start = clock();

        thread_pool thread_pool(numThreads, 'i');

        thread_pool.high1 = high;

        for (int i = 0; i < numThreads; i++)
        {
            thread_pool.set_lambda(std::move(lambda));
        }

        std::vector<std::pair<int, int>> partition;
        int partition_size = high / numThreads;
        for (int i = 0; i < numThreads; i++)
        {
            partition.push_back(std::make_pair(i * partition_size, (i + 1) * partition_size));
        }

        for (int i = 0; i < numThreads; i++)
        {
            thread_pool.partition1 = partition[i];
            thread_pool.create_thread(thread_pool.thread_id[i], thread_stub_pf_2, static_cast<void *>(&thread_pool));
        }

        for (int i = 0; i < numThreads; i++)
        {
            thread_pool.join_thread(thread_pool.thread_id[i]);
        }

        printf("StaMp Statistics: Threads = %d, Parallel execution time = %f seconds\n", numThreads, (double)(clock() - start) / CLOCKS_PER_SEC);
    }

    /// @brief
    /// @param low1
    /// @param high1
    /// @param stride1
    /// @param low2
    /// @param high2
    /// @param stride2
    /// @param lambda
    /// @param numThreads
    void parallel_for(int low1, int high1, int stride1, int low2, int high2, int stride2,
                      std::function<void(int, int)> &&lambda, int numThreads)
    {

        // This version of parallel_for is for parallelizing two-dimensional for-loops, i.e., an outter for-i loop and
        // an inner for-j loop. Loop properties, i.e. low, high, and stride are mentioned below for both outter
        // and inner for-loops. The suffixes “1” and “2” represents outter and inner loop properties respectively.

        clock_t start = clock();

        thread_pool thread_pool(numThreads, 'k');

        thread_pool.low1 = low1;
        thread_pool.high1 = high1;
        thread_pool.stride1 = stride1;
        thread_pool.low2 = low2;
        thread_pool.high2 = high2;
        thread_pool.stride2 = stride2;

        for (int i = 0; i < numThreads; i++)
        {
            thread_pool.set_lambda(std::move(lambda));
        }

        std::vector<std::pair<int, int>> partition1, partition2;
        int first_partition_size = (high1 - low1) / numThreads;
        for (int i = 0; i < numThreads; i++)
        {
            partition1.push_back(std::make_pair(i * first_partition_size, (i + 1) * first_partition_size));
        }
        int second_partition_size = (high2 - low2) / numThreads;
        for (int i = 0; i < numThreads; i++)
        {
            partition2.push_back(std::make_pair(i * second_partition_size, (i + 1) * second_partition_size));
        }

        for (int i = 0; i < numThreads; i++)
        {
            thread_pool.partition1 = partition1[i];
            thread_pool.partition2 = partition2[i];
            thread_pool.create_thread(thread_pool.thread_id[i], thread_stub_pf_3, static_cast<void *>(&thread_pool));
        }

        for (int i = 0; i < numThreads; i++)
        {
            thread_pool.join_thread(thread_pool.thread_id[i]);
        }

        printf("StaMp Statistics: Threads = %d, Parallel execution time = %f seconds\n", numThreads, (double)(clock() - start) / CLOCKS_PER_SEC);
    }

    /// @brief
    /// @param high1
    /// @param high2
    /// @param lambda
    /// @param numThreads
    void parallel_for(int high1, int high2, std::function<void(int, int)> &&lambda, int numThreads)
    {

        // Shorthand for using parallel_for if for both outter and inner for-loops, lowbound is zero and stride is one.
        // In that case only highBounds for both these loop should be sufficient to provide. The suffixes “1” and “2”
        // represents outter and inner loop properties respectively.

        clock_t start = clock();

        thread_pool thread_pool(numThreads, 'k');

        thread_pool.high1 = high1;
        thread_pool.high2 = high2;

        for (int i = 0; i < numThreads; i++)
        {
            thread_pool.set_lambda(std::move(lambda));
        }
        std::vector<std::pair<int, int>> partition1, partition2;
        int first_partition_size = high1 / numThreads;
        for (int i = 0; i < numThreads; i++)
        {
            partition1.push_back(std::make_pair(i * first_partition_size, (i + 1) * first_partition_size));
        }
        int second_partition_size = high2 / numThreads;
        for (int i = 0; i < numThreads; i++)
        {
            partition2.push_back(std::make_pair(i * second_partition_size, (i + 1) * second_partition_size));
        }

        for (int i = 0; i < numThreads; i++)
        {
            thread_pool.partition1 = partition1[i];
            thread_pool.partition2 = partition2[i];
            thread_pool.create_thread(thread_pool.thread_id[i], thread_stub_pf_4, static_cast<void *>(&thread_pool.lambda_int_int_array[i]));
        }

        for (int i = 0; i < numThreads; i++)
        {
            thread_pool.join_thread(thread_pool.thread_id[i]);
        }

        printf("StaMp Statistics: Threads = %d, Parallel execution time = %f seconds\n", numThreads, (double)(clock() - start) / CLOCKS_PER_SEC);
    }
} // namespace stamp