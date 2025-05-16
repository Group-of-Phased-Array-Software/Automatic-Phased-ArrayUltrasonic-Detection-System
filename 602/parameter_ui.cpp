#include "parameter_ui.h"

Parameter_ui::Parameter_ui(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    //��ʼ�����ؼ�
    set_ui_position();
}

//��ʼ�����ؼ�λ��
void Parameter_ui::set_ui_position()
{
    setWindowTitle("Scan set");
    setFixedSize(600, 350);
    ui.tabWidget->setGeometry(0, 0, 600, 350);

    //���������ҳ
    ui.Pa_22s_groupBox->setGeometry(10, 10, 280, 190);
    ui.linescan_rule_groupBox->setGeometry(300, 10, 280, 290);
    ui.scan_model_groupBox->setGeometry(10, 220, 280, 80);
    //���õ�λ
    /*ui.pitch_doubleSpinBox->setSuffix(" mm");
    ui.focus_angle_doubleSpinBox->setSuffix(" ��");
    ui.focus_distance_doubleSpinBox->setSuffix(" mm");
    ui.sound_velocity_doubleSpinBox->setSuffix(" m/s");*/
    //��start_elm_spinBox��ֵ�仯ʱ��end_elm_spinBox��ֵҪ��Ӧ��+1
    /*connect(ui.start_elm_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        ui.end_elm_spinBox->setValue(value + 1);
        });*/

        //ɨ�����ҳ
    ui.scanpatameter_widget->showFullScreen();

    //������������
    ui.dgain_doubleSpinBox->setRange(0, 500);
    dgain = 30.0;
    ui.dgain_doubleSpinBox->setValue(dgain);
    connect(ui.dgain_doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double value) {
        dgain = value;
        emit setdgain(dgain);
        });

    //���÷����ѹ
    ui.volt_spinBox->setRange(0, 31);
    volt = 1;
    ui.volt_spinBox->setValue(volt);
    connect(ui.volt_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        volt = value;
        emit setvolt(volt);
        });

    //����������
    ui.pulse_width_spinBox->setRange(10, 300);
    ui.pulse_width_spinBox->setSingleStep(10);
    pulse_width = 150;
    ui.pulse_width_spinBox->setValue(pulse_width);
    connect(ui.pulse_width_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        pulse_width = value;
        emit setpulsewidth(pulse_width);
        });

    //��������1
    ui.vga1_spinBox->setRange(0, 500);
    vga1 = 100;
    ui.vga1_spinBox->setValue(vga1);
    connect(ui.vga1_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        vga1 = value;
        emit setvga(vga1, vga2);
        });

    //��������2
    ui.vga2_spinBox->setRange(0, 500);
    vga2 = 50;
    ui.vga2_spinBox->setValue(vga2);
    connect(ui.vga2_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        vga2 = value;
        emit setvga(vga1, vga2);
        });

    //�����ظ�Ƶ��
    ui.prf_spinBox->setRange(500, 3000);
    ui.prf_spinBox->setSingleStep(100);
    prf = 2000;
    ui.prf_spinBox->setValue(prf);
    connect(ui.prf_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        prf = value;
        emit setprf(prf);
        });

    //����ѹ����
    ui.range_ratio_doubleSpinBox->setRange(0.01, 1);
    ui.range_ratio_doubleSpinBox->setSingleStep(0.01);
    range_ratio = 0.118;
    ui.range_ratio_doubleSpinBox->setValue(range_ratio);
    connect(ui.range_ratio_doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double value)
        {
            range_ratio = value;
            emit setrangeratio(range_ratio);
        });

    //���������˲���0 ~ 15
    ui.dfilter_spinBox->setRange(0, 15);
    dfilter = 0;
    ui.dfilter_spinBox->setValue(dfilter);
    connect(ui.dfilter_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        dfilter = value;
        emit setdfilter(dfilter);
        });

    //������Ƶ�첨��0~4
    ui.video_detect_spinBox->setRange(0, 4);
    video_detect = 2;
    ui.video_detect_spinBox->setValue(video_detect);
    connect(ui.dfilter_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        video_detect = value;
        emit setvideodetect(video_detect);
        });

    //��ɨ����
    //��Ԫ����
    ui.elm_num_spinBox->setRange(1, 128);
    myline_rule.elm_num = 64;
    ui.elm_num_spinBox->setValue(myline_rule.elm_num);
    connect(ui.elm_num_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        myline_rule.elm_num = value;
        emit setpalscan(myline_rule);
        });

    //��Ԫ���(mm)
    ui.pitch_doubleSpinBox->setRange(0.1, 3.5);
    myline_rule.pitch = 0.6;
    ui.pitch_doubleSpinBox->setValue(myline_rule.pitch);
    connect(ui.pitch_doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double value)
        {
            myline_rule.pitch = value;
            emit setpalscan(myline_rule);
        });

    //�׾���С
    ui.aperture_size_spinBox->setRange(1, myline_rule.elm_num);
    myline_rule.aperture_size = 29;

    ui.aperture_size_spinBox->setValue(myline_rule.aperture_size);
    connect(ui.aperture_size_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        myline_rule.aperture_size = value;
        emit setpalscan(myline_rule);
        });

    //�۽��Ƕ�
    ui.focus_angle_doubleSpinBox->setRange(0, 180);
    myline_rule.focus_angle = 0;
    ui.focus_angle_doubleSpinBox->setValue(myline_rule.focus_angle);
    connect(ui.focus_angle_doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double value)
        {
            myline_rule.focus_angle = value;
            emit setpalscan(myline_rule);
        });

    //�۽�����(mm)
    ui.focus_distance_doubleSpinBox->setRange(0, 100);
    myline_rule.focus_distance = 0;
    ui.focus_angle_doubleSpinBox->setValue(myline_rule.focus_distance);
    connect(ui.focus_distance_doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double value)
        {
            myline_rule.focus_distance = value;
            emit setpalscan(myline_rule);
        });

    //��������(m/s)
    ui.sound_velocity_doubleSpinBox->setRange(1, 10000);
    myline_rule.sound_velocity = 5900;
    ui.sound_velocity_doubleSpinBox->setValue(myline_rule.sound_velocity);
    connect(ui.sound_velocity_doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=](double value)
        {
            myline_rule.sound_velocity = value;
            emit setpalscan(myline_rule);
        });

    //��ʼ��������Ԫ
    ui.start_elm_spinBox->setRange(1, 127);
    ui.end_elm_spinBox->setRange(2, 128);
    myline_rule.start_elm = 1;
    myline_rule.end_elm = 64;
    ui.start_elm_spinBox->setValue(1);
    ui.end_elm_spinBox->setValue(64);
    connect(ui.start_elm_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        myline_rule.start_elm = value;
        emit setpalscan(myline_rule);
        });
    connect(ui.end_elm_spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        myline_rule.end_elm = value;
        emit setpalscan(myline_rule);
        });

    //����������
    connect(ui.encoder_btn, &QPushButton::clicked, [=]() {
        emit resetencoder();
        });

    //բ�Ű�ť
    //��ʱ��ť
    ui.ys_horizontalScrollBar->setSingleStep(10);
    ui.ys_horizontalScrollBar->setRange(0, 200);
    connect(ui.ys_horizontalScrollBar, &QScrollBar::valueChanged, [=](int value) {
        ys = value;
        QString str = QString("��ʱ: %1").arg(value);
        ui.ys_label->setText(str);
        send_ys_door(ys, top_door, bot_door, left_door, right_door);
        });
    //��
    ui.top_horizontalScrollBar->setRange(0, bot_door);
    ui.top_horizontalScrollBar->setValue(top_door);
    connect(ui.top_horizontalScrollBar, &QScrollBar::valueChanged, [=](int value) {
        top_door = value;
        ui.top_label->setText(QString("�ϣ�%1").arg(value));
        ui.bot_horizontalScrollBar->setRange(top_door + minwidth, B_Scan_label_height);
        send_ys_door(ys, top_door, bot_door, left_door, right_door);
        });
    //��
    ui.bot_horizontalScrollBar->setRange(top_door, B_Scan_label_height);
    ui.bot_horizontalScrollBar->setValue(bot_door);
    connect(ui.bot_horizontalScrollBar, &QScrollBar::valueChanged, [=](int value) {
        bot_door = value;
        ui.bot_label->setText(QString("�£�%1").arg(value));
        ui.top_horizontalScrollBar->setRange(0, bot_door - minwidth);
        send_ys_door(ys, top_door, bot_door, left_door, right_door);
        });
    //��
    ui.left_horizontalScrollBar->setRange(0, right_door);
    ui.left_horizontalScrollBar->setValue(left_door);
    connect(ui.left_horizontalScrollBar, &QScrollBar::valueChanged, [=](int value) {
        left_door = value;
        ui.left_label->setText(QString("��%1").arg(value));
        ui.right_horizontalScrollBar->setRange(left_door + minwidth, B_Scan_label_width);
        send_ys_door(ys, top_door, bot_door, left_door, right_door);
        });
    //��
    ui.right_horizontalScrollBar->setRange(left_door, B_Scan_label_width);
    ui.right_horizontalScrollBar->setValue(right_door);
    connect(ui.right_horizontalScrollBar, &QScrollBar::valueChanged, [=](int value) {
        right_door = value;
        ui.right_label->setText(QString("�ң�%1").arg(value));
        ui.left_horizontalScrollBar->setRange(0, right_door - minwidth);
        send_ys_door(ys, top_door, bot_door, left_door, right_door);
        });

    //���´���
    connect(ui.reprocess_pushButton, &QPushButton::clicked, [=]() {
        this->close();
        qDebug() << "�����";
        //emit reprocess_CImage();
        });

    //������ɨ�龫�ȣ�X������1mm��22s������λ��Ϊ144
    ui.encoder_accuracy_comboBox->setCurrentIndex(1);//Ĭ�Ͼ���Ϊ0.5mm
    connect(ui.encoder_accuracy_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int dex) {
        switch (dex)
        {
        case 0:
            emit send_encoder_accuracy(0.2 * 144);
            break;
        case 1:
            emit send_encoder_accuracy(0.5 * 144);
            break;
        case 2:
            emit send_encoder_accuracy(1 * 144);
            break;
        case 3:
            emit send_encoder_accuracy(2 * 144);
            break;
        default:
            break;
        }
        });

    //������������
    //�̱�ˮ������
    ui.S_water_pushButton->setCheckable(true);
    connect(ui.S_water_pushButton, &QPushButton::toggled, [=](bool status) {
        if (status) {
            dmc_write_outbit(0, 0, 0);
            ui.S_water_pushButton->setText("�̱�ˮ����");
        }
        else {
            dmc_write_outbit(0, 0, 1);
            ui.S_water_pushButton->setText("�̱�ˮ����");
        }
        });

    //����ˮ������
    ui.L_water_pushButton->setCheckable(true);
    connect(ui.L_water_pushButton, &QPushButton::toggled, [=](bool status) {
        if (status) {
            dmc_write_outbit(0, 1, 0);
            ui.L_water_pushButton->setText("����ˮ����");
        }
        else {
            dmc_write_outbit(0, 1, 1);
            ui.L_water_pushButton->setText("����ˮ����");
        }
        });

    //R��ˮ������
    ui.R_water_pushButton->setEnabled(false);//����R��ˮ����ť
    ui.R_water_pushButton->setCheckable(true);
    connect(ui.R_water_pushButton, &QPushButton::toggled, [=](bool status) {
        if (status) {
            dmc_write_outbit(0, 2, 0);
            ui.R_water_pushButton->setText("R��ˮ����");
        }
        else {
            dmc_write_outbit(0, 2, 1);
            ui.R_water_pushButton->setText("R��ˮ����");
        }
        });

    //��ˮ�ÿ���
    ui.water_turbo_pushButton->setCheckable(true);
    connect(ui.water_turbo_pushButton, &QPushButton::toggled, [=](bool status) {
        if (status) {
            dmc_write_outbit(0, 3, 0);
            ui.water_turbo_pushButton->setText("��ˮ�ù�");
        }
        else {
            dmc_write_outbit(0, 3, 1);


            ui.water_turbo_pushButton->setText("��ˮ�ÿ�");
        }
        });

    //ɨ��·�����
    ui.tableWidget->resizeRowsToContents();//���������ݴ�С
    ui.tableWidget->setColumnCount(5);//��������
    ui.tableWidget->setRowCount(0);//������������ͷ�����������
    ui.tableWidget->horizontalHeader()->setDefaultSectionSize(200);//����ͷ��С
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//����������Ӧ���
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);//Ȼ������Ҫ��������ʹ�ÿ�ȵ���

    //���ñ���ͷ������
    QStringList header;
    header << tr("���Ƶ���") << tr("X������") << tr("Y������") << tr("Z������") << tr("����Ƕ�");
    ui.tableWidget->setHorizontalHeaderLabels(header);

    //���ñ���ͷ��������ʽ
    QFont font = ui.tableWidget->horizontalHeader()->font();
    font.setBold(true);
    ui.tableWidget->horizontalHeader()->setFont(font);

    ui.tableWidget->horizontalHeader()->setStretchLastSection(true); //���ó�������
    ui.tableWidget->verticalHeader()->setDefaultSectionSize(10); //�����о�
    ui.tableWidget->setFrameShape(QFrame::NoFrame); //�����ޱ߿�
    ui.tableWidget->setShowGrid(true); //���ò���ʾ������
    ui.tableWidget->verticalHeader()->setVisible(false); //�����к���,trueΪ��ʾ
    ui.tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection); //�ɶ�ѡ��Ctrl��Shift�� Ctrl+A�����ԣ�
    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //����ѡ����Ϊʱÿ��ѡ��һ��
    ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ò��ɱ༭
    ui.tableWidget->horizontalHeader()->resizeSection(0, 100);//���ñ�ͷ��һ�еĿ��Ϊ100
    ui.tableWidget->horizontalHeader()->setFixedHeight(30); //���ñ�ͷ�ĸ߶�
    ui.tableWidget->setStyleSheet("selection-background-color:lightblue;"); //����ѡ�б���ɫ
    ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:white;}"); //���ñ�ͷ����ɫ
    ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//ʹ4�б��ȿ�

    //����ˮƽ����ֱ��������ʽ,���ͷ�ļ� #include <QScrollBar>
    ui.tableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
        "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
        "QScrollBar::handle:hover{background:gray;}"
        "QScrollBar::sub-line{background:transparent;}"
        "QScrollBar::add-line{background:transparent;}");
    ui.tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
        "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
        "QScrollBar::handle:hover{background:gray;}"
        "QScrollBar::sub-line{background:transparent;}"
        "QScrollBar::add-line{background:transparent;}");

    ui.tableWidget->clearContents();//���������������������ݣ����ǲ������ͷ
}

