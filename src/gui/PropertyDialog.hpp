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
#include <QtGui/QAction>
#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtCore/QVariant>
#include <QtGui/QTabWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QStackedWidget>
#include "GraphWidget.hpp"
#include "LayerWidget.hpp"
#include "CustomDialog.hpp"

#define WIDTH 306
#define PADDING 7
#define DEFAULT_NFRAMES 3
#define DEFAULT_NBUTTONS 9
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

    PropertyDialog(GraphWidget *widget, LayerWidget *layerWidget, QMainWindow *mainWindow, QStackedWidget *stackedWidget, bool dragDropIsChecked = false, bool vertexFocused = false, bool edgeFocused = false)
    : mpStackedWidget(stackedWidget)
    , mpGraphWidget(widget)
    , mpLayerWidget(layerWidget)
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
        int commonExtraPadding;
        int nframes = DEFAULT_NFRAMES;
        int nbuttons = DEFAULT_NBUTTONS;
        int linePoints  = DEFAULT_LINE_POINTS;
        int buttonPoints  = DEFAULT_BUTTON_POINTS;
        mHeight = 31 + linePoints * nframes + buttonPoints * nbuttons;
        commonExtraPadding = mHeight;
        if (Dialog->objectName().isEmpty())
        {
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        }
        tabWidget = new QTabWidget(Dialog);
        horizontalLayoutWidget = new QWidget();
        tabWidget->addTab(horizontalLayoutWidget, "&Properties");
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setContentsMargins(5, 0, 5, 0);
        int verticalLayoutLeftover;
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
            mpAddNodeButton->setToolTip(QString("adds a new node to the graph"));

            verticalLayout->addWidget(mpAddNodeButton);
            addFrame(verticalLayout);

            mpImportButton = new QPushButton(horizontalLayoutWidget);
            mpImportButton->setObjectName(QString::fromUtf8("mpImportButton"));
            mpImportButton->setIcon(*(mpGraphWidget->getIcon("import")));
            mpImportButton->setCheckable(false);
            mpImportButton->setChecked(false);
            mpImportButton->setToolTip(QString("opens an existing graph file"));

            verticalLayout->addWidget(mpImportButton);

            mpExportButton = new QPushButton(horizontalLayoutWidget);
            mpExportButton->setObjectName(QString::fromUtf8("mpExportButton"));
            mpExportButton->setIcon(*(mpGraphWidget->getIcon("export")));
            mpExportButton->setCheckable(false);
            mpExportButton->setChecked(false);
            mpExportButton->setToolTip(QString("saves the graph to file"));

            verticalLayout->addWidget(mpExportButton);
            addFrame(verticalLayout);

            mpRefreshButton = new QPushButton(horizontalLayoutWidget);
            mpRefreshButton->setObjectName(QString::fromUtf8("mpRefreshButton"));
            mpRefreshButton->setIcon(*(mpGraphWidget->getIcon("refresh")));
            mpRefreshButton->setCheckable(false);
            mpRefreshButton->setChecked(false);
            mpRefreshButton->setToolTip(QString("reloads the graph view"));

            verticalLayout->addWidget(mpRefreshButton);

            mpShuffleButton = new QPushButton(horizontalLayoutWidget);
            mpShuffleButton->setObjectName(QString::fromUtf8("mpShuffleButton"));
            mpShuffleButton->setIcon(*(mpGraphWidget->getIcon("shuffle")));
            mpShuffleButton->setCheckable(false);
            mpShuffleButton->setChecked(false);
            mpShuffleButton->setToolTip(QString("randomly spreads around existing vertices in this view"));

            verticalLayout->addWidget(mpShuffleButton);

            mpResetButton = new QPushButton(horizontalLayoutWidget);
            mpResetButton->setObjectName(QString::fromUtf8("mpResetButton"));
            mpResetButton->setIcon(*(mpGraphWidget->getIcon("reset")));
            mpResetButton->setCheckable(false);
            mpResetButton->setChecked(false);
            mpResetButton->setToolTip(QString("completely deletes the graph"));

            verticalLayout->addWidget(mpResetButton);

            mpLayoutButton = new QPushButton(horizontalLayoutWidget);
            mpLayoutButton->setObjectName(QString::fromUtf8("mpLayoutButton"));
            mpLayoutButton->setIcon(*(mpGraphWidget->getIcon("layout")));
            mpLayoutButton->setCheckable(false);
            mpLayoutButton->setChecked(false);
            mpLayoutButton->setToolTip(QString("applies a new layout to the graph"));

            verticalLayout->addWidget(mpLayoutButton);
            addFrame(verticalLayout);

            mpDragDropButton = new QPushButton(horizontalLayoutWidget);
            mpDragDropButton->setObjectName(QString::fromUtf8("mpDragDropButton"));
            mpDragDropButton->setIcon(*(mpGraphWidget->getIcon("dragndrop")));
            mpDragDropButton->setCheckable(true);
            mpDragDropButton->setChecked(dragDropIsChecked);
            mpDragDropButton->setToolTip(QString("toggles the drag-n-drop mode"));

            verticalLayout->addWidget(mpDragDropButton);

            // adding bottom padding
            verticalLayoutLeftover = 0 * linePoints + 1 * buttonPoints;
            if(commonExtraPadding > verticalLayoutLeftover)
            {
                commonExtraPadding = verticalLayoutLeftover;
            }
        }

