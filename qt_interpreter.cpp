#include "qt_interpreter.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>

#include <QBrush>
#include <QDebug>

#include "qgraphics_arc_item.hpp"


#include "interpreter_semantic_error.hpp"
#include <QtWidgets>

QtInterpreter::QtInterpreter(QObject * parent): QObject(parent)
{
  // TODO: your code here...
}

void QtInterpreter::parseAndEvaluate(QString entry) {
    try {
        std::istringstream expressionStream(entry.toStdString());
        bool success = parse(expressionStream);
        if (success) {
            // Instead of calling eval, we directly use evaluateExpression on the AST
            Expression result = evaluateExpression(ast);

            emit clearCanvasSignal();

            // Now we draw the result, which could be a single expression or a list
            drawExpression(result);
        }
        else {
            emit error("Failed to parse the expression.");
        }
    }
    catch (const InterpreterSemanticError& e) {
        emit error(QString::fromStdString(e.what()));
    }
    catch (const std::exception& e) {
        emit error(QString::fromStdString(e.what()));
    }
}

void QtInterpreter::drawExpression(const Expression& result) {
    std::string resultStr;
    QGraphicsEllipseItem* pointItem = nullptr;
    QGraphicsLineItem* lineItem = nullptr;
    QGraphicsArcItem* arcItem = nullptr;
    qreal x, y, width, height;

    switch (result.head.type)
    {
    case BooleanType:
        resultStr = result.head.value.bool_value ? "True" : "False";
        break;
    case NumberType:
    {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(0) << result.head.value.num_value;
        resultStr = "(" + stream.str() + ")";
        break;
    }
    case SymbolType:
        resultStr = result.head.value.sym_value;
        break;

    case PointType:
        resultStr = "(" + std::to_string(result.head.value.point_value.x) + ", " + std::to_string(result.head.value.point_value.y) + ")";
        pointItem = new QGraphicsEllipseItem(result.head.value.point_value.x, result.head.value.point_value.y, 1, 1);
        break;

    case LineType:
        resultStr = "((" + std::to_string(result.head.value.line_value.first.x) + ", " + std::to_string(result.head.value.line_value.first.y) + "), (" + std::to_string(result.head.value.line_value.second.x) + ", " + std::to_string(result.head.value.line_value.second.y) + "))";
        lineItem = new QGraphicsLineItem(result.head.value.line_value.first.x, result.head.value.line_value.first.y, result.head.value.line_value.second.x, result.head.value.line_value.second.y);

        break;

    case ArcType:
        x = result.head.value.arc_value.center.x;
        y = result.head.value.arc_value.center.y;
        width = 2 * (result.head.value.arc_value.start.x - x);
        height = result.head.value.arc_value.span;
        arcItem = new QGraphicsArcItem(x - width / 2, y - height / 2, width, height); // Adjusted to center the arc at (x,y)
        resultStr = "((" + std::to_string(x) + ", " + std::to_string(y) + "), (" + std::to_string(result.head.value.arc_value.start.x) + ", " + std::to_string(result.head.value.arc_value.start.y) + "), " + std::to_string(result.head.value.arc_value.span) + ")";
        break;

    case ListType:
        // If the expression is a list, iterate and draw each sub-expression
        for (const auto& subExpr : result.tail) 
        {
            drawExpression(subExpr);
            qDebug() << "ARC: " << arcItem;
        }
        return; // Early return to avoid emitting signals for list itself
    
    default:
        resultStr = "Unknown Type";
        break;
    }

    // Emit signals for drawing if applicable
    if (!resultStr.empty()) {
        emit info(QString::fromStdString(resultStr));
    }
    if (lineItem) {
        emit drawGraphic(lineItem);
    }
    if (arcItem) {
        emit drawGraphic(arcItem);
    }
    if (pointItem) {
        emit drawGraphic(pointItem);
    }
}

//
//void QtInterpreter::parseAndEvaluate(QString entry)
//{
//    try
//    {
//        std::istringstream expressionStream(entry.toStdString());
//
//        bool success = parse(expressionStream);
//        if (success)
//        {
//            Expression result = eval();
//            std::string resultStr;
//            QGraphicsEllipseItem* pointItem = nullptr;
//            QGraphicsLineItem* lineItem = nullptr;
//            QGraphicsArcItem* arcItem = nullptr;
//            qreal x, y, width, height;
//            
//            switch (result.head.type)
//            {
//            case BooleanType:
//                resultStr = result.head.value.bool_value ? "True" : "False";
//                break;
//            case NumberType:
//            {
//                std::ostringstream stream;
//                stream << std::fixed << std::setprecision(0) << result.head.value.num_value;
//                resultStr = "(" + stream.str() + ")";
//                break;
//            }
//            case SymbolType:
//                resultStr = result.head.value.sym_value;
//                break;
//
//            case PointType:
//                resultStr = "(" + std::to_string(result.head.value.point_value.x) + ", " + std::to_string(result.head.value.point_value.y) + ")";
//                pointItem = new QGraphicsEllipseItem(result.head.value.point_value.x, result.head.value.point_value.y, 1, 1);
//                break;
//
//            case LineType:
//                resultStr = "((" + std::to_string(result.head.value.line_value.first.x) + ", " + std::to_string(result.head.value.line_value.first.y) + "), (" + std::to_string(result.head.value.line_value.second.x) + ", " + std::to_string(result.head.value.line_value.second.y) + "))";
//                lineItem = new QGraphicsLineItem(result.head.value.line_value.first.x, result.head.value.line_value.first.y, result.head.value.line_value.second.x, result.head.value.line_value.second.y);
//
//                break;
//
//            case ArcType:
//                x = result.head.value.arc_value.center.x;
//                y = result.head.value.arc_value.center.y;
//                width = 2 * (result.head.value.arc_value.start.x - x); 
//                height = result.head.value.arc_value.span; 
//                arcItem = new QGraphicsArcItem(x - width / 2, y - height / 2, width, height); // Adjusted to center the arc at (x,y)
//                resultStr = "((" + std::to_string(x) + ", " + std::to_string(y) + "), (" + std::to_string(result.head.value.arc_value.start.x) + ", " + std::to_string(result.head.value.arc_value.start.y) + "), " + std::to_string(result.head.value.arc_value.span) + ")";
//                break;
//            
//            default:
//                resultStr = "Unknown Type";
//                break;
//            }
//           
//            emit clearCanvasSignal();
//
//            emit info(QString::fromStdString(resultStr));
//            if (lineItem) emit drawGraphic(lineItem);
//            if (arcItem) emit drawGraphic(arcItem);
//            if (pointItem) emit drawGraphic(pointItem);
//            qDebug() << "Arc: " << arcItem;
//        }
//        else
//        {
//            emit error("Failed to parse the expression.");
//        }
//    }
//    catch (const InterpreterSemanticError& e)
//    {
//        emit error(QString::fromStdString(e.what()));
//    }
//    catch (const std::exception& e)
//    {
//        emit error(QString::fromStdString(e.what()));
//    }
//   
//}



