#ifndef GUI_THEME_MANAGER_H
#define GUI_THEME_MANAGER_H

// 临时注释掉Qt相关头文件，因为可能缺少Qt依赖
// #include <QString>
// #include <QMap>
// #include <QColor>
// #include <QPalette>
// #include <QApplication>

namespace gui {

/**
 * @brief 主题管理器类，用于管理应用程序的主题
 */
class ThemeManager {
public:
    /**
     * @brief 主题类型枚举
     */
    enum class ThemeType {
        Light,   ///< 浅色主题
        Dark,    ///< 深色主题
        Custom   ///< 自定义主题
    };

    /**
     * @brief 构造函数
     */
    ThemeManager();

    /**
     * @brief 析构函数
     */
    ~ThemeManager();

    /**
     * @brief 设置当前主题
     * @param theme 主题类型
     */
    void setTheme(ThemeType theme);

    /**
     * @brief 获取当前主题类型
     * @return 当前主题类型
     */
    ThemeType getCurrentTheme() const;

    /**
     * @brief 应用当前主题到应用程序
     */
    void applyTheme();

    /**
     * @brief 设置自定义主题颜色
     * @param background 背景颜色
     * @param foreground 前景色
     * @param accent 强调色
     */
    void setCustomTheme(const void* background, const void* foreground, const void* accent);

    /**
     * @brief 获取当前主题的背景颜色
     * @return 背景颜色
     */
    void* getBackgroundColor() const;

    /**
     * @brief 获取当前主题的前景颜色
     * @return 前景色
     */
    void* getForegroundColor() const;

    /**
     * @brief 获取当前主题的强调色
     * @return 强调色
     */
    void* getAccentColor() const;

private:
    /// 当前主题类型
    ThemeType current_theme_;

    /// 自定义主题颜色映射
    void* custom_colors_;

    /// 浅色主题名称
    static constexpr const char* LIGHT_THEME_NAME = "light";

    /// 深色主题名称
    static constexpr const char* DARK_THEME_NAME = "dark";

    /// 默认背景颜色
    static constexpr const char* DEFAULT_BACKGROUND = "#ffffff";

    /// 默认前景颜色
    static constexpr const char* DEFAULT_FOREGROUND = "#000000";

    /// 默认强调色
    static constexpr const char* DEFAULT_ACCENT = "#007acc";

    /**
     * @brief 初始化默认主题
     */
    void initializeDefaultThemes();

    /**
     * @brief 根据主题类型获取颜色
     * @param theme 主题类型
     * @param color_name 颜色名称
     * @return 颜色值
     */
    void* getColorForTheme(ThemeType theme, const char* color_name) const;
};

} // namespace gui

#endif // GUI_THEME_MANAGER_H