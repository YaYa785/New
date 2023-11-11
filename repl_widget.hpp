#ifndef REPL_WIDGET_HPP
#define REPL_WIDGET_HPP

#include <QWidget>
#include <QLineEdit>
#include <QString>
#include <QVector>

class REPLWidget: public QWidget{
Q_OBJECT

public:

  REPLWidget(QWidget * parent = nullptr);

signals:

  void lineEntered(QString entry);

private slots:

  void changed();

private:

	QLineEdit* inputLine;
	QVector<QString> history;
	int historyIndex;

protected:
	void keyPressEvent(QKeyEvent* event) override;
};

#endif
