#include "motioncontrol.h"

Motioncontrol::Motioncontrol(QObject* parent)
    : QObject(parent)
{
    //���ƿ���ʼ��
    dmc_board_init();
    //ֹͣ״̬��������λ�����߰���ֹͣ������ֹͣ�˶���true:ֹͣ�˶���false:�����˶�
    axis_stopstatus = false;
}

//�ȸ�λ
void Motioncontrol::axis_reset()
{
    //���ƿ���ʼ��
    dmc_board_init();
    //�ȸ�λ�����ʾ
    dmc_soft_reset(0);
    Sleep(6e3);//ϵͳ����

    //�ȸ�λ�����ʾ
    QMessageBox* cpt = new QMessageBox;
    cpt->setWindowTitle(QString::fromLocal8Bit("��ʾ"));
    cpt->setText(QString::fromLocal8Bit("�ȸ�λ��ɣ�"));
    cpt->setIcon(QMessageBox::Information);
    cpt->addButton(QMessageBox::Ok);
    // ������ʾ�������ڶ�����ʾ
    cpt->setWindowFlags(cpt->windowFlags() | Qt::WindowStaysOnTopHint);
    cpt->exec();

}

//������ʹ��
void Motioncontrol::axis_enable()
{
    DWORD TotalAxis;//������
    nmc_get_total_axes(0, &TotalAxis);
    for (int i = 0; i < TotalAxis; i++)
        nmc_set_axis_enable(0, i);
}

//������ʧ��
void Motioncontrol::axis_disable()
{
    DWORD TotalAxis;//������
    nmc_get_total_axes(0, &TotalAxis);
    for (int i = 0; i < TotalAxis; i++)
        nmc_set_axis_disable(0, i);
}

//x�ᶨ���˶�
void Motioncontrol::X_axis_pmove(double speed, double len)
{
    //�趨�ٶ�
    double Min_Vel = 0.25 * speed;        //��ʼ�ٶ�
    double Max_Vel = speed;             //����ٶ�
    double Stop_Vel = 0.25 * speed;       //ֹͣ�ٶ�
    double Tacc = 1;                //����ʱ��
    double Tdec = 1;                //����ʱ��
    dmc_set_profile_unit(0, 0, Min_Vel, Max_Vel, Tacc, Tdec, Stop_Vel);

    //�趨s��ʱ��
    WORD s_mode = 0;                  //�����������̶�ֵΪ0
    double s_para = 0.4;              //S��ʱ��
    dmc_set_s_profile(0, 0, s_mode, s_para);

    //��������ģʽ��1�������0�����˶�
    WORD Master_axis = 0;             //����
    WORD Slave_axis = 1;              //������
    WORD CardNo = 0;                  //�˶�������
    WORD Enable = 1;                  //����ʹ��״̬
    double ratio = 1;                 //��������
    dmc_set_gear_follow_profile(CardNo, Slave_axis, Enable, Master_axis, ratio);

    //����������������ֹͣ��ֵ
    double dstp_error = 50;           //����ֹͣ�����ֵ����λ��unit
    double emg_error = 100;           //����ֹͣ�����ֵ����λ��unit
    dmc_set_grant_error_protect_unit(CardNo, Slave_axis, Enable, dstp_error, emg_error);//���д������ᶼ���ԣ�ֻҪ�����˶���������ֵ������

    //ִ�ж����˶�
    WORD posi_mode = 0;               //����˶�ģʽ��0�������˶�ģʽ��1
    dmc_pmove_unit(0, 0, len, posi_mode);//��������˶�ģʽ
}

//x�������˶�
void Motioncontrol::X_axis_vmove(double speed, int dir)
{
    //�趨�ٶ�
    double Min_Vel = 0.25 * speed;        //��ʼ�ٶ�
    double Max_Vel = speed;               //����ٶ�
    double Stop_Vel = 0.25 * speed;       //ֹͣ�ٶ�
    double Tacc = 1;                    //����ʱ��
    double Tdec = 1;                    //����ʱ��
    dmc_set_profile_unit(0, 0, Min_Vel, Max_Vel, Tacc, Tdec, Stop_Vel);

    //�趨s��ʱ��
    WORD s_mode = 0;                  //�����������̶�ֵΪ0
    double s_para = 0.4;              //S��ʱ��
    dmc_set_s_profile(0, 0, s_mode, s_para);

    //��������ģʽ��1�������0�����˶�
    WORD Master_axis = 0;             //����
    WORD Slave_axis = 1;              //������
    WORD CardNo = 0;                  //�˶�������
    WORD Enable = 1;                  //����ʹ��״̬
    double ratio = 1;                 //��������
    dmc_set_gear_follow_profile(CardNo, Slave_axis, Enable, Master_axis, ratio);

    //����������������ֹͣ��ֵ
    double dstp_error = 50;           //����ֹͣ�����ֵ����λ��unit 
    double emg_error = 100;           //����ֹͣ�����ֵ����λ��unit
    dmc_set_grant_error_protect_unit(CardNo, Slave_axis, Enable, dstp_error, emg_error);//���д������ᶼ���ԣ�ֻҪ�����˶���������ֵ������

    //ִ�������˶�
    dmc_vmove(0, 0, dir);//dir:�˶�����0��1
}

