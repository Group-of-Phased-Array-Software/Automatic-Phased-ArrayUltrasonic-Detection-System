#include "ui_parameter_ui.h"
#include<qdebug.h>
#include"Line_rule_struct.h"
#include<QFileDialog>
#include<QMessageBox>
#include"LTDMC.h"

//��������������
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

class Parameter_ui : public QWidget
{
    Q_OBJECT

public:
    Parameter_ui(QWidget* parent = nullptr);
    ~Parameter_ui();

    //��ʼ�����ؼ�λ��
    void set_ui_position();
    Ui::Parameter_uiClass ui;

    //��������
    double dgain;
    //�����ѹ
    int volt;
    //������
    int pulse_width;
    //����1,2
    int vga1;
    int vga2;
    //�ظ�Ƶ��
    int prf;
    //ѹ����
    double range_ratio;
    //�����˲�
    int dfilter;
    //��Ƶ�첨
    int video_detect;
    //��ɨ������ز���
    Line_rule myline_rule;

    //բ�����
    int ys = 0;
    int top_door = 200;
    int bot_door = 400;
    int left_door = 0;
    int right_door = 800;
    int minwidth = 40;//�������С���
    int B_Scan_label_height = 600;
    int B_Scan_label_width = 800;

    //���ع���
    void readTechnique();

    //���湤��
    void saveTechnique();

    //�½�ɨ��·��
    void creat_path();

    //��ӿ��Ƶ�
    void add_pos(double x, double y, double z, double angle);

    //ɾ�����Ƶ�
    void delete_pos();

    //���ؿ��Ƶ�
    void read_path();

    //������Ƶ�
    void save_path();




private:

signals:
    //������������
    void setdgain(double dgain);
    //���÷����ѹ
    void setvolt(int volt);
    //����������
    void setpulsewidth(int pulse_width);
    //��������1,2
    void setvga(int vga1, int vga2);
    //�����ظ�Ƶ��
    void setprf(int prf);
    //����ѹ����
    void setrangeratio(double range_ratio);
    //���������˲�
    void setdfilter(int dfilter);
    //������Ƶ�첨
    void setvideodetect(int video_detect);
    //������ɨ����
    void setpalscan(Line_rule myline_rule);
    //����������
    void resetencoder();
    //��ʱ��բ����ֵ�ı䣬��ֵ���ݳ�ȥ
    void send_ys_door(int ys, int top_door, int bot_door, int left_door, int right_door);
    //�����Ѷ�ȡ��·�������߳�
    void send_path(int pathmodel_SLR, QVector<QVector<double>>pos_Vector);
    //���ͱ���������
    void send_encoder_accuracy(double);
    //����բ�����´���Cɨͼ��
    void reprocess_CImage();
};
