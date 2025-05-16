#pragma once

#include <QObject>
#include<QLabel>
#include<qevent.h>
#include<qwidget.h>
#include<QWheelEvent>

//����¼���
class MouseEventLabel : public QLabel
{
	Q_OBJECT

public:
	MouseEventLabel(QWidget* parent);
	~MouseEventLabel();

	//��갴ѹ�¼�
	virtual void mousePressEvent(QMouseEvent* ev);

	//��껬���¼�
	virtual void mouseMoveEvent(QMouseEvent* ev);

	//�������¼�
	virtual void wheelEvent(QWheelEvent* ev);

	//���˫���¼�
	virtual void mouseDoubleClickEvent(QMouseEvent* event);

signals:
	void mousePressd(int x);
	void mousewheel(int x);
	void doubleClicked();
	void mouseMoved(int x, int y);
};
