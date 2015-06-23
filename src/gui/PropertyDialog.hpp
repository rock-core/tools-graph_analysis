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
    PropertyDialog(GraphWidget *widget, bool dragDropIsChecked = false)
    : mpGraphWidget(widget)
    {
        setupUi(&mDialog, dragDropIsChecked);
        mDialog.show();
    }
    ~PropertyDialog(){}

    bool isRunning() { return mDialog.isVisible(); }
    void setDragDrop(bool toggle) { mpDragDropButton->setChecked(toggle); }
    void setupUi(QDialog *Dialog, bool dragDropIsChecked = false)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(163, 221);
        Dialog->setMinimumSize(163, 221);
        horizontalLayoutWidget = new QWidget(Dialog);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 141, 201));
        verticalLayout = new QVBoxLayout(horizontalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        mpDragDropButton = new QPushButton(horizontalLayoutWidget);
        mpDragDropButton->setObjectName(QString::fromUtf8("mpDragDropButton"));
        mpDragDropButton->setCheckable(true);
        mpDragDropButton->setChecked(dragDropIsChecked);

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

        mpImportButton = new QPushButton(horizontalLayoutWidget);
        mpImportButton->setObjectName(QString::fromUtf8("mpImportButton"));
        mpImportButton->setCheckable(false);
        mpImportButton->setChecked(false);

        verticalLayout->addWidget(mpImportButton);

        mpExportButton = new QPushButton(horizontalLayoutWidget);
        mpExportButton->setObjectName(QString::fromUtf8("mpExportButton"));
        mpExportButton->setCheckable(false);
        mpExportButton->setChecked(false);

        verticalLayout->addWidget(mpExportButton);

        mpLayoutButton = new QPushButton(horizontalLayoutWidget);
        mpLayoutButton->setObjectName(QString::fromUtf8("mpLayoutButton"));
        mpLayoutButton->setCheckable(false);
        mpLayoutButton->setChecked(false);

        verticalLayout->addWidget(mpLayoutButton);

        retranslateUi(Dialog);

        QObject::connect(mpDragDropButton, SIGNAL(toggled(bool)), mpGraphWidget, SLOT(updateDragDrop(bool)));
        QObject::connect(mpRefreshButton, SIGNAL(clicked()), mpGraphWidget, SLOT(refresh()));
        QObject::connect(mpShuffleButton, SIGNAL(clicked()), mpGraphWidget, SLOT(shuffle()));
        QObject::connect(mpImportButton, SIGNAL(clicked()), mpGraphWidget, SLOT(importGraph()));
        QObject::connect(mpExportButton, SIGNAL(clicked()), mpGraphWidget, SLOT(exportGraph()));
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
        mpImportButton->setText(QApplication::translate("Dialog", "Import", 0, QApplication::UnicodeUTF8));
        mpExportButton->setText(QApplication::translate("Dialog", "Export", 0, QApplication::UnicodeUTF8));
        mpLayoutButton->setText(QApplication::translate("Dialog", "Change Layout", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

public slots:

private:
    QDialog mDialog;
    GraphWidget *mpGraphWidget;
    QWidget *horizontalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *mpDragDropButton;
    QPushButton *mpAddNodeButton;
    QPushButton *mpRefreshButton;
    QPushButton *mpShuffleButton;
    QPushButton *mpImportButton;
    QPushButton *mpExportButton;
    QPushButton *mpLayoutButton;

};

} // end namespace gui
} // end namespace graph_analysis

#endif // PROPERTYDIALOG_H
