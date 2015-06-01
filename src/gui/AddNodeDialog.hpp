/********************************************************************************
** Form generated from reading UI file 'add_node_dialog_gui.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ADDNODEDIALOG_H
#define ADDNODEDIALOG_H

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
#include <QtGui/QLineEdit>
#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/VertexTypeManager.hpp>

namespace graph_analysis {
namespace gui {

class AddNodeDialog : public QObject
{
    Q_OBJECT
public:
    AddNodeDialog();
    ~AddNodeDialog(){}
    void setupUi(QDialog *Dialog);
    void retranslateUi(QDialog *Dialog);
    bool isValid() { return mValid; }
    std::string getNodeLabel() { return mNodeLabel; }
    std::string getNodeType () { return mNodeType;  }

public slots:
    void nodeAccept();
    void nodeReject();

private:
    QDialog mDialog;
    std::string mNodeLabel;
    std::string mNodeType;

    bool mValid;
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QComboBox *comboBox;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // ADDNODEDIALOG_H
