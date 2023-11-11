#include "main_window.hpp"

#include <iostream>

#include <QLayout>
#include <fstream>
#include <QDebug>

#include "message_widget.hpp"
#include "canvas_widget.hpp"
#include "repl_widget.hpp"
#include "interpreter_semantic_error.hpp"

MainWindow::MainWindow(QWidget * parent): MainWindow("", parent)
{
  // TODO: your code here...
}

MainWindow::MainWindow(std::string filename, QWidget * parent): QWidget(parent)
{
    // Create the widgets
    MessageWidget* messageWidget = new MessageWidget(this);
    CanvasWidget* canvasWidget = new CanvasWidget(this);
    REPLWidget* replWidget = new REPLWidget(this);

    // Set up the layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(messageWidget);
    mainLayout->addWidget(canvasWidget);
    mainLayout->addWidget(replWidget);
    setLayout(mainLayout);

    // Connect signals and slots
    connect(&interp, &QtInterpreter::info, messageWidget, &MessageWidget::info);
    connect(&interp, &QtInterpreter::error, messageWidget, &MessageWidget::error);
    connect(replWidget, &REPLWidget::lineEntered, &interp, &QtInterpreter::parseAndEvaluate);
    connect(&interp, &QtInterpreter::drawGraphic, canvasWidget, &CanvasWidget::addGraphic);
    connect(&interp, &QtInterpreter::clearCanvasSignal, canvasWidget, &CanvasWidget::clearCanvas);
    connect(&interp, &QtInterpreter::error, canvasWidget, &CanvasWidget::clearCanvas);



    // If a filename is provided, try to preload the script
    if (!filename.empty()) 
    {
        try {
            std::ifstream ifs(filename);
            if (ifs) 
            {
                std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
                QString qContent = QString::fromStdString(content);
                interp.parseAndEvaluate(qContent);
            }
            else 
            {
                messageWidget->error("Error: Could not open file for reading.");
            }
        }
        catch (InterpreterSemanticError& e) 
        {
            messageWidget->error(QString::fromStdString(e.what()));
        }
    }
}
