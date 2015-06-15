/********************************************************************************
** Form generated from reading UI file 'property_dialog_gui.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include <QObject>
#include <QCloseEvent>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "GraphWidget.hpp"

namespace graph_analysis {
namespace gui {
    class GraphWidget;

class PropertyDialog : public QObject
{
    Q_OBJECT
public:
    PropertyDialog(GraphWidget *widget)
    : mpGraphWidget(widget)
    {
        setupUi(&mDialog);
        mDialog.show();
    }
    ~PropertyDialog(){}

    void closeDialog() { mDialog.done(0); }
    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(163, 281);
        horizontalLayoutWidget = new QWidget(Dialog);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 141, 261));
        verticalLayout = new QVBoxLayout(horizontalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        mpDragDropButton = new QPushButton(horizontalLayoutWidget);
        mpDragDropButton->setObjectName(QString::fromUtf8("mpDragDropButton"));
        mpDragDropButton->setCheckable(true);
        mpDragDropButton->setChecked(false);

        verticalLayout->addWidget(mpDragDropButton);

        mpAddNodeButton = new QPushButton(horizontalLayoutWidget);
        mpAddNodeButton->setObjectName(QString::fromUtf8("mpAddNodeButton"));
        mpAddNodeButton->setCheckable(false);
        mpAddNodeButton->setChecked(false);

        verticalLayout->addWidget(mpAddNodeButton);

        mpRefreshButton = new QPushButton(horizontalLayoutWidget);
        mpRefreshButton->setObjectName(QString::fromUtf8("mpRefreshButton"));
        mpRefreshButton->setCheckable(false);
        mpRefreshButton->setChecked(false);

        verticalLayout->addWidget(mpRefreshButton);

        mpShuffleButton = new QPushButton(horizontalLayoutWidget);
        mpShuffleButton->setObjectName(QString::fromUtf8("mpShuffleButton"));
        mpShuffleButton->setCheckable(false);
        mpShuffleButton->setChecked(false);

        verticalLayout->addWidget(mpShuffleButton);

        mpImportFromXmlButton = new QPushButton(horizontalLayoutWidget);
        mpImportFromXmlButton->setObjectName(QString::fromUtf8("mpImportFromXmlButton"));
        mpImportFromXmlButton->setCheckable(false);
        mpImportFromXmlButton->setChecked(false);

        verticalLayout->addWidget(mpImportFromXmlButton);

        mpExportToXmlButton = new QPushButton(horizontalLayoutWidget);
        mpExportToXmlButton->setObjectName(QString::fromUtf8("mpExportToXmlButton"));
        mpExportToXmlButton->setCheckable(false);
        mpExportToXmlButton->setChecked(false);

        verticalLayout->addWidget(mpExportToXmlButton);

        mpImportFromYmlButton = new QPushButton(horizontalLayoutWidget);
        mpImportFromYmlButton->setObjectName(QString::fromUtf8("mpImportFromYmlButton"));
        mpImportFromYmlButton->setCheckable(false);
        mpImportFromYmlButton->setChecked(false);

        verticalLayout->addWidget(mpImportFromYmlButton);

        mpExportToYmlButton = new QPushButton(horizontalLayoutWidget);
        mpExportToYmlButton->setObjectName(QString::fromUtf8("mpExportToYmlButton"));
        mpExportToYmlButton->setCheckable(false);
        mpExportToYmlButton->setChecked(false);

        verticalLayout->addWidget(mpExportToYmlButton);

        mpExportToDotButton = new QPushButton(horizontalLayoutWidget);
        mpExportToDotButton->setObjectName(QString::fromUtf8("mpExportToDotButton"));
        mpExportToDotButton->setCheckable(false);
        mpExportToDotButton->setChecked(false);

        verticalLayout->addWidget(mpExportToDotButton);

        mpLayoutButton = new QPushButton(horizontalLayoutWidget);
        mpLayoutButton->setObjectName(QString::fromUtf8("mpLayoutButton"));
        mpLayoutButton->setCheckable(false);
        mpLayoutButton->setChecked(false);

        verticalLayout->addWidget(mpLayoutButton);


        retranslateUi(Dialog);

        QObject::connect(mpDragDropButton, SIGNAL(toggled(bool)), mpGraphWidget, SLOT(updateDragDrop(bool)));
        QObject::connect(mpRefreshButton, SIGNAL(clicked()), mpGraphWidget, SLOT(refresh()));
        QObject::connect(mpShuffleButton, SIGNAL(clicked()), mpGraphWidget, SLOT(shuffle()));
        QObject::connect(mpImportFromXmlButton, SIGNAL(clicked()), mpGraphWidget, SLOT(importGraphFromXml()));
        QObject::connect(mpExportToXmlButton, SIGNAL(clicked()), mpGraphWidget, SLOT(exportGraphToXml()));
        QObject::connect(mpImportFromYmlButton, SIGNAL(clicked()), mpGraphWidget, SLOT(importGraphFromYml()));
        QObject::connect(mpExportToYmlButton, SIGNAL(clicked()), mpGraphWidget, SLOT(exportGraphToYml()));
        QObject::connect(mpExportToDotButton, SIGNAL(clicked()), mpGraphWidget, SLOT(exportGraphToDot()));
        QObject::connect(mpLayoutButton,  SIGNAL(clicked()), mpGraphWidget, SLOT(changeLayout()));
        QObject::connect(mpAddNodeButton, SIGNAL(clicked()), mpGraphWidget, SLOT(addNodeAdhoc()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Properties", 0, QApplication::UnicodeUTF8));
        mpDragDropButton->setText(QApplication::translate("Dialog", "Drag'n'Drop", 0, QApplication::UnicodeUTF8));
        mpAddNodeButton->setText(QApplication::translate("Dialog", "Add Node", 0, QApplication::UnicodeUTF8));
        mpRefreshButton->setText(QApplication::translate("Dialog", "Refresh", 0, QApplication::UnicodeUTF8));
        mpShuffleButton->setText(QApplication::translate("Dialog", "Shuffle", 0, QApplication::UnicodeUTF8));
        mpImportFromXmlButton->setText(QApplication::translate("Dialog", "Import", 0, QApplication::UnicodeUTF8));
        mpExportToXmlButton->setText(QApplication::translate("Dialog", "Export", 0, QApplication::UnicodeUTF8));
        mpImportFromYmlButton->setText(QApplication::translate("Dialog", "Import .yml", 0, QApplication::UnicodeUTF8));
        mpExportToYmlButton->setText(QApplication::translate("Dialog", "Export as .yml", 0, QApplication::UnicodeUTF8));
        mpExportToDotButton->setText(QApplication::translate("Dialog", "Export as .dot", 0, QApplication::UnicodeUTF8));
        mpLayoutButton->setText(QApplication::translate("Dialog", "Layout", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

public slots:

protected:
    void closeEvent(QCloseEvent* event)
    {
        event->accept();
    }
    void keyPressEvent(QKeyEvent *event)
    {
        // check for a combination of user clicks
        Qt::KeyboardModifiers modifiers = event->modifiers();

        if(modifiers & Qt::ControlModifier)
        {
            switch (event->key())
            {
                case Qt::Key_Q:
                case Qt::Key_W:
                    exit(0);
                break;
            }
        }
    }

private:
    QDialog mDialog;
    GraphWidget *mpGraphWidget;
    QWidget *horizontalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *mpDragDropButton;
    QPushButton *mpAddNodeButton;
    QPushButton *mpRefreshButton;
    QPushButton *mpShuffleButton;
    QPushButton *mpImportFromXmlButton;
    QPushButton *mpImportFromYmlButton;
    QPushButton *mpExportToXmlButton;
    QPushButton *mpExportToYmlButton;
    QPushButton *mpExportToDotButton;
    QPushButton *mpLayoutButton;

};

} // end namespace gui
} // end namespace graph_analysis

#endif // PROPERTYDIALOG_H
