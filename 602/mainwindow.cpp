#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //��ʼ�����ؼ�λ��
    set_ui_position();

    //�����Ӵ���
    motion_ui = new Motion_ui;
    parameter_ui = new Parameter_ui;
    colorlevel_ui = new ColorLevel_ui;
    //�ò�������ui,�˶�����uiͼ����ʾλ�ڶ���
    parameter_ui->setWindowFlags(Qt::WindowStaysOnTopHint);
    motion_ui->setWindowFlags(Qt::WindowStaysOnTopHint);
    colorlevel_ui->setWindowFlags(Qt::WindowStaysOnTopHint); 

    //�������ģ��
    Servo_model = new Servocontrol;
    //22s����ģ��
    Pa22s_model = new Pa22scontrol;

    //�˶������߳�
    Motionthread = new Motioncontrol;
    QThread* motionobjectthread = new QThread;
    Motionthread->moveToThread(motionobjectthread);
    motionobjectthread->start();
    //���ݴ����߳�
    Dataprocessthread = new DataProcess;//����ָ�������� 
    QThread* dataobjectthread = new QThread;
    Dataprocessthread->moveToThread(dataobjectthread);
    dataobjectthread->start();
    //���ݱ����߳�
    Datasavethread = new DataSave;
    QThread* datasaveobjectthread = new QThread;
    Datasavethread->moveToThread(datasaveobjectthread);
    datasaveobjectthread->start();


    //�����˶����ƴ��ڸ��ؼ�
    connectmotion_ui();
    //����ɨ�����ô��ڸ��ؼ�
    connectparatemer_ui();
}

//��ʼ�����ؼ�λ��
void MainWindow::set_ui_position()
{
    //ui�ؼ�λ�ô�С����
    setFixedSize(1300, 900);
    setWindowTitle("��������������ϵͳ");

    //ͼ����ʾLabel
    ui.A_Scan_widget->setGeometry(20, 10, 290, 600);
    ui.B_Scan_label->setGeometry(320, 10, 800, 600);
    ui.C_Scan_label->setGeometry(20, 620, 1080, 240);//�����Ϊ1080
    ui.color_label->setGeometry(1101, 620, 20, 240);

    //GroupBox
    ui.control_groupBox->setGeometry(1140, 10, 140, 320);
    ui.live_status_groupBox->setGeometry(1140, 350, 140, 510);

    //��������ť������ui��ƽ����н���ť���뵽��Ӧ��QGroupBox��
    ui.Scan_pushButton->setGeometry(10, 20, 120, 40);
    ui.parameter_set_pushButton->setGeometry(10, 70, 120, 40);
    ui.motion_set_pushButton->setGeometry(10, 120, 120, 40);
    ui.save_data_pushButton->setGeometry(10, 170, 120, 40);
    ui.read_data_pushButton->setGeometry(10, 220, 120, 40);
    ui.stop_pushButton->setGeometry(10, 270, 120, 40);
    ui.stop_pushButton->setStyleSheet("background-color: pink;");

    //״̬���ؼ�
    ui.scms_label->setGeometry(10, 20, 111, 31);
    ui.scmsxs_label->setGeometry(10, 50, 111, 31);
    ui.yscjl_label->setGeometry(10, 90, 111, 31);
    ui.yscjlxs_label->setGeometry(10, 120, 111, 31);
    ui.sfzt_label->setGeometry(10, 160, 111, 31);
    ui.sfztxs_label->setGeometry(10, 190, 111, 31);
    ui.djzt_label->setGeometry(10, 230, 111, 31);
    ui.djztxs_label->setGeometry(10, 260, 111, 31);
    ui.xkzwd_label->setGeometry(10, 300, 111, 31);
    ui.xkzwdxs_label->setGeometry(10, 330, 111, 31);

}

