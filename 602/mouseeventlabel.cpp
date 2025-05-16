#include "mouseeventlabel.h"

MouseEventLabel::MouseEventLabel(QWidget* parent)
	: QLabel(parent)
{
	//开启鼠标追踪
	setMouseTracking(true);
}

//鼠标按压事件
void MouseEventLabel::mousePressEvent(QMouseEvent* ev)
{
	emit mousePressd(ev->x());
}

//鼠标滑动事件
void MouseEventLabel::mouseMoveEvent(QMouseEvent* ev)
{
	emit mouseMoved(ev->x(), ev->y());
}

//鼠标滚轮事件
void MouseEventLabel::wheelEvent(QWheelEvent* ev)
{
	emit mousewheel(ev->angleDelta().y());//输出滚轮正反移动的距离
}

//鼠标双击事件
void MouseEventLabel::mouseDoubleClickEvent(QMouseEvent* event)
{
	Q_UNUSED(event);
	emit doubleClicked();
}

MouseEventLabel::~MouseEventLabel()
{}
