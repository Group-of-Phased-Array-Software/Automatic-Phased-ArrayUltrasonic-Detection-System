#pragma once

#include <QWidget>
#include "ui_colorlevel_ui.h"

class ColorLevel_ui : public QWidget
{
	Q_OBJECT

public:
	ColorLevel_ui(QWidget *parent = nullptr);
	~ColorLevel_ui();

private:
	Ui::ColorLevel_uiClass ui;

signals:
	void send_amp_interval(double);
};
