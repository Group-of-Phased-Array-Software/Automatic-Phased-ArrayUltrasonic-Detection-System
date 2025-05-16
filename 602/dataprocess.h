#pragma once

#include <QObject>
#include<qvector.h>
#include"Line_rule_struct.h"
#include<qdebug.h>
#include<qimage.h>
#include <algorithm>//�����ȴ�С��ȡ�����������ֵ

class DataProcess : public QObject
{
	Q_OBJECT

public:
	DataProcess(QObject* parent = Q_NULLPTR);
	~DataProcess();

	//��parameter_ui�н�����ɨ��������
	void getLine_rule(Line_rule line_rule);
	Line_rule myline_rule;//��ɨ��������
	int signal_num;//�ź���

	//�����̻߳�ȡ���ݺ���
	void getdata(QByteArray data);
	QByteArray Data;//�洢����

	int ys = 0;
	int B_Scan_label_height = 600;
	int B_Scan_label_width = 800;
	int C_Scan_label_width = 1080;//ԭ��Ϊ1100

	//Bɨ
	void showBscan();//����ʵʱBɨ���ݼ�����Aɨ���������߳�
	QImage BscaledImage;//Bɨ����ͼ

	//Cɨ
	void showCscan();
	void ys_door_changed(int ys, int top_door, int bot_door, int left_door, int right_door);	//��ʱ��բ�Ÿı�ʱ����Ӧ�Ĳ���
	QImage CImage;
	QImage CscaledImage;//Cɨ����ͼ
	int Cscandex;//����Cɨͼ�������
	int CscanStartsignaldex;//Cɨ��ʼ�ź��������Եڼ����źſ�ʼ����
	int CscanStopsignaldex;;//Cɨ�����ź����������ڼ����źŽ�������
	int CImageHeight;//�ж������ź����ڳ���
	int startdex;//�źŵĵڼ�������г���
	int signallength;//�źŵĳ��ȣ�ѡȡ������ȷ�Χ�ڵ����ֵ���г���

	//ͼ����ɫ��ʾ
	QColor interpolateColors(const QColor& startColor, const QColor& endColor, qreal ratio);
	QColor valuetocolor(int value);
	QColor customBlue = QColor(60, 115, 217);//����ɫ
	qreal Amp_interval=64.0;//��ɫ��ֵ���
	QColor valueToColor01(int value);

	//������
	//������������ָ����0Ϊ����1Ϊ��
	int encoderdex = 0;//Ĭ��Ϊ����
	// ��������ʷλ��
	int encoderlastpos;
	void getencoderpos();//��ȡ������λ�ú���
	bool EncoderCscanstatus = false;//������ɨ��״̬  true:��������ʱʵʱ��ȡ������λ�ã�false:��������ʱ������ȡ������λ��
	int EncoderCscandex;//ǰ100��ȡλ��������ֱ��return
	//������ɨ�龫��
	int encoder_accuracy =  0.5 * 144;//Ĭ��ɨ�龫��Ϊ0.05mm,144Ϊϵ��
	bool encoder_first_scanstatus;//�ж��ǲ��ǵ�һ�α�����ɨ�飬true:Ϊ��һ��ɨ�飬false:Ϊ�ڶ���ɨ��

	bool datastatus;//datastatusΪture,�����������������ļ���Ϊfalse���������ļ�ɾ��һ������


signals:
	//������õ�A��B��Cɨ���ݷ��������߳���
	void sendAscanData(QVector<int>);
	void sendBscanImage(QImage);
	void sendCscanImage(QImage);

	void sendEncoderpos(int pos);//���ͱ�����λ��
	void startEncoderCscan();//���̷߳����źţ���Cɨ����showCscan()���������߳���,Ϊʲô�޷���ͬһ�߳��е������������Σ�
	void sendDataToSave(QVector<int>, int Cscandex, int signal_num);//QVector���ݲ������õķ�ʽ����Ϊ�ǿ��̴߳������ݣ������ÿ��ܻᵼ�¶���̷߳���ͳһ����
	void deletedata(int signal_num);
};
