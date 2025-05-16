#include "datasave.h"

DataSave::DataSave(QObject* parent)
    : QObject(parent)
{
    //��ʱ�ļ�λ��
    tempfilePath = QDir::currentPath() + "/tempdatafile/temp.bin";
}

//������  //ʹ�ó������üȱ����˶Բ����ĸ���Ӱ�����ܣ��ֱ��������ù����в�����������޸�,�����ܱ������߳�ͬʱ����ͬһ������
void DataSave::savetempdata(QVector<int> data, int TimeCscandex, int signal_num)
{
    QFile file(tempfilePath);
    if (file.open(QIODevice::Append))//ʹ��QIODevice::Append������׷���������ļ��У������Ǹ�д
    {
        // ��λ��Ҫ��д����ʼλ��
        qint64 startPos = ((TimeCscandex)*signal_num * 400 + 10) * sizeof(int);//7Ϊ�ڴ�����֮ǰ������10��int ������

        if (!file.seek(startPos))
        {
            qDebug() << "Failed to seek to position:" << startPos;
            file.close();
            return;
        }

        QDataStream out(&file);
        // д�����ݵ��������ļ�
        for (int value : data)
        {
            out << value;
        }
        file.close();
    }
}

//����ʱ����ת��Ϊ��ʽ����
void DataSave::savedata(QString filePath)
{
        if (filePath.isEmpty())
        {
            qDebug() << "User canceled the operation.";
            return;
        }

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Failed to open file for writing:" << file.errorString();
            return;
        }

        QFile tempFile(tempfilePath); // ��ʱ�������ļ�
        if (!tempFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Failed to open temp file for reading:" << tempFile.errorString();
            file.close();
            return;
        }

       qint64 totalBytes = tempFile.size();
       qint64 bytesWritten = 0; 
       int dex = 1; 

        // ��ȡԴ�ļ����ݲ�д��Ŀ���ļ�
        QByteArray buffer;//��һ�����飬���Կ��Դ���4096���ֽڴ�С
        while (!tempFile.atEnd()) {
            buffer = tempFile.read(4096); // һ�ζ�ȡ 4KB ����
            file.write(buffer);

            bytesWritten += 4096;
            int percentage = (int)((bytesWritten * 100) / totalBytes);
            if (percentage == dex){
                emit progressUpdated(percentage); // ���ͱ�������ź�
                dex++;
            }
        }

        // �ر��ļ�
        tempFile.close();
        file.close();

         // ɾ��ԭ�ļ�
        tempFile.remove();
        qDebug() << "Saving progress: already";
    

}


//�����ź�������aɨ�ź���������������ʱ�ļ�
void DataSave::startsave(int signal_num, int top, int bot, int left, int right, int ys, int encoder_accuracy)
{
    int placeholder_01 = 0;
    int placeholder_02 = 0;//����ռλ���ݣ������Ժ����������չ
    qDebug() << "main encoder_accuracy= " << encoder_accuracy;

    QFile file(tempfilePath);
    if (file.open(QIODevice::Append))
    {
        int Clength = 0;//Cɨ���ݳ��ȣ���ռλ
        QDataStream out(&file);
        // д�����ݵ��������ļ�
        out << Clength;
        out << signal_num; //��������int������
        out << top;
        out << bot;
        out << left;
        out << right;
        out << ys;
        out << encoder_accuracy;
        out << placeholder_01;
        out << placeholder_02;//����ռλ
        file.close();
    }
}
//��ֹͣɨ��󣬴���Cɨ���ݳ���
void DataSave::stopsave(int Cscandex, int signal_num)
{
    //qDebug() << "TimeCscandex:" << TimeCscandex; 
    int Clength = Cscandex + 1;
    QFile file(tempfilePath);
    //�Ƚ����ݳ��Ƚ���д��
    if (file.open(QIODevice::Append))
    {
        // ��λ��Ҫ��д����ʼλ��
        if (!file.seek(0))
        {
            qDebug() << "Failed to seek to position:" << 0;
            file.close();
            return;
        }
        QDataStream out(&file);
        out << Clength;//д��Cɨ���ݳ���
        file.close();
    }
    //ȷ����ʱ�ļ����ճ��ȣ����������ʱ�ļ�
    if (file.open(QIODevice::ReadWrite))
    {
        // ��ȡ�ļ��ܵ�������
        qint64 bytesToKeep = (Clength * signal_num * 400 + 10) * sizeof(int); // ����Ҫ�������ֽ���//�˴����� TimeCscandex *signal_num*400+2)

        if (file.resize(bytesToKeep))//�����ݽ��нضϣ�������Ҫ�������һ����
        {
            qDebug() << "File truncated successfully to"<< bytesToKeep<<" b";
        }
        else {
            qDebug() << "Failed to truncate file.";
        }

        file.close();
    }
    qDebug() << "stop savedata ";
}

DataSave::~DataSave()
{}