//���ع���
void Parameter_ui::readTechnique()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Save Binary File", QDir::currentPath() + "/ɨ�鹤��", "TEC Files (*.tec)");
    if (filePath.isEmpty())
    {
        qDebug() << "User canceled the operation.";
        return;
    }

    QFile file(filePath);

    QFileInfo fileInfo(file);
    qint64 fileSize = fileInfo.size(); // ��ȡ�ļ���С����λΪ�ֽ�

    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);

        in >> dgain;//��������
        in >> volt;//�����ѹ
        in >> pulse_width;//������
        in >> vga1;//����1
        in >> vga2;//����2
        in >> prf;//�ظ�Ƶ��
        in >> range_ratio;//ѹ����
        in >> dfilter;//�����˲�
        in >> video_detect;//��Ƶ�첨
        in >> myline_rule.elm_num;//��Ԫ��
        in >> myline_rule.pitch;//��Ԫ���
        in >> myline_rule.aperture_size;//�׾���С
        in >> myline_rule.focus_angle;//�۽��Ƕ�
        in >> myline_rule.focus_distance;//�۽�����
        in >> myline_rule.sound_velocity;//��������
        in >> myline_rule.start_elm;//��ʼ��Ԫ
        in >> myline_rule.end_elm;//������Ԫ
        in >> top_door;
        in >> bot_door;
        in >> left_door;
        in >> right_door;
        in >> ys;

        file.close();
    }
    else
        return;

    ui.dgain_doubleSpinBox->setValue(dgain);
    ui.volt_spinBox->setValue(volt);
    ui.pulse_width_spinBox->setValue(pulse_width);
    ui.vga1_spinBox->setValue(vga1);
    ui.vga2_spinBox->setValue(vga2);
    ui.prf_spinBox->setValue(prf);
    ui.range_ratio_doubleSpinBox->setValue(range_ratio);
    ui.dfilter_spinBox->setValue(dfilter);
    ui.video_detect_spinBox->setValue(video_detect);
    ui.elm_num_spinBox->setValue(myline_rule.elm_num);
    ui.pitch_doubleSpinBox->setValue(myline_rule.pitch);
    ui.aperture_size_spinBox->setValue(myline_rule.aperture_size);
    ui.focus_angle_doubleSpinBox->setValue(myline_rule.focus_angle);
    ui.focus_distance_doubleSpinBox->setValue(myline_rule.focus_distance);
    ui.sound_velocity_doubleSpinBox->setValue(myline_rule.sound_velocity);
    ui.start_elm_spinBox->setValue(myline_rule.start_elm);
    ui.end_elm_spinBox->setValue(myline_rule.end_elm);
    ui.top_horizontalScrollBar->setValue(top_door);
    ui.bot_horizontalScrollBar->setValue(bot_door);
    ui.left_horizontalScrollBar->setValue(left_door);
    ui.right_horizontalScrollBar->setValue(right_door);
    ui.ys_horizontalScrollBar->setValue(ys);

    QMessageBox msgBox;
    msgBox.setText("���ռ�����ɣ�");
    msgBox.setWindowTitle("��ʾ");
    msgBox.setWindowFlags(msgBox.windowFlags() | Qt::WindowStaysOnTopHint);
    msgBox.exec();

}

