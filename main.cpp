#include <thread>
#include <vector>
#include <atomic>
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif

#include <unistd.h>

int generalGetPageSize() {
#ifdef _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwPageSize;
#else
    return getpagesize();
#endif
}

void* generalAllocatePage(int pageSize) {
#ifdef _WIN32
    void *memAddr = VirtualAlloc(nullptr, pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
    void *memAddr = mmap(nullptr, pageSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif

    return memAddr;
}

int getCacheLineSize() {
    // Hard-coded because it's really troublesome to get L1 cache line size on Windows
    return 64;
}

void worker(int index, int pageSize) {
    void *memAddr = generalAllocatePage(pageSize);

    // Ensure that the memory address can cross cache lines
    if (std::uint32_t remainder = (std::uint32_t)reinterpret_cast<uintptr_t>(memAddr) % pageSize; remainder != 0) {
        std::printf("[#%d] ERROR: memAddr %% pageSize(%d) = %u != 0 which indicates that allocated memory is not aligned to a page. Exit.", index, pageSize, remainder);
        std::exit(1);
    }

    std::printf("[#%d] Running split-lock instructions...\n", index);

    uint32_t num = static_cast<uint32_t>(index);
    uint32_t *ptr = reinterpret_cast<uint32_t *>((std::uintptr_t) memAddr + getCacheLineSize() - 2);

    while (true) {
        num = __atomic_exchange_n(ptr, num, __ATOMIC_SEQ_CST);
        num = __atomic_exchange_n(ptr, num, __ATOMIC_SEQ_CST);
        num = __atomic_exchange_n(ptr, num, __ATOMIC_SEQ_CST);
        num = __atomic_exchange_n(ptr, num, __ATOMIC_SEQ_CST);
        num = __atomic_exchange_n(ptr, num, __ATOMIC_SEQ_CST);
        num = __atomic_exchange_n(ptr, num, __ATOMIC_SEQ_CST);
        num = __atomic_exchange_n(ptr, num, __ATOMIC_SEQ_CST);
        num = __atomic_exchange_n(ptr, num, __ATOMIC_SEQ_CST);
        num = __atomic_exchange_n(ptr, num, __ATOMIC_SEQ_CST);
        num = __atomic_exchange_n(ptr, num, __ATOMIC_SEQ_CST);
    }
}

int main(int argc, char* argv[]) {
    int opt;
    const char* optString = "t:";

    int numThreads = -1;
    while ((opt = getopt(argc, argv, optString)) != -1) {
        switch (opt) {
            case 't':
                numThreads = std::atoi(optarg);
                break;
            default:
                std::printf("Usage: %s [-t numThreads]\n", argv[0]);
                std::exit(1);
        }
    }

    if (numThreads <= 0) {
        std::printf("Number of threads must be specified\nUsage: %s [-t numThreads]\n", argv[0]);
        std::exit(1);
    }

    std::vector<std::thread> threads;
    int pageSize = generalGetPageSize();

    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread(worker, i + 1, pageSize));
    }

    for (auto &thread: threads) {
        thread.join();
    }
}
