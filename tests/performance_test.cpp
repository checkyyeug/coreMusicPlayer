#include <gtest/gtest.h>
#include "utils/performance_counter.h"

// 测试性能计数器功能
TEST(PerformanceCounterTest, CounterCreation) {
    // 创建性能计数器实例
    auto counter = utils::PerformanceCounter::instance();
    
    // 验证计数器可以创建
    EXPECT_NE(counter, nullptr);
}

TEST(PerformanceCounterTest, TimerFunctionality) {
    // 创建性能计数器实例
    auto counter = utils::PerformanceCounter::instance();
    
    // 重置计数器
    counter->reset();
    
    // 开始计时
    counter->start_timer("test_timer");
    
    // 模拟一些工作
    volatile int sum = 0;
    for (int i = 0; i < 1000; ++i) {
        sum += i;
    }
    
    // 停止计时
    counter->stop_timer("test_timer");
    
    // 获取耗时
    double elapsed = counter->get_elapsed_ms("test_timer");
    
    // 验证耗时大于0
    EXPECT_GT(elapsed, 0.0);
}

TEST(PerformanceCounterTest, MultipleTimers) {
    // 创建性能计数器实例
    auto counter = utils::PerformanceCounter::instance();
    
    // 重置计数器
    counter->reset();
    
    // 开始多个计时器
    counter->start_timer("timer1");
    counter->start_timer("timer2");
    
    // 模拟工作
    volatile int sum1 = 0;
    for (int i = 0; i < 500; ++i) {
        sum1 += i;
    }
    
    counter->stop_timer("timer1");
    
    volatile int sum2 = 0;
    for (int i = 0; i < 1000; ++i) {
        sum2 += i;
    }
    
    counter->stop_timer("timer2");
    
    // 获取耗时
    double elapsed1 = counter->get_elapsed_ms("timer1");
    double elapsed2 = counter->get_elapsed_ms("timer2");
    
    // 验证两个计时器都记录了时间
    EXPECT_GT(elapsed1, 0.0);
    EXPECT_GT(elapsed2, 0.0);
}

TEST(PerformanceCounterTest, ReportGeneration) {
    // 创建性能计数器实例
    auto counter = utils::PerformanceCounter::instance();
    
    // 重置计数器
    counter->reset();
    
    // 开始计时
    counter->start_timer("report_test");
    
    // 模拟一些工作
    volatile int sum = 0;
    for (int i = 0; i < 100; ++i) {
        sum += i;
    }
    
    // 停止计时
    counter->stop_timer("report_test");
    
    // 验证报告生成不会崩溃
    // 注意：这里我们只测试函数调用，不验证输出内容
    counter->print_report();
    
    // 验证可以获取计时结果
    double elapsed = counter->get_elapsed_ms("report_test");
    EXPECT_GT(elapsed, 0.0);
}