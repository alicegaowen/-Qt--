#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QTimer>
#include <QFileDialog>
//#include <QKeyEvent>
#include <QMouseEvent>
#include <QFrame>
#include<QApplication>

#include <QResizeEvent>
//#include "keepratiowidget.h"

using namespace cv;
using namespace std;

//int pos = 0;
//VideoCapture cap;
//int allframes = 0;
int percent = 0;

//int allframes = cap.get(CAP_PROP_FRAME_COUNT);
//**************************



//**************************
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) ,
    is_play(false)
{
   // this->setStyleSheet("QMainWindow {background-image:url(:/home/gaowenli/Desktop/bi.jpg)}");
    ui->setupUi(this);
    //createToolButton();
    //this->setMouseTracking(true);
    this->centralWidget()->setMouseTracking(true);
    this->setMouseTracking(true);
    ui->label->setMouseTracking(true);

   // setMouseTracking(true);
    ui->slider_progress->installEventFilter(this);
    //实例化定时器
    timer = new QTimer(this);
    //实例化图像
    image = new QImage();
    //将定时器与读取帧连接起来
    connect(timer,SIGNAL(timeout()),this,SLOT(readFarme()));
   // connect(timer,SIGNAL(timeout()),this,SLOT(readall()));
    //设置进度条的范围
    max_range = 1000;
    ui->slider_progress->setRange(0,max_range);
  //  this->grabKeyboard();
   //connect(ui->slider_progress,SIGNAL(sliderMoved(int)),this,SLOT(onSliderMoved(int)));
    is_fullscreen = false;
    is_play = false;
   // ui->play->setFlat(true);
    ui->play->setText("");
    ui->play->setFlat(true);

    QPixmap pixmap("/home/gaowenli/Desktop/stop.png");
    QPixmap fitpixmap = pixmap.scaled(40,40,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->play->setIcon(QIcon(fitpixmap));
    ui->play->setIconSize(QSize(40,40));

     //ui->play->setStyleSheet("background-image:url(/home/gaowenli/Desktop/stop.png)");

     ui->FullScreen->setFlat(true);
   //  ui->listView->colorCount();
    // ui->FullScreen->setStyleSheet("background-image:url(/home/gaowenli/Desktop/full.png)");

   // button->setFlat(true);
     ui->FullScreen->setText("");
     QPixmap pixmap1("/home/gaowenli/Desktop/full.png");
     QPixmap fitpixmap1 = pixmap1.scaled(40,40,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
     ui->FullScreen->setIcon(QIcon(fitpixmap1));
     ui->FullScreen->setIconSize(QSize(40,40));
      //createToolButton();

    ui->comboBox->addItem(tr("0.5x"));
    ui->comboBox->addItem(tr("1.0x"));
    ui->comboBox->addItem(tr("1.25x"));
    ui->comboBox->addItem(tr("1.5x"));
    ui->comboBox->addItem(tr("2.0x"));
    ui->comboBox->setCurrentIndex(1);

    this->setGeometry(480,270,960,540);
    /********************************/
    Model = new QStringListModel(strlist);//QStringListModel将QStringList包装了起来
    ui->listView->setModel(Model);        //填充到listview
    //设置item内容不可编辑
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->listView->setStyleSheet(
                "QListView{border:0px;}"
                "QListView::item:hover{"
                    "background:rgb(100,149,237);color:rgb(255,255,255);"
                "}"
                "QListView::item:selected{"
                    "background:rgb(65,105,225);color:rgb(255,255,255);"
                "}");
    //建立信号和槽，点击某个item时触发执行itemClick函数
    connect(ui->listView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(itemClick(QModelIndex)));


//    allframes = cap.get(CAP_PROP_FRAME_COUNT);
//    on_play_clicked();


//    if(file_name.completeBaseName() == "gwl")
//    {

//        cout<<"it"<<it.toStdString()<<endl;
//        //cout<<"chuji"<<mymap.find(directory)->first()<<endl;
//    }
//    QString directory = QFileDialog::getOpenFileName(this,tr("选择文件"),"/","(*.mp4);;All files(*.*)");
//    QFileInfo file_name(directory);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::itemClick(QModelIndex index)
{



    QString str;
    str = index.data().toString();//获取当前点击项数据
     //mymap.insert(pair<QString, QString>(str, directory));


    qDebug()<<"str:"<<str;

    QString it = mymap.find(str)->second;
    cap.open(it.toStdString());

    nowframe = 0;
    allframes = cap.get(CAP_PROP_FRAME_COUNT);



}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    //解决QSlider点击不能到鼠标指定位置的问题
    if(obj==ui->slider_progress)
    {
        if (event->type()==QEvent::MouseButtonPress)           //判断类型
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)	//判断左键
            {
               int value = QStyle::sliderValueFromPosition(ui->slider_progress->minimum(), ui->slider_progress->maximum(), mouseEvent->pos().x(), ui->slider_progress->width());
               ui->slider_progress->setValue(value);
            }
        }
    }
    return QObject::eventFilter(obj,event);
}

