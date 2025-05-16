#include "dataprocess.h"

DataProcess::DataProcess(QObject* parent)
    : QObject(parent)
{
    //��ʼ����ɨ����
    myline_rule.elm_num = 64;
    myline_rule.pitch = 0.6;
    myline_rule.aperture_size = 29;
    myline_rule.focus_angle = 0;
    myline_rule.focus_distance = 0;
    myline_rule.sound_velocity = 5900;
    myline_rule.start_elm = 1;
    myline_rule.end_elm = 64;
    //��ʼ���ź�����
    signal_num = myline_rule.end_elm - myline_rule.start_elm + 1 - myline_rule.aperture_size + 1;
    //��ʼ��Cɨͼ���������
    ys_door_changed(0, 200, 400, 0, 800);//����ֵ����parameter_ui��door�����ĳ�ʼ��ֵ
}

//�����̻߳�ȡ���ݺ���
void DataProcess::getdata(QByteArray data)
{
    Data = data;
    if (EncoderCscanstatus)//��ʼ������ɨ���ٿ�ʼ��ȡ������λ��
    {
        getencoderpos();
    }
}

//��parameter_ui������ɨ��������
void DataProcess::getLine_rule(Line_rule line_rule)
{
    myline_rule = line_rule;
    signal_num = myline_rule.end_elm - myline_rule.start_elm + 1 - myline_rule.aperture_size + 1;
}


//����Bɨ���ݣ�����Aɨ���ݷ��͸�
void DataProcess::showBscan()
{
    //�ж��������Ƿ�Ϊ�ջ��߲�����Ӧ����������С,���̻߳�ȡ����ʱ�����Ѿ���ǰ��16���ֽڵ�ʶ���ɾ����
    if ((Data.isEmpty()) || (Data.size() != (signal_num * 512)))
        return;

    QVector<int> data;

    for (int i = 0; i < Data.size(); i++)
    {
        int dex = i / 512;
        if (((0 + 512 * dex) <= i) && (i <= (399 + 512 * dex)))
        {
            data.append((int)(unsigned char)Data[i]);//ֻ���ֵ����
        }
    }
    if (data.size() == signal_num * 400)//��ֹQVector����index out of range
    {
        //�����ݷ��͸����̣߳�������ʾAɨ����
        emit sendAscanData(data);

        //200���㣬ÿ���㶼��ʾ
        //QImage Bimage((signal_num + (signal_num - 1) * 9), 200, QImage::Format_RGB32);
        //for (int x = 0; x < signal_num; x++)
        //{
        //    for (int y = 0; y < 200; y++)
        //    {
        //        int value = data[y + x * 400 + ys];
        //        Bimage.setPixelColor(x * 10, y, valueToColor01(value)/*QColor(200, 100, 100)*/);
        //    }
        //}
        ////��Ӳ�ֵ��
        //for (int i = 0; i < signal_num - 1; i++)
        //{
        //    for (int x = 1; x < 10; x++)
        //    {
        //        for (int y = 0; y < 200; y++)
        //        {
        //            int value = data[y + 400 * i + ys] * (10 - x) / 10 + data[y + 400 * i + 400 + ys] * x / 10;//int���ܹ���������
        //            Bimage.setPixelColor(x + i * 10, y, valueToColor01(value)/*QColor(200, 100, 100)*/);
        //        }
        //    }
        //}
        
        //200���㣬ȡ������֮������ֵ����ʾ100����
        QImage Bimage((signal_num + (signal_num - 1) * 9), 100, QImage::Format_RGB32);
        // Ԥ�ȼ���ÿ����ֵ���Ȩ��
        int weight[10];
        for (int x = 0; x < 10; ++x) {
            weight[x] = (10 - x) * 10;
        }

        // ��ѭ������ÿ���źŵ���д���
        for (int x = 0; x < signal_num; ++x) {
            for (int y = 0; y < 100; ++y) {
                //int value = data[y + x * 400 + ys];
                int value = std::max(data[2*y + x * 400 + ys], data[2*y+1 + x * 400 + ys]);
                Bimage.setPixelColor(x * 10, y, valuetocolor(value));
            }

            // ��ѭ������ÿ�����źŵ�֮����Ӳ�ֵ��
            if (x < signal_num - 1) {
                for (int x_offset = 1; x_offset < 10; ++x_offset) {
                    for (int y = 0; y < 100; ++y) {
                        // �����ֵ
                        //int value = (data[y + 400 * x + ys] * weight[x_offset] +data[y + 400 * (x + 1) + ys] * (weight[10-x_offset])) / 100;
                        int value_01 = std::max(data[2*y + 400 * x + ys], data[2*y +1+ 400 * x + ys]);
                        int value_02 = std::max(data[2 * y + 400 * (x + 1) + ys], data[2 * y + 1 + 400 * (x + 1) + ys]);
                        int value = (value_01 * weight[x_offset] + value_02 * weight[10 - x_offset]) / 100;
                        //qDebug() << "��ֵ value= " << value;
                        Bimage.setPixelColor(x * 10 + x_offset, y, valuetocolor(value));
                    }
                }
            }
        }
        
        //�����ÿ����ķ�ֵ���ٸ��ݷ�ֵ���г���
        //{
        //    QVector<QVector<int>> imagevector;
        //    imagevector.resize((signal_num + (signal_num - 1) * 9));//��Ҫ�����趨һ��QVector�Ĵ�С��������ָ���ط�����ֵ
        //    // Ԥ�ȼ���ÿ����ֵ���Ȩ��
        //    int weight[10];
        //    for (int x = 0; x < 10; ++x)
        //    {
        //        weight[x] = (10 - x) * 10;
        //    }
        //    // ��ѭ������ÿ���źŵ���д���
        //    for (int x = 0; x < signal_num; ++x)
        //    {
        //        QVector<int>tempvector;
        //        for (int y = 0; y < 200; ++y)
        //        {
        //            int value = data[y + x * 400 + ys];
        //            //Bimage.setPixelColor(x * 10, y, valuetocolor(value));
        //            tempvector.append(value);
        //        }
        //        imagevector[x * 10].append(tempvector);
        //        // ��ѭ������ÿ�����źŵ�֮����Ӳ�ֵ��
        //        if (x < signal_num - 1) {
        //            for (int x_offset = 1; x_offset < 10; ++x_offset)
        //            {
        //                QVector<int>tempvector;
        //                for (int y = 0; y < 200; ++y)
        //                {
        //                    int value = (data[y + 400 * x + ys] * weight[x_offset] + data[y + 400 * (x + 1) + ys] * (weight[10 - x_offset])) / 100;
        //                    tempvector.append(value);
        //                }
        //                imagevector[x * 10 + x_offset].append(tempvector);
        //            }
        //        }
        //    }
        //    QImage Bimage((signal_num + (signal_num - 1) * 9), 200, QImage::Format_RGB32);
        //    for (int x = 0; x < (signal_num + (signal_num - 1) * 9); x++)
        //    {
        //        for (int y = 0; y < 200; y++)
        //        {
        //            Bimage.setPixelColor(x, y, valuetocolor(imagevector[x][y]));
        //        }
        //    }
        //}

        //ͼ������
        BscaledImage = Bimage.copy(0, 0, Bimage.width(), Bimage.height());
        emit sendBscanImage(BscaledImage); 
    }

}

