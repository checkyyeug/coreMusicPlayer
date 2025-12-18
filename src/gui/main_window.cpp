#include "main_window.h"
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      fileMenu_(nullptr),
      editMenu_(nullptr),
      viewMenu_(nullptr),
      helpMenu_(nullptr),
      fileToolBar_(nullptr),
      playToolBar_(nullptr),
      statusBar_(nullptr),
      centralWidget_(nullptr),
      layout_(nullptr) {
    
    // 设置窗口标题
    setWindowTitle(tr("coreMusicPlayer"));
    
    // 初始化界面组件
    createMenus();
    createToolBars();
    createStatusBar();
    setupLayout();
    
    // 状态栏显示初始信息
    statusBar_->showMessage(tr("Ready"));
}

MainWindow::~MainWindow() = default;

void MainWindow::createMenus() {
    // 创建文件菜单
    fileMenu_ = menuBar()->addMenu(tr("&File"));
    fileMenu_->addAction(tr("&Open"), this, &MainWindow::openFile);
    fileMenu_->addAction(tr("&Save"), this, &MainWindow::saveFile);
    fileMenu_->addSeparator();
    fileMenu_->addAction(tr("E&xit"), qApp, &QApplication::quit);
    
    // 创建编辑菜单
    editMenu_ = menuBar()->addMenu(tr("&Edit"));
    editMenu_->addAction(tr("&Copy"), this, &MainWindow::openFile);
    editMenu_->addAction(tr("&Paste"), this, &MainWindow::saveFile);
    
    // 创建视图菜单
    viewMenu_ = menuBar()->addMenu(tr("&View"));
    viewMenu_->addAction(tr("&Fullscreen"), this, &MainWindow::openFile);
    
    // 创建帮助菜单
    helpMenu_ = menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(tr("&About"), this, &MainWindow::saveFile);
}

void MainWindow::createToolBars() {
    // 创建文件工具栏
    fileToolBar_ = addToolBar(tr("File"));
    fileToolBar_->addAction(tr("Open"), this, &MainWindow::openFile);
    fileToolBar_->addAction(tr("Save"), this, &MainWindow::saveFile);
    
    // 创建播放工具栏
    playToolBar_ = addToolBar(tr("Playback"));
    playToolBar_->addAction(tr("Play"), this, &MainWindow::playMusic);
    playToolBar_->addAction(tr("Pause"), this, &MainWindow::pauseMusic);
    playToolBar_->addAction(tr("Stop"), this, &MainWindow::stopMusic);
}

void MainWindow::createStatusBar() {
    statusBar_ = statusBar();
}

void MainWindow::setupLayout() {
    // 创建中央部件
    centralWidget_ = new QWidget(this);
    setCentralWidget(centralWidget_);
    
    // 创建布局管理器
    layout_ = new QVBoxLayout(centralWidget_);
    
    // 添加一些示例内容
    QLabel* label = new QLabel(tr("coreMusicPlayer GUI Interface"), centralWidget_);
    label->setAlignment(Qt::AlignCenter);
    layout_->addWidget(label);
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), 
                                                   "", tr("Audio Files (*.mp3 *.wav *.flac)"));
    if (!fileName.isEmpty()) {
        statusBar_->showMessage(tr("Opened file: %1").arg(fileName));
    }
}

void MainWindow::saveFile() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), 
                                                   "", tr("Audio Files (*.mp3 *.wav *.flac)"));
    if (!fileName.isEmpty()) {
        statusBar_->showMessage(tr("Saved file: %1").arg(fileName));
    }
}

void MainWindow::playMusic() {
    statusBar_->showMessage(tr("Playing music..."));
}

void MainWindow::pauseMusic() {
    statusBar_->showMessage(tr("Paused music"));
}

void MainWindow::stopMusic() {
    statusBar_->showMessage(tr("Stopped music"));
}