//����ɨ�����ô��ڸ��ؼ�
void MainWindow::connectparatemer_ui()
{
    //ɨ�����ô�����ʾ
    connect(ui.parameter_set_pushButton, &QPushButton::clicked, [=]() {
        parameter_ui->setWindowFlags(Qt::WindowStaysOnTopHint);
        parameter_ui->show();
        parameter_ui->ui.tabWidget->setCurrentIndex(0);
        });
    //B_Label˫������ɨ������
    connect(ui.B_Scan_label, &MouseEventLabel::doubleClicked, [=]() {
        if (!parameter_ui->isVisible())
            parameter_ui->show();
        parameter_ui->ui.tabWidget->setCurrentIndex(2);
        });

    //��ض�ʱ��
    refreshtimer = new QTimer(this);
    TimeCscantimer = new QTimer(this);
    temperaturetimer = new QTimer(this);

    //�¶ȶ�ʱ������
    connect(temperaturetimer, &QTimer::timeout, [=]() {
        ui.xkzwdxs_label->setText(Pa22s_model->gettemprature()+"��C");
        });

    //ɨ��ģʽѡ��
    //Ĭ�϶̱�ɨ��
    scanmodel_SLR = 1;
    parameter_ui->ui.short_radioButton->setChecked(true);
    ui.scmsxs_label->setText(QString("�̱�ɨ��"));
    connect(parameter_ui->ui.short_radioButton, &QRadioButton::clicked, [=]() {
        scanmodel_SLR = 1;
        ui.scmsxs_label->setText(QString("�̱�ɨ��"));
        });
    connect(parameter_ui->ui.long_radioButton, &QRadioButton::clicked, [=]() {
        scanmodel_SLR = 2;
        ui.scmsxs_label->setText(QString("����ɨ��"));
        });
    connect(parameter_ui->ui.R_radioButton, &QRadioButton::clicked, [=]() {
        scanmodel_SLR = 3;
        ui.scmsxs_label->setText(QString("R��ɨ��"));
        });
    //ɨ��ģʽ  0-������  1-ʱ��
    parameter_ui->ui.scanmodel_comboBox->setCurrentIndex(0);
    connect(parameter_ui->ui.scanmodel_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int dex) {
        scanmodel = dex;
        });
    // ������������ 0 - ����  1 - ����
    parameter_ui->ui.encodermodel_comboBox->setCurrentIndex(0);
    connect(parameter_ui->ui.encodermodel_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int dex) {
        Dataprocessthread->encoderdex = dex;
        });

    //�������ָ��
    //����Pa22s
    connect(parameter_ui->ui.connect_to_22s_pushButton, &QPushButton::clicked, [=]() {
        Pa22s_model->connect_Pa22s();
        temperaturetimer->start(10000);
        });
    //�Ͽ���Pa22s������
    connect(parameter_ui->ui.disconnect_to_22s_pushButton, &QPushButton::clicked, [=]() {
        Pa22s_model->disconnect_Pa22s();
        temperaturetimer->stop();
        });
    //�ػ�
    connect(parameter_ui->ui.close_22s_pushButton, &QPushButton::clicked, [=]() {
        Pa22s_model->turn_off_Pa22s();
        });
    //ɨ���źſ�
    connect(parameter_ui->ui.open_22s_signal_pushButton, &QPushButton::clicked, [=]() {
        Pa22s_model->open_signal();
        refreshtimer->start(30);
        B_LabelMouseStatus = true;
        });
    //ɨ���źŹ�
    connect(parameter_ui->ui.close_22s_signal_pushButton, &QPushButton::clicked, [=]() {
        Pa22s_model->close_signal();
        refreshtimer->stop();
        B_LabelMouseStatus = false;
        });


    //������������
    connect(parameter_ui, &Parameter_ui::setdgain, Pa22s_model, &Pa22scontrol::setdgain);
    //���÷����ѹ
    connect(parameter_ui, &Parameter_ui::setvolt, Pa22s_model, &Pa22scontrol::setvolt);
    //����������
    connect(parameter_ui, &Parameter_ui::setpulsewidth, Pa22s_model, &Pa22scontrol::setpulsewidth);
    //��������1������2
    connect(parameter_ui, &Parameter_ui::setvga, Pa22s_model, &Pa22scontrol::setvga);
    //�����ظ�Ƶ��
    connect(parameter_ui, &Parameter_ui::setprf, Pa22s_model, &Pa22scontrol::setprf);
    //����ѹ����
    connect(parameter_ui, &Parameter_ui::setrangeratio, Pa22s_model, &Pa22scontrol::setrangeratio);
    //���������˲���0 ~ 15
    connect(parameter_ui, &Parameter_ui::setdfilter, Pa22s_model, &Pa22scontrol::setdfilter);
    //������Ƶ�첨��0~4    //��Ƶ�첨�޷�����
    connect(parameter_ui, &Parameter_ui::setvideodetect, Pa22s_model, &Pa22scontrol::setvideodetect);
    //������ɨ����
    connect(parameter_ui, &Parameter_ui::setpalscan, Pa22s_model, &Pa22scontrol::setpalscan);
    //����������
    connect(parameter_ui, &Parameter_ui::resetencoder, Pa22s_model, &Pa22scontrol::resetencoder);



    // aɨ���γ�ʼ��
    chart = new QChart;
    Xaixs = new QValueAxis;
    Yaixs = new QValueAxis;
    chart->addAxis(Xaixs, Qt::AlignBottom);
    chart->addAxis(Yaixs, Qt::AlignLeft);
    Yaixs->setRange(0, 256);
    Xaixs->setRange(0, 200);//Ϊʲô���400�����ڴ�ռ�þͺ�����
    Xaixs->hide();
    Yaixs->hide();
    chart->legend()->hide();
    ui.A_Scan_widget->setChart(chart);
    ui.A_Scan_widget->rotate(90);//��ת90����ʾ 

    //Bɨբ����أ�բ�ſ��ʼ��
    top_door = 200;
    bot_door = 400;
    left_door = 0;
    right_door = ui.B_Scan_label->width();
    BScanDoor = QRect(left_door, top_door, right_door - left_door, bot_door - top_door);
    BScanDoorPixmap = QPixmap(ui.B_Scan_label->size());
    BScanDoorPixmap.fill(Qt::transparent);//͸������
    BScanPainter.begin(&BScanDoorPixmap);//���û��ƵĶ���
    BScanPainter.setPen(Qt::red);
    BScanPainter.drawRect(BScanDoor);
    BScanPainter.end();

    //��ʼɨ��
    //��ʼɨ��/����ɨ��
    ui.Scan_pushButton->setCheckable(true);
    ui.Scan_pushButton->setEnabled(false);//δѡ��ɨ��·��ǰ�����޷����°�ť
    connect(ui.Scan_pushButton, &QPushButton::toggled, [=](bool status) {
        if (status)//��ť���ڰ���״̬������ɨ��
        {
            ui.Scan_pushButton->setText("����ɨ��");
            ui.Scan_pushButton->setStyleSheet("background-color: red;");
            Dataprocessthread->Cscandex = 0;
            Dataprocessthread->CImage = QImage(36000, Dataprocessthread->CImageHeight, QImage::Format_RGB32);//�ȿ��ߣ�ȷ����ʱCɨͼ��Ĵ�С
            Dataprocessthread->CImage.fill(Qt::white);//������ɫ
            ui.C_Scan_label->setPixmap(QPixmap::fromImage(Dataprocessthread->CscaledImage));//ǿ������ת�� 
            Pa22s_model->open_signal();
            B_LabelMouseStatus = true;
            C_LabelMouseStatus = false;
            data_status = false;
            emit startsave(Dataprocessthread->signal_num, top_door, bot_door, left_door, right_door, ys, Dataprocessthread->encoder_accuracy);//������Ϊ����������
            if (scanmodel == 0) {//������ɨ��
                Pa22s_model->resetencoder();
                Dataprocessthread->EncoderCscandex = 0;//ɨ����󣬱�����λ�ò����Ҷ�
                Dataprocessthread->encoderlastpos = 0;//��������ʷλ�ù���
                Dataprocessthread->encoder_first_scanstatus = true;
                Dataprocessthread->EncoderCscanstatus = true;
            }
            else {//ʱ��ɨ��
                //���߳�Cɨ�������˴������趨ʱ��Cɨ����
                TimeCscantimer->start(20);
                TimeCscanstatus = true;
            }
            //�����Զ�ɨ��
            emit start_auto_scan(pos_Vector, pathmodel_SLR);

        }
        else//��ť�ָ�������ɨ��
        {
            //ֹͣ���˶�
            //����ֹͣ
            Motionthread->stop_all_axis();
            Motionthread->axis_stopstatus = true;

            //�ر�ʵʱBɨ��ֹͣ���ݲɼ�
            Pa22s_model->close_signal();
            refreshtimer->stop();//ʱ��
            ui.Scan_pushButton->setText("��ʼɨ��");
            ui.Scan_pushButton->setStyleSheet("");
            B_LabelMouseStatus = false;

            if (scanmodel == 0) {
                Dataprocessthread->EncoderCscanstatus = false;//������
                Pa22s_model->resetencoder();//������
            }
            else {
                TimeCscantimer->stop();//ʱ��
                TimeCscanstatus = false;//ʱ��
            }
            //֪ͨ���߳�ֹͣ�洢����
            emit stopsave(Dataprocessthread->Cscandex, Dataprocessthread->signal_num);

            //�滻��ʾ��ͼƬ
            CImagelength = Dataprocessthread->Cscandex + 1;
            CImage = Dataprocessthread->CImage.copy(0, 0, CImagelength/*ʵ����Ƭ����*/, Dataprocessthread->CImage.height());//��ͼ����
            if (Dataprocessthread->Cscandex >= ui.C_Scan_label->width())
            {
                showCImagedex = Dataprocessthread->Cscandex - ui.C_Scan_label->width() + 1;//��Ƭ��ʼ��ʾ�������൱��CImagelength-ui.C_Scan_label->width()
                //����CImagelength=1000��ui.C_Scan_label->width()=1000ʱ�����뼴���Ƶ�����Ӧ��ϵ��
                QImage copyImage = CImage.copy(showCImagedex, 0, ui.C_Scan_label->width(), CImage.height());
                CScaledImage = copyImage.scaled(ui.C_Scan_label->width(), ui.C_Scan_label->height());
                ui.C_Scan_label->setPixmap(QPixmap::fromImage(CScaledImage));//ǿ������ת��
            }
            else
            {
                QImage copyImage = CImage.copy(0, 0, CImagelength, CImage.height());
                CScaledImage = copyImage.scaled(copyImage.width(), ui.C_Scan_label->height());
                ui.C_Scan_label->setPixmap(QPixmap::fromImage(CScaledImage));//ǿ������ת��
            }
        }
        });

    //�ֶ�ɨ��
    parameter_ui->ui.start_Mt_scan_pushButton->setCheckable(true);
    connect(parameter_ui->ui.start_Mt_scan_pushButton, &QPushButton::toggled, [=](bool status) {
        if (status)//��ť���ڰ���״̬������ɨ��
        {
            parameter_ui->ui.start_Mt_scan_pushButton->setText("����ɨ��");
            parameter_ui->ui.start_Mt_scan_pushButton->setStyleSheet("background-color: red;");
            Dataprocessthread->Cscandex = 0;
            Dataprocessthread->CImage = QImage(36000, Dataprocessthread->CImageHeight, QImage::Format_RGB32);//�ȿ��ߣ�ȷ����ʱCɨͼ��Ĵ�С
            Dataprocessthread->CImage.fill(Qt::white);//������ɫ
            ui.C_Scan_label->setPixmap(QPixmap::fromImage(Dataprocessthread->CscaledImage));//ǿ������ת�� 
            Pa22s_model->open_signal();
            B_LabelMouseStatus = true;
            C_LabelMouseStatus = false;
            data_status = false;
            emit startsave(Dataprocessthread->signal_num, top_door, bot_door, left_door, right_door, ys,Dataprocessthread->encoder_accuracy);
            if (scanmodel == 0) {//������ɨ��
                Pa22s_model->resetencoder();
                Dataprocessthread->EncoderCscandex = 0;//ɨ����󣬱�����λ�ò����Ҷ�
                Dataprocessthread->encoderlastpos = 0;//��������ʷλ�ù���
                Dataprocessthread->encoder_first_scanstatus = true;
                Dataprocessthread->EncoderCscanstatus = true;
            }
        }
        else//��ť�ָ�������ɨ��
        {
            //�ر�ʵʱBɨ��ֹͣ���ݲɼ�
            Pa22s_model->close_signal();
            refreshtimer->stop();//ʱ��
            parameter_ui->ui.start_Mt_scan_pushButton->setText("��ʼɨ��");
            parameter_ui->ui.start_Mt_scan_pushButton->setStyleSheet("");
            B_LabelMouseStatus = false;

            if (scanmodel == 0) {
                Dataprocessthread->EncoderCscanstatus = false;//������
                Pa22s_model->resetencoder();//������
            }
            //֪ͨ���߳�ֹͣ�洢����
            emit stopsave(Dataprocessthread->Cscandex, Dataprocessthread->signal_num);

            //�滻��ʾ��ͼƬ
            CImagelength = Dataprocessthread->Cscandex + 1;
            CImage = Dataprocessthread->CImage.copy(0, 0, CImagelength/*ʵ����Ƭ����*/, Dataprocessthread->CImage.height());//��ͼ����
            if (Dataprocessthread->Cscandex >= ui.C_Scan_label->width())
            {
                showCImagedex = Dataprocessthread->Cscandex - ui.C_Scan_label->width() + 1;//��Ƭ��ʼ��ʾ�������൱��CImagelength-ui.C_Scan_label->width()
                //����CImagelength=1000��ui.C_Scan_label->width()=1000ʱ�����뼴���Ƶ�����Ӧ��ϵ��
                QImage copyImage = CImage.copy(showCImagedex, 0, ui.C_Scan_label->width(), CImage.height());
                CScaledImage = copyImage.scaled(ui.C_Scan_label->width(), ui.C_Scan_label->height());
                ui.C_Scan_label->setPixmap(QPixmap::fromImage(CScaledImage));//ǿ������ת��
            }
            else
            {
                QImage copyImage = CImage.copy(0, 0, CImagelength, CImage.height());
                CScaledImage = copyImage.scaled(copyImage.width(), ui.C_Scan_label->height());
                ui.C_Scan_label->setPixmap(QPixmap::fromImage(CScaledImage));//ǿ������ת��
            }
        }
        });

    //��ʾˢ�¶�ʱ��
    connect(refreshtimer, &QTimer::timeout, [=]() {
        emit startshowBscan();//���߳��н���Bɨ���ݴ���
        //showLiveAscan();//��ʾAɨ����

        // ��ʾբ�ſ򣬽���ʱ��QPixmap���ӵ�BImage��
        QImage scaledimage = BImage.scaled(ui.B_Scan_label->width(), ui.B_Scan_label->height());
        QPixmap scaledImagePixmap = QPixmap::fromImage(scaledimage);
        QPainter combinedPainter(&scaledImagePixmap);
        combinedPainter.drawPixmap(0, 0, BScanDoorPixmap);
        // ��QLabel����ʾ���Ӻ��ͼƬ
        ui.B_Scan_label->setPixmap(scaledImagePixmap);

        //��Cɨδ��ʼʱ��������ˢ��
        if (TimeCscanstatus || Dataprocessthread->EncoderCscanstatus) {
            //��ͼ����зŴ�
            QImage scaledimage = CImage.scaled(Dataprocessthread->CscaledImage.width(), ui.C_Scan_label->height());
            ui.C_Scan_label->setPixmap(QPixmap::fromImage(scaledimage));
        }
        });

    //ʱ��Cɨ��ʱ��
    connect(TimeCscantimer, &QTimer::timeout, [=]() {
        Dataprocessthread->Cscandex++;
        emit startshowCscan();
        });

    //����ɨ������
    connect(ui.save_data_pushButton, &QPushButton::clicked, [=]() {
        QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Binary File", QDir::currentPath() + "/ɨ������", "Binary Files (*.bin)");
        emit savedata(filePath);
        });
    //��ȡɨ������
    connect(ui.read_data_pushButton, &QPushButton::clicked, [=]() {
        data_status = true;
        read_data();
        });
    //���ع���
    connect(parameter_ui->ui.readTechnique_pushButton, &QPushButton::clicked, [=]() {
        Pa22s_model->close_signal();
        parameter_ui->readTechnique();
        Pa22s_model->open_signal();
        });
    //���湤��
    connect(parameter_ui->ui.saveTechnique_pushButton, &QPushButton::clicked, [=]() {
        parameter_ui->saveTechnique();
        });
    //���´���ť
    connect(parameter_ui->ui.reprocess_pushButton, &QPushButton::clicked, [=]() {
        if (data_status)
            showCImage();
        });
    //�½�ɨ��·��
    connect(parameter_ui->ui.creat_path_pushButton, &QPushButton::clicked, [=]() {
        parameter_ui->creat_path();
        });
    //��ӿ��Ƶ�
    connect(parameter_ui->ui.add_pos_pushButton, &QPushButton::clicked, [=]() {
        switch (scanmodel_SLR)
        {
        case 1:
            parameter_ui->add_pos(Motionthread->get_axis_position(0), Motionthread->get_axis_position(2), Motionthread->get_axis_position(3), motion_ui->ui.S_angle_doubleSpinBox->value());
            break;
        case 2:
            parameter_ui->add_pos(Motionthread->get_axis_position(0), Motionthread->get_axis_position(2), Motionthread->get_axis_position(3), motion_ui->ui.L_angle_doubleSpinBox->value());
            break;
        case 3:
            parameter_ui->add_pos(Motionthread->get_axis_position(0), Motionthread->get_axis_position(2), Motionthread->get_axis_position(4), motion_ui->ui.R_angle_doubleSpinBox->value());
            break;
        default:
            break;
        }
        });
    //ɾ�����Ƶ�
    connect(parameter_ui->ui.delete_pos_pushButton, &QPushButton::clicked, [=]() {
        parameter_ui->delete_pos();
        });
    //���ؿ��Ƶ�
    connect(parameter_ui->ui.read_path_pushButton, &QPushButton::clicked, [=]() {
        parameter_ui->read_path();
        //����·��֮���Զ�ɨ�鰴ťΪ����״̬
        //ui.Scan_pushButton->setEnabled(true);
        });
    //������Ƶ�
    connect(parameter_ui->ui.save_path_pushButton, &QPushButton::clicked, [=]() {
        parameter_ui->save_path();
        });


    //�����߳�������Bɨ���ݴ���
    connect(this, &MainWindow::startshowBscan, Dataprocessthread, &DataProcess::showBscan);
    //�����߳�������ʱ��Cɨ
    connect(this, &MainWindow::startshowCscan, Dataprocessthread, &DataProcess::showCscan);
    //�����߳�������������Cɨ
    connect(Dataprocessthread, &DataProcess::startEncoderCscan, Dataprocessthread, &DataProcess::showCscan);
    //Dataprocess�����ݴ��ݵ�Datasave�н��б���
    qRegisterMetaType<QVector<int>>("QVector<int>"); //ע��QVector<int>���ͣ��������źŲ۴���
    connect(Dataprocessthread, &DataProcess::sendDataToSave, Datasavethread, &DataSave::savetempdata);
    connect(this, &MainWindow::startsave, Datasavethread, &DataSave::startsave);
    connect(this, &MainWindow::stopsave, Datasavethread, &DataSave::stopsave);
    connect(this, &MainWindow::savedata, Datasavethread, &DataSave::savedata);
    //���ݱ������
    connect(Datasavethread, &DataSave::progressUpdated, this, &MainWindow::show_savedata_progress);
    //22s���ݴ��ݵ�Dataprocess�߳�
    connect(Pa22s_model, &Pa22scontrol::sendData, Dataprocessthread, &DataProcess::getdata);
    //����ɨ�������ݴ�parameter_ui�д��ݵ�Dataprocess��,Line_ruleΪע�Ὣ������Ϊ�������źŲ��д���
    qRegisterMetaType<Line_rule>("Line_rule"); //ע��Line_rule���ͣ��������źŲ۴���
    connect(parameter_ui, &Parameter_ui::setpalscan, Dataprocessthread, &DataProcess::getLine_rule);
    //����Dataprocess�̵߳����ݣ�����ʾAɨ
    connect(Dataprocessthread, &DataProcess::sendAscanData, [=](QVector<int>data) {
        QVector<int>tempdata = data;//��tempdata��һ�����壬����AscanData�Ƚ������ݣ��ֶ�����
        // ʹ���ź���ۻ���ʱ��ȷ�������߳��н��� UI ����
        QMetaObject::invokeMethod(this, [=]() { 
            AscanData = tempdata; // �����ݸ�ֵ�����̵߳ĳ�Ա����
            showLiveAscan(); // �����߳��е�����ʾ����
            });
        });
    //����Dataprocess�߳��е�Bɨͼ
    connect(Dataprocessthread, &DataProcess::sendBscanImage, [=](QImage Image) {
        //BImage = Image;
        BImage = Image.copy(0, 0, Image.width(), Image.height());//����ֱ�Ӹ�ֵ��ֱ�Ӹ�ֵ�൱�����ã���Image����ʱ��BImage���޷����ʣ������ڴ���ʳ�ͻ
        //showLiveBCscan();
        });
    //����Dataprocess�߳��е�Cɨͼ
    connect(Dataprocessthread, &DataProcess::sendCscanImage, [=](QImage Image) {
        CImage = Image.copy(0, 0, Image.width(), Image.height());
        });
    //��ʱ��բ�Ÿı�ʱ��������Ӧ����
    connect(parameter_ui, &Parameter_ui::send_ys_door, this, &MainWindow::ys_door_changed);
    connect(parameter_ui, &Parameter_ui::send_ys_door, Dataprocessthread, &DataProcess::ys_door_changed);
    //���´���Cɨͼ��
    connect(parameter_ui, &Parameter_ui::reprocess_CImage, this, &MainWindow::showCImage);
    //���ݱ���������
    connect(parameter_ui, &Parameter_ui::send_encoder_accuracy, [=](double value) {
        Dataprocessthread->encoder_accuracy = value;
        });
    //ɨ��·������
    qRegisterMetaType<QVector<QVector<double>>>("QVector<QVector<double>>"); //ע��QVector<QVector<double>>����
    connect(parameter_ui, &Parameter_ui::send_path, [=](int path, QVector<QVector<double>>vector) {
        pathmodel_SLR = path;
        pos_Vector = vector;
        ui.Scan_pushButton->setEnabled(true);
        qDebug() << "�ٴν��������� ";
        });
    connect(this, &MainWindow::start_auto_scan, Motionthread, &Motioncontrol::auto_scan);//�����Զ�ɨ��
    connect(Motionthread, &Motioncontrol::start_duoji, Servo_model, &Servocontrol::writeToclient);//���ն�������ź�


    //��������¼�
    //�������ƶ��¼�
    connect(ui.C_Scan_label, &MouseEventLabel::mousewheel, this, [=](int value) {//valueΪ����ƶ��ľ���
        if (CImage.width() > ui.C_Scan_label->width() && !TimeCscantimer->isActive()) {//ֻ�е�Cɨ��������ͼƬ��ȴ���500ʱ
            for (int i = 0; i < 20; i++) {//��ͼƬ�ƶ�20��
                showCImage_usingwheel(value);
            }
        }
        });
    //����Bɨ��ʾͼx���꣬��ʾ�ڼ������ݵ�Aɨ
    connect(ui.B_Scan_label, &MouseEventLabel::mousePressd, this, [=](int x) {
        if (B_LabelMouseStatus)
        {
            if (refreshtimer->isActive())
            {
                LiveAscandex = (x / (ui.B_Scan_label->width() / Dataprocessthread->signal_num));//����ǵڼ�������
            }
            if (data_status)
            {
                data_AWavedex = (x / (ui.B_Scan_label->width() / data_signal_num));//����ǵڼ�������
                showAWave();
            }
        }
        });
    //���ն�ȡ����ʱCɨͼ���x��������ʾBɨͼ��
    data_BScandex = 0;
    connect(ui.C_Scan_label, &MouseEventLabel::mousePressd, this, [=](int value) {
        if (C_LabelMouseStatus)
        {
            data_BScandex = value;
            showBImage();
        }
        });
    //���ݶ�ȡʱ����ȡ����������Ϣ
    connect(ui.C_Scan_label, &MouseEventLabel::mouseMoved, this, [=](int x, int y) {
        if (C_LabelMouseStatus)
        {
            data_BScandex = x;
            //int dex = showCImagedex + data_BScandex;
            ui.data_xposition_label->setText(QString::number((x + showCImagedex)*data_encoder_accuracy) + "mm");
        }
        });
    //B_Label˫������ɨ������
    connect(ui.B_Scan_label, &MouseEventLabel::doubleClicked, [=]() {
        if (!parameter_ui->isVisible())
            parameter_ui->show();
        parameter_ui->ui.tabWidget->setCurrentIndex(2);
        });
    //color_label˫������ɨ������
    connect(ui.color_label, &MouseEventLabel::doubleClicked, [=]() {
        if (!colorlevel_ui->isVisible())
            colorlevel_ui->show();
        });

    set_color_icon();
    //��ɫ��ֵ����仯
    connect(colorlevel_ui, &ColorLevel_ui::send_amp_interval, [=](double value) {
        Dataprocessthread->Amp_interval = value;
        set_color_icon();
        });
}

