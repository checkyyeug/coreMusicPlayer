#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
QT_END_NAMESPACE

/**
 * @brief 主窗口类，用于应用程序的主界面
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~MainWindow();

private slots:
    /**
     * @brief 打开文件
     */
    void openFile();

    /**
     * @brief 保存文件
     */
    void saveFile();

    /**
     * @brief 播放音乐
     */
    void playMusic();

    /**
     * @brief 暂停音乐
     */
    void pauseMusic();

    /**
     * @brief 停止音乐
     */
    void stopMusic();

private:
    /**
     * @brief 初始化菜单栏
     */
    void createMenus();

    /**
     * @brief 初始化工具栏
     */
    void createToolBars();

    /**
     * @brief 初始化状态栏
     */
    void createStatusBar();

    /**
     * @brief 初始化主窗口布局
     */
    void setupLayout();

    /// 文件菜单
    QMenu* fileMenu_;
    
    /// 编辑菜单
    QMenu* editMenu_;
    
    /// 视图菜单
    QMenu* viewMenu_;
    
    /// 帮助菜单
    QMenu* helpMenu_;
    
    /// 文件工具栏
    QToolBar* fileToolBar_;
    
    /// 播放工具栏
    QToolBar* playToolBar_;
    
    /// 状态栏
    QStatusBar* statusBar_;
    
    /// 主窗口中央部件
    QWidget* centralWidget_;
    
    /// 布局管理器
    QVBoxLayout* layout_;
};

#endif // GUI_MAIN_WINDOW_H