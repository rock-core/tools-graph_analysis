/********************************************************************************
** Form generated from reading UI file 'property_dialog_gui.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include <vector>
#include <QObject>
#include <QCloseEvent>
#include <QMainWindow>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QWidget>
#include "GraphWidget.hpp"
#include "CustomDialog.hpp"

#define DEFAULT_NBUTTONS 8
#define DEFAULT_BUTTON_POINTS 21

namespace graph_analysis {
namespace gui {
    class GraphWidget;

class PropertyDialog : public QObject
{
    Q_OBJECT
public:
    typedef std::vector<QFrame *> QFrames;

    PropertyDialog(GraphWidget *widget, QMainWindow *mainWindow, bool dragDropIsChecked = false, bool vertexFocused = false, bool edgeFocused = false)
    : mpGraphWidget(widget)
    , mpMainWindow(mainWindow)
    , mVertexFocused(vertexFocused)
    , mEdgeFocused(edgeFocused)
    {
        setupUi(&mDialog, dragDropIsChecked);
        mpMainWindow->addDockWidget(Qt::RightDockWidgetArea, &mDialog, Qt::Horizontal);
    }
    ~PropertyDialog(){}

    bool isRunning() { return (&mDialog)->isVisible(); }
    void setDragDrop(bool toggle) { mpDragDropButton->setChecked(toggle); }
    void addFrame(QVBoxLayout* verticalLayout)
    {
        mFrames.push_back(new QFrame());
        mFrames.back()->setFrameShape(QFrame::HLine);
        verticalLayout->addWidget(mFrames.back());
    }

    void addFrame(QHBoxLayout* horizontalLayout)
    {
        mFrames.push_back(new QFrame());
        mFrames.back()->setFrameShape(QFrame::VLine);
        horizontalLayout->addWidget(mFrames.back());
    }

    void setupUi(CustomDialog *Dialog, bool dragDropIsChecked = false)
    {
        int nbuttons = DEFAULT_NBUTTONS;
        int npoints  = DEFAULT_BUTTON_POINTS;
        if ((Dialog)->objectName().isEmpty())
        {
            (Dialog)->setObjectName(QString::fromUtf8("Dialog"));
        }
        horizontalLayoutWidget = new QWidget(Dialog);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setContentsMargins(0, 25, 0, 0);

        {
            // main properties
            verticalLayout = new QVBoxLayout();
            horizontalLayout->addLayout(verticalLayout);
            verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
            verticalLayout->setContentsMargins(0, 5, 0, 0);
//            addFrame(verticalLayout);

            mpAddNodeButton = new QPushButton(horizontalLayoutWidget);
            mpAddNodeButton->setObjectName(QString::fromUtf8("mpAddNodeButton"));
            mpAddNodeButton->setIcon(*(mpGraphWidget->getIcon("addNode")));
            mpAddNodeButton->setCheckable(false);
            mpAddNodeButton->setChecked(false);

            verticalLayout->addWidget(mpAddNodeButton);
            addFrame(verticalLayout);

            mpImportButton = new QPushButton(horizontalLayoutWidget);
            mpImportButton->setObjectName(QString::fromUtf8("mpImportButton"));
            mpImportButton->setIcon(*(mpGraphWidget->getIcon("import")));
            mpImportButton->setCheckable(false);
            mpImportButton->setChecked(false);

            verticalLayout->addWidget(mpImportButton);

            mpExportButton = new QPushButton(horizontalLayoutWidget);
            mpExportButton->setObjectName(QString::fromUtf8("mpExportButton"));
            mpExportButton->setIcon(*(mpGraphWidget->getIcon("export")));
            mpExportButton->setCheckable(false);
            mpExportButton->setChecked(false);

            verticalLayout->addWidget(mpExportButton);
            addFrame(verticalLayout);

            mpRefreshButton = new QPushButton(horizontalLayoutWidget);
            mpRefreshButton->setObjectName(QString::fromUtf8("mpRefreshButton"));
            mpRefreshButton->setIcon(*(mpGraphWidget->getIcon("refresh")));
            mpRefreshButton->setCheckable(false);
            mpRefreshButton->setChecked(false);

            verticalLayout->addWidget(mpRefreshButton);

            mpShuffleButton = new QPushButton(horizontalLayoutWidget);
            mpShuffleButton->setObjectName(QString::fromUtf8("mpShuffleButton"));
            mpShuffleButton->setIcon(*(mpGraphWidget->getIcon("shuffle")));
            mpShuffleButton->setCheckable(false);
            mpShuffleButton->setChecked(false);

            verticalLayout->addWidget(mpShuffleButton);

            mpResetButton = new QPushButton(horizontalLayoutWidget);
            mpResetButton->setObjectName(QString::fromUtf8("mpResetButton"));
            mpResetButton->setIcon(*(mpGraphWidget->getIcon("reset")));
            mpResetButton->setCheckable(false);
            mpResetButton->setChecked(false);

            verticalLayout->addWidget(mpResetButton);

            mpLayoutButton = new QPushButton(horizontalLayoutWidget);
            mpLayoutButton->setObjectName(QString::fromUtf8("mpLayoutButton"));
            mpLayoutButton->setIcon(*(mpGraphWidget->getIcon("layout")));
            mpLayoutButton->setCheckable(false);
            mpLayoutButton->setChecked(false);

            verticalLayout->addWidget(mpLayoutButton);
            addFrame(verticalLayout);

            mpDragDropButton = new QPushButton(horizontalLayoutWidget);
            mpDragDropButton->setObjectName(QString::fromUtf8("mpDragDropButton"));
            mpDragDropButton->setIcon(*(mpGraphWidget->getIcon("dragndrop")));
            mpDragDropButton->setCheckable(true);
            mpDragDropButton->setChecked(dragDropIsChecked);

            verticalLayout->addWidget(mpDragDropButton);

        }
        horizontalLayout->addSpacing(5);
        addFrame(horizontalLayout);
        horizontalLayout->addSpacing(5);
        {
            // focus-based properties
            verticalLayoutFocus = new QVBoxLayout();
            horizontalLayout->addLayout(verticalLayoutFocus);
            verticalLayoutFocus->setObjectName(QString::fromUtf8("verticalLayoutFocus"));
            verticalLayoutFocus->setContentsMargins(0, 5, 0, 0);
//            addFrame(verticalLayoutFocus);

            mpChangeNodeLabelButton = new QPushButton(horizontalLayoutWidget);
            mpChangeNodeLabelButton->setObjectName(QString::fromUtf8("mpChangeNodeLabelButton"));
            mpChangeNodeLabelButton->setIcon(*(mpGraphWidget->getIcon("label")));
            mpChangeNodeLabelButton->setEnabled(mVertexFocused);
            mpChangeNodeLabelButton->setCheckable(false);
            mpChangeNodeLabelButton->setChecked(false);

            verticalLayoutFocus->addWidget(mpChangeNodeLabelButton);

            mpAddPortButton = new QPushButton(horizontalLayoutWidget);
            mpAddPortButton->setObjectName(QString::fromUtf8("mpAddPortButton"));
            mpAddPortButton->setIcon(*(mpGraphWidget->getIcon("addPort")));
            mpAddPortButton->setEnabled(mVertexFocused);
            mpAddPortButton->setCheckable(false);
            mpAddPortButton->setChecked(false);

            verticalLayoutFocus->addWidget(mpAddPortButton);




            addFrame(verticalLayoutFocus);
        }

        retranslateUi(Dialog);

        QObject::connect(mpRefreshButton, SIGNAL(clicked()), (QGraphicsView *)mpGraphWidget, SLOT(refresh()));
        QObject::connect(mpShuffleButton, SIGNAL(clicked()), (QGraphicsView *)mpGraphWidget, SLOT(shuffle()));
        QObject::connect(mpImportButton, SIGNAL(clicked()), (QGraphicsView *)mpGraphWidget, SLOT(importGraph()));
        QObject::connect(mpExportButton, SIGNAL(clicked()), (QGraphicsView *)mpGraphWidget, SLOT(exportGraph()));
        QObject::connect(mpResetButton,  SIGNAL(clicked()), (QGraphicsView *)mpGraphWidget, SLOT(resetGraph()));
        QObject::connect(mpLayoutButton,  SIGNAL(clicked()), (QGraphicsView *)mpGraphWidget, SLOT(changeLayout()));
        QObject::connect(mpAddNodeButton, SIGNAL(clicked()), (QGraphicsView *)mpGraphWidget, SLOT(addNodeAdhoc()));
        QObject::connect(mpDragDropButton, SIGNAL(toggled(bool)), mpGraphWidget, SLOT(updateDragDrop(bool)));
        QObject::connect(mpChangeNodeLabelButton, SIGNAL(clicked()), (QGraphicsView *)mpGraphWidget, SLOT(changeSelectedVertexLabel()));
        QObject::connect(mpAddPortButton, SIGNAL(clicked()), (QGraphicsView *)mpGraphWidget, SLOT(addPortFocused()));

        QMetaObject::connectSlotsByName(Dialog);

        (Dialog)->setFixedSize(370 /* 163 */, 101 + npoints * (nbuttons + mFrames.size()));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 350 /* 141 */, 81 + npoints * (nbuttons + mFrames.size())));
    } // setupUi

    void retranslateUi(CustomDialog *Dialog)
    {
        (Dialog)->setWindowTitle(QApplication::translate("Dialog", "Properties", 0, QApplication::UnicodeUTF8));
        mpAddNodeButton->setText(QApplication::translate("Dialog", "Add Node", 0, QApplication::UnicodeUTF8));
        mpRefreshButton->setText(QApplication::translate("Dialog", "Refresh", 0, QApplication::UnicodeUTF8));
        mpShuffleButton->setText(QApplication::translate("Dialog", "Shuffle", 0, QApplication::UnicodeUTF8));
        mpImportButton->setText(QApplication::translate("Dialog", "Import", 0, QApplication::UnicodeUTF8));
        mpExportButton->setText(QApplication::translate("Dialog", "Export", 0, QApplication::UnicodeUTF8));
        mpResetButton->setText(QApplication::translate("Dialog", "Reset", 0, QApplication::UnicodeUTF8));
        mpLayoutButton->setText(QApplication::translate("Dialog", "Layout", 0, QApplication::UnicodeUTF8));
        mpDragDropButton->setText(QApplication::translate("Dialog", "Drag'n'Drop", 0, QApplication::UnicodeUTF8));
        mpChangeNodeLabelButton->setText(QApplication::translate("Dialog", "Change Node Label", 0, QApplication::UnicodeUTF8));
        mpAddPortButton->setText(QApplication::translate("Dialog", "Add Port", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

    void setVertexFocused(bool vertexFocused)
    {
        if(vertexFocused != mVertexFocused)
        {
            mpChangeNodeLabelButton->setEnabled(vertexFocused);
            mpAddPortButton->setEnabled(vertexFocused);
        }
        mVertexFocused = vertexFocused;
    }
    void setEdgeFocused(bool edgeFocused)
    {
        if(edgeFocused != mEdgeFocused)
        {
        }
        mEdgeFocused = edgeFocused;
    }

    bool getVertexFocused() { return mVertexFocused; }
    bool getEdgeFocused  () { return mEdgeFocused;   }

public slots:

private:
    CustomDialog mDialog;
    GraphWidget *mpGraphWidget;
    QMainWindow *mpMainWindow;
    QWidget *horizontalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayoutFocus;
    QHBoxLayout *horizontalLayout;
    QPushButton *mpAddNodeButton;
    QPushButton *mpRefreshButton;
    QPushButton *mpShuffleButton;
    QPushButton *mpImportButton;
    QPushButton *mpExportButton;
    QPushButton *mpResetButton;
    QPushButton *mpLayoutButton;
    QPushButton *mpDragDropButton;
    QPushButton *mpChangeNodeLabelButton;
    QPushButton *mpAddPortButton;
    QFrames mFrames;
    bool mVertexFocused;
    bool mEdgeFocused;

};

} // end namespace gui
} // end namespace graph_analysis

#endif // PROPERTYDIALOG_H
