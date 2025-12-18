#include "theme_manager.h"
#include <QApplication>
#include <QPalette>

namespace gui {

ThemeManager::ThemeManager() : current_theme_(ThemeType::Light) {
    initializeDefaultThemes();
}

ThemeManager::~ThemeManager() = default;

void ThemeManager::setTheme(ThemeType theme) {
    current_theme_ = theme;
}

ThemeManager::ThemeType ThemeManager::getCurrentTheme() const {
    return current_theme_;
}

void ThemeManager::applyTheme() {
    QPalette palette;
    
    switch (current_theme_) {
        case ThemeType::Light:
            // 浅色主题
            palette.setColor(QPalette::Window, QColor(240, 240, 240));
            palette.setColor(QPalette::WindowText, Qt::black);
            palette.setColor(QPalette::Base, Qt::white);
            palette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
            palette.setColor(QPalette::ToolTipBase, Qt::white);
            palette.setColor(QPalette::ToolTipText, Qt::black);
            palette.setColor(QPalette::Text, Qt::black);
            palette.setColor(QPalette::Button, QColor(240, 240, 240));
            palette.setColor(QPalette::ButtonText, Qt::black);
            palette.setColor(QPalette::BrightText, Qt::red);
            palette.setColor(QPalette::Link, QColor(0, 0, 255));
            palette.setColor(QPalette::Highlight, QColor(0, 120, 215));
            palette.setColor(QPalette::HighlightedText, Qt::white);
            break;
            
        case ThemeType::Dark:
            // 深色主题
            palette.setColor(QPalette::Window, QColor(30, 30, 30));
            palette.setColor(QPalette::WindowText, Qt::white);
            palette.setColor(QPalette::Base, QColor(45, 45, 45));
            palette.setColor(QPalette::AlternateBase, QColor(30, 30, 30));
            palette.setColor(QPalette::ToolTipBase, Qt::white);
            palette.setColor(QPalette::ToolTipText, Qt::black);
            palette.setColor(QPalette::Text, Qt::white);
            palette.setColor(QPalette::Button, QColor(45, 45, 45));
            palette.setColor(QPalette::ButtonText, Qt::white);
            palette.setColor(QPalette::BrightText, Qt::red);
            palette.setColor(QPalette::Link, QColor(0, 120, 215));
            palette.setColor(QPalette::Highlight, QColor(0, 120, 215));
            palette.setColor(QPalette::HighlightedText, Qt::white);
            break;
            
        case ThemeType::Custom:
            // 自定义主题
            palette.setColor(QPalette::Window, custom_colors_.value("background", Qt::white));
            palette.setColor(QPalette::WindowText, custom_colors_.value("foreground", Qt::black));
            palette.setColor(QPalette::Base, custom_colors_.value("base", Qt::lightGray));
            palette.setColor(QPalette::AlternateBase, custom_colors_.value("alternateBase", Qt::gray));
            palette.setColor(QPalette::ToolTipBase, Qt::white);
            palette.setColor(QPalette::ToolTipText, Qt::black);
            palette.setColor(QPalette::Text, custom_colors_.value("text", Qt::black));
            palette.setColor(QPalette::Button, custom_colors_.value("button", Qt::lightGray));
            palette.setColor(QPalette::ButtonText, custom_colors_.value("buttonText", Qt::black));
            palette.setColor(QPalette::BrightText, Qt::red);
            palette.setColor(QPalette::Link, QColor(0, 0, 255));
            palette.setColor(QPalette::Highlight, QColor(0, 120, 215));
            palette.setColor(QPalette::HighlightedText, Qt::white);
            break;
    }
    
    QApplication::setPalette(palette);
}

void ThemeManager::setCustomTheme(const QColor& background, const QColor& foreground, const QColor& accent) {
    custom_colors_["background"] = background;
    custom_colors_["foreground"] = foreground;
    custom_colors_["accent"] = accent;
    current_theme_ = ThemeType::Custom;
}

QColor ThemeManager::getBackgroundColor() const {
    return getColorForTheme(current_theme_, "background");
}

QColor ThemeManager::getForegroundColor() const {
    return getColorForTheme(current_theme_, "foreground");
}

QColor ThemeManager::getAccentColor() const {
    return getColorForTheme(current_theme_, "accent");
}

void ThemeManager::initializeDefaultThemes() {
    // 初始化默认主题颜色
    custom_colors_["light_background"] = QColor(240, 240, 240);
    custom_colors_["light_foreground"] = Qt::black;
    custom_colors_["light_accent"] = QColor(0, 120, 215);
    
    custom_colors_["dark_background"] = QColor(30, 30, 30);
    custom_colors_["dark_foreground"] = Qt::white;
    custom_colors_["dark_accent"] = QColor(0, 120, 215);
}

QColor ThemeManager::getColorForTheme(ThemeType theme, const QString& color_name) const {
    switch (theme) {
        case ThemeType::Light:
            return custom_colors_.value("light_" + color_name, Qt::white);
        case ThemeType::Dark:
            return custom_colors_.value("dark_" + color_name, Qt::black);
        case ThemeType::Custom:
            return custom_colors_.value(color_name, Qt::black);
    }
    return Qt::black;
}

} // namespace gui