//���湤��
void Parameter_ui::saveTechnique()
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Binary File", QDir::currentPath() + "/ɨ�鹤��", "TEC Files (*.tec)");
    if (filePath.isEmpty())
    {
        qDebug() << "User canceled the operation.";
        return;
    }
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);

        out << dgain;//��������
        out << volt;//�����ѹ
        out << pulse_width;//������
        out << vga1;//����1
        out << vga2;//����2
        out << prf;//�ظ�Ƶ��
        out << range_ratio;//ѹ����
        out << dfilter;//�����˲�
        out << video_detect;//��Ƶ�첨
        out << myline_rule.elm_num;//��Ԫ��
        out << myline_rule.pitch;//��Ԫ���
        out << myline_rule.aperture_size;//�׾���С
        out << myline_rule.focus_angle;//�۽��Ƕ�
        out << myline_rule.focus_distance;//�۽�����
        out << myline_rule.sound_velocity;//��������
        out << myline_rule.start_elm;//��ʼ��Ԫ
        out << myline_rule.end_elm;//������Ԫ
        out << top_door;
        out << bot_door;
        out << left_door;
        out << right_door;
        out << ys;
        file.close();

        QMessageBox msgBox;
        msgBox.setText("���ձ���ɹ���");
        msgBox.setWindowTitle("��ʾ");
        msgBox.setWindowFlags(msgBox.windowFlags() | Qt::WindowStaysOnTopHint);
        msgBox.exec();
    }
}

