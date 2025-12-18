#ifndef CORE_AUDIO_PLAYLIST_H
#define CORE_AUDIO_PLAYLIST_H

#include "core/audio_buffer.h"
#include <string>
#include <vector>
#include <memory>

namespace core {

// 音频播放列表类
class AudioPlaylist {
public:
    // 构造函数
    AudioPlaylist();
    
    // 析构函数
    ~AudioPlaylist();
    
    // 初始化播放列表
    bool initialize();
    
    // 关闭播放列表
    void shutdown();
    
    // 添加音频文件到播放列表
    bool addFile(const std::string& filename);
    
    // 从播放列表中移除音频文件
    bool removeFile(const std::string& filename);
    
    // 清空播放列表
    void clear();
    
    // 获取播放列表中的文件数量
    size_t getFileCount() const;
    
    // 获取指定位置的音频文件名
    std::string getFileName(size_t index) const;
    
    // 获取当前播放的文件索引
    size_t getCurrentIndex() const;
    
    // 设置当前播放的文件索引
    bool setCurrentIndex(size_t index);
    
    // 播放下一个文件
    bool next();
    
    // 播放上一个文件
    bool previous();
    
    // 获取播放列表状态
    void getStatus(std::vector<std::string>& files, 
                   size_t& current_index, 
                   size_t& file_count) const;
    
private:
    // 私有成员变量
    bool initialized_;
    std::vector<std::string> files_;
    size_t current_index_;
};

} // namespace core

#endif // CORE_AUDIO_PLAYLIST_H