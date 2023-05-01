#include "stamp.h"

/// @brief
namespace stamp
{

    /// @brief
    /// @param context
    /// @return
    void *thread_stub(void *context)
    {
        // context is static_cast<void*>(&f) below. We reverse the cast to
        // void* and call the std::function object.
        std::function<void *()> &func = *static_cast<std::function<void *()> *>(context);
        return func();
    }

    /// @brief
    /// @param lambda1
    /// @param lambda2
    void execute_tuple(std::function<void()> &&lambda1, std::function<void()> &&lambda2)
    {

        clock_t start = clock();

        pthread_t thread_id1, thread_id2;

        pthread_create(&thread_id1, NULL, thread_stub, static_cast<void *>(&lambda1));
        pthread_create(&thread_id2, NULL, thread_stub, static_cast<void *>(&lambda2));
        pthread_join(thread_id1, NULL);
        pthread_join(thread_id2, NULL);

        printf("StaMp Statistics: Threads = 2, Parallel execution time = %f seconds\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    }

    /// @brief
    /// @param low
    /// @param high
    /// @param stride
    /// @param lambda
    /// @param numThreads
    void parallel_for(int low, int high, int stride, std::function<void(int)> &&lambda, int numThreads)
    {
        clock_t start = clock();

        std::vector<pthread_t> thread_id(numThreads);

        for (int i = 0; i < numThreads; i++)
        {
            pthread_create(&thread_id[i], NULL, thread_stub, static_cast<void *>(&lambda));
        }
        for (int i = 0; i < numThreads; i++)
        {
            pthread_join(thread_id[i], NULL);
        }

        for (int i = low; i < high; i += stride)
        {
            lambda(i);
        }

        printf("StaMp Statistics: Threads = %d, Parallel execution time = %f seconds\n", numThreads, (double)(clock() - start) / CLOCKS_PER_SEC);
    }

    /// @brief
    /// @param high
    /// @param lambda
    /// @param numThreads
    void parallel_for(int high, std::function<void(int)> &&lambda, int numThreads)
    {
        clock_t start = clock();
        std::vector<pthread_t> thread_id(numThreads);

        for (int i = 0; i < numThreads; i++)
        {
            pthread_create(&thread_id[i], NULL, thread_stub, static_cast<void *>(&lambda));
        }
        for (int i = 0; i < numThreads; i++)
        {
            pthread_join(thread_id[i], NULL);
        }

        for (int i = 0; i < high; i++)
        {
            lambda(i);
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

        clock_t start = clock();
        std::vector<pthread_t> thread_id(numThreads);

        for (int i = 0; i < numThreads; i++)
        {
            pthread_create(&thread_id[i], NULL, thread_stub, static_cast<void *>(&lambda));
        }
        for (int i = 0; i < numThreads; i++)
        {
            pthread_join(thread_id[i], NULL);
        }

        for (int i = low1; i < high1; i += stride1)
        {
            for (int j = low2; j < high2; j += stride2)
            {
                lambda(i, j);
            }
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
        clock_t start = clock();
        std::vector<pthread_t> thread_id(numThreads);

        for (int i = 0; i < numThreads; i++)
        {
            pthread_create(&thread_id[i], NULL, thread_stub, static_cast<void *>(&lambda));
        }
        for (int i = 0; i < numThreads; i++)
        {
            pthread_join(thread_id[i], NULL);
        }

        for (int i = 0; i < high1; i++)
        {
            for (int j = 0; j < high2; j++)
            {
                lambda(i, j);
            }
        }

        printf("StaMp Statistics: Threads = %d, Parallel execution time = %f seconds\n", numThreads, (double)(clock() - start) / CLOCKS_PER_SEC);
    }
} // namespace stamp