#ifndef CORE_AUDIO_METADATA_H
#define CORE_AUDIO_METADATA_H

#include <string>
#include <map>
#include <memory>

namespace core {

// 音频元数据类
class AudioMetadata {
public:
    // 构造函数
    AudioMetadata();
    
    // 析构函数
    ~AudioMetadata();
    
    // 初始化元数据处理器
    bool initialize();
    
    // 关闭元数据处理器
    void shutdown();
    
    // 从文件读取元数据
    bool readFromFile(const std::string& filename);
    
    // 写入元数据到文件
    bool writeToFile(const std::string& filename);
    
    // 获取标题
    std::string getTitle() const;
    
    // 设置标题
    bool setTitle(const std::string& title);
    
    // 获取艺术家
    std::string getArtist() const;
    
    // 设置艺术家
    bool setArtist(const std::string& artist);
    
    // 获取专辑
    std::string getAlbum() const;
    
    // 设置专辑
    bool setAlbum(const std::string& album);
    
    // 获取年份
    std::string getYear() const;
    
    // 设置年份
    bool setYear(const std::string& year);
    
    // 获取流派
    std::string getGenre() const;
    
    // 设置流派
    bool setGenre(const std::string& genre);
    
    // 获取时长
    double getDuration() const;
    
    // 设置时长
    bool setDuration(double duration);
    
    // 获取所有元数据
    std::map<std::string, std::string> getAllMetadata() const;
    
    // 设置所有元数据
    bool setAllMetadata(const std::map<std::string, std::string>& metadata);
    
private:
    // 私有成员变量
    bool initialized_;
    std::map<std::string, std::string> metadata_;
    double duration_;
};

} // namespace core

#endif // CORE_AUDIO_METADATA_H