//Cɨ
void DataProcess::showCscan()
{
    //�ж��������Ƿ�Ϊ�ջ��߲�����Ӧ����������С
    if ((Data.isEmpty()) || (Data.size() != (signal_num * 512)))
        return;

    QVector<int> data;
    for (int i = 0; i < Data.size(); i++)
    {
        int dex = i / 512;
        if ((0 + 512 * dex <= i) && (i <= 399 + 512 * dex))
        {
            data.append((int)(unsigned char)Data[i]);//ֻ���ֵ����
        }
    }
    emit sendDataToSave(data, Cscandex, signal_num);
    //Cɨ
    for (int y = 0; y < CImageHeight; y++)
    {
        //�ױ���
        QVector<int>temp = data.mid(startdex + (y + CscanStartsignaldex) * 400, signallength);
        int  value = *std::max_element(temp.constBegin(), temp.constEnd());

        CImage.setPixelColor(Cscandex, y, valuetocolor(value));
    }

    if (Cscandex >= C_Scan_label_width)//����ʾ��ͼƬ���ȴ���labelʱ��ͼƬ��������ʾ
    {
        //��QImage�е�copy��������ֱ�� =
        CscaledImage = CImage.copy(Cscandex - C_Scan_label_width + 1, 0, C_Scan_label_width, CImage.height());
    }
    else
    {
        CscaledImage = CImage.copy(0, 0, Cscandex + 1, CImage.height());
    }
    emit sendCscanImage(CscaledImage);
}

