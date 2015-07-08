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

#define WIDTH 306
#define PADDING 7
#define DEFAULT_NFRAMES 3
#define DEFAULT_NBUTTONS 8
#define DEFAULT_LINE_POINTS 15
#define DEFAULT_BUTTON_POINTS 31

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
        int nframes = DEFAULT_NFRAMES;
        int nbuttons = DEFAULT_NBUTTONS;
        int linePoints  = DEFAULT_LINE_POINTS;
        int buttonPoints  = DEFAULT_BUTTON_POINTS;
        mHeight = 31 + linePoints * nframes + buttonPoints * nbuttons;
        if (Dialog->objectName().isEmpty())
        {
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
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
            verticalLayout->setContentsMargins(0, PADDING, 0, 0);
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
            verticalLayoutFocus->setContentsMargins(0, PADDING, 0, 0);
//            addFrame(verticalLayoutFocus);

            mpRenameNodeButton = new QPushButton(horizontalLayoutWidget);
            mpRenameNodeButton->setObjectName(QString::fromUtf8("mpRenameNodeButton"));
            mpRenameNodeButton->setIcon(*(mpGraphWidget->getIcon("label")));
            mpRenameNodeButton->setEnabled(mVertexFocused);
            mpRenameNodeButton->setCheckable(false);
            mpRenameNodeButton->setChecked(false);

            verticalLayoutFocus->addWidget(mpRenameNodeButton);

            mpAddPortButton = new QPushButton(horizontalLayoutWidget);
            mpAddPortButton->setObjectName(QString::fromUtf8("mpAddPortButton"));
            mpAddPortButton->setIcon(*(mpGraphWidget->getIcon("addPort")));
            mpAddPortButton->setEnabled(mVertexFocused);
            mpAddPortButton->setCheckable(false);
            mpAddPortButton->setChecked(false);

            verticalLayoutFocus->addWidget(mpAddPortButton);

            mpRenamePortButton = new QPushButton(horizontalLayoutWidget);
            mpRenamePortButton->setObjectName(QString::fromUtf8("mpRenamePortButton"));
            mpRenamePortButton->setIcon(*(mpGraphWidget->getIcon("portLabel")));
            mpRenamePortButton->setEnabled(mVertexFocused);
            mpRenamePortButton->setCheckable(false);
            mpRenamePortButton->setChecked(false);

            verticalLayoutFocus->addWidget(mpRenamePortButton);

            mpRemovePortButton = new QPushButton(horizontalLayoutWidget);
            mpRemovePortButton->setObjectName(QString::fromUtf8("mpRemovePortButton"));
            mpRemovePortButton->setIcon(*(mpGraphWidget->getIcon("remove")));
            mpRemovePortButton->setEnabled(mVertexFocused);
            mpRemovePortButton->setCheckable(false);
            mpRemovePortButton->setChecked(false);

            verticalLayoutFocus->addWidget(mpRemovePortButton);

            mpRemovePortsButton = new QPushButton(horizontalLayoutWidget);
            mpRemovePortsButton->setObjectName(QString::fromUtf8("mpRemovePortsButton"));
            mpRemovePortsButton->setIcon(*(mpGraphWidget->getIcon("removeAll")));
            mpRemovePortsButton->setEnabled(mVertexFocused);
            mpRemovePortsButton->setCheckable(false);
            mpRemovePortsButton->setChecked(false);

            verticalLayoutFocus->addWidget(mpRemovePortsButton);

            mpRemoveNodeButton = new QPushButton(horizontalLayoutWidget);
            mpRemoveNodeButton->setObjectName(QString::fromUtf8("mpRemoveNodeButton"));
            mpRemoveNodeButton->setIcon(*(mpGraphWidget->getIcon("remove")));
            mpRemoveNodeButton->setEnabled(mVertexFocused);
            mpRemoveNodeButton->setCheckable(false);
            mpRemoveNodeButton->setChecked(false);

            verticalLayoutFocus->addWidget(mpRemoveNodeButton);

            addFrame(verticalLayoutFocus);

            mpRenameEdgeButton = new QPushButton(horizontalLayoutWidget);
            mpRenameEdgeButton->setObjectName(QString::fromUtf8("mpRenameEdgeButton"));
            mpRenameEdgeButton->setIcon(*(mpGraphWidget->getIcon("label")));
            mpRenameEdgeButton->setEnabled(mEdgeFocused);
            mpRenameEdgeButton->setCheckable(false);
            mpRenameEdgeButton->setChecked(false);

            verticalLayoutFocus->addWidget(mpRenameEdgeButton);

            mpRemoveEdgeButton = new QPushButton(horizontalLayoutWidget);
            mpRemoveEdgeButton->setObjectName(QString::fromUtf8("mpRemoveEdgeButton"));
            mpRemoveEdgeButton->setIcon(*(mpGraphWidget->getIcon("remove")));
            mpRemoveEdgeButton->setEnabled(mEdgeFocused);
            mpRemoveEdgeButton->setCheckable(false);
            mpRemoveEdgeButton->setChecked(false);

            verticalLayoutFocus->addWidget(mpRemoveEdgeButton);

            // adding bottom padding
            verticalLayoutFocus->addSpacing(mHeight - 2 * PADDING - (1 * linePoints + 8 * buttonPoints));
        }

        retranslateUi(Dialog);

        QObject::connect(mpRefreshButton, SIGNAL(clicked()), mpGraphWidget, SLOT(refresh()));
        QObject::connect(mpShuffleButton, SIGNAL(clicked()), mpGraphWidget, SLOT(shuffle()));
        QObject::connect(mpImportButton, SIGNAL(clicked()), mpGraphWidget, SLOT(importGraph()));
        QObject::connect(mpExportButton, SIGNAL(clicked()), mpGraphWidget, SLOT(exportGraph()));
        QObject::connect(mpResetButton,  SIGNAL(clicked()), mpGraphWidget, SLOT(resetGraph()));
        QObject::connect(mpLayoutButton,  SIGNAL(clicked()), mpGraphWidget, SLOT(changeLayout()));
        QObject::connect(mpAddNodeButton, SIGNAL(clicked()), mpGraphWidget, SLOT(addNodeAdhoc()));
        QObject::connect(mpDragDropButton, SIGNAL(toggled(bool)), mpGraphWidget, SLOT(updateDragDrop(bool)));
        QObject::connect(mpRenameNodeButton, SIGNAL(clicked()), mpGraphWidget, SLOT(changeFocusedVertexLabel()));
        QObject::connect(mpAddPortButton, SIGNAL(clicked()), mpGraphWidget, SLOT(addPortFocused()));
        QObject::connect(mpRenamePortButton, SIGNAL(clicked()), mpGraphWidget, SLOT(renamePortFocused()));
        QObject::connect(mpRemovePortButton, SIGNAL(clicked()), mpGraphWidget, SLOT(removePortFocused()));
        QObject::connect(mpRemovePortsButton, SIGNAL(clicked()), mpGraphWidget, SLOT(removePortsFocused()));
        QObject::connect(mpRemoveNodeButton, SIGNAL(clicked()), mpGraphWidget, SLOT(removeFocusedVertex()));
        QObject::connect(mpRenameEdgeButton, SIGNAL(clicked()), mpGraphWidget, SLOT(changeFocusedEdgeLabel()));
        QObject::connect(mpRemoveEdgeButton, SIGNAL(clicked()), mpGraphWidget, SLOT(removeFocusedEdge()));

        QMetaObject::connectSlotsByName(Dialog);

        Dialog->setFixedSize(WIDTH + 20, 20 + mHeight);
        horizontalLayoutWidget->setGeometry(QRect(10, 10, WIDTH, mHeight));
    } // setupUi

    void retranslateUi(CustomDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Properties", 0, QApplication::UnicodeUTF8));
        mpAddNodeButton->setText(QApplication::translate("Dialog", "Add Node", 0, QApplication::UnicodeUTF8));
        mpRefreshButton->setText(QApplication::translate("Dialog", "Refresh", 0, QApplication::UnicodeUTF8));
        mpShuffleButton->setText(QApplication::translate("Dialog", "Shuffle", 0, QApplication::UnicodeUTF8));
        mpImportButton->setText(QApplication::translate("Dialog", "Import", 0, QApplication::UnicodeUTF8));
        mpExportButton->setText(QApplication::translate("Dialog", "Export", 0, QApplication::UnicodeUTF8));
        mpResetButton->setText(QApplication::translate("Dialog", "Reset", 0, QApplication::UnicodeUTF8));
        mpLayoutButton->setText(QApplication::translate("Dialog", "Layout", 0, QApplication::UnicodeUTF8));
        mpDragDropButton->setText(QApplication::translate("Dialog", "Drag'n'Drop", 0, QApplication::UnicodeUTF8));
        mpRenameNodeButton->setText(QApplication::translate("Dialog", "Rename Node", 0, QApplication::UnicodeUTF8));
        mpAddPortButton->setText(QApplication::translate("Dialog", "Add Port", 0, QApplication::UnicodeUTF8));
        mpRenamePortButton->setText(QApplication::translate("Dialog", "Rename a Port", 0, QApplication::UnicodeUTF8));
        mpRemovePortButton->setText(QApplication::translate("Dialog", "Remove a Port", 0, QApplication::UnicodeUTF8));
        mpRemovePortsButton->setText(QApplication::translate("Dialog", "Remove Ports", 0, QApplication::UnicodeUTF8));
        mpRemoveNodeButton->setText(QApplication::translate("Dialog", "Remove Node", 0, QApplication::UnicodeUTF8));
        mpRenameEdgeButton->setText(QApplication::translate("Dialog", "Rename Edge", 0, QApplication::UnicodeUTF8));
        mpRemoveEdgeButton->setText(QApplication::translate("Dialog", "Remove Edge", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

    void setVertexFocused(bool vertexFocused)
    {
        if(vertexFocused != mVertexFocused)
        {
            mpRenameNodeButton->setEnabled(vertexFocused);
            mpAddPortButton->setEnabled(vertexFocused);
            mpRenamePortButton ->setEnabled(vertexFocused);
            mpRemovePortButton ->setEnabled(vertexFocused);
            mpRemovePortsButton->setEnabled(vertexFocused);
            mpRemoveNodeButton ->setEnabled(vertexFocused);
        }
        mVertexFocused = vertexFocused;
    }
    void setEdgeFocused(bool edgeFocused)
    {
        if(edgeFocused != mEdgeFocused)
        {
            mpRenameEdgeButton->setEnabled(edgeFocused);
            mpRemoveEdgeButton->setEnabled(edgeFocused);
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
    QPushButton *mpRenameNodeButton;
    QPushButton *mpAddPortButton;
    QPushButton *mpRenamePortButton;
    QPushButton *mpRemovePortButton;
    QPushButton *mpRemovePortsButton;
    QPushButton *mpRemoveNodeButton;
    QPushButton *mpRenameEdgeButton;
    QPushButton *mpRemoveEdgeButton;
    QFrames mFrames;
    bool mVertexFocused;
    bool mEdgeFocused;
    int mHeight;
};

} // end namespace gui
} // end namespace graph_analysis

#endif // PROPERTYDIALOG_H