//        horizontalLayout->addSpacing(5);
        addFrame(horizontalLayout);
//        horizontalLayout->addSpacing(5);

        int verticalLayoutFocusLeftover;
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
            mpRenameNodeButton->setToolTip(QString("changes the label of the selected vertex"));

            verticalLayoutFocus->addWidget(mpRenameNodeButton);

            mpAddPortButton = new QPushButton(horizontalLayoutWidget);
            mpAddPortButton->setObjectName(QString::fromUtf8("mpAddPortButton"));
            mpAddPortButton->setIcon(*(mpGraphWidget->getIcon("addPort")));
            mpAddPortButton->setEnabled(mVertexFocused);
            mpAddPortButton->setCheckable(false);
            mpAddPortButton->setChecked(false);
            mpAddPortButton->setToolTip(QString("adds a port to the selected vertex"));

            verticalLayoutFocus->addWidget(mpAddPortButton);

            mpSwapPortsButton = new QPushButton(horizontalLayoutWidget);
            mpSwapPortsButton->setObjectName(QString::fromUtf8("mpSwapPortsButton"));
            mpSwapPortsButton->setIcon(*(mpGraphWidget->getIcon("swap")));
            mpSwapPortsButton->setEnabled(mVertexFocused);
            mpSwapPortsButton->setCheckable(false);
            mpSwapPortsButton->setChecked(false);
            mpSwapPortsButton->setToolTip(QString("interchanges the positions of two ports of the selected vertex"));

            verticalLayoutFocus->addWidget(mpSwapPortsButton);

            mpRenamePortButton = new QPushButton(horizontalLayoutWidget);
            mpRenamePortButton->setObjectName(QString::fromUtf8("mpRenamePortButton"));
            mpRenamePortButton->setIcon(*(mpGraphWidget->getIcon("portLabel")));
            mpRenamePortButton->setEnabled(mVertexFocused);
            mpRenamePortButton->setCheckable(false);
            mpRenamePortButton->setChecked(false);
            mpRenamePortButton->setToolTip(QString("changes the label of a port of the selected vertex"));

            verticalLayoutFocus->addWidget(mpRenamePortButton);

            mpRemovePortButton = new QPushButton(horizontalLayoutWidget);
            mpRemovePortButton->setObjectName(QString::fromUtf8("mpRemovePortButton"));
            mpRemovePortButton->setIcon(*(mpGraphWidget->getIcon("remove")));
            mpRemovePortButton->setEnabled(mVertexFocused);
            mpRemovePortButton->setCheckable(false);
            mpRemovePortButton->setChecked(false);
            mpRemovePortButton->setToolTip(QString("removes a port of the selected vertex"));

            verticalLayoutFocus->addWidget(mpRemovePortButton);

            mpRemovePortsButton = new QPushButton(horizontalLayoutWidget);
            mpRemovePortsButton->setObjectName(QString::fromUtf8("mpRemovePortsButton"));
            mpRemovePortsButton->setIcon(*(mpGraphWidget->getIcon("removeAll")));
            mpRemovePortsButton->setEnabled(mVertexFocused);
            mpRemovePortsButton->setCheckable(false);
            mpRemovePortsButton->setChecked(false);
            mpRemovePortsButton->setToolTip(QString("removes all ports of the selected vertex"));

            verticalLayoutFocus->addWidget(mpRemovePortsButton);

            mpRemoveNodeButton = new QPushButton(horizontalLayoutWidget);
            mpRemoveNodeButton->setObjectName(QString::fromUtf8("mpRemoveNodeButton"));
            mpRemoveNodeButton->setIcon(*(mpGraphWidget->getIcon("remove")));
            mpRemoveNodeButton->setEnabled(mVertexFocused);
            mpRemoveNodeButton->setCheckable(false);
            mpRemoveNodeButton->setChecked(false);
            mpRemoveNodeButton->setToolTip(QString("removes the selected vertex from the graph"));

            verticalLayoutFocus->addWidget(mpRemoveNodeButton);

            addFrame(verticalLayoutFocus);

            mpRenameEdgeButton = new QPushButton(horizontalLayoutWidget);
            mpRenameEdgeButton->setObjectName(QString::fromUtf8("mpRenameEdgeButton"));
            mpRenameEdgeButton->setIcon(*(mpGraphWidget->getIcon("label")));
            mpRenameEdgeButton->setEnabled(mEdgeFocused);
            mpRenameEdgeButton->setCheckable(false);
            mpRenameEdgeButton->setChecked(false);
            mpRenameEdgeButton->setToolTip(QString("changes the label of the selected edge"));

            verticalLayoutFocus->addWidget(mpRenameEdgeButton);

            mpRemoveEdgeButton = new QPushButton(horizontalLayoutWidget);
            mpRemoveEdgeButton->setObjectName(QString::fromUtf8("mpRemoveEdgeButton"));
            mpRemoveEdgeButton->setIcon(*(mpGraphWidget->getIcon("remove")));
            mpRemoveEdgeButton->setEnabled(mEdgeFocused);
            mpRemoveEdgeButton->setCheckable(false);
            mpRemoveEdgeButton->setChecked(false);
            mpRemoveEdgeButton->setToolTip(QString("removes the selected edge from the graph"));

            verticalLayoutFocus->addWidget(mpRemoveEdgeButton);

            // adding bottom padding
            verticalLayoutFocusLeftover = 3 * linePoints + 0 * buttonPoints;
            if(commonExtraPadding > verticalLayoutFocusLeftover)
            {
                commonExtraPadding = verticalLayoutFocusLeftover;
            }
        }

        horizontalLayoutLayerWidget = new QWidget();
        tabWidget->addTab(horizontalLayoutLayerWidget, "&Layers");
        horizontalLayoutLayerWidget->setObjectName(QString::fromUtf8("horizontalLayoutLayerWidget"));
        horizontalLayoutLayer = new QHBoxLayout(horizontalLayoutLayerWidget);
        horizontalLayoutLayer->setContentsMargins(5, 0, 5, 0);
        verticalLayoutLayer = new QVBoxLayout();
        horizontalLayoutLayer->addLayout(verticalLayoutLayer);
        verticalLayoutLayer->setObjectName(QString::fromUtf8("verticalLayoutLayer"));
        verticalLayoutLayer->setContentsMargins(0, PADDING, 0, 0);
        int verticalLayoutLayerLeftover;
        {

            mpImportLayerButton = new QPushButton(horizontalLayoutLayerWidget);
            mpImportLayerButton->setObjectName(QString::fromUtf8("mpImportLayerButton"));
            mpImportLayerButton->setIcon(*(mpGraphWidget->getIcon("import")));
            mpImportLayerButton->setCheckable(false);
            mpImportLayerButton->setChecked(false);
            mpImportLayerButton->setToolTip(QString("opens an existing graph file"));

            verticalLayoutLayer->addWidget(mpImportLayerButton);

            mpExportLayerButton = new QPushButton(horizontalLayoutWidget);
            mpExportLayerButton->setObjectName(QString::fromUtf8("mpExportLayerButton"));
            mpExportLayerButton->setIcon(*(mpGraphWidget->getIcon("export")));
            mpExportLayerButton->setCheckable(false);
            mpExportLayerButton->setChecked(false);
            mpExportLayerButton->setToolTip(QString("saves the graph to file"));

            verticalLayoutLayer->addWidget(mpExportLayerButton);
            addFrame(verticalLayoutLayer);

            mpRefreshLayerButton = new QPushButton(horizontalLayoutWidget);
            mpRefreshLayerButton->setObjectName(QString::fromUtf8("mpRefreshLayerButton"));
            mpRefreshLayerButton->setIcon(*(mpGraphWidget->getIcon("refresh")));
            mpRefreshLayerButton->setCheckable(false);
            mpRefreshLayerButton->setChecked(false);
            mpRefreshLayerButton->setToolTip(QString("reloads this graph view"));

            verticalLayoutLayer->addWidget(mpRefreshLayerButton);

            mpShuffleLayerButton = new QPushButton(horizontalLayoutWidget);
            mpShuffleLayerButton->setObjectName(QString::fromUtf8("mpShuffleLayerButton"));
            mpShuffleLayerButton->setIcon(*(mpGraphWidget->getIcon("shuffle")));
            mpShuffleLayerButton->setCheckable(false);
            mpShuffleLayerButton->setChecked(false);
            mpShuffleLayerButton->setToolTip(QString("randomly spreads around existing vertices"));

            verticalLayoutLayer->addWidget(mpShuffleLayerButton);

            mpResetLayerButton = new QPushButton(horizontalLayoutWidget);
            mpResetLayerButton->setObjectName(QString::fromUtf8("mpResetLayerButton"));
            mpResetLayerButton->setIcon(*(mpGraphWidget->getIcon("reset")));
            mpResetLayerButton->setCheckable(false);
            mpResetLayerButton->setChecked(false);
            mpResetLayerButton->setToolTip(QString("completely deletes the graph"));

            verticalLayoutLayer->addWidget(mpResetLayerButton);

            mpLayoutLayerButton = new QPushButton(horizontalLayoutWidget);
            mpLayoutLayerButton->setObjectName(QString::fromUtf8("mpLayoutLayerButton"));
            mpLayoutLayerButton->setIcon(*(mpGraphWidget->getIcon("layout")));
            mpLayoutLayerButton->setCheckable(false);
            mpLayoutLayerButton->setChecked(false);
            mpLayoutLayerButton->setToolTip(QString("applies a new layout to the graph"));

            verticalLayoutLayer->addWidget(mpLayoutLayerButton);

            // adding bottom padding
            verticalLayoutLayerLeftover = 2 * linePoints + 3 * buttonPoints;
            if(commonExtraPadding > verticalLayoutLayerLeftover)
            {
                commonExtraPadding = verticalLayoutLayerLeftover;
            }
        }

        horizontalLayoutLayer->addSpacing(WIDTH / 2);

        verticalLayout->addSpacing(verticalLayoutLeftover - commonExtraPadding);
        verticalLayoutFocus->addSpacing(verticalLayoutFocusLeftover - commonExtraPadding);
        verticalLayoutLayer->addSpacing(verticalLayoutLayerLeftover - commonExtraPadding);

        retranslateUi(Dialog);

        PropertyDialog::connect(mpRefreshButton, SIGNAL(clicked()), mpGraphWidget, SLOT(refresh()));
        PropertyDialog::connect(mpRefreshLayerButton, SIGNAL(clicked()), mpLayerWidget, SLOT(refresh()));
        PropertyDialog::connect(mpShuffleButton, SIGNAL(clicked()), mpGraphWidget, SLOT(shuffle()));
        PropertyDialog::connect(mpShuffleLayerButton, SIGNAL(clicked()), mpLayerWidget, SLOT(shuffle()));
        PropertyDialog::connect(mpImportButton, SIGNAL(clicked()), mpGraphWidget, SLOT(importGraph()));
        PropertyDialog::connect(mpImportLayerButton, SIGNAL(clicked()), mpGraphWidget, SLOT(importGraphLayer()));
        PropertyDialog::connect(mpExportButton, SIGNAL(clicked()), mpGraphWidget, SLOT(exportGraph()));
        PropertyDialog::connect(mpExportLayerButton, SIGNAL(clicked()), mpGraphWidget, SLOT(exportGraph()));
        PropertyDialog::connect(mpResetButton,  SIGNAL(clicked()), mpGraphWidget, SLOT(resetGraph()));
        PropertyDialog::connect(mpResetLayerButton,  SIGNAL(clicked()), mpGraphWidget, SLOT(resetGraphLayer()));
        PropertyDialog::connect(mpLayoutButton,  SIGNAL(clicked()), mpGraphWidget, SLOT(changeLayout()));
        PropertyDialog::connect(mpLayoutLayerButton,  SIGNAL(clicked()), mpLayerWidget, SLOT(changeLayout()));
        PropertyDialog::connect(mpAddNodeButton, SIGNAL(clicked()), mpGraphWidget, SLOT(addNodeAdhoc()));
        PropertyDialog::connect(mpDragDropButton, SIGNAL(toggled(bool)), mpGraphWidget, SLOT(updateDragDrop(bool)));
        PropertyDialog::connect(mpRenameNodeButton, SIGNAL(clicked()), mpGraphWidget, SLOT(changeFocusedVertexLabel()));
        PropertyDialog::connect(mpAddPortButton, SIGNAL(clicked()), mpGraphWidget, SLOT(addPortFocused()));
        PropertyDialog::connect(mpSwapPortsButton, SIGNAL(clicked()), mpGraphWidget, SLOT(swapPortsFocused()));
        PropertyDialog::connect(mpRenamePortButton, SIGNAL(clicked()), mpGraphWidget, SLOT(renamePortFocused()));
        PropertyDialog::connect(mpRemovePortButton, SIGNAL(clicked()), mpGraphWidget, SLOT(removePortFocused()));
        PropertyDialog::connect(mpRemovePortsButton, SIGNAL(clicked()), mpGraphWidget, SLOT(removePortsFocused()));
        PropertyDialog::connect(mpRemoveNodeButton, SIGNAL(clicked()), mpGraphWidget, SLOT(removeFocusedVertex()));
        PropertyDialog::connect(mpRenameEdgeButton, SIGNAL(clicked()), mpGraphWidget, SLOT(changeFocusedEdgeLabel()));
        PropertyDialog::connect(mpRemoveEdgeButton, SIGNAL(clicked()), mpGraphWidget, SLOT(removeFocusedEdge()));
        PropertyDialog::connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateMainWidget(int)));

        QMetaObject::connectSlotsByName(Dialog);

        Dialog->setFixedSize(WIDTH + 20, 20 + mHeight - commonExtraPadding);
        tabWidget->setGeometry(QRect(5, 5, WIDTH + 10, 10 + mHeight - commonExtraPadding));
        tabWidget->setCurrentIndex(mpStackedWidget->currentIndex());
        horizontalLayoutWidget->setGeometry(QRect(0, 0, WIDTH, mHeight - commonExtraPadding));
        horizontalLayoutLayerWidget->setGeometry(QRect(0, 0, WIDTH / 2, mHeight - commonExtraPadding));
    } // setupUi

    void retranslateUi(CustomDialog *Dialog)
    {
//        Dialog->setWindowTitle(QApplication::translate("Dialog", "Properties", 0, QApplication::UnicodeUTF8));
        mpAddNodeButton->setText(QApplication::translate("Dialog", "Add Node", 0, QApplication::UnicodeUTF8));
        mpRefreshButton->setText(QApplication::translate("Dialog", "Refresh", 0, QApplication::UnicodeUTF8));
        mpRefreshLayerButton->setText(QApplication::translate("Dialog", "Refresh", 0, QApplication::UnicodeUTF8));
        mpShuffleButton->setText(QApplication::translate("Dialog", "Shuffle", 0, QApplication::UnicodeUTF8));
        mpShuffleLayerButton->setText(QApplication::translate("Dialog", "Shuffle", 0, QApplication::UnicodeUTF8));
        mpImportButton->setText(QApplication::translate("Dialog", "Import", 0, QApplication::UnicodeUTF8));
        mpImportLayerButton->setText(QApplication::translate("Dialog", "Import", 0, QApplication::UnicodeUTF8));
        mpExportButton->setText(QApplication::translate("Dialog", "Export", 0, QApplication::UnicodeUTF8));
        mpExportLayerButton->setText(QApplication::translate("Dialog", "Export", 0, QApplication::UnicodeUTF8));
        mpResetButton->setText(QApplication::translate("Dialog", "Reset", 0, QApplication::UnicodeUTF8));
        mpResetLayerButton->setText(QApplication::translate("Dialog", "Reset", 0, QApplication::UnicodeUTF8));
        mpLayoutButton->setText(QApplication::translate("Dialog", "Layout", 0, QApplication::UnicodeUTF8));
        mpLayoutLayerButton->setText(QApplication::translate("Dialog", "Layout", 0, QApplication::UnicodeUTF8));
        mpDragDropButton->setText(QApplication::translate("Dialog", "Drag'n'Drop", 0, QApplication::UnicodeUTF8));
        mpRenameNodeButton->setText(QApplication::translate("Dialog", "Rename Node", 0, QApplication::UnicodeUTF8));
        mpAddPortButton->setText(QApplication::translate("Dialog", "Add Port", 0, QApplication::UnicodeUTF8));
        mpRenamePortButton->setText(QApplication::translate("Dialog", "Rename a Port", 0, QApplication::UnicodeUTF8));
        mpRemovePortButton->setText(QApplication::translate("Dialog", "Remove a Port", 0, QApplication::UnicodeUTF8));
        mpSwapPortsButton->setText(QApplication::translate("Dialog", "Swap Ports", 0, QApplication::UnicodeUTF8));
        mpRemovePortsButton->setText(QApplication::translate("Dialog", "Remove Ports", 0, QApplication::UnicodeUTF8));
        mpRemoveNodeButton->setText(QApplication::translate("Dialog", "Remove Node", 0, QApplication::UnicodeUTF8));
        mpRenameEdgeButton->setText(QApplication::translate("Dialog", "Rename Edge", 0, QApplication::UnicodeUTF8));
        mpRemoveEdgeButton->setText(QApplication::translate("Dialog", "Remove Edge", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

    void setVertexFocused(bool vertexFocused)
    {
        if(vertexFocused != mVertexFocused)
        {
            mpRenameNodeButton -> setEnabled(vertexFocused);
            mpAddPortButton    -> setEnabled(vertexFocused);
            mpRenamePortButton -> setEnabled(vertexFocused);
            mpRemovePortButton -> setEnabled(vertexFocused);
            mpRemovePortsButton-> setEnabled(vertexFocused);
            mpSwapPortsButton  -> setEnabled(vertexFocused);
            mpRemoveNodeButton -> setEnabled(vertexFocused);
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
    void updateMainWidget(int index)
    {
        mpStackedWidget->setCurrentIndex(index);
    }

private:
    CustomDialog mDialog;
    QStackedWidget* mpStackedWidget;
    GraphWidget *mpGraphWidget;
    LayerWidget *mpLayerWidget;
    QMainWindow *mpMainWindow;
    QTabWidget *tabWidget;
    QWidget *horizontalLayoutWidget;
    QWidget *horizontalLayoutLayerWidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayoutLayer;
    QVBoxLayout *verticalLayoutFocus;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayoutLayer;
    QPushButton *mpAddNodeButton;
    QPushButton *mpRefreshButton;
    QPushButton *mpRefreshLayerButton;
    QPushButton *mpShuffleButton;
    QPushButton *mpShuffleLayerButton;
    QPushButton *mpImportButton;
    QPushButton *mpImportLayerButton;
    QPushButton *mpExportButton;
    QPushButton *mpExportLayerButton;
    QPushButton *mpResetButton;
    QPushButton *mpResetLayerButton;
    QPushButton *mpLayoutButton;
    QPushButton *mpLayoutLayerButton;
    QPushButton *mpDragDropButton;
    QPushButton *mpRenameNodeButton;
    QPushButton *mpAddPortButton;
    QPushButton *mpRenamePortButton;
    QPushButton *mpRemovePortButton;
    QPushButton *mpRemovePortsButton;
    QPushButton *mpRemoveNodeButton;
    QPushButton *mpRenameEdgeButton;
    QPushButton *mpRemoveEdgeButton;
    QPushButton *mpSwapPortsButton;
    QFrames mFrames;
    bool mVertexFocused;
    bool mEdgeFocused;
    int mHeight;
};

} // end namespace gui
} // end namespace graph_analysis

#endif // PROPERTYDIALOG_H
