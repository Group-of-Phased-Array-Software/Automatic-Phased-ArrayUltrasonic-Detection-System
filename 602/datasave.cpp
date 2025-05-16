#include "datasave.h"

DataSave::DataSave(QObject* parent)
    : QObject(parent)
{
    //临时文件位置
    tempfilePath = QDir::currentPath() + "/tempdatafile/temp.bin";
}

//存数据  //使用常量引用既避免了对参数的复制影响性能，又避免了引用过程中不会参数产生修改,但不能避免多个线程同时访问同一个数据
void DataSave::savetempdata(QVector<int> data, int TimeCscandex, int signal_num)
{
    QFile file(tempfilePath);
    if (file.open(QIODevice::Append))//使用QIODevice::Append，将会追加数据至文件中，而不是覆写
    {
        // 定位到要覆写的起始位置
        qint64 startPos = ((TimeCscandex)*signal_num * 400 + 10) * sizeof(int);//7为在存数据之前，存了10个int 型数据

        if (!file.seek(startPos))
        {
            qDebug() << "Failed to seek to position:" << startPos;
            file.close();
            return;
        }

        QDataStream out(&file);
        // 写入数据到二进制文件
        for (int value : data)
        {
            out << value;
        }
        file.close();
    }
}

//将临时数据转存为正式数据
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

        QFile tempFile(tempfilePath); // 临时二进制文件
        if (!tempFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Failed to open temp file for reading:" << tempFile.errorString();
            file.close();
            return;
        }

       qint64 totalBytes = tempFile.size();
       qint64 bytesWritten = 0; 
       int dex = 1; 

        // 读取源文件内容并写入目标文件
        QByteArray buffer;//是一个数组，所以可以储存4096个字节大小
        while (!tempFile.atEnd()) {
            buffer = tempFile.read(4096); // 一次读取 4KB 数据
            file.write(buffer);

            bytesWritten += 4096;
            int percentage = (int)((bytesWritten * 100) / totalBytes);
            if (percentage == dex){
                emit progressUpdated(percentage); // 发送保存进度信号
                dex++;
            }
        }

        // 关闭文件
        tempFile.close();
        file.close();

         // 删除原文件
        tempFile.remove();
        qDebug() << "Saving progress: already";
    

}


//存入信号数量（a扫信号条数）及创建临时文件
void DataSave::startsave(int signal_num, int top, int bot, int left, int right, int ys, int encoder_accuracy)
{
    int placeholder_01 = 0;
    int placeholder_02 = 0;//两个占位数据，方便以后进行数据拓展
    qDebug() << "main encoder_accuracy= " << encoder_accuracy;

    QFile file(tempfilePath);
    if (file.open(QIODevice::Append))
    {
        int Clength = 0;//C扫数据长度，先占位
        QDataStream out(&file);
        // 写入数据到二进制文件
        out << Clength;
        out << signal_num; //存了两个int型数据
        out << top;
        out << bot;
        out << left;
        out << right;
        out << ys;
        out << encoder_accuracy;
        out << placeholder_01;
        out << placeholder_02;//数据占位
        file.close();
    }
}
//按停止扫查后，存入C扫数据长度
void DataSave::stopsave(int Cscandex, int signal_num)
{
    //qDebug() << "TimeCscandex:" << TimeCscandex; 
    int Clength = Cscandex + 1;
    QFile file(tempfilePath);
    //先将数据长度进行写入
    if (file.open(QIODevice::Append))
    {
        // 定位到要覆写的起始位置
        if (!file.seek(0))
        {
            qDebug() << "Failed to seek to position:" << 0;
            file.close();
            return;
        }
        QDataStream out(&file);
        out << Clength;//写入C扫数据长度
        file.close();
    }
    //确定临时文件最终长度，获得最终临时文件
    if (file.open(QIODevice::ReadWrite))
    {
        // 获取文件总的数据量
        qint64 bytesToKeep = (Clength * signal_num * 400 + 10) * sizeof(int); // 计算要保留的字节数//此处存疑 TimeCscandex *signal_num*400+2)

        if (file.resize(bytesToKeep))//对数据进行截断，保存需要保存的这一部分
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