//�����˶����ƴ��ڸ��ؼ�
void MainWindow::connectmotion_ui()
{
    // �˶����ƴ�����ʾ
    connect(ui.motion_set_pushButton, &QPushButton::clicked, [=]() {
        motion_ui->show();
        });

    //�ŷ��˶��ٶ�����
    //X
    motion_ui->ui.X_speed_spinBox->setValue(1);//��ʼ�ٶ�Ϊ1mm/s
    motion_ui->ui.X_speed_spinBox->setRange(1, 200);//�ٶȷ�Χ��1~200mm/s
    connect(motion_ui->ui.X_speed_spinBox, &QSpinBox::editingFinished, [=]() {
        X_speed = motion_ui->ui.X_speed_spinBox->value() * X_pitch;
        qDebug() << "X_speed= " << X_speed;
        });
    //Y
    motion_ui->ui.Y_speed_spinBox->setValue(Y_speed / Y_pitch);
    motion_ui->ui.Y_speed_spinBox->setRange(1, 20);//�ٶȷ�Χ��1~20mm/s
    connect(motion_ui->ui.Y_speed_spinBox, &QSpinBox::editingFinished, [=]() {
        Y_speed = motion_ui->ui.Y_speed_spinBox->value() * Y_pitch;
        qDebug() << "Y_speed= " << Y_speed;
        });
    //Z1
    motion_ui->ui.Z1_speed_spinBox->setValue(Z1_speed / Z1_pitch);
    motion_ui->ui.Z1_speed_spinBox->setRange(1, 10);//�ٶȷ�Χ��1~10mm/s
    connect(motion_ui->ui.Z1_speed_spinBox, &QSpinBox::editingFinished, [=]() {
        Z1_speed = motion_ui->ui.Z1_speed_spinBox->value() * Z1_pitch;
        qDebug() << "Z1_speed= " << Z1_speed;
        });
    //Z2
    motion_ui->ui.Z2_speed_spinBox->setValue(Z2_speed / Z2_pitch);
    motion_ui->ui.Z2_speed_spinBox->setRange(1, 10);//�ٶȷ�Χ��1~10mm/s
    connect(motion_ui->ui.Z2_speed_spinBox, &QSpinBox::editingFinished, [=]() {
        Z2_speed = motion_ui->ui.Z2_speed_spinBox->value() * Z2_pitch;
        qDebug() << "Z2_speed= " << Z2_speed;
        });

    //�ŷ��˶������ȡ
    //X
    motion_ui->ui.X_distance_doubleSpinBox->setValue(0);
    motion_ui->ui.X_distance_doubleSpinBox->setSingleStep(1);
    connect(motion_ui->ui.X_distance_doubleSpinBox, &QDoubleSpinBox::editingFinished, [=]() {
        X_distance = motion_ui->ui.X_distance_doubleSpinBox->value() * X_pitch;
        qDebug() << "X_distance= " << X_distance;
        });
    //Y
    motion_ui->ui.Y_distance_doubleSpinBox->setValue(0);
    motion_ui->ui.Y_distance_doubleSpinBox->setSingleStep(1);
    connect(motion_ui->ui.Y_distance_doubleSpinBox, &QDoubleSpinBox::editingFinished, [=]() {
        Y_distance = motion_ui->ui.Y_distance_doubleSpinBox->value() * Y_pitch;
        qDebug() << "Y_distance= " << Y_distance;
        });
    //Z1
    motion_ui->ui.Z1_distance_doubleSpinBox->setValue(0);
    motion_ui->ui.Z1_distance_doubleSpinBox->setSingleStep(1);
    connect(motion_ui->ui.Z1_distance_doubleSpinBox, &QDoubleSpinBox::editingFinished, [=]() {
        Z1_distance = motion_ui->ui.Z1_distance_doubleSpinBox->value() * Z1_pitch;
        qDebug() << "Z1_distance= " << Z1_distance;
        });
    //Z2
    motion_ui->ui.Z2_distance_doubleSpinBox->setValue(0);
    motion_ui->ui.Z2_distance_doubleSpinBox->setSingleStep(1);
    connect(motion_ui->ui.Z2_distance_doubleSpinBox, &QDoubleSpinBox::editingFinished, [=]() {
        Z2_distance = motion_ui->ui.Z2_distance_doubleSpinBox->value() * Z2_pitch;
        qDebug() << "Z2_distance= " << Z2_distance;
        });

    //��ȡ��λ����λ�ö�ʱ��
    read_axis_inport_timer = new QTimer(this);
    connect(read_axis_inport_timer, &QTimer::timeout, [=]() {
        //��ȡ��λ�����ź�
        read_axis_inport();
        //��ʾ��λ��
        motion_ui->ui.X_position_label->setText(QString("X��λ�ã�\n%1").arg(Motionthread->get_axis_position(0)));
        motion_ui->ui.Y_position_label->setText(QString("Y��λ�ã�\n%1").arg(Motionthread->get_axis_position(2)));
        motion_ui->ui.Z1_position_label->setText(QString("Z1��λ�ã�\n%1").arg(Motionthread->get_axis_position(3)));
        motion_ui->ui.Z2_position_label->setText(QString("Z2��λ�ã�\n%1").arg(Motionthread->get_axis_position(4)));

        });

    //�ȸ�λ
    connect(motion_ui->ui.reset_pushButton, &QPushButton::clicked, [=]() {
        Motionthread->axis_reset();
        });

    //ʹ��
    connect(motion_ui->ui.enable_pushButton, &QPushButton::clicked, [=]() {
        Motionthread->axis_enable();
        read_axis_inport_timer->start(50);
        });

    //ʧ��
    connect(motion_ui->ui.disable_pushButton, &QPushButton::clicked, [=]() {
        Motionthread->axis_disable();
        read_axis_inport_timer->stop();
        });

    //����ֹͣ
    connect(ui.stop_pushButton, &QPushButton::clicked, [=]() {
        Motionthread->stop_all_axis();
        Motionthread->axis_stopstatus = true;
        });

    //ԭ������
    connect(motion_ui->ui.setzeroposition_pushButton, &QPushButton::clicked, [=]() {
        Motionthread->set_axis_pos_zero();
        });

    //X���˶�
    //�����˶�,������λ��־Ҫ����,���������˶�(ʵ�ʷ����˶�)
    connect(motion_ui->ui.X_D_pushButton, &QPushButton::pressed, [=]() {
        if (!IO_0) {
            if (X_distance == 0) {//�����˶�
                Motionthread->X_axis_vmove(X_speed, 0);
            }
            else {//�����˶�
                Motionthread->X_axis_pmove(X_speed, (-1) * X_distance);
            }
        }
        });
    connect(motion_ui->ui.X_D_pushButton, &QPushButton::released, [=]() {
        Motionthread->X_axis_stop();
        });
    //�����˶����򶫷����˶���ʵ�������˶���
    connect(motion_ui->ui.X_R_pushButton, &QPushButton::pressed, [=]() {
        if (!IO_1) {
            if (X_distance == 0) {//�����˶�
                Motionthread->X_axis_vmove(X_speed, 1);
            }
            else {//�����˶�
                Motionthread->X_axis_pmove(X_speed, X_distance);
            }
        }
        });
    connect(motion_ui->ui.X_R_pushButton, &QPushButton::released, [=]() {
        Motionthread->X_axis_stop();
        });


    //Y��(2����)
    //�����˶�,����
    connect(motion_ui->ui.Y_D_pushButton, &QPushButton::pressed, [=]() {
        if (!IO_3) {
            //ʵ�ʷ���dir=0
            if (Y_distance == 0) {//�����˶�
                Motionthread->other_axis_vmove(2, Y_speed, 0);
            }
            else {//�����˶�
                Motionthread->other_axis_pmove(2, Y_speed, (-1) * Y_distance);
            }
        }
        });
    connect(motion_ui->ui.Y_D_pushButton, &QPushButton::released, [=]() {
        Motionthread->other_axis_stop(2);
        });
    //�����˶�������
    connect(motion_ui->ui.Y_R_pushButton, &QPushButton::pressed, [=]() {
        if (!IO_2) {
            //ʵ������dir=1        
            if (Y_distance == 0) {//�����˶�
                Motionthread->other_axis_vmove(2, Y_speed, 1);
            }
            else {//�����˶�
                Motionthread->other_axis_pmove(2, Y_speed, Y_distance);
            }
        }
        });
    connect(motion_ui->ui.Y_R_pushButton, &QPushButton::released, [=]() {
        Motionthread->other_axis_stop(2);
        });

    //Z1�ᣨ3���ᣩ
    //�����˶�������
    connect(motion_ui->ui.Z1_D_pushButton, &QPushButton::pressed, [=]() {
        if (!IO_4) {
            if (Z1_distance == 0) {//�����˶�
                Motionthread->other_axis_vmove(3, Z1_speed, 1);
                Z1_stopstatus = true;//������λֹͣ�󣬿����ٴ��˶����ָ���λ���״̬����Ҫ��㼸���˶��߳���λ��
            }
            else {//�����˶�
                Motionthread->other_axis_pmove(3, Z1_speed, Z1_distance);
                Z1_stopstatus = true;
            }
        }
        });
    connect(motion_ui->ui.Z1_D_pushButton, &QPushButton::released, [=]() {
        Motionthread->other_axis_stop(3);
        });
    //�����˶�������
    connect(motion_ui->ui.Z1_R_pushButton, &QPushButton::pressed, [=]() {
        if (!IO_5) {
            if (Z1_distance == 0) {//�����˶�
                Motionthread->other_axis_vmove(3, Z1_speed, 0);
                Z1_stopstatus = true;
            }
            else {//�����˶�
                Motionthread->other_axis_pmove(3, Z1_speed, (-1) * Z1_distance);
                Z1_stopstatus = true;
            }
        }
        });
    connect(motion_ui->ui.Z1_R_pushButton, &QPushButton::released, [=]() {
        Motionthread->other_axis_stop(3);
        });

    //Z2�ᣨ4���ᣩ
    //�����˶�������
    connect(motion_ui->ui.Z2_D_pushButton, &QPushButton::pressed, [=]() {
        if (!IO_6) {
            if (Z2_distance == 0) {//�����˶�
                Motionthread->other_axis_vmove(4, Z2_speed, 1);
            }
            else {//�����˶�
                Motionthread->other_axis_pmove(4, Z2_speed, Z2_distance);
            }
        }
        });
    connect(motion_ui->ui.Z2_D_pushButton, &QPushButton::released, [=]() {
        Motionthread->other_axis_stop(4);
        });
    //�����˶�������
    connect(motion_ui->ui.Z2_R_pushButton, &QPushButton::pressed, [=]() {
        if (!IO_7) {
            if (Z2_distance == 0) {//�����˶�
                Motionthread->other_axis_vmove(4, Z2_speed, 0);
            }
            else {//�����˶�
                Motionthread->other_axis_pmove(4, Z2_speed, (-1) * Z2_distance);
            }
        }
        });
    connect(motion_ui->ui.Z2_R_pushButton, &QPushButton::released, [=]() {
        Motionthread->other_axis_stop(4);
        });

    //��ʱ����ȡ���λ�ö�ʱ��
    read_servo_timer = new QTimer(this);

    connect(read_servo_timer, &QTimer::timeout, [=]() {
        switch (scanmodel_SLR)
        {
        case 1:
            motion_ui->ui.S_angle_label->setText(QString("ʵ��λ�ã�%1").arg(Servo_model->readToclient(1)));
            break;
        case 2:
            motion_ui->ui.L_angle_label->setText(QString("ʵ��λ�ã�%1").arg(Servo_model->readToclient(2)));
            break;
        case 3:
            motion_ui->ui.R_angle_label->setText(QString("ʵ��λ�ã�%1").arg(Servo_model->readToclient(3)));
            break;
        }
        });

    //���Ӷ��
    connect(motion_ui->ui.connect_duoji_pushButton, &QPushButton::clicked, [=]() {
        Servo_model->connectToservo();
        read_servo_timer->start(50);
        //���Ӷ�����趨��������ĳ�ʼֵ,�������޷��趨��ʼֵ������
        motion_ui->ui.S_duoji_horizontalSlider->setValue(Servo_model->readToclient(1) * (4095.0 / 360.0));
        motion_ui->ui.L_duoji_horizontalSlider->setValue(Servo_model->readToclient(2) * (4095.0 / 360.0));
        motion_ui->ui.R_duoji_horizontalSlider->setValue(Servo_model->readToclient(3) * (4095.0 / 360.0));

        });
    //�Ͽ��������
    connect(motion_ui->ui.disconnect_duoji_pushButton, &QPushButton::clicked, [=]() {
        Servo_model->disconnectToservo();
        read_servo_timer->stop();
        });

    //����ƶ�,�����Ҫ�ƶ��Ķ���뵱ǰɨ��ģʽ��ƥ�䣬������ƶ����
    connect(motion_ui, &Motion_ui::writeToclient, [=](unsigned int regStartAddr, unsigned int number, unsigned int serverAddress) {
        if (serverAddress == scanmodel_SLR)
            Servo_model->writeToclient(regStartAddr, number, serverAddress);
        });

}

