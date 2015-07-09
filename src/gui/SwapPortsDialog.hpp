/********************************************************************************
** Form generated from reading UI file 'add_node_dialog_gui.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef SWAPPORTSDIALOG_H
#define SWAPPORTSDIALOG_H

#include <set>
#include <string>
#include <QObject>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/VertexTypeManager.hpp>

namespace graph_analysis {
namespace gui {

    class NodeItem;

class SwapPortsDialog : public QObject
{
    Q_OBJECT
public:
    SwapPortsDialog(NodeItem *nodeItem);
    ~SwapPortsDialog(){}
    void setupUi(QDialog *Dialog);
    void retranslateUi(QDialog *Dialog);
    bool isValid() { return mValid; }
    std::string getPort1ID () { return mPort1ID;  }
    std::string getPort2ID () { return mPort2ID;  }

public slots:
    void nodeAccept();
    void nodeReject();

private:
    QDialog mDialog;
    std::string mNewLabel;
    std::string mPort1ID;
    std::string mPort2ID;

    NodeItem *mpNodeItem;
    bool mValid;
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *warning;
    QLabel *warningText;
    QComboBox *comboBox1;
    QComboBox *comboBox2;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // SWAPPORTSDIALOG_H
