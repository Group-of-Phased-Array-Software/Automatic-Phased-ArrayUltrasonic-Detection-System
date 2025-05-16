#include "pa22scontrol.h"


Pa22scontrol::Pa22scontrol(QObject* parent)
    : QObject(parent)
{
    //ʵ�����׽��ֶ���
    order_tcp = new QTcpSocket;
    data_tcp = new QTcpSocket;

    //���ݽ���
    connect(data_tcp, &QTcpSocket::readyRead, this, &Pa22scontrol::receive_data);

    //��ʼ����ɨ����
    myline_rule.elm_num = 64;
    myline_rule.pitch = 0.6;
    myline_rule.aperture_size = 29;
    myline_rule.focus_angle = 0;
    myline_rule.focus_distance = 0;
    myline_rule.sound_velocity = 5900;
    myline_rule.start_elm = 1;
    myline_rule.end_elm = 64;

}

//����22s
void Pa22scontrol::connect_Pa22s()
{
    //ÿ������ǰ���Ƚ���������
    receivedData.clear();
    //���ô���(��ֹ���������޷����ӣ�����ǿ�����³����)
    order_tcp->setProxy(QNetworkProxy::NoProxy);
    data_tcp->setProxy(QNetworkProxy::NoProxy);

    //����
    order_tcp->connectToHost(Pa22s_ip, port1);
    data_tcp->connectToHost(Pa22s_ip, port2);

    //���ӳɹ�ʱ��ִ����һ������
    //ָ������ӳɹ�ʱ
    connect(order_tcp, &QTcpSocket::connected, [=]() {
        qDebug() << "ָ��˳ɹ�����";
        });
    //���ݶ����ӳɹ�  
    connect(data_tcp, &QTcpSocket::connected, [=]() {
        //ִ����Ӧ����
        QMessageBox msgBox;
        msgBox.setText("���ӳɹ���");
        msgBox.setWindowTitle("��ʾ");
        msgBox.setWindowFlags(msgBox.windowFlags() | Qt::WindowStaysOnTopHint);
        msgBox.exec();

        //�첨
        QJsonObject jsonObject8;
        jsonObject8["rectify"] = "full_wave";
        transferinstruction(jsonObject8);

        //�������ݳ��ȣ�����Ϊ512��
        QJsonObject jsonObject2;
        jsonObject2["a_data_len"] = 512;
        transferinstruction(jsonObject2);

        //����A��ƽ�� 
        QJsonObject jsonObject11;
        jsonObject11["a_smooth"] = 0;
        transferinstruction(jsonObject11);

        //����������
        QJsonObject jsonObject12;
        jsonObject12["encoder_reset"] = QJsonObject{
            {"idx", 0}
        };
        transferinstruction(jsonObject12);

        //������ɨ����
        setpalscan(myline_rule);

        });
}

//�Ͽ�22s����
void Pa22scontrol::disconnect_Pa22s()
{
    //ֹͣ���ݲɼ�
    QJsonObject jsonObject;
    jsonObject["scan"] = 0;
    transferinstruction(jsonObject);
    //�Ͽ�Socket
    order_tcp->close();
    data_tcp->close();

    //ֻҪ�ͻ���������������ӶϿ��ˣ��ͻᷢ��disconnected�ź�
    connect(order_tcp, &QTcpSocket::disconnected, this, [=]() {
        order_tcp->close();
        });

    //���ݶ˶Ͽ�
    connect(data_tcp, &QTcpSocket::disconnected, this, [=]() {
        data_tcp->close();
        if (!order_tcp->isOpen()) {
            QMessageBox msgBox;
            msgBox.setText("�Ͽ����ӣ�");
            msgBox.setWindowTitle("��ʾ");
            msgBox.setWindowFlags(msgBox.windowFlags() | Qt::WindowStaysOnTopHint);
            msgBox.exec();

            qDebug() << "�ͻ����ѶϿ�";

            //ֹͣBɨ���ݲɼ�
            QJsonObject jsonObject;
            jsonObject["scan"] = 0;
            transferinstruction(jsonObject);
        }
        });

    //�Ͽ����Ӻ󣬰��������
    receivedData.clear();
}