//��ȡ��λ״̬������ֹͣ��־���ŷ�ʹ��ʱ�������ŷ�ʧ��ʱ�ر�
void MainWindow::read_axis_inport()
{
    IO_0 = dmc_read_inbit(0, 0);
    IO_1 = dmc_read_inbit(0, 1);
    IO_2 = dmc_read_inbit(0, 2);
    IO_3 = dmc_read_inbit(0, 3);
    IO_4 = dmc_read_inbit(0, 4);
    IO_5 = dmc_read_inbit(0, 5);
    IO_6 = dmc_read_inbit(0, 6);
    IO_7 = dmc_read_inbit(0, 7);

    //qDebug() << "Y����λ�����" << IO_3;
    if (IO_0 || IO_1)
    {
        Motionthread->X_axis_stop();
        //ֹͣ��־����
        Motionthread->axis_stopstatus = true;
        X_stopstatus = false;
    }

    if (IO_2 || IO_3)
    {
        Motionthread->other_axis_stop(2);
        //ֹͣ��־����
        Motionthread->axis_stopstatus = true;
        Y_stopstatus = false;
    }

    if (IO_4 || IO_5)
    {
        if (Z1_stopstatus)
        {
            Motionthread->other_axis_stop(3);
            Motionthread->axis_stopstatus = true;
            Z1_stopstatus = false;//ֹͣһ�κ�
        }
        //ֹͣ��־����
        Motionthread->axis_stopstatus = true;
    }

    if (IO_6 || IO_7)
    {
        Motionthread->other_axis_stop(4);
        //ֹͣ��־����
        Motionthread->axis_stopstatus = true;
        Z2_stopstatus = false;
    }
}

