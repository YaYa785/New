#include "message_widget.hpp"

#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QDebug>
MessageWidget::MessageWidget(QWidget* parent) : QWidget(parent)
{
    // Initialize the QLineEdit to display messages
    QLineEdit* messageEdit = new QLineEdit(this);
    messageEdit->setObjectName("messageEdit"); 
    messageEdit->setReadOnly(true); 

    QLabel* messageLabel = new QLabel("Message: ", this);

    // Set up a horizontal layout to place the label to the left of the QLineEdit
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(messageLabel);
    layout->addWidget(messageEdit);
    setLayout(layout);
}


void MessageWidget::info(QString message) 
{
    QLineEdit* messageEdit = findChild<QLineEdit*>("messageEdit");

    messageEdit->clear();

    // Clear any error formatting
    QPalette palette = messageEdit->palette();
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Window, Qt::white);
    palette.setColor(QPalette::Highlight, Qt::transparent);
    messageEdit->setPalette(palette);
    messageEdit->setText(message);
    messageEdit->deselect(); // Deselect any selected text
}

void MessageWidget::error(QString message) 
{
    QLineEdit* messageEdit = findChild<QLineEdit*>("messageEdit");

    messageEdit->clear();

    // Set the text to black on a white background and highlight in red
    QPalette palette = messageEdit->palette();
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Window, Qt::white);
    palette.setColor(QPalette::Highlight, Qt::red);
    palette.setColor(QPalette::HighlightedText, Qt::black);
    messageEdit->setPalette(palette);
    messageEdit->setText(message);
    messageEdit->selectAll(); 
}

void MessageWidget::clear() 
{
    QLineEdit* messageEdit = findChild<QLineEdit*>("messageEdit");
    messageEdit->clear();
}