//��ȡ������λ�ú���
void DataProcess::getencoderpos()
{
    //�ж��������Ƿ�Ϊ�ջ��߲�����Ӧ����������С
    if ((Data.isEmpty()) || (Data.size() != (signal_num * 512)))
        return;
    //������λ�� 412-415�е�ֵΪencoderposA;416-419�е�ֵΪencoderposB;
    quint32 encoderposA = 0;
    quint32 encoderposB = 0;
    // �� Data �ж�ȡС�˴洢���޷�����������
    QDataStream streamA(Data.mid(412, sizeof(quint32)));
    streamA.setByteOrder(QDataStream::LittleEndian); // ����ΪС�˴洢
    streamA >> encoderposA;
    QDataStream streamB(Data.mid(416, sizeof(quint32)));
    streamB.setByteOrder(QDataStream::LittleEndian); // ����ΪС�˴洢
    streamB >> encoderposB;

    if (encoder_first_scanstatus)
    {
        emit sendEncoderpos(0);//��λ��0���ͳ�ȥ����ֹ������λ������
        encoder_first_scanstatus = false;
        return;
    }


    if (EncoderCscandex < 100)
    {
        //�ж�A,B��ľ���ֵ��ֵ�Ƿ���󣬹���Ļ���ֱ��return
        int abs_AB = encoderposA - encoderposB;
        if (abs(abs_AB) > 10000)
        {
            //qDebug() << "abs_AB=" << abs(abs_AB);
            return;
        }
        EncoderCscandex++;
    }

    int currentpos;
    if (encoderdex == 0)//����
    {
        currentpos = encoderposA - encoderposB;
        emit sendEncoderpos(currentpos);//����λ�ø����߳�������ʾ
    }
    else
    {
        currentpos = encoderposB - encoderposA;
        emit sendEncoderpos(currentpos);
    }

    //������Cɨ�źŴ���
    if (currentpos >= encoderlastpos + encoder_accuracy)
    {
        //��֤λ��ÿ����encoder_accuracy���ܴ���һ��Cɨ�źţ�encoder_accuracy�����滻����������
        int value = (currentpos - encoderlastpos) / encoder_accuracy;
        for (int i = 0; i < value; i++)
        {
            Cscandex++;
            emit startEncoderCscan();
        }
        //���ϴ����Cɨ���ݵĵ��ٴ�10���������һ�㣬�������������Ա�֤ÿ10���������������һ��Cɨ����
        encoderlastpos = currentpos - (currentpos - encoderlastpos) % encoder_accuracy;
    }
    else if ((encoderlastpos - currentpos >= encoder_accuracy) && (Cscandex > 0))
    {

        int value = (encoderlastpos - currentpos) / encoder_accuracy;
        for (int i = 0; (i < value) && (Cscandex > 0); i++)
        {
            Cscandex--;
            emit startEncoderCscan();//���д����ܹ���ͼ��ʵ�ֹ���Ч��
        }
        encoderlastpos = currentpos + (encoderlastpos - currentpos) % encoder_accuracy;
    }

}

//��ʱ��բ�Ÿı�ʱ����Ӧ�Ĳ���
void DataProcess::ys_door_changed(int value, int top_door, int bot_door, int left_door, int right_door)
{
    ys = value;
    startdex = top_door / (B_Scan_label_height / 200) + ys;
    qDebug() << "startdex= " << startdex;
    //signallength = bot_door / (B_Scan_label_height / 200) - startdex;//���ȣ��źų���Ӧ�ð����湫ʽ����
    signallength = (bot_door-top_door) / (B_Scan_label_height / 200);//����
    CscanStartsignaldex = (left_door / (B_Scan_label_width / signal_num));
    CscanStopsignaldex = (right_door / (B_Scan_label_width / signal_num));
    CImageHeight = CscanStopsignaldex - CscanStartsignaldex;
}

// ��ɫ���ɺ���
QColor DataProcess::interpolateColors(const QColor& startColor, const QColor& endColor, qreal ratio)//qreal��ʾ˫���ȸ�����
{
    // ratio��0��1֮�䣬��ʾ����ʼ��ɫ���ɵ�������ɫ�ĳ̶�
    int r = startColor.red() + ratio * (endColor.red() - startColor.red());
    int g = startColor.green() + ratio * (endColor.green() - startColor.green());
    int b = startColor.blue() + ratio * (endColor.blue() - startColor.blue());
    return QColor(r, g, b);
}