//ʵʱAɨ
void MainWindow::showLiveAscan()
{
    if (AscanData.isEmpty()||(AscanData.size()!=Dataprocessthread->signal_num * 400))//AscanDataΪ400������Ϊһ����λ
        return;
    // ɾ��֮ǰ�� Series ����
    if (chart->series().count() > 0)
    {
        QAbstractSeries* oldSeries = chart->series().at(0);//����һ��QSplineSeries����ĵ�ַ���ݸ�oldSeries
        chart->removeSeries(oldSeries);
        delete oldSeries;
    }
    QSplineSeries* Series = new QSplineSeries;
    for (int i = ys; i < 200 + ys; i++)
    {
        Series->append(i, AscanData[i + LiveAscandex * 400]);
    }
    chart->addSeries(Series);//��������ߣ��ٸ����߰�����
    Series->attachAxis(Xaixs);//�Ȱ�����Ļ�������ʾ���߲���QChart��
    Series->attachAxis(Yaixs);
}

//ʵʱBɨ
void MainWindow::showLiveBCscan()
{
    // ��ʾբ�ſ򣬽���ʱ��QPixmap���ӵ�BImage��
    QImage scaledimage = BImage.scaled(ui.B_Scan_label->width(), ui.B_Scan_label->height());
    QPixmap scaledImagePixmap = QPixmap::fromImage(scaledimage);
    QPainter combinedPainter(&scaledImagePixmap);
    combinedPainter.drawPixmap(0, 0, BScanDoorPixmap);
    // ��QLabel����ʾ���Ӻ��ͼƬ
    ui.B_Scan_label->setPixmap(scaledImagePixmap);

    //��Cɨδ��ʼʱ��������ˢ��
    if (TimeCscanstatus || Dataprocessthread->EncoderCscanstatus) {
        //��ͼ����зŴ�
        QImage scaledimage = CImage.scaled(Dataprocessthread->CscaledImage.width(), ui.C_Scan_label->height());
        ui.C_Scan_label->setPixmap(QPixmap::fromImage(scaledimage));
    }
}

