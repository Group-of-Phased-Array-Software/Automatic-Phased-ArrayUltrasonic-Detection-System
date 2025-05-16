#pragma once

#include <QtWidgets/QMainWindow>
#include<qchart.h>
#include<qchartview.h>
QT_CHARTS_USE_NAMESPACE //Qt5Ҫʹ��QChart��Ҫ��������ռ䣬Qt6����
#include "ui_mainwindow.h"
#include"parameter_ui.h"
#include"motion_ui.h"
#include"motioncontrol.h"
#include"servocontrol.h"
#include "colorlevel_ui.h"
#include<qthread.h>
#include<qtimer.h>
#include<windef.h>//�������ͷ�ļ�������ʹ��WORD�ͱ���
#include"pa22scontrol.h"
#include"Line_rule_struct.h"
#include "dataprocess.h"
#include "datasave.h"
#include<qvalueaxis.h>
#include<qsplineseries.h>
#include<QProgressBar>
#include<QStatusBar>

//��������������
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    //��ʼ�����ؼ�λ��
    void set_ui_position();
    //�����˶����ƴ��ڸ��ؼ�
    void connectmotion_ui();
    //����ɨ�����ô��ڸ��ؼ�
    void connectparatemer_ui();

    //��ض�ʱ��
    QTimer* refreshtimer;//��ʱˢ��Aɨ��Bɨ��Cɨͼ
    QTimer* TimeCscantimer;//ʱ��Cɨ��ʱ��
    QTimer* temperaturetimer;//��ȡ�¶�22s��ʱ��

    //��ʾբ��
    QRect BScanDoor;
    QPainter BScanPainter;
    QPixmap BScanDoorPixmap;
    int ys = 0;
    int top_door = 200;
    int bot_door = 400;
    int left_door = 0;
    int right_door = 800;

    //��ʱ��բ�Ÿı�ʱ����Ӧ�Ĳ���
    void ys_door_changed(int ys, int top_door, int bot_door, int left_door, int right_door);

    //Aɨ��������
    QVector<int>AscanData;
    //ʵʱAɨ
    void showLiveAscan();
    void showLiveBCscan();
    //A����ʾ
    QChart* chart;
    QValueAxis* Xaixs;
    QValueAxis* Yaixs;

    QImage BImage;//���մ����̴߳��ݵ�QImage,��ʾ�Ŵ��BɨͼƬ
    QImage CImage;//���մ����̴߳��ݵ�QImage,��ʾ�Ŵ��CɨͼƬ
    int showCImagedex;//CɨͼƬ��������ʾImage��ʼ����
    QImage CScaledImage;//��Ž��зŴ��ͼ��
    //�鿴Cɨ�������Cɨͼ�񣨿����ù���ǰ���ƶ��鿴��
    void showCImage_usingwheel(int value);
    bool TimeCscanstatus = false;//�Ƿ���ʱ��Cɨ��״̬

    //��ʾ�������
    void show_savedata_progress(int);


    //ɨ��ģʽ���̱�ɨ�飬����ɨ�飬R��ɨ��
    unsigned int scanmodel_SLR = 1;//1,����̱ߣ�2�����ߣ�3����R��,Ĭ��Ϊ�̱�
    //ɨ��ģʽ��0Ϊ��������1Ϊʱ��
    int scanmodel = 0;//Ĭ��Ϊ������ɨ��

    //���ݺ������
    QString filePath;    //��ȡ���ݵĶ������ļ�·������������ʱ����·����Ҳ�������Լ�ѡ���·��
    int data_signal_num;
    int data_Clength;
    int data_TimeCscandex;
    int data_CscanStartdex;//�ڼ������ݿ�ʼCɨ����
    int data_CscanStopdex;
    int data_CImageHeight;
    int data_startdex;//���ݵĵڼ�������г���
    int data_signallength;//����
    double data_encoder_accuracy;//���ݱ�����ɨ�龫�ȣ���ʵ�ľ��ȣ����߳��еľ��ȳ�����һ��ϵ��
    bool data_status = false;//�ж��Ƿ��ڶ������ݵ�״̬
    void read_data();//��ȡ����
    int CImagelength;//Cɨͼ��ĳ��� 

    void showCImage();//��ȡ���������ݣ�������Cɨ����
    void showBImage();//��ȡ���������ݣ�������Bɨ����
    void showAWave();//��ȡ���������ݣ�����ʾ��Aɨ����

    int data_BScandex;//��ȡ���ݣ���ʾ�ڼ������ݵ�Bɨ
    int data_AWavedex = 0;//��ȡ���ݣ���ʾ�ڼ������ݵ�Aɨ����
    int LiveAscandex = 0;//ʵʱ��ʾ�ڼ�������

    //B��CLabel����¼��Ƿ����,��ֹ���ʿն���
    bool B_LabelMouseStatus = false;
    bool C_LabelMouseStatus = false;

    //ɨ��·��
    QVector<QVector<double>>pos_Vector;
    int pathmodel_SLR = 1;

    //�˶��������
    //�ŷ����
    // �ݾࣨ���תһȦ10000unit,ģ�����ж��پ��룩
    double X_pitch = 10000.0 / 17.35;
    double Y_pitch = 10000.0 / 10.0;
    double Z1_pitch = 10000.0 / 10.0;
    double Z2_pitch = 10000.0 / 10.0;

    //�����ٶ� �趨��ʼ�˶��ٶ�
    double X_speed = 1 * X_pitch;//��ʼ�˶��ٶ�Ϊ��1mm/s
    double Y_speed = 1000;
    double Z1_speed = 1000;
    double Z2_speed = 1000;

    //���о��루���м����������������˶��뷴���˶���,��ʼ�˶�����Ϊ0�������˶���������
    double X_distance = 0;
    double Y_distance = 0;
    double Z1_distance = 0;
    double Z2_distance = 0;

    //��λ��־��0���͵�ƽ����λδ�ڵ���1���ߵ�ƽ����λ���ڵ�
    WORD IO_0;//X����
    WORD IO_1;//X�ᶫ
    WORD IO_2;//Y����
    WORD IO_3;//Y����
    WORD IO_4;//Z1����
    WORD IO_5;//Z1����
    WORD IO_6;//Z2����
    WORD IO_7;//Z2����

    //��ֹͣ��־��true��������λֹͣ��false��������λ��ֹͣ��������λ�ˣ������������㼸���˶������ܹ��ƶ�����λ��
    bool X_stopstatus = true;
    bool Y_stopstatus = true;
    bool Z1_stopstatus = true;
    bool Z2_stopstatus = true;

    //��ȡ��λ��ʱ��
    QTimer* read_axis_inport_timer;

    //��ȡ��λ״̬������ֹͣ��־���ŷ�ʹ��ʱ�������ŷ�ʧ��ʱ�ر�
    void read_axis_inport();

    //��ʱ����ȡ���λ�ö�ʱ��
    QTimer* read_servo_timer;

    //����ɫ��
    void set_color_icon();

private:
    Ui::MainWindowClass ui;

    //�����Ӵ���
    Motion_ui* motion_ui;
    Parameter_ui* parameter_ui;
    ColorLevel_ui* colorlevel_ui;//ɫ�׵�������

    //�˶������߳�
    Motioncontrol* Motionthread;
    //���ݴ����߳�
    DataProcess* Dataprocessthread;
    //���ݱ����߳�
    DataSave* Datasavethread;

    //�������ģ��
    Servocontrol* Servo_model;

    //22s����ģ��
    Pa22scontrol* Pa22s_model;

signals:
    void startshowBscan();//�����߳�������Bɨ
    void startshowCscan();//�����߳�������ʱ��Cɨ
    void startsave(int signal_num, int top, int bot, int left, int right, int ys,int encoder_accuracy);//�����ź�������aɨ�ź���������������ʱ�ļ�,�����߳�
    void stopsave(int Cscandex, int signal_num);//����Cɨ���ݳ��ȣ������߳�
    void savedata(QString);//�����߳��б�������
    void start_auto_scan(QVector<QVector<double>>, int scan_model);//��ʼ�Զ�ɨ��

};