//x��ֹͣ
void Motioncontrol::X_axis_stop()
{
    WORD stop_mode = 0;//�ƶ���ʽ��0������ֹͣ��1������ֹͣ
    dmc_stop(0, 0, stop_mode);
}

//�����ᶨ���˶�
void Motioncontrol::other_axis_pmove(int axis, double speed, double len)
{
    //�����x�ᣬ��ֹͣ�˶�
    if (axis == 0 || axis == 1)
        return;

    //�趨�ٶ�
    double Min_Vel = 0.25 * speed;        //��ʼ�ٶ�
    double Max_Vel = speed;               //����ٶ�
    double Stop_Vel = 0.25 * speed;       //ֹͣ�ٶ�
    double Tacc = 0.5;                    //����ʱ��
    double Tdec = 0.5;                    //����ʱ��
    dmc_set_profile_unit(0, axis, Min_Vel, Max_Vel, Tacc, Tdec, Stop_Vel);

    //�趨s��ʱ��
    WORD s_mode = 0;                  //�����������̶�ֵΪ0
    double s_para = 0.2;              //S��ʱ��
    dmc_set_s_profile(0, axis, s_mode, s_para);

    //ִ�ж����˶�
    WORD posi_mode = 0;//����˶�ģʽ��0�������˶�ģʽ��1
    dmc_pmove_unit(0, axis, len, posi_mode);//��������˶�ģʽ
}

//�����������˶�
void Motioncontrol::other_axis_vmove(int axis, double speed, int dir)
{
    //�����x�ᣬ��ֹͣ�˶�
    if (axis == 0 || axis == 1)
        return;

    //�趨�ٶ�
    double Min_Vel = 0.25 * speed;        //��ʼ�ٶ�
    double Max_Vel = speed;               //����ٶ�
    double Stop_Vel = 0.25 * speed;       //ֹͣ�ٶ�
    double Tacc = 0.5;                    //����ʱ��
    double Tdec = 0.5;                    //����ʱ��
    dmc_set_profile_unit(0, axis, Min_Vel, Max_Vel, Tacc, Tdec, Stop_Vel);

    //�趨s��ʱ��
    WORD s_mode = 0;                  //�����������̶�ֵΪ0
    double s_para = 0.2;              //S��ʱ��
    dmc_set_s_profile(0, axis, s_mode, s_para);
    qDebug() << "zhixl";

    //ִ�������˶�
    dmc_vmove(0, axis, dir);//dir:�˶�����0��1
}

//������ֹͣ
void Motioncontrol::other_axis_stop(int axis)
{
    WORD stop_mode = 0;//�ƶ���ʽ��0������ֹͣ��1������ֹͣ
    dmc_stop(0, axis, stop_mode);
}

//����ֹͣ
void Motioncontrol::stop_all_axis()
{
    DWORD TotalAxis;//������
    nmc_get_total_axes(0, &TotalAxis);

    WORD stop_mode = 0;//�ƶ���ʽ��0������ֹͣ��1������ֹͣ
    for (int i = 0; i < TotalAxis; i++)
        dmc_stop(0, i, stop_mode);
}

//��ȡ������λ��
double Motioncontrol::get_axis_position(WORD axis)
{
    double axis_pos;
    dmc_get_encoder_unit(0, axis, &axis_pos);
    return axis_pos;
}

//�����������λ�ù���
void Motioncontrol::set_axis_pos_zero()
{
    DWORD TotalAxis;//������
    nmc_get_total_axes(0, &TotalAxis);
    for (int i = 0; i < TotalAxis; i++)
    {
        dmc_set_encoder_unit(0, i, 0);
    }
}