//��ʱ��բ�Ÿı�ʱ����Ӧ�Ĳ���
void MainWindow::ys_door_changed(int ui_ys, int ui_top_door, int ui_bot_door, int ui_left_door, int ui_right_door)
{
    ys = ui_ys;
    top_door = ui_top_door;
    bot_door = ui_bot_door;
    left_door = ui_left_door;
    right_door = ui_right_door;
    Xaixs->setRange(ys, 200 + ys);//���귶Χ������Ӧ�ı�
    //�ı�բ�ſ��С
    BScanDoorPixmap.fill(Qt::transparent);
    BScanDoor.setRect(left_door, top_door, right_door - left_door, bot_door - top_door);
    BScanPainter.begin(&BScanDoorPixmap);//���û��ƵĶ���
    BScanPainter.setPen(Qt::red);
    BScanPainter.drawRect(BScanDoor);
    BScanPainter.end();
    //���ݶ�ȡʱ����ӦA,Bͼ�����ˢ��
    if (data_status)
    {
        showAWave();//����ʱ�ı�ʱ��ˢ��Aɨ
        showBImage();
    }
}

//�鿴Cɨ�������Cɨͼ�񣨿����ù���ǰ���ƶ��鿴��
void MainWindow::showCImage_usingwheel(int value)
{
    //ʱ��ɨ��ʱ����һ��TimeCImagedex,��ͼƬ�ĳ��ĳ���500ʱ�����dex��������ȡͼƬ����ʼxֵ
    if (value > 0)//������ת��ͼƬ�����ƶ�
    {
        //showCImagedex��ȡͼƬ����ʼxֵ�����ֵ����С������ͼƬ��βxֵ-499�����򣬽��ڴ���ʳ�ͻ
        if (showCImagedex < CImagelength - ui.C_Scan_label->width())//Dataprocessthread->Cscandex+1=����Cɨͼ��ĳ���
        {
            QImage copyImage = CImage.copy(showCImagedex + 1, 0, ui.C_Scan_label->width(), CImage.height());
            CScaledImage = copyImage.scaled(ui.C_Scan_label->width(), ui.C_Scan_label->height());
            ui.C_Scan_label->setPixmap(QPixmap::fromImage(CScaledImage));
            showCImagedex = showCImagedex + 1;
        }
    }
    else//���ַ�ת��ͼƬ�����ƶ�
    {
        if (showCImagedex > 0)
        {
            QImage copyImage = CImage.copy(showCImagedex - 1, 0, ui.C_Scan_label->width(), CImage.height());
            CScaledImage = copyImage.scaled(ui.C_Scan_label->width(), ui.C_Scan_label->height());
            ui.C_Scan_label->setPixmap(QPixmap::fromImage(CScaledImage));
            showCImagedex = showCImagedex - 1;
        }
    }
}

