#ifndef PLATFORM_MEMORY_MANAGER_H
#define PLATFORM_MEMORY_MANAGER_H

#include <cstddef>
#include <memory>

namespace platform {

// 内存管理工具类
class MemoryManager {
public:
    // 获取单例实例
    static std::shared_ptr<MemoryManager> instance();

    // 分配对齐的内存块（用于音频处理）
    void* allocate_aligned(size_t size, size_t alignment = 32);

    // 释放对齐的内存块
    void deallocate_aligned(void* ptr);

    // 创建内存池管理器
    class MemoryPool {
    public:
        explicit MemoryPool(size_t block_size, size_t num_blocks);
        ~MemoryPool();

        // 分配一块内存
        void* allocate();

        // 释放一块内存
        void deallocate(void* ptr);

        // 获取块大小
        size_t get_block_size() const { return block_size_; }

        // 获取总块数
        size_t get_total_blocks() const { return num_blocks_; }

    private:
        size_t block_size_;
        size_t num_blocks_;
        void** free_list_;
        char* pool_memory_;
    };

private:
    MemoryManager() = default;
    ~MemoryManager() = default;
};

} // namespace platform

#endif // PLATFORM_MEMORY_MANAGER_H