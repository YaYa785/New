#include "repl_widget.hpp"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QKeyEvent>
#include <QDebug>

REPLWidget::REPLWidget(QWidget * parent): QWidget(parent), historyIndex(-1) 
{
    // Create a QLineEdit for user input
    inputLine = new QLineEdit(this);

    QLabel* replLabel = new QLabel("slisp> ", this);

    // Set up a horizontal layout to place the label to the left of the QLineEdit
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(replLabel);
    layout->addWidget(inputLine);
    setLayout(layout);

    // Connect the QLineEdit's returnPressed signal to our custom slot
    connect(inputLine, &QLineEdit::returnPressed, this, &REPLWidget::changed);
}

void REPLWidget::changed() 
{
	QString currentText = inputLine->text();

	// Emit the lineEntered signal with the current text
	emit lineEntered(currentText);

	// Add the current text to the history and reset the input line
	history.push_back(currentText);
	historyIndex = history.size();
	inputLine->clear();
}

// Override the keyPressEvent to handle up and down arrow keys for history
void REPLWidget::keyPressEvent(QKeyEvent* event) 
{
	if (event->key() == Qt::Key_Up) 
	{
		if (historyIndex > 0) 
		{
			historyIndex--;
			inputLine->setText(history[historyIndex]);
		}
	}
	else if (event->key() == Qt::Key_Down) 
	{
		if (historyIndex < history.size() - 1)
		{
			historyIndex++;
			inputLine->setText(history[historyIndex]);
		}
		else 
		{
			inputLine->clear();
			historyIndex = history.size();
		}
	}
	else
	{
		QWidget::keyPressEvent(event);
	}
}