void MainWindow::on_Open_triggered()
{
   //开启定时器，start里的数字越大，视频播放的越缓慢，可以从这儿入手视频快进慢放的功能
    timer->start(0);

}
void MainWindow::on_Stop_triggered()
{
    // 停止读取数据。
    timer->stop();
   // cap.release();没啥用


}
//这个应该是用来读取帧的
void MainWindow::readFarme()
{
    //cap.open("/home/gaowenli/Desktop/Video.mp4");
    //在视频打开的前提下读取
    if(cap.isOpened())
    {
        cap.read(src_image);
         double fps = cap.get(CAP_PROP_FPS);
         //timer->start(1000/(fps*1.0));

        Size dsize = Size(ui->label->width(), ui->label->height());


//        if(is_fullscreen)
//        {
//           // (width()/((height()*19)/20-15))
//           cv::resize(src_image,src_image,Size(width()*(width()/((height()*19)/20-15)),height()*(width()/((height()*19)/20-15))));
//            //ui->horizontalLayout->setGeometry(QRect(10,900,1900,30));
//            //ui->label->setGeometry(0,0,1920,1080);//前两个参数表示label左上角位置后面分别是宽和高
//        }
//        else
//        {
//            cv::resize(src_image,src_image,Size(width(),(height()*19)/20-15));
//           // ui->label->setGeometry(0,0,400,300);
//            cout<<"kuan:"<<ui->label->width()<<endl;
//        }

        Mat shrink;
        // cv::resize(src_image,src_image,Size(width(),(height()*19)/20-15));
        //cv::resize(src_image, src_image, dsize, 0, 0, INTER_AREA);

       QImage imag = MatImageToQt(src_image);
       ui->label->setPixmap(QPixmap::fromImage(imag.scaled(ui->label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation)));
       //imag.scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio);
        //将QImage的大小收缩或拉伸，与label的大小保持一致。这样label中能显示完整的图片
//        QImage imageScale = imag.scaled(QSize(ui->label->width(),
//        ui->label->height()));
//        QPixmap pixmap = QPixmap::fromImage(imageScale);
//        ui->label->setPixmap(pixmap);


      //  ui->label->setGeometry(0,0,imag.width(),imag.height());

       // ui->label->setPixmap(QPixmap::fromImage(imag));
        nowframe = int(round(cap.get(1)));//当前读取到的第几帧
        //cout<< ui->slider_progress->maximumWidth()<<endl;
        //cout<<this->width()<<endl;

        int percent = (nowframe*1.0/allframes)*max_range;
         ui->slider_progress->setValue(percent);//参数是百分比
        if(is_fullscreen)
        {

           // ui->horizontalLayout->setGeometry(QRect(10,10,1900,30));
        }
//        ui->slider_progress->raise();
//        ui->FullScreen->raise();
        //ui->label->lower();
        //width(),(height()*19)/20-15




    }
    else
    {
       //QMessageBox::warning(this,tr("警告信息"),tr("用户确认警告信息"));
        //弹出提示窗
    }

    //整个进度条的宽度




}

