/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *startButton;
    QComboBox *comboBox;
    QRadioButton *ascendingRadioButton;
    QRadioButton *descendingRadioButton;
    QLabel *textLabel;
    QSpinBox *elementsCount;
    QSpinBox *delay;
    QLabel *elementsCountLabel;
    QLabel *delayLabel;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1280, 720);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1280, 720));
        MainWindow->setMaximumSize(QSize(1280, 720));
        MainWindow->setBaseSize(QSize(1280, 720));
        MainWindow->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        MainWindow->setAcceptDrops(false);
        MainWindow->setAutoFillBackground(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        startButton = new QPushButton(centralwidget);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(20, 20, 101, 31));
        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(20, 60, 101, 21));
        ascendingRadioButton = new QRadioButton(centralwidget);
        ascendingRadioButton->setObjectName("ascendingRadioButton");
        ascendingRadioButton->setGeometry(QRect(20, 90, 91, 22));
        ascendingRadioButton->setChecked(true);
        descendingRadioButton = new QRadioButton(centralwidget);
        descendingRadioButton->setObjectName("descendingRadioButton");
        descendingRadioButton->setGeometry(QRect(20, 110, 91, 22));
        textLabel = new QLabel(centralwidget);
        textLabel->setObjectName("textLabel");
        textLabel->setGeometry(QRect(140, 10, 1121, 671));
        textLabel->setScaledContents(true);
        textLabel->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        elementsCount = new QSpinBox(centralwidget);
        elementsCount->setObjectName("elementsCount");
        elementsCount->setGeometry(QRect(20, 160, 91, 25));
        elementsCount->setMinimum(2);
        elementsCount->setMaximum(1000);
        elementsCount->setValue(100);
        delay = new QSpinBox(centralwidget);
        delay->setObjectName("delay");
        delay->setGeometry(QRect(20, 210, 91, 25));
        delay->setMinimum(1);
        delay->setMaximum(10000);
        delay->setValue(1);
        elementsCountLabel = new QLabel(centralwidget);
        elementsCountLabel->setObjectName("elementsCountLabel");
        elementsCountLabel->setGeometry(QRect(20, 140, 91, 16));
        delayLabel = new QLabel(centralwidget);
        delayLabel->setObjectName("delayLabel");
        delayLabel->setGeometry(QRect(20, 190, 111, 16));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Sorting Algorithms Visualizer", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Bubble Sort", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Merge Sort", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Selection Sort", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Insertion Sort", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("MainWindow", "Quick Sort", nullptr));

        ascendingRadioButton->setText(QCoreApplication::translate("MainWindow", "Ascending", nullptr));
        descendingRadioButton->setText(QCoreApplication::translate("MainWindow", "Descending", nullptr));
        textLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        elementsCountLabel->setText(QCoreApplication::translate("MainWindow", "Elements Count:", nullptr));
        delayLabel->setText(QCoreApplication::translate("MainWindow", "Delay (ms):", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
