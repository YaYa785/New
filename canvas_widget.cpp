#include "canvas_widget.hpp"

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLayout>
#include <QDebug>

CanvasWidget::CanvasWidget(QWidget * parent): QWidget(parent)
{
	// Initialize the QGraphicsScene
	scene = new QGraphicsScene(this);

	// Initialize the QGraphicsView with the scene
	QGraphicsView* view = new QGraphicsView(scene, this);

	// Set up the layout to include the QGraphicsView
	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(view);
	setLayout(layout);
}

void CanvasWidget::addGraphic(QGraphicsItem * item)
{
	// Add the provided QGraphicsItem to the scene
	scene->addItem(item);
}

void CanvasWidget::clearCanvas()
{
	if (scene)
	{
		scene->clear();
	}
}

