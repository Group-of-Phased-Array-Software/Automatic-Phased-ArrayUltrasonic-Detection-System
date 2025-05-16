#pragma once

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QModbusDataUnit>
#include <QModbusRtuSerialMaster>
#include<qvariant.h>
//#include<qmodbusrtuserialclient.h>//Qt6��Ҫ�������д���
#include<qmessagebox.h>
#include<qdebug.h>

class Servocontrol : public QObject
{
	Q_OBJECT

public:
	Servocontrol(QObject* parent = Q_NULLPTR);
	~Servocontrol();

	//������
	QModbusClient* modbustoolDevice;//���Ӵ���
	QModbusDataUnit readUnit;//��ȡ���λ����Ϣ���ݵ�Ԫ�����ڷ������ݵ��ӻ�

	//���Ӷ��
	void connectToservo();
	//�Ͽ��������
	void disconnectToservo();

	//��λ������λ����ĳ���Ĵ���д������ ����Ϊ�Ĵ�����ַ��д������(regStartAddrΪ�����ǶȼĴ�������ַΪ128��
	// numberΪд������Ϊת��Ŀ��λ��0~4095��serverAddressΪ�ӻ���ַ��01��02��03)
	void writeToclient(unsigned int regStartAddr, unsigned int number, unsigned int serverAddress);

	//��ȡλ����Ϣ���ݣ�scanmodel_SLR��ʾҪ��ȡ�ĸ��ӻ���λ��
	double readToclient(int scanmodel_SLR);
	int live_angle_value = 0;
	double live_angle = 0;
};