#include "servocontrol.h"

Servocontrol::Servocontrol(QObject* parent)
    : QObject(parent)
{
    // ������������
    modbustoolDevice = new QModbusRtuSerialMaster(this);//Qt5ʹ��
    //modbustoolDevice = new QModbusRtuSerialClient(this);//Qt6ʹ��
    //����������ز�������
    modbustoolDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
        "COM11");//�������� 
    modbustoolDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
        QSerialPort::NoParity);//������żУ��λ
    modbustoolDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
        115200);//���ڲ�����
    modbustoolDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
        QSerialPort::Data8);//��������λ
    modbustoolDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
        QSerialPort::OneStop);//����ֹͣλ 

    //�����ôӻ�����Ӧʱ�Ķ���
    modbustoolDevice->setTimeout(1000);//���豸�ظ���Ϣ�ĳ�ʱʱ��
    modbustoolDevice->setNumberOfRetries(2);//�ظ����ʹ���

    //����Modbus�������ݵ�Ԫ����ȡ���ݣ�ģʽ0x03�����ּĴ������Ĵ�����ַ257������ȡ���ݳ���1
    readUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 257, 1);
}

//���Ӷ��
void Servocontrol::connectToservo()
{
    //��ʼ���Ӵ��ڣ����ж�����״̬
    if (modbustoolDevice->connectDevice()) {
        //����ʾ
    }
    else {
        QMessageBox* cpt = new QMessageBox;
        cpt->setWindowTitle(QString::fromLocal8Bit("�������ʧ�� "));//�ӿո�Ͳ�����
        cpt->setText(QString::fromLocal8Bit("modbustoolDevice->errorString() "));
        cpt->setIcon(QMessageBox::Information);
        cpt->addButton(QMessageBox::Ok);
        // ������ʾ�������ڶ�����ʾ
        cpt->setWindowFlags(cpt->windowFlags() | Qt::WindowStaysOnTopHint);
        cpt->exec();
        return;
    }
}

//�Ͽ��������
void Servocontrol::disconnectToservo()
{
    modbustoolDevice->disconnectDevice();
}

//��λ������λ����ĳ���Ĵ���д������ ����Ϊ�Ĵ�����ַ
//д������(regStartAddrΪ�����ǶȼĴ�������ַΪ128��numberΪд������Ϊת��Ŀ��λ��0~4095��serverAddressΪ�ӻ���ַ��01��02��03)
void Servocontrol::writeToclient(unsigned int regStartAddr, unsigned int number, unsigned int serverAddress)
{
    //���û�ж���modbus���ڣ�ֱ�ӷ���
    if (!modbustoolDevice) {
        //statusBar()->showMessage(tr("ModbusͨѶʧ�ܣ����鴮�����ӣ�"), 5000);
        return;
    }
    //���״̬����ʾ
    //statusBar()->clearMessage();

    //����Modbus�Ļ������ݵ�Ԫ������������modbusΪ0x06дģʽ���Ĵ�����ַ���Լ��������ݵĴ�С
    QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, regStartAddr, 1);//06
    //Ҫд�������
    writeUnit.setValue(0, number);

    //����Modbus��ʽ�����ݵ��ӻ���ַΪserverAddress�Ĵӻ���serverAddress��ʾ�ڼ����ӻ���01��02��03
    if (auto* reply = modbustoolDevice->sendWriteRequest(writeUnit, serverAddress)) {
        if (!reply->isFinished()) {
            //������յ���Ӧ��Ϣ
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError) {
                    //���յ�����Ӧ��Ϣ��Э����������״̬����������
                    //statusBar()->showMessage(tr("д�����ݴ���: %1 (Mobus exception: 0x%2)")
                    //    .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16),
                    //    5000);
                }
                else if (reply->error() != QModbusDevice::NoError) {
                    //���յ�����Ӧ��Ϣ���������������״̬����������
                    //statusBar()->showMessage(tr("д�����ݴ���: %1 (code: 0x%2)").
                    //    arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
                else {
                    //��ȡ��Ӧ��Ϣ�����ݵ�modbus��������QModbusDataUnit
                    const QModbusDataUnit unit = reply->result();

                    //ͳ����Ӧ��Ϣ�����ݵĸ���
                    QString showdata = tr("������%1 ֵΪ��").arg(unit.valueCount());

                    //����չʾ��Ӧ��Ϣ�е�����
                    for (uint i = 0; i < unit.valueCount(); i++) {
                        const QString entry = tr("%1 ").arg(unit.value(i));
                        showdata = showdata + entry;
                    }
                    //����Ϣ���������̨
                    qDebug() << showdata;
                }
                reply->deleteLater();
                });
        }
        else {
            reply->deleteLater();
        }
    }
    else {
        //����û�гɹ�������״̬����Ϣ
        //statusBar()->showMessage(tr("д�����ݴ���: ") + modbustoolDevice->errorString(), 5000);
    }
}

//��ȡλ����Ϣ����
double Servocontrol::readToclient(int scanmodel_SLR)
{
    //int live_angle_value;//���ն��ʵ��λ�ã�0~4095��
    if (auto* reply = modbustoolDevice->sendReadRequest(readUnit, scanmodel_SLR))//sendReadRequest����sendWirteRequest
    {
        if (!reply->isFinished()) {
            //������յ���Ӧ��Ϣ
            connect(reply, &QModbusReply::finished, this, [this, reply]() {
                if (reply->error() == QModbusDevice::ProtocolError)
                {
                    //���յ�����Ӧ��Ϣ��Э����������״̬����������
                    //statusBar()->showMessage(tr("д�����ݴ���: %1 (Mobus exception: 0x%2)")
                    //    .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16), 5000);
                }
                else if (reply->error() != QModbusDevice::NoError) {
                    //���յ�����Ӧ��Ϣ���������������״̬����������
                    //statusBar()->showMessage(tr("д�����ݴ���: %1 (code: 0x%2)").
                    //    arg(reply->errorString()).arg(reply->error(), -1, 16), 5000);
                }
                else {
                    const QModbusDataUnit unit = reply->result();
                    //ͳ����Ӧ��Ϣ�����ݵĸ���
                    QString showdata = tr("������%1 ֵΪ��").arg(unit.valueCount());
                    //����չʾ��Ӧ��Ϣ�е�����
                    for (uint i = 0; i < unit.valueCount(); i++) {
                        const QString entry = tr("%1 ").arg(unit.value(i));
                        showdata = showdata + entry;
                    }
                    //��ʾ�ڽ�����
                    //qDebug() << "output" << showdata;
                    live_angle_value = unit.value(0);
                    double num = (360.0 / 4095.0);
                    live_angle = num * unit.value(0);
                }
                reply->deleteLater();
                });
        }
        else {
            reply->deleteLater();
        }
    }
    else {
        //statusBar()->showMessage(tr("д�����ݴ���: ") + modbustoolDevice->errorString(), 5000);
    }
    return live_angle;
}

Servocontrol::~Servocontrol()
{}
