#ifndef CORE_MULTI_FORMAT_STRATEGY_H
#define CORE_MULTI_FORMAT_STRATEGY_H

#include "core/player_strategy.h"
#include <string>
#include <vector>

namespace core {

class MultiFormatStrategy : public Strategy {
public:
    MultiFormatStrategy();
    ~MultiFormatStrategy() override = default;

    bool initialize() override;
    bool play() override;
    bool stop() override;
    bool pause() override;
    bool set_volume(float volume) override;
    std::string get_status() const override;
    long long get_position_ms() const override;
    bool seek(long long position_ms) override;
    bool set_playlist(const std::vector<std::string>& playlist) override;
    std::string get_current_track() const override;
    std::string get_name() const override;

private:
    // 多格式策略特有的成员变量
    std::string current_format_;
    bool auto_detect_format_;
};

} // namespace core

#endif // CORE_MULTI_FORMAT_STRATEGY_H