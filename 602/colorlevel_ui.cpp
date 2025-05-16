#include "colorlevel_ui.h"

ColorLevel_ui::ColorLevel_ui(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.amp_interval_doubleSpinBox->setSingleStep(1.0);
	ui.amp_interval_doubleSpinBox->setValue(64.0);
	ui.amp_interval_doubleSpinBox->setRange(20.0, 100.0);
	connect(ui.app_amp_interval_pushButton, &QPushButton::clicked, [=]() {
		double value = ui.amp_interval_doubleSpinBox->value();
		send_amp_interval(value);
		this->close();
		});
}

ColorLevel_ui::~ColorLevel_ui()
{}