//�ػ�
void Pa22scontrol::turn_off_Pa22s()
{
    //��Ϣ��ʾ���ܹ��ڶ�����ʾ
    QMessageBox* messageBox = new QMessageBox(NULL);
    messageBox->setWindowTitle("��ʾ");
    messageBox->setText("ȷ�Ϲػ���");
    messageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox->setDefaultButton(QMessageBox::No); // Ĭ��ѡ�� No ��ť

    // ������Ϣ�����ڶ�����ʾ
    messageBox->setWindowFlags(messageBox->windowFlags() | Qt::WindowStaysOnTopHint);

    // ��ʾ��Ϣ�򲢵ȴ��û���Ӧ
    int ref = messageBox->exec();

    // �����û���ѡ��ִ�в�ͬ����
    if (ref == QMessageBox::Yes)
    {
        // �û������ Yes ��ť��ִ�йػ�ָ��
        QJsonObject jsonObjectclose;
        jsonObjectclose["power"] = "off";
        transferinstruction(jsonObjectclose);
    }
    else if (ref == QMessageBox::No)
    {
        // �û������ No ��ť����ִ���κβ���
        return;
    }
}

//ɨ���źſ�
void Pa22scontrol::open_signal()
{
    //ÿ������ǰ���Ƚ���������
    receivedData.clear();

    QJsonObject jsonObject;
    jsonObject["scan"] = 1;
    transferinstruction(jsonObject);
}

//ɨ���źŹ�
void Pa22scontrol::close_signal()
{
    QJsonObject jsonObject;
    jsonObject["scan"] = 0;
    transferinstruction(jsonObject);
}

//����ָ���
void Pa22scontrol::transferinstruction(QJsonObject jsonObject)
{

    //QJsonObject jsonObject; // ����һ�� QJsonObject �������ڴ洢 JSON ����
    //jsonObject["volt"] = 20 ;  //�����ѹ����Ϊ20V

    //QJsonDocument jsonDocument(jsonObject);
    //QByteArray jsonData = jsonDocument.toJson(QJsonDocument::Compact);  //ת���� JSON �ַ���
    //jsonData.append(char(0x1E));  //��ӽ�����
    //order_tcp->write(jsonData);  // ������õ� JSON ����д�����������У����͵�ָ��Ŀ��


    //order_tcp->waitForBytesWritten();
    //order_tcp->waitForReadyRead();

    QJsonDocument jsonDocument(jsonObject); 
    QByteArray jsonData = jsonDocument.toJson(QJsonDocument::Compact); 
    jsonData.append(char(0x1E)); 
    qDebug() << jsonData; 
    order_tcp->write(jsonData); 
    order_tcp->waitForBytesWritten(); 
    order_tcp->waitForReadyRead(); 

    QByteArray receivedData = order_tcp->readAll();
    qDebug() << receivedData;
    //�ж��Ƿ����óɹ�
    QString str = QString::fromUtf8(receivedData);
    if (str.contains("error"))
    {
        QMessageBox::warning(nullptr, "����", "�������ô���");//��ָ��������һ�㴰����ؼ��ĸ������Ǵ����࣬������ǻ���QObject���
        return;
    }
}

//��ȡ22sоƬ�¶�
QString Pa22scontrol::gettemprature()
{
    //����JSONָ���JSONָ��д�ɴ�����ʽ
    QJsonObject jsonObject;
    jsonObject["board_temp"] = QJsonValue::Null;
    // �� JSON ����ת��Ϊ JSON �ĵ�
    QJsonDocument jsonDocument(jsonObject);
    // �� JSON �ĵ�ת��Ϊ QByteArray��ʽ
    QByteArray jsonData = jsonDocument.toJson(QJsonDocument::Compact);

    //ÿ��ָ��Ҫ�� RS(0x1E) �ַ������������ת����QByteArray�󣬼����ַ�
    jsonData.append(char(0x1E));

    // �������ݵ�������
    order_tcp->write(jsonData);
    //�ȴ���������ȫ��������
    order_tcp->waitForBytesWritten();
    //�ȴ����������͵�����
    order_tcp->waitForReadyRead();

    //��ȡ����
     //�������ݣ�����QByteArray���ͽ��д���
    QByteArray receivedData = order_tcp->readAll();

    QString str = QString::fromUtf8(receivedData);
    if (str.contains("error"))
    {
        QMessageBox::warning(nullptr, "����", "�������ô���");
        return "";
    }
    else
    {
        // ����������ʽģʽ������ƥ�� "result" ��������ֲ���
        QRegularExpression result("\"result\":([0-9]+\\.?[0-9]*)");

        // ���ַ���������ƥ�������
        QRegularExpressionMatch match = result.match(str);

        // ����ҵ�ƥ�������ȡ��ֵ����
        if (match.hasMatch()) {
            QString resultString = match.captured(1); // ��ȡ��һ���������е�����            
            return resultString;
        }
        else {
            return "";
        }
    }
}

