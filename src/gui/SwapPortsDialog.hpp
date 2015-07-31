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

/**
 * \class SwapPortsDialog
 * \file SwapPortsDialog.hpp
 * \brief pop-up dialog manager for swapping two ports of a cluster node
 */
class SwapPortsDialog : public QObject
{
    Q_OBJECT
public:
    /// constructor; stores the concerned cluster node reference
    SwapPortsDialog(NodeItem *nodeItem);
    /// destructor
    ~SwapPortsDialog(){}
    /// sets up the provided qt dialog instance
    void setupUi(QDialog *Dialog);
    /// sets up visible titles for the scene (e.g. buttons labels)
    void retranslateUi(QDialog *Dialog);
    /// getter method for boolean validation witness
    bool isValid() { return mValid; }
    /// getter method for the first port ID dialog result
    std::string getPort1ID () { return mPort1ID;  }
    /// getter method for the second port ID dialog result
    std::string getPort2ID () { return mPort2ID;  }

public slots:
    /// slot for listening for whenever the user presses "Ok" in the resulting pop-up dialog window
    void swapAccept();
    /// slot for listening for whenever the user presses "Cancel" (or ESC/close window corner button) in the resulting pop-up dialog window
    void swapReject();

private:
    /// main qt dialog instance to be used
    QDialog mDialog;
    /// stores the chosen first port ID as text user-interaction result
    std::string mPort1ID;
    /// stores the chosen second port ID as text user-interaction result
    std::string mPort2ID;

    /// concerned cluster node
    NodeItem *mpNodeItem;
    /// boolean witness for validation; true whenever the user-interaction ends with the "Ok" button being pressed; false otherwise
    bool mValid;
    /// in-scene set of buttons for this pop-window ("Ok" and "Cancel")
    QDialogButtonBox *mpButtonBox;
    /// in scene label displaying "Port ID:"
    QLabel *mpPort1Label;
    /// in scene label displaying "Port ID:"
    QLabel *mpPort2Label;
    /// in scene label displaying "Warning:" in case full label swapping is not supported (i.e. edges are not affected by swapping - deprecated)
    QLabel *mpWarningLabel;
    /// in scene label displaying the warning message in case full label swapping is not supported (i.e. edges are not affected by swapping - deprecated)
    QLabel *mpWarningTextLabel;
    /// GUI element from which the user selects the first desired port for swapping
    QComboBox *mpComboBox1;
    /// GUI element from which the user selects the second desired port for swapping
    QComboBox *mpComboBox2;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // SWAPPORTSDIALOG_H