//��ʾ�������
void MainWindow::show_savedata_progress(int percentage)
{
    // ��״̬������ʾ�������
    statusBar()->showMessage("���ݱ������: " + QString::number(percentage) + "%", 5000);
    //QProgressBar* existingProgressBar = findChild<QProgressBar*>("saveProgressBar");
    //if (existingProgressBar) {
    //    existingProgressBar->setValue(percentage);  // �������еĽ�������ֵ
    //}
    //else {
    //    // ����һ���µĽ�����
    //    QProgressBar* progressBar = new QProgressBar();
    //    progressBar->setObjectName("saveProgressBar");
    //    progressBar->setRange(0, 100);
    //    progressBar->setValue(percentage);

    //    // ����һ�������������ı��ͽ�����
    //    QWidget* container = new QWidget();
    //    QHBoxLayout* layout = new QHBoxLayout(container);
    //    layout->addWidget(new QLabel("���ݱ������: "));  // ����ı���ǩ
    //    layout->addWidget(progressBar);  // ��ӽ�����
    //    layout->setContentsMargins(10, 0, 0, 0);  // ���ñ߾�Ϊ0

    //    // ��״̬��������µĽ����� - ����������
    //    statusBar()->insertWidget(0, container);

    //    if (percentage == 100)
    //    {
    //        QTimer::singleShot(2000, this, [=]() {
    //            /*QStatusBar* bar = statusBar();
    //            QList<QWidget*> widgets = bar->findChildren<QWidget*>();
    //            for (QWidget* widget : widgets) {
    //                bar->removeWidget(widget);
    //                delete widget;*/
    //            //statusBar()->removeAction();
    //    });
    //}
    //}
}

//��ȡ����
void MainWindow::read_data()
{
    filePath = QFileDialog::getOpenFileName(nullptr, "Save Binary File", QDir::currentPath() + "/ɨ������", "Binary Files (*.bin)");
    if (filePath.isEmpty())
    {
        qDebug() << "User canceled the operation.";
        return;
    }

    B_LabelMouseStatus = true;
    C_LabelMouseStatus = true;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open temp file for reading:" << file.errorString();
        file.close();
        return;
    }

    int tempencoder;
//��9��10λΪռλ���ݣ����Բ���ȡ
    QDataStream in(&file);
    in >> data_Clength;
    in >> data_signal_num;
    in >> top_door;
    in >> bot_door;
    in >> left_door;
    in >> right_door;
    in >> ys;
    in >> tempencoder;
    file.close();

    data_encoder_accuracy = tempencoder / 144.0;//����һ��ϵ�����õ���ʵ�ı���������
    qDebug() << "true encoder_accuracy= " << data_encoder_accuracy;
    CImagelength = data_Clength;//�൱������Cɨͼ��ĳ��� 

    //�����Slider��ֵ����
    parameter_ui->ui.top_horizontalScrollBar->setValue(top_door);
    parameter_ui->ui.bot_horizontalScrollBar->setValue(bot_door);
    parameter_ui->ui.left_horizontalScrollBar->setValue(left_door);
    parameter_ui->ui.right_horizontalScrollBar->setValue(right_door);
    parameter_ui->ui.ys_horizontalScrollBar->setValue(ys);
    //��������series�е�x����
    Xaixs->setRange(ys, 200 + ys);

    showCImagedex = 0;//��ʾCɨͼƬ����ʼ����

    showCImage();//�Ƚ������������ݣ����һ��CImage;
    showBImage();
    showAWave();
}