//������������
void Pa22scontrol::setdgain(double dgain)
{
    QJsonObject jsonObject;
    //��������
    jsonObject["dgain"] = dgain;
    transferinstruction(jsonObject);
}

//���÷����ѹ
void Pa22scontrol::setvolt(int volt)
{
    QJsonObject jsonObject;
    jsonObject["volt"] = volt;
    transferinstruction(jsonObject);
}

//����������
void Pa22scontrol::setpulsewidth(int pulse_width)
{
    QJsonObject jsonObject;
    jsonObject["pulse_width"] = pulse_width;
    transferinstruction(jsonObject);
}

//��������1,2
void Pa22scontrol::setvga(int vga1, int vga2)
{
    QJsonObject jsonObject;
    jsonObject["gain"] = QJsonObject({
       {"vga1", vga1},
        {"vga2", vga2}
        });
    transferinstruction(jsonObject);
}

//�����ظ�Ƶ��
void Pa22scontrol::setprf(int prf)
{
    QJsonObject jsonObject;
    jsonObject["prf"] = prf;
    transferinstruction(jsonObject);
}

//����ѹ����
void Pa22scontrol::setrangeratio(double range_ratio)
{
    QJsonObject jsonObject;
    jsonObject["range_ratio"] = range_ratio;
    transferinstruction(jsonObject);
}

//���������˲�
void Pa22scontrol::setdfilter(int dfilter)
{
    QJsonObject jsonObject;
    jsonObject["dfilter"] = dfilter;
    transferinstruction(jsonObject);
}

//������Ƶ�첨
void Pa22scontrol::setvideodetect(int video_detect)
{
    QJsonObject jsonObject;
    jsonObject["video_detect"] = 2;
    transferinstruction(jsonObject);
}

//������ɨ����  Ĭ����бШ
void Pa22scontrol::setpalscan(Line_rule line_rule)
{
    myline_rule = line_rule;
    qDebug() << "Pa22scontrol all : " << myline_rule.elm_num;

    QJsonObject jsonObject;
    jsonObject["pa_lscan"] = QJsonObject{
        {"linear_pa_probe", QJsonObject{
            {"elm_num", myline_rule.elm_num},//��Ԫ����
            {"pitch", myline_rule.pitch}
        }},
        /*{"slope_wedge", QJsonObject{
            {"angle", 37.0},
            {"height", 20.0},
            {"sound_velocity", 2337.0}
        }},*/
        {"aperture", QJsonObject{
            {"start_elm", myline_rule.start_elm - 1},//��һ����Ԫ ����Ϊ0
            {"stop_elm", myline_rule.end_elm - 1},
            {"size", myline_rule.aperture_size}//13���׾�
        }},
        {"focus", QJsonObject{
            {"angle", myline_rule.focus_angle},
            {"distance", myline_rule.focus_distance}
        }},
        {"workpiece", QJsonObject{
            {"sound_velocity", myline_rule.sound_velocity}
        }}
    };
    transferinstruction(jsonObject);
    //����֮ǰ���������
    //receivedData.clear();
}

//����������
void Pa22scontrol::resetencoder()
{
    QJsonObject jsonObject;
    jsonObject["encoder_reset"] = QJsonObject{
        {"idx", 0}
    };
    transferinstruction(jsonObject);
}

//��������
void Pa22scontrol::receive_data()
{
    int signal_num = myline_rule.elm_num - myline_rule.aperture_size + 1;
    //qDebug() << "22scontrol signal_num is " << signal_num;

    receivedData.append(data_tcp->readAll());
    if (!receivedData.isEmpty())
    {
        while (receivedData.size() >= (signal_num * 512 + 16))//���ϵȺţ����������"n <= d.size - pos"�ı���
        {
            Data = receivedData.mid(16, signal_num * 512);//�˴����Ѿ�ɾ����16���ֽڵ�ʶ���
            emit sendData(Data); //ͨ���źźͲ۽����ݴ��ݳ�ȥ 
            receivedData.remove(0, (signal_num * 512 + 16));
        }
    }
    //qDebug() << "receive data";
}

Pa22scontrol::~Pa22scontrol()
{}