//��ֵת��Ӧ��ɫ
QColor DataProcess::valuetocolor(int value)
{
    //QColor color;
    //if (value <= 63)
    //{
    //    // ��ɫ����ɫ����
    //    color = interpolateColors(Qt::white, customBlue, value / 63.0);
    //}
    //else if (value <= 127)
    //{
    //    // ��ɫ����ɫ����
    //    color = interpolateColors(customBlue, Qt::green, (value - 63) / 64.0);
    //}
    //else if (value <= 191)
    //{
    //    // ��ɫ����ɫ����
    //    color = interpolateColors(Qt::green, Qt::yellow, (value - 127) / 64.0);
    //}
    //else
    //{
    //    // ��ɫ����ɫ����
    //    color = interpolateColors(Qt::yellow, Qt::red, (value - 191) / 64.0);
    //}
    //return color;
    QColor color;
    if (value <= (Amp_interval-1))
    {
        // ��ɫ����ɫ����
        color = interpolateColors(Qt::white, customBlue, value / (Amp_interval - 1));
    }
    else if (value <= (2*Amp_interval - 1))
    {
        // ��ɫ����ɫ����
        color = interpolateColors(customBlue, Qt::green, (value - (Amp_interval - 1)) / Amp_interval);
    }
    else if (value <= (3 * Amp_interval - 1))
    {
        // ��ɫ����ɫ����
        color = interpolateColors(Qt::green, Qt::yellow, (value - (2 * Amp_interval - 1)) / Amp_interval);
    }
    else
    {
        // ��ɫ����ɫ����
        qreal ratio;
        if ((value - (3 * Amp_interval - 1)) / Amp_interval > 1) {
            ratio = 1;
        }
        else {
            ratio = (value - (3 * Amp_interval - 1)) / Amp_interval;
        }
        color = interpolateColors(Qt::yellow, Qt::red, ratio);
    }
    return color;
}

QColor DataProcess::valueToColor01(int value)
{
    //qreal ratio;
    //QColor startColor, endColor;

    //if (value <= 63) {
    //    // ��ɫ����ɫ����
    //    startColor = Qt::white;
    //    endColor = customBlue;
    //    ratio = static_cast<qreal>(value) / 63.0;
    //}
    //else if (value <= 127) {
    //    // ��ɫ����ɫ����
    //    startColor = customBlue;
    //    endColor = Qt::green;
    //    ratio = static_cast<qreal>(value - 63) / 64.0;
    //}
    //else if (value <= 191) {
    //    // ��ɫ����ɫ����
    //    startColor = Qt::green;
    //    endColor = Qt::yellow;
    //    ratio = static_cast<qreal>(value - 127) / 64.0;
    //}
    //else {
    //    // ��ɫ����ɫ����
    //    startColor = Qt::yellow;
    //    endColor = Qt::red;
    //    ratio = static_cast<qreal>(value - 191) / 64.0;
    //}

    //// ������ɫ��ֵ
    //int r = startColor.red() + ratio * (endColor.red() - startColor.red());
    //int g = startColor.green() + ratio * (endColor.green() - startColor.green());
    //int b = startColor.blue() + ratio * (endColor.blue() - startColor.blue());

    //return QColor(r, g, b);


    // Ԥ�ȶ���ÿ���׶ε���ʼ��ɫ��������ɫ�Ͷ�Ӧ�ı���
    struct ColorTransition {
        QColor startColor;
        QColor endColor;
        qreal ratio;
    };

    static const ColorTransition transitions[] = {
        { Qt::white, customBlue, 1.0 / 63.0 },        // 0-63
        { customBlue, Qt::green, 1.0 / 64.0 },        // 64-127
        { Qt::green, Qt::yellow, 1.0 / 64.0 },        // 128-191
        { Qt::yellow, Qt::red, 1.0 / 64.0 }           // 192-255
    };

    // ȷ����ǰ�׶ε���ʼ��ɫ��������ɫ�ͱ���
    const ColorTransition& transition = transitions[value / 64];

    qreal ratio = (value % 64) * transition.ratio;

    // ������ɫ��ֵ
    int r = transition.startColor.red() + ratio * (transition.endColor.red() - transition.startColor.red());
    int g = transition.startColor.green() + ratio * (transition.endColor.green() - transition.startColor.green());
    int b = transition.startColor.blue() + ratio * (transition.endColor.blue() - transition.startColor.blue());

    return QColor(r, g, b);
}

DataProcess::~DataProcess()
{}
