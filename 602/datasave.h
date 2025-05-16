#pragma once

#include <QObject>
#include<qvector.h>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include<qdebug.h>
#include<qthread.h>
#include<QCoreApplication>
#include<QFileDialog>

class DataSave : public QObject
{
	Q_OBJECT

public:
	DataSave(QObject* parent = Q_NULLPTR);
	~DataSave();

	//��ʱ�ļ�λ��
	QString tempfilePath;

	void savetempdata(QVector<int> newData, int TimeCscandex, int signal_num);//�洢��ʱ����
	void savedata(QString);//����ʱ����ת��Ϊ��ʽ����
	void startsave(int signal_num, int top, int bot, int left, int right, int ys,int encoder_accuracy);//�����ź�������aɨ�ź�������,բ�����������ҵ�λ�ã���������ʱ�ļ�
	void stopsave(int TimeCscandex, int signal_num);//����Cɨ���ݳ���

signals:
	void progressUpdated(int);//���ݱ������
};