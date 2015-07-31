/********************************************************************************
** Form generated from reading UI file 'add_node_dialog_gui.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef RENAMEPORTDIALOG_H
#define RENAMEPORTDIALOG_H

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
 * \class RenamePortDialog
 * \file RenamePortDialog.hpp
 * \brief pop-up dialog manager for renaming a port of a cluster node
 */
class RenamePortDialog : public QObject
{
    Q_OBJECT
public:
    /// constructor; stores the concerned cluster node reference
    RenamePortDialog(NodeItem *nodeItem);
    /// destructor
    ~RenamePortDialog(){}
    /// sets up the provided qt dialog instance
    void setupUi(QDialog *Dialog);
    /// sets up visible titles for the scene (e.g. buttons labels)
    void retranslateUi(QDialog *Dialog);
    /// getter method for boolean validation witness
    bool isValid() { return mValid; }
    /// getter method for the new port label dialog result
    std::string getNewLabel() { return mNewLabel; }
    /// getter method for the node type dialog result
    std::string getPortID () { return mPortID;  }

public slots:
    /// slot for listening for whenever the user presses "Ok" in the resulting pop-up dialog window
    void renameAccept();
    /// slot for listening for whenever the user presses "Cancel" (or ESC/close window corner button) in the resulting pop-up dialog window
    void renameReject();

private:
    /// main qt dialog instance to be used
    QDialog mDialog;
    /// stores the new port label user-interaction result
    std::string mNewLabel;
    /// stores the port node ID user-interaction result
    std::string mPortID;

    /// concerned cluster node
    NodeItem *mpNodeItem;
    /// boolean witness for validation; true whenever the user-interaction ends with the "Ok" button being pressed; false otherwise
    bool mValid;
    /// in-scene set of buttons for this pop-window ("Ok" and "Cancel")
    QDialogButtonBox *mpButtonBox;
    /// in scene label displaying "New Label:"
    QLabel *mpNewLabel;
    /// in scene label displaying "Port ID:"
    QLabel *mpPortLabel;
    /// GUI element where the user types the new desired port label
    QLineEdit *mpLineEdit;
    /// GUI element from which the user selects the desired port to be renamed
    QComboBox *mpComboBox;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // RENAMEPORTDIALOG_H
