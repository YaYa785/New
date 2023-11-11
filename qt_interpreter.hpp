#ifndef QT_INTERPRETER_HPP
#define QT_INTERPRETER_HPP

#include <string>

#include <QObject>
#include <QString>
#include <QGraphicsItem>

#include "interpreter.hpp"
#include "canvas_widget.hpp"

class QtInterpreter: public QObject, private Interpreter{
Q_OBJECT

public:

  QtInterpreter(QObject * parent = nullptr);

signals:

  void drawGraphic(QGraphicsItem * item);

  void info(QString message);

  void error(QString message);

  void clearCanvasSignal();

public slots:

  void parseAndEvaluate(QString entry);
  void drawExpression(const Expression& expr);
};

#endif
