#include "motion_ui.h"

Motion_ui::Motion_ui(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    //��ʼ�����ؼ�λ��
    set_ui_position();
}

//��ʼ�����ؼ�λ��
void Motion_ui::set_ui_position()
{
    setWindowTitle("Motion control");
    setFixedSize(740, 350);

    //���group����
    ui.groupBox->setGeometry(10, 10, 420, 325);
    ui.groupBox_2->setGeometry(440, 10, 390, 325);

    ui.connect_duoji_pushButton->setGeometry(20, 20, 94, 28);
    ui.disconnect_duoji_pushButton->setGeometry(130, 20, 94, 28);
    ui.groupBox_3->setGeometry(10, 60, 270, 80);
    ui.groupBox_5->setGeometry(10, 150, 270, 80);
    ui.groupBox_6->setGeometry(10, 240, 270, 80);

    //����ƶ�
    //�趨��ؿؼ��ķ�Χ���Ƕ����ƽ�Ϊ500��Χ�����һȦ4095

    //�̱�,��Χ0~800
    ui.S_angle_doubleSpinBox->setRange(0, (360.0 / 4095.0) * 800);
    ui.S_angle_doubleSpinBox->setSingleStep(0.1);
    ui.S_angle_doubleSpinBox->setDecimals(1);//��ֵ��ʾС�����һλ
    ui.S_duoji_horizontalSlider->setRange(0, 800);
    //Slider�ؼ�
    connect(ui.S_duoji_horizontalSlider, &QSlider::valueChanged, [=](int value) {
        //����Ŀ��λ��
        double num = (360.0 / 4095.0) * value;
        ui.S_angle_doubleSpinBox->setValue(num);//��ȥ���м��ֵ
        //�ڵ�ַΪ128�ļĴ����еĵ�0��λ��д������
        emit writeToclient(128, value, 1);
        });
    //Spinbox�ؼ���ȷ������
    connect(ui.S_angle_doubleSpinBox, &QDoubleSpinBox::editingFinished, [=]() {
        ui.S_duoji_horizontalSlider->setValue(ui.S_angle_doubleSpinBox->value() * (4095.0 / 360.0));
        });


    //���ߣ��Ƕ�����Ϊ0~800
    ui.L_angle_doubleSpinBox->setRange(0, (360.0 / 4095.0) * 800);
    ui.L_angle_doubleSpinBox->setSingleStep(0.1);
    ui.L_angle_doubleSpinBox->setDecimals(1);//��ֵ��ʾС�����һλ
    ui.L_duoji_horizontalSlider->setRange(0, 800);
    //Slider�ؼ�
    connect(ui.L_duoji_horizontalSlider, &QSlider::valueChanged, [=](int value) {
        //����Ŀ��λ��
        double num = (360.0 / 4095.0) * value;
        ui.L_angle_doubleSpinBox->setValue(num);//��ȥ���м��ֵ
        //�ڵ�ַΪ128�ļĴ����еĵ�0��λ��д������
        emit writeToclient(128, value, 2);
        });
    //Spinbox�ؼ���ȷ�����밴ť
    connect(ui.L_angle_doubleSpinBox, &QDoubleSpinBox::editingFinished, [=]() {
        ui.L_duoji_horizontalSlider->setValue(ui.L_angle_doubleSpinBox->value() * (4095.0 / 360.0));
        });


    //R�� �Ƕ����Ʒ�Χ0~1000
    ui.R_angle_doubleSpinBox->setRange(0, (360.0 / 4095.0) * 1000);
    ui.R_angle_doubleSpinBox->setSingleStep(0.1);
    ui.R_angle_doubleSpinBox->setDecimals(1);//��ֵ��ʾС�����һλ
    ui.R_duoji_horizontalSlider->setRange(0, 1000);
    //Slider�ؼ�
    connect(ui.R_duoji_horizontalSlider, &QSlider::valueChanged, [=](int value) {

        //����Ŀ��λ��
        double num = (360.0 / 4095.0) * value;
        ui.R_angle_doubleSpinBox->setValue(num);//��ȥ���м��ֵ
        //�ڵ�ַΪ128�ļĴ����еĵ�0��λ��д������
        emit writeToclient(128, value, 3);

        });
    //Spinbox�ؼ���ȷ�����밴ť
    connect(ui.R_angle_doubleSpinBox, &QDoubleSpinBox::editingFinished, [=]() {
        ui.R_duoji_horizontalSlider->setValue(ui.R_angle_doubleSpinBox->value() * (4095.0 / 360.0));
        });
}

Motion_ui::~Motion_ui()
{}