//�½�ɨ��·��
void Parameter_ui::creat_path()
{
    ui.tableWidget->clearContents(); // ������е�Ԫ���е�����
    ui.tableWidget->setRowCount(0); // ��������������Ϊ�㣬�Ӷ�ɾ��������
}

//��ӿ��Ƶ�
void Parameter_ui::add_pos(double x, double y, double z, double angle)
{
    int curRow = ui.tableWidget->rowCount();
    ui.tableWidget->insertRow(curRow);//�ڱ��β�����һ��

    //�ڱ�����������
    ui.tableWidget->setItem(curRow, 0, new QTableWidgetItem(QString::number(curRow + 1)));
    ui.tableWidget->setItem(curRow, 1, new QTableWidgetItem(QString::number(x)));
    ui.tableWidget->setItem(curRow, 2, new QTableWidgetItem(QString::number(y)));
    ui.tableWidget->setItem(curRow, 3, new QTableWidgetItem(QString::number(z)));
    ui.tableWidget->setItem(curRow, 4, new QTableWidgetItem(QString::number(angle)));

}

//ɾ�����Ƶ�
void Parameter_ui::delete_pos()
{
    int curRow = ui.tableWidget->currentRow();
    ui.tableWidget->removeRow(curRow); //ɾ����ǰ�м���items

    //ɾ�����Ƶ����Ҫ���¿��Ƶ���
    int allRow = ui.tableWidget->rowCount();//������
    for (int i = 0; i < allRow; i++)
        ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
}

