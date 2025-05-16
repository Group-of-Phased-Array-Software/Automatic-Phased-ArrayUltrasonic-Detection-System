#include "mouseeventlabel.h"

MouseEventLabel::MouseEventLabel(QWidget* parent)
	: QLabel(parent)
{
	//�������׷��
	setMouseTracking(true);
}

//��갴ѹ�¼�
void MouseEventLabel::mousePressEvent(QMouseEvent* ev)
{
	emit mousePressd(ev->x());
}

//��껬���¼�
void MouseEventLabel::mouseMoveEvent(QMouseEvent* ev)
{
	emit mouseMoved(ev->x(), ev->y());
}

//�������¼�
void MouseEventLabel::wheelEvent(QWheelEvent* ev)
{
	emit mousewheel(ev->angleDelta().y());//������������ƶ��ľ���
}

//���˫���¼�
void MouseEventLabel::mouseDoubleClickEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	emit doubleClicked();
}

MouseEventLabel::~MouseEventLabel()
{}
