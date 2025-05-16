#pragma once

#include <QObject>
#include"LTDMC.h"
#include<qmessagebox.h>
#include"Windows.h"
#include<qdebug.h>

//��������������
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

class Motioncontrol : public QObject
{
	Q_OBJECT

public:
	Motioncontrol(QObject* parent = Q_NULLPTR);
	~Motioncontrol();

	//ֹͣ״̬��������λ�����߰���ֹͣ������ֹͣ�˶���true:ֹͣ�˶���false:�����˶�
	bool axis_stopstatus;

	//���涨���˶��������˶������������߳��е��ã������߳���ִ��
	//�Զ�ɨ�麯�����ö��̣߳������߳���ִ��

	//�ȸ�λ
	void axis_reset();

	//������ʹ��
	void axis_enable();

	//������ʧ��
	void axis_disable();

	//x�ᶨ���˶�
	void X_axis_pmove(double speed, double len);//���˶��ٶȣ��˶����루�������ģʽ����ֵ���������˶�����ֵ���������˶���

	//x�������˶�
	void X_axis_vmove(double speed, int dir);//���˶��ٶȣ��˶�����

	//x��ֹͣ
	void X_axis_stop();

	//�����ᶨ���˶�
	void other_axis_pmove(int axis, double speed, double len);

	//�����������˶�
	void other_axis_vmove(int axis, double speed, int dir);

	//������ֹͣ
	void other_axis_stop(int axis);

	//����ֹͣ
	void stop_all_axis();

	//��ȡ������λ��
	double get_axis_position(WORD axis);

	//�����������λ�ù���
	void set_axis_pos_zero();

	//�Զ�ɨ��
	void auto_scan(QVector<QVector<double>>pos_Vector, int scan_model);

signals:
	//�����������
	//д������(regStartAddrΪ�����ǶȼĴ�������ַΪ128��numberΪд������Ϊת��Ŀ��λ��0~4095��serverAddressΪ�ӻ���ַ��01��02��03)
	void start_duoji(unsigned int regStartAddr, unsigned int number, unsigned int serverAddress);
};