//���ؿ��Ƶ�
void Parameter_ui::read_path()
{
    QVector<QVector<double>>pos_Vector;

    QString scanpath_filepath = QFileDialog::getOpenFileName(nullptr, "Save Binary File", QDir::currentPath() + "/ɨ��·��", "Binary Files (*.bin)");
    if (scanpath_filepath.isEmpty())
    {
        qDebug() << "User canceled the operation.";
        return;
    }

    ////����·��֮���Զ�ɨ�鰴ťΪ����״̬
    //ui.Scan_pushButton->setEnabled(true);

    QFile file(scanpath_filepath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open temp file for reading:" << file.errorString();
        file.close();
        return;
    }

    QDataStream in(&file);

    int allRow;
    int pathmodel_SLR;
    in >> allRow;
    in >> pathmodel_SLR;

    for (int row = 0; row < allRow; row++)
    {
        QVector<double>temp_Vector;
        for (int column = 1; column < 5; column++)
        {
            double tempvalue;
            in >> tempvalue;
            temp_Vector.append(tempvalue);
        }
        pos_Vector.append(temp_Vector);
    }

    ui.tableWidget->setColumnCount(5);//��������
    ui.tableWidget->setRowCount(allRow);//������������ͷ�����������
    for (int i = 0; i < pos_Vector.size(); i++)
    {
        ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        for (int j = 0; j < pos_Vector[i].size(); j++)
        {
            qDebug() << "�� " << i + 1 << "���Ƶ㣬�� " << j + 1 << "��ֵΪ��" << pos_Vector[i][j];
            ui.tableWidget->setItem(i, j + 1, new QTableWidgetItem(QString::number(pos_Vector[i][j])));
        }
    }

    file.close();
    emit send_path(pathmodel_SLR, pos_Vector);
}

