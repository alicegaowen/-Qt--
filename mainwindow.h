#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<newwindow.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QTimer>
#include <QFrame>
#include <QRect>
#include <QSpinBox>
#include <QLayout>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QStringListModel>
#include <QModelIndex>
#include  <map>
#include <typeinfo>
#include <QString>
QT_BEGIN_NAMESPACE
using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QImage MatImageToQt(const Mat &src);


    ~MainWindow();
private slots:
    void itemClick(QModelIndex index);
    void readFarme();
    //void readall();
    void on_Open_triggered();
    void on_Stop_triggered();
    void on_play_clicked();


    //void on_pushButton_2_clicked();
    //void keyPressEvent(QKeyEvent *event);
    //void valueChanged();

//     void onPlayerDurationChanged(qint64 duration);
//     void onPlayerPositionChanged(qint64 position);  //
//     void onSliderMoved(int value);



    void on_slider_progress_sliderMoved(int position);

    void on_slider_progress_valueChanged(int value);

    void on_slider_progress_actionTriggered(int action);

    void on_slider_progress_sliderPressed();

    void on_slider_progress_sliderReleased();

    void on_pushButton_clicked();

  //  void on_pushButton_6_clicked();



    void on_FullScreen_clicked();
   // void createToolButton();

    //void on_graphicsView_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint);

    void on_MainWindow_iconSizeChanged(const QSize &iconSize);
    void ChangeButton();

//    void on_pushButton_2_clicked();
//    void initMenu();
//    void menuSlot();



    //void on_toolButton_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    VideoCapture cap;
    Mat src_image;
    Mat frame;
    QTimer *timer;
    QImage *image;
    int allframes;
    int nowframe;
    int max_range;
    bool is_opened;
    bool is_fullscreen;//是否全屏
    bool is_play;//是否播放
    QStringListModel *Model;
    QStringList strlist;
   // QStringList strlist;
     map<QString, QString> mymap;

     QFileInfo file_name;
      QString directory;
     //QString directory = QFileDialog::getOpenFileName(this,tr("选择文件"),"/","(*.mp4);;All files(*.*)");
    // return;
     //文件名
     //QFileInfo file_name(directory);
    // QString directory = QFileDialog::getOpenFileName(this,tr("选择文件"),"/","(*.mp4);;All files(*.*)");
    // return;
     //文件名
    // QFileInfo file_name;

    //test

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    // int value;
    void resizeEvent(QResizeEvent *event);
    void enterEvent(QEvent *e);                      //进入QWidget瞬间事件

    void leaveEvent(QEvent *e);                      //离开QWidget瞬间事件
    void mouseMoveEvent(QMouseEvent *ev);
};

#endif // MAINWINDOW_H
