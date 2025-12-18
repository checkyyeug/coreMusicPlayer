#include "platform/memory_manager.h"
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/mman.h>
    #include <unistd.h>
#endif

namespace platform {

std::shared_ptr<MemoryManager> MemoryManager::instance() {
    static std::shared_ptr<MemoryManager> manager =
        std::make_shared<MemoryManager>();
    return manager;
}

void* MemoryManager::allocate_aligned(size_t size, size_t alignment) {
#ifdef _WIN32
    // Windows平台使用 _aligned_malloc
    return _aligned_malloc(size, alignment);
#else
    // POSIX系统使用 memalign 或 posix_memalign
    void* ptr = nullptr;
    if (posix_memalign(&ptr, alignment, size) != 0) {
        return nullptr;
    }
    return ptr;
#endif
}

void MemoryManager::deallocate_aligned(void* ptr) {
#ifdef _WIN32
    // Windows平台使用 _aligned_free
    _aligned_free(ptr);
#else
    // POSIX系统使用 free
    free(ptr);
#endif
}

MemoryManager::MemoryPool::MemoryPool(size_t block_size, size_t num_blocks)
    : block_size_(block_size), num_blocks_(num_blocks) {

    // 分配内存池
    pool_memory_ = static_cast<char*>(allocate_aligned(num_blocks * block_size, 32));
    if (!pool_memory_) {
        return;
    }

    // 初始化空闲列表
    free_list_ = new void*[num_blocks];
    for (size_t i = 0; i < num_blocks; ++i) {
        free_list_[i] = pool_memory_ + i * block_size_;
    }
}

MemoryManager::MemoryPool::~MemoryPool() {
    if (pool_memory_) {
        deallocate_aligned(pool_memory_);
    }
    delete[] free_list_;
}

void* MemoryManager::MemoryPool::allocate() {
    if (!free_list_ || num_blocks_ == 0) {
        return nullptr;
    }

    // 简单的FIFO分配策略
    void** block = static_cast<void**>(free_list_[num_blocks_ - 1]);
    --num_blocks_;
    return block;
}

void MemoryManager::MemoryPool::deallocate(void* ptr) {
    if (!ptr || !free_list_) {
        return;
    }

    // 简单的FIFO释放策略
    free_list_[num_blocks_] = ptr;
    ++num_blocks_;
}

} // namespace platform