//��ȡ���������ݣ�����ʾ��Aɨ����
void MainWindow::showAWave()
{
    //�ڼ���Bɨ����
    int dex = showCImagedex + data_BScandex;//2�ǷŴ���
    QVector<int>data;
    qint64 startPosition = (10 + dex * data_signal_num * 400 + data_AWavedex * 400) * sizeof(int);//10Ϊ����10int  ֱ�ӽ�ȫ��������ȡ����400������
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        if (!file.seek(startPosition))
        {
            qDebug() << "Failed to seek to the start position.";
            file.close();
            return;
        }
        QDataStream in(&file);
        int i = 0;
        while (i < 400)
        {
            int value;
            in >> value; // ��ȡһ������
            data.append(value);
            i++;
        }
    }
    file.close();
    //��ʾA��
    if (chart->series().count() > 0)
    {
        QAbstractSeries* oldSeries = chart->series().at(0);
        chart->removeSeries(oldSeries);
        delete oldSeries;
    }
    QSplineSeries* Series = new QSplineSeries;
    for (int i = ys; i < 200 + ys; i++)
    {
        Series->append(i, data[i]);
    }
    chart->addSeries(Series);
    Series->attachAxis(Xaixs);
    Series->attachAxis(Yaixs);
}

//��ȡ���������ݣ�������Bɨ����
void MainWindow::showBImage()
{
    QVector<int>BScanData;
    //�ڼ���Bɨ����
    int dex = showCImagedex + data_BScandex;
    QImage Bimage((data_signal_num + (data_signal_num - 1) * 9), 200, QImage::Format_RGB32);//ȡ200����
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        for (int x = 0; x < data_signal_num; x++)
        {
            QVector<int>data;
            qint64 startPosition = (10 + dex * data_signal_num * 400 + x * 400) * sizeof(int);//ֱ�ӽ�ȫ��������ȡ����400������
            if (!file.seek(startPosition))
            {
                qDebug() << "Failed to seek to the start position.";
                file.close();
                return;
            }
            QDataStream in(&file);
            int i = 0;
            while (i < 400)//����ys��ʱ���ܣ���ȫ���������
            {
                int value;
                in >> value; // ��ȡһ������
                data.append(value);
                i++;
            }
            for (int y = 0; y < 200; y++)
            {
                int value = data[y + ys];//������ʱ����
                BScanData.append(value);
                Bimage.setPixelColor(x * 10, y, Dataprocessthread->valuetocolor(value));
            }
        }
    }
    file.close();

    //��Ӳ�ֵ��
    for (int i = 0; i < data_signal_num - 1; i++)
    {
        for (int x = 1; x < 10; x++)
        {
            for (int y = 0; y < 200; y++)
            {
                int value = BScanData[y + 200 * i] * (10 - x) / 10 + BScanData[y + 200 * i + 200] * x / 10;//int���ܹ���������
                Bimage.setPixelColor(x + i * 10, y, Dataprocessthread->valuetocolor(value));
            }
        }
    }

    //BScanDoorPixmap.fill(Qt::transparent);
    //BScanDoor.setRect(left_door, top_door, right_door - left_door, bot_door - top_door);
    //BScanPainter.begin(&BScanDoorPixmap);//���û��ƵĶ���
    //BScanPainter.setPen(Qt::red);
    //BScanPainter.drawRect(BScanDoor);
    //BScanPainter.end();
    // ����ʱ��QPixmap���ӵ�BscaledImage��   //ͼ������
    //BscaledImage = Bimage.scaled(signal_num * 15, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QImage BscaledImage = Bimage.scaled(ui.B_Scan_label->width(), ui.B_Scan_label->height());//ƽ������
    //QImage BscaledImage = Bimage.scaled(ui.B_Scan_label->width(), ui.B_Scan_label->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap scaledImagePixmap = QPixmap::fromImage(BscaledImage);
    QPainter combinedPainter(&scaledImagePixmap);
    combinedPainter.drawPixmap(0, 0, BScanDoorPixmap);
    // ��QLabel����ʾ���Ӻ��ͼƬ
    ui.B_Scan_label->setPixmap(scaledImagePixmap);
    //qDebug() << "������showdatabscan";

}

//��ȡ���������ݣ�������Cɨ����
void MainWindow::showCImage()
{
    qDebug() << "������";
    data_CscanStartdex = left_door / (ui.B_Scan_label->width() / data_signal_num);
    data_CscanStopdex = right_door / (ui.B_Scan_label->width() / data_signal_num);
    data_CImageHeight = data_CscanStopdex - data_CscanStartdex;//����բ�ŵ����ҿ�ȣ�����м������ݽ���Cɨ����ԭ����+1��
    //������ʱ��Cɨͼ��ÿ�������ʼ�㣬��400���㣬�ӵ�100���㿪ʼѡ�����ֵ����
    data_startdex = top_door / (ui.B_Scan_label->height() / 200) + ys;
    int data_signallength = bot_door / (ui.B_Scan_label->height() / 200) - top_door / (ui.B_Scan_label->height() / 200);//Cɨ����Χ���

    CImage = QImage(data_Clength, data_CImageHeight, QImage::Format_RGB32);
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        int dex = 1;
        for (int x = 0; x < data_Clength; x++)
        {
            for (int y = 0; y < data_CImageHeight; y++)
            {
                QVector<int>data;
                qint64 startPosition = (10 + x * data_signal_num * 400 + (y + data_CscanStartdex) * 400 + (data_startdex - 1)) * sizeof(int);//ȡ�����е�40~80λ���г���
                if (!file.seek(startPosition))
                {
                    qDebug() << "Failed to seek to the start position.";
                    file.close();
                    return;
                }
                // ��ȡ����
                QDataStream in(&file);
                int i = 0;
                while (i < data_signallength)
                {
                    int value;
                    in >> value; // ��ȡһ������
                    data.append(value);
                    i++;
                }
                //��ȡ���ֵ����ͼ��
                int  value = *std::max_element(data.constBegin(), data.constEnd());
                CImage.setPixelColor(x, y, Dataprocessthread->valuetocolor(value));
            }
            //���ݶ�ȡ������ʾ
            if (((x * 100) / data_Clength) == dex)
            {
                dex++;
                statusBar()->showMessage("���ݶ�ȡ����: " + QString::number(dex) + "%", 5000);
                qDebug() << "dex=" << dex;
                // �����¼���ȷ��״̬���ĸ���
                qApp->processEvents();//���� qApp->processEvents(); �������߳����������¼������е��¼������������ý��漴ʱ���£�������ʾ״̬���ĸ�����Ϣ
            }
        }
    }
    file.close();
    //��ȡ��ͼ�񣬽�����ʾ
    if (data_Clength > ui.C_Scan_label->width())
    {
        QImage copyImage = CImage.copy(0, 0, ui.C_Scan_label->width(), CImage.height());
        CScaledImage = copyImage.scaled(ui.C_Scan_label->width(), ui.C_Scan_label->height());
        ui.C_Scan_label->setPixmap(QPixmap::fromImage(CScaledImage));//ǿ������ת��
    }
    else
    {
        QImage copyImage = CImage.copy(0, 0, CImage.width(), CImage.height());
        CScaledImage = copyImage.scaled(copyImage.width(), ui.C_Scan_label->height());
        ui.C_Scan_label->setPixmap(QPixmap::fromImage(CScaledImage));
    }
}

//����ɫ��
void MainWindow::set_color_icon()
{
    QImage colorimage(1, 256, QImage::Format_RGB32);
    for (int i = 0; i < 256; i++) {
        colorimage.setPixelColor(0, (255-i), Dataprocessthread->valuetocolor(i));
    }
    QImage copyImage = colorimage.copy(0, 0, colorimage.width(), colorimage.height());
    QImage CScaledImage = copyImage.scaled(ui.color_label->width(), ui.color_label->height());
    ui.color_label->setPixmap(QPixmap::fromImage(CScaledImage));//ǿ������ת��
}

MainWindow::~MainWindow()
{}