//�Զ�ɨ��
void Motioncontrol::auto_scan(QVector<QVector<double>>pos_Vector, int scanmodel_SLR)
{
    qDebug() << "��ʼ�Զ�ɨ��";
    switch (scanmodel_SLR)
    {
    case 1://�̱�ɨ��
        for (int i = 0; i < pos_Vector.size(); i++)
        {
            if (axis_stopstatus)
                break;
            //Y���������50mm��ʵ�ʵ�һ�����Ƶ���Ϊ��㣬�����ˣ���Y��������Ӧ����������
            other_axis_pmove(2, 15000, (-1) * 50000);
            while (dmc_check_done(0, 2) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);

            if (axis_stopstatus)
                break;
            //Y������˶�
            if (i == (pos_Vector.size() - 1))//�����һ�����Ƶ�ʱ���������������һ�����Ƶ�ֻ��Ϊ�յ㣬������
                return;
            if (i != 0)//���úü�����ֱ�ӿ�ʼ�˶���������һ�ο��Ƶ����
                other_axis_pmove(2, 15000, pos_Vector[i][1] - pos_Vector[i - 1][1]);
            while (dmc_check_done(0, 2) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);

            //�������
            if (i != 0)
                emit start_duoji(128, pos_Vector[i][3] * (4095.0 / 360.0), 01);
            Sleep(1e3);
            //Z1�����
            if (i != 0)//���úü�����ֱ�ӿ�ʼ�˶���������һ�ο��Ƶ����
                other_axis_pmove(3, 10000, pos_Vector[i][2] - pos_Vector[i - 1][2]);
            while (dmc_check_done(0, 3) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);

            if (axis_stopstatus)
                break;
            //Y���������50mm
            other_axis_pmove(2, 15000, 50000);
            while (dmc_check_done(0, 2) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);

            //X���˶�
            if (axis_stopstatus)
                break;
            X_axis_pmove((10000.0 / 17.35) * 20, pos_Vector[i + 1][0] - pos_Vector[i][0]);//X��ɨ���ٶ�20mm/s
            while (dmc_check_done(0, 0) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);
        }
        break;
    case 2://����ɨ��
        for (int i = 0; i < pos_Vector.size(); i++)
        {
            if (axis_stopstatus)
                break;
            //Y���������50mm��ʵ�ʵ�һ�����Ƶ���Ϊ��㣬�����ˣ���Y��������Ӧ����������
            other_axis_pmove(2, 15000, 50000);
            while (dmc_check_done(0, 2) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);

            if (axis_stopstatus)
                break;
            //Y������˶�
            if (i == (pos_Vector.size() - 1))//�����һ�����Ƶ�ʱ���������������һ�����Ƶ�ֻ��Ϊ�յ㣬������
                return;
            if (i != 0)//���úü�����ֱ�ӿ�ʼ�˶���������һ�ο��Ƶ����
                other_axis_pmove(2, 15000, pos_Vector[i][1] - pos_Vector[i - 1][1]);
            while (dmc_check_done(0, 2) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);

            //�������
            if (i != 0)
                emit start_duoji(128, pos_Vector[i][3] * (4095.0 / 360.0), 02);
            Sleep(1e3);
            //Z1�����
            if (i != 0)//���úü�����ֱ�ӿ�ʼ�˶���������һ�ο��Ƶ����
                other_axis_pmove(3, 10000, pos_Vector[i][2] - pos_Vector[i - 1][2]);
            while (dmc_check_done(0, 3) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);

            if (axis_stopstatus)
                break;
            //Y���������50mm
            other_axis_pmove(2, 15000, (-1) * 50000);
            while (dmc_check_done(0, 2) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);

            //X���˶�
            if (axis_stopstatus)
                break;
            X_axis_pmove((10000.0 / 17.35) * 20, pos_Vector[i + 1][0] - pos_Vector[i][0]);//X��ɨ���ٶ�20mm/s
            while (dmc_check_done(0, 0) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);
        }
        break;
    case 3://R��ɨ��
        for (int i = 0; i < pos_Vector.size(); i++)
        {
            if (axis_stopstatus)
                break;
            //Z2���������50mm��ʵ�ʵ�һ�����Ƶ���Ϊ��㣬�����ˣ���Z2��������Ӧ����������
            other_axis_pmove(4, 10000, 50000);
            while (dmc_check_done(0, 4) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);

            //�������
            if (i != 0)
                emit start_duoji(128, pos_Vector[i][3] * (4095.0 / 360.0), 03);
            Sleep(1e3);
            //Y�����
            if (i != 0)//���úü�����ֱ�ӿ�ʼ�˶���������һ�ο��Ƶ����
                other_axis_pmove(2, 15000, pos_Vector[i][1] - pos_Vector[i - 1][1]);
            while (dmc_check_done(0, 2) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);

            if (axis_stopstatus)
                break;
            //Z2������˶�
            if (i == (pos_Vector.size() - 1))//�����һ�����Ƶ�ʱ���������������һ�����Ƶ�ֻ��Ϊ�յ㣬������
                return;
            if (i != 0)//���úü�����ֱ�ӿ�ʼ�˶���������һ�ο��Ƶ����
                other_axis_pmove(4, 10000, pos_Vector[i][2] - pos_Vector[i - 1][2]);
            while (dmc_check_done(0, 4) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);

            if (axis_stopstatus)
                break;
            //Z2���������50mm
            other_axis_pmove(4, 10000, (-1) * 50000);
            while (dmc_check_done(0, 4) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);

            //X���˶�
            if (axis_stopstatus)
                break;
            X_axis_pmove((10000.0 / 17.35) * 20, pos_Vector[i + 1][0] - pos_Vector[i][0]);//X��ɨ���ٶ�20mm/s
            while (dmc_check_done(0, 0) == 0)//�ȴ����˶���ɣ�����ֵΪ0��˵��ָ������������
            {
            }
            Sleep(1e3);
        }
        break;
    default:
        break;
    }
    axis_stopstatus = false;
}

Motioncontrol::~Motioncontrol()
{
    //�رտ��ƿ����ͷ�ϵͳ��Դ
    dmc_board_close();
}
