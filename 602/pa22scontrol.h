#pragma once

#include <QObject>
#include<qtcpsocket.h>
#include<QJsonObject>
#include<QJsonDocument>
#include<QNetworkProxy>//�������ͷ�ļ������Ե���setProxy(QNetworkProxy::NoProxy)����VPN��ʱ��Ҳ��������22S
#include<qmessagebox.h>
#include<QRegularExpression>//Ϊ��ʹ��������ʽ
#include"Line_rule_struct.h"

//��������������
#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

class Pa22scontrol : public QObject
{
	Q_OBJECT

public:
	Pa22scontrol(QObject* parent = Q_NULLPTR);//����������Ϊnull����ֹ������Ĭ�Ϲ������
	~Pa22scontrol();

	//Tcpͨ�Žӿ�
	QTcpSocket* order_tcp;
	QTcpSocket* data_tcp;

	//22s�豸ip��ַ
	QString Pa22s_ip = "192.168.22.121";

	//22sָ��˿ں�
	unsigned short port1 = 51007;
	//���ݶ�
	unsigned short port2 = 51005;

	//����22s
	void connect_Pa22s();
	//�Ͽ�22s����
	void disconnect_Pa22s();
	//�ػ�
	void turn_off_Pa22s();
	//ɨ���źſ�
	void open_signal();
	//ɨ���źŹ�
	void close_signal();
	//����ָ���
	void transferinstruction(QJsonObject jsonObject);
	//��ȡ22sоƬ�¶�
	QString gettemprature();
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
	//������ɨ����  Ĭ����бШ
	void setpalscan(Line_rule line_rule);//������ɨ����
	Line_rule myline_rule;//��ɨ��������
	//����������
	void resetencoder();

	//��������
	void receive_data();
	//��������
	QByteArray receivedData;
	QByteArray Data;

signals:
	void sendData(QByteArray Data);

};