//把视频构造为一个个的图像块
QImage MainWindow::MatImageToQt(const Mat &src)
{
    //CV_8UC1 8位无符号的单通道---灰度图片
    if(src.type() == CV_8UC1)
    {
        //使用给定的大小和格式构造图像
        //QImage(int width, int height, Format format)
        QImage qImage(src.cols,src.rows,QImage::Format_Indexed8);
        //扩展颜色表的颜色数目
        qImage.setColorCount(256);

        //在给定的索引设置颜色
        for(int i = 0; i < 256; i ++)
        {
            //得到一个黑白图
            qImage.setColor(i,qRgb(i,i,i));
        }
        //复制输入图像,data数据段的首地址
        uchar *pSrc = src.data;
        //
        for(int row = 0; row < src.rows; row ++)
        {
            //遍历像素指针
            uchar *pDest = qImage.scanLine(row);
            //从源src所指的内存地址的起始位置开始拷贝n个
            //字节到目标dest所指的内存地址的起始位置中
            memcmp(pDest,pSrc,src.cols);
            //图像层像素地址
            pSrc += src.step;
        }
        return qImage;
    }
    //为3通道的彩色图片
    else if(src.type() == CV_8UC3)
    {
        //得到图像的的首地址
        const uchar *pSrc = (const uchar*)src.data;
        //以src构造图片
        QImage qImage(pSrc,src.cols,src.rows,src.step,QImage::Format_RGB888);
        //在不改变实际图像数据的条件下，交换红蓝通道
        return qImage.rgbSwapped();
    }
    //四通道图片，带Alpha通道的RGB彩色图像
    else if(src.type() == CV_8UC4)
    {
        const uchar *pSrc = (const uchar*)src.data;
        QImage qImage(pSrc, src.cols, src.rows, src.step, QImage::Format_ARGB32);
        //返回图像的子区域作为一个新图像
        return qImage.copy();
    }
    else
    {
        return QImage();
    }
}