//������Ƶ�
void Parameter_ui::save_path()
{
    int allRow = ui.tableWidget->rowCount();//��ȡ���Ƶ�����
    int temppath;
    //��ȡ���������ʲô·��
    QMessageBox msgBox;
    msgBox.setText("Choose an path:");
    msgBox.addButton("S_path", QMessageBox::ActionRole);
    msgBox.addButton("L_path", QMessageBox::ActionRole);
    msgBox.addButton("R_path", QMessageBox::ActionRole);
    // ������Ϣ�����ڶ�����ʾ
    msgBox.setWindowFlags(msgBox.windowFlags() | Qt::WindowStaysOnTopHint);
    msgBox.exec();

    // ��ȡ�û�ѡ��İ�ť
    QAbstractButton* selectedButton = msgBox.clickedButton();
    if (selectedButton) {
        QString buttonText = selectedButton->text();
        if (buttonText == "S_path") {
            temppath = 1;
        }
        else if (buttonText == "L_path") {
            temppath = 2;
        }
        else if (buttonText == "R_path") {
            temppath = 3;
        }
    }

    QString tempfilePath = QFileDialog::getSaveFileName(nullptr, "Save Binary File", QDir::currentPath() + "/ɨ��·��", "Binary Files (*.bin)");
    if (tempfilePath.isEmpty())
    {
        qDebug() << "User canceled the operation.";
        return;
    }

    QFile file(tempfilePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to open temp file for reading:" << file.errorString();
        file.close();
        return;
    }

    QDataStream out(&file);

    out << allRow;//��¼�ж��ٸ����Ƶ�
    out << temppath;//����ɨ��ģʽ

    for (int row = 0; row < allRow; row++)
    {
        for (int column = 1; column < 5; column++)
        {
            QTableWidgetItem* item = ui.tableWidget->item(row, column);
            QString cellText = item->text();
            double value = cellText.toDouble();
            out << value;
        }
    }

    file.close();
}

Parameter_ui::~Parameter_ui()
{}
