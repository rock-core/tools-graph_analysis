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

    class NodeItem;

/**
 * \file AddNodeDialog.hpp
 * \class AddNodeDialog
 * \brief pop-up dialog manager for adding a node to the graph
 * \details runs a pop-window to prompt the user for the new node
 *      label and for its type
 */
class AddNodeDialog : public QObject
{
    Q_OBJECT
public:
    /// empty constructor
    AddNodeDialog();
    /// destructor
    ~AddNodeDialog(){}
    /// sets up the provided qt dialog instance
    void setupUi(QDialog *Dialog);
    /// sets up visible titles for the scene (e.g. buttons labels)
    void retranslateUi(QDialog *Dialog);
    /// getter method for boolean validation witness
    bool isValid() { return mValid; }
    /// getter method for the node label dialog result
    std::string getNodeLabel() { return mNodeLabel; }
    /// getter method for the node type dialog result
    std::string getNodeType () { return mNodeType;  }

public slots:
    /// slot for listening for whenever the user presses "Ok" in the resulting pop-up dialog window
    void nodeAccept();
    /// slot for listening for whenever the user presses "Cancel" (or ESC/close window corner button) in the resulting pop-up dialog window
    void nodeReject();

private:
    /// main qt dialog instance to be used
    QDialog mDialog;
    /// stores the node label user-interaction result
    std::string mNodeLabel;
    /// stores the node type user-interaction result
    std::string mNodeType;

    /// boolean witness for validation; true whenever the user-interaction ends with the "Ok" button being pressed; false otherwise
    bool mValid;
    /// in-scene set of buttons for this pop-window ("Ok" and "Cancel")
    QDialogButtonBox *mpButtonBox;
    /// in scene label displaying "Node Label:"
    QLabel *mpNodeLabel;
    /// in scene label displaying "Node Type:"
    QLabel *mpNodeTypeLabel;
    /// GUI element where the user types the desired node label
    QLineEdit *mpLineEdit;
    /// GUI element from which the user selects the desired node type
    QComboBox *mpComboBox;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // ADDNODEDIALOG_H