void MainWindow::on_play_clicked()
{
    //ui->play->setStyleSheet("background-image:url(/home/gaowenli/Desktop/stop.png)");
    //    background-image:url(":/demo/pictures/1.png");




    if(cap.isOpened())
    {
       // ui->play->setFlat(true);
        if(timer->isActive())
        {


           // backgroundimage:url(":/home/gaowenli/Desktop/stop.png");
            //ui->play->setStyleSheet("background-image:url(/home/gaowenli/Desktop/play.png)");
            ui->play->setText("");
            QPixmap pixmap("/home/gaowenli/Desktop/stop.png");
            QPixmap fitpixmap = pixmap.scaled(40,40,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            ui->play->setIcon(QIcon(fitpixmap));
            ui->play->setIconSize(QSize(40,40));
            on_Stop_triggered();
        }
       else
        {
           // ui->play->setStyleSheet("background-image:url(/home/gaowenli/Desktop/stop.png)");

            ui->play->setText("");
            QPixmap pixmap("/home/gaowenli/Desktop/play.png");
            QPixmap fitpixmap = pixmap.scaled(40,40,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            ui->play->setIcon(QIcon(fitpixmap));
            ui->play->setIconSize(QSize(40,40));
            allframes = cap.get(CAP_PROP_FRAME_COUNT);  //总帧数
            on_Open_triggered();



        }
        is_play = !is_play;
    }

    if(!cap.isOpened())
    {
        //QString directory = QFileDialog::getOpenFileName(this,tr("选择文件"),"/","(*.mp4);;All files(*.*)");
        //cap.open(directory.toStdString());



        // cap.open("/home/gaowenli/Desktop/gwl.mp4");
//         int w = (int)cap.get(CAP_PROP_FRAME_WIDTH);
//         int h = (int)cap.get(CAP_PROP_FRAME_HEIGHT);
//;
       //  ui->label->setGeometry(ui->label->x(),ui->label->y(),w,h);
         //timer->start();

    }

}

//void MainWindow::on_pushButton_2_clicked()
//{
//    on_Stop_triggered();


//}
//void MainWindow::keyPressEvent(QKeyEvent *event)
//{
//    if(event->key()==Qt::Key_V)//也可以是等于键值对应的数值 如 event->key()==86  ,86是 v的十进制键值
//    {
//    }
//}

//value是进度条实时显示的位置
void MainWindow::on_slider_progress_valueChanged(int value)
{



}
//position是长按鼠标拖动时进度条的位置
//void MainWindow::on_slider_progressWidget_sliderMoved(int position)
//{


//}


void MainWindow::on_slider_progress_actionTriggered(int action)
{
    //cout<<"action:"<<action<<endl;
}
//按下进度条的时候，关闭定时器

void MainWindow::on_slider_progress_sliderPressed()
{
//定时器是否开启
    if(timer->isActive())
    {
        is_opened = true;
    }
    else
    {
        is_opened = false;
    }

    timer->stop();


   // cout<<"pressed"<<endl;
}

void MainWindow::on_slider_progress_sliderReleased()
{

    //拖动进度条鼠标抬起的情况下，开启定时器
    if(!cap.isOpened())
    {
//        cap.open("/home/gaowenli/Desktop/Video.mp4");

    }
    else
    {
       // timer->start();

       //目的位置的value
       ui->slider_progress->value();
       ui->slider_progress->maximumWidth();
       //目的占比
       double Newpercent = ui->slider_progress->value()*1.0/max_range;
       //目的帧数
       int NewFrame = allframes*Newpercent;



   //目的帧数比原来的帧数大，说明在往前拖动进度条
       cap.set(CAP_PROP_POS_FRAMES,NewFrame-1);//跳转到帧数NewFrame
       cap.read(src_image);

       QImage imag = MatImageToQt(src_image);
       ui->label->setPixmap(QPixmap::fromImage(imag));

       nowframe = NewFrame;
       if(is_opened)
       {
            timer->start();
       }
       else
       {

       }

    }

}

//从文件中选择视频
void MainWindow::on_pushButton_clicked()
{
    timer->stop();
    QString directory = QFileDialog::getOpenFileName(this,tr("选择文件"),"/","(*.mp4);;All files(*.*)");
    QFileInfo file_name(directory);
//    cout << "类型：" << typeid(file_name.completeBaseName()).name() << endl;
//     cout << "2类型：" << typeid(directory).name() << endl;

    cap.open(directory.toStdString());
    max_range = cap.get(CAP_PROP_FRAME_COUNT);
    ui->slider_progress->setRange(0,max_range);

    allframes = cap.get(CAP_PROP_FRAME_COUNT);
    on_play_clicked();
    if(cap.isOpened())
    {
        strlist.insert(0,file_name.completeBaseName());
        //strlist.append(file_name.completeBaseName());
    }
    Model = new QStringListModel(strlist);//QStringListModel将QStringList包装了起来
    ui->listView->setModel(Model);        //填充到listview
    //设置item内容不可编辑
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->listView->setStyleSheet(
                "QListView{border:0px;}"
                "QListView::item:hover{"
                    "background:rgb(100,149,237);color:rgb(255,255,255);"
                "}"
                "QListView::item:selected{"
                    "background:rgb(65,105,225);color:rgb(255,255,255);"
                "}");
    //建立信号和槽，点击某个item时触发执行itemClick函数
    connect(ui->listView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(itemClick(QModelIndex)));
    mymap.insert(pair<QString, QString>(file_name.completeBaseName(), directory));
      //itemClick(0);
//        index= 0;

//        QString str;
//        str = index.data().toString();//获取当前点击项数据


//        //qDebug()<<"str:"<<str;


//    if(str == file_name.completeBaseName())
//    {
//         QString it = mymap.find(file_name.completeBaseName())->second;
//         cap.open(it.toStdString());
//    }



    //map容器
   // itemClick(0);


    //itemClick(1);
//    if()
//    {

//    }


   // cout<<find(file_name.completeBaseName())->first



}

void MainWindow::on_slider_progress_sliderMoved(int position)
{

}
//void MainWindow::mousePressEvent(QMouseEvent *ev){
//    //代表鼠标左键按下
//    if(ev->button()==Qt::LeftButton){
//        int i=ev->x();
//        int j=ev->y();
//        cout<<"i:"<<i<<endl;
//         cout<<"ui->slider_progress->x():"<<ui->slider_progress->x()<<endl;

//        if(i<=ui->slider_progress->x()&&j<= ui->slider_progress->y())
//        {
//            on_slider_progress_sliderReleased();
//        }
//        QString text=QString("<center><h1>Mouse Press：(%1,%2)</h1></center>").arg(i).arg(j);
//        //this->setText(text);
//    }
//}


//void MainWindow::on_pushButton_6_clicked()
//{

//}
//0.5

//全屏操作
void MainWindow::on_FullScreen_clicked()
{



//    //退出全屏时：
//    this->setWindowFlags (Qt::SubWindow);
//    this->showNormal ();
//    //设置全屏显示
    //this->showFullScreen();
   // is_fullscreen = !is_fullscreen;



    if(!is_fullscreen)
    {
        this->setWindowState(Qt::WindowFullScreen);
        this->move(0,0);

       // ui->FullScreen->setText("退出全屏");
       // this->XDisplayWidth
//        QDesktopWidget *qDesktopWidget= QApplication::desktop();
//        QRect qrect1 = qDesktopWidget->screen(0)->rect();
//        qDebug() << "width:" << qrect1.width() << "height:" << qrect1.height();
        ui->label->setGeometry(0,0,1920,1080);
//        cout<<"宽："<<this->frameGeometry().width()<<endl;
//        cout<<"高："<<this->frameGeometry().height()<<endl;
    }
    else
    {
        this->setWindowFlags (Qt::SubWindow);
        this->showNormal();
        this->setGeometry(480,270,960,540);
       // ui->FullScreen->setText("全屏");
    }
    is_fullscreen = !is_fullscreen;

   //*********************
//    newWindow *configWindow = new newWindow;
//    configWindow->show();
//***********************
    //退出全屏时你可以有鼠标右键单击屏幕弹出菜单获得，也可以捕获键盘事件获得如：

    //ui->label->resize(ui->MainWindow->size());

//    cv::namedWindow("img", WND_PROP_FULLSCREEN);
//    cv::moveWindow("img", 0, 0);
//    cv::setWindowProperty("img", WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
//    imshow("img",src_image);
    //cv::waitKey();
//    if(waitKey(20) == 27)
//    {
//        exit(-1);
//    }




    //cv::Mat img = cv::Mat::zeros(100, 100, CV_8UC3);

//    char c = cv::waitKey(20);

//     while (1)//读视频的循环
//     {

//        if(c==' ')
//       {
//         cv::waitKey(0);
//       }
//       if(c=='q' || c=='Q')
//       {
//          break;
//        }
//     }


}
    //cv::imshow("video", myimage);    }


void MainWindow::on_MainWindow_iconSizeChanged(const QSize &iconSize)
{

}
void MainWindow::ChangeButton()
{
    QPoint point_end = QPoint(0,0);
    ui->play->setFixedSize(40,40);
    //ui->pushButton_2->setFixedSize(40,40);

    ui->FullScreen->setFixedSize(40,40);
    ui->pushButton->setFixedSize(80,40);
    ui->comboBox->setFixedSize(80,40);
    //ui->speedButton->setFixedSize(80,40);
    ui->listView->setFixedSize(width()/4,(9*height())/10);

    int start_space_ = width()/60.0f;
    int btn_space = width()/60.0f;
    int y_pos_ = (19*height())/20-10;


    ui->pushButton->move(point_end.x() + start_space_,y_pos_);
    point_end.setX(ui->pushButton->x() + ui->pushButton->width());


    ui->play->move(point_end.x() + btn_space,y_pos_);
    point_end.setX(ui->play->x() + ui->play->width());
    //ui->pushButton_2->move(width()/20+80,(19*height())/20-10);

    int end_x_ = width() - start_space_ - ui->FullScreen->width() - btn_space - ui->pushButton->width();

    end_x_ = end_x_ > point_end.x() ? end_x_ : point_end.x();

    ui->comboBox->move(end_x_,y_pos_);

    ui->FullScreen->move(end_x_ + btn_space + ui->comboBox->width(),y_pos_);



    ui->slider_progress->setFixedSize(width()-10,5);
    ui->slider_progress->move(width()/60,(19*height())/20-18);
    //ui->speedButton->move(width()/20+80*12,(19*height())/20-10);
    ui->listView->move((width()*3)/4,0);

    if(is_fullscreen)
    {
        ui->label->setFixedSize(width(),height());
    }
    else
    {
        ui->label->setFixedSize(width(),(19*height()/20)-10);
    }


    ui->label->lower();


    ui->label->move(0,0);


//    QPixmap fitpixmap=src_image.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    ui->label->setPixmap(src_image);
//    ui->label->setAlignment(Qt::AlignCenter);
//    ui->label->show();


      ui->label->setAlignment(Qt::AlignCenter);


}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    //QWidget::resizeEvent(event);
    //cout<< width() << ":" << height();

    ChangeButton();
   // ui->FullScreen->move(int x,int y);



}

void MainWindow::enterEvent(QEvent *e)

{
   // if(is_fullscreen)
    {

//        ui->play->show();
//        ui->pushButton_2->show();
//        ui->partButton->show();
//        ui->oneButton->show();
//        ui->partonepartButton->show();
//        ui->onepartButton->show();
//        ui->twoButton->show();
//        ui->FullScreen->show();

    }

//ui.PicTarget->setStyleSheet("background: rgba(0,0,0,0.60);");


}

void MainWindow::leaveEvent(QEvent *e)//离开隐藏closebutton

{
    //if(is_fullscreen)
//    {
//        ui->play->hide();
//        ui->pushButton_2->hide();
//        ui->partButton->hide();
//        ui->oneButton->hide();
//        ui->partonepartButton->hide();
//        ui->onepartButton->hide();
//        ui->twoButton->hide();
//        ui->FullScreen->hide();
//        ui->slider_progress->hide();
//    }


}
void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    //if(is_fullscreen)
    {
//        cout<<ev->x()<<endl;
//        cout<<ev->y()<<endl;
    }
    if(is_fullscreen)
    {
        if(ev->y() < (18*height())/20)
        {
            ui->play->hide();
            //ui->pushButton_2->hide();

            ui->FullScreen->hide();
            ui->slider_progress->hide();
            ui->comboBox->hide();

           ui->pushButton->hide();

        }
        else if(ev->y() > (18*height())/20)
        {

            ui->play->show();
           // ui->pushButton_2->show();

            ui->FullScreen->show();
            ui->slider_progress->show();
            ui->comboBox->show();

           ui->pushButton->show();


        }
    }
    else
    {
        if(ev->x()>(3*width())/4 && ev->y()<(9*height()))
        {
            ui->listView->show();
        }
        else
        {
            ui->listView->hide();
        }
        //给listView设置隐藏功能
    }



}


//下拉菜单










void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
 //  allframes = cap.get(CAP_PROP_FRAME_COUNT);
    cout<<"arg1"<<arg1.toStdString()<<endl;
    if(cap.isOpened())
    {
        if(arg1.toStdString() == "0.5x")
        {
            double fps = cap.get(CAP_PROP_FPS);//获取帧率

            timer->start(1000/(fps*0.5));

        }
        else if(arg1.toStdString() == "1.0x")
        {
            double fps = cap.get(CAP_PROP_FPS);//获取帧率

            timer->start(1000/(fps*1.0));
        }
        else if(arg1.toStdString() == "1.25x")
        {
            double fps = cap.get(CAP_PROP_FPS);//获取帧率

            timer->start(1000/(fps*1.25));
        }
        else if(arg1.toStdString() == "1.5x")
        {
            double fps = cap.get(CAP_PROP_FPS);//获取帧率

            timer->start(1000/(fps*1.5));
        }
        else if(arg1.toStdString() == "2.0x")
        {
            double fps = cap.get(CAP_PROP_FPS);//获取帧率

            timer->start(1000/(fps*2.0));
        }
    }
    else
    {

    }

}
