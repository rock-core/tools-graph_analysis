/********************************************************************************
** Form generated from reading UI file 'add_node_dialog_gui.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef RENAMEFILTERDIALOG_H
#define RENAMEFILTERDIALOG_H

#include "../FilterManager.hpp"

#include <set>
#include <string>
#include <QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

namespace graph_analysis {
namespace gui {
namespace dialogs {

/**
 * \file RenameFilterDialog.hpp
 * \class RenameFilterDialog
 * \brief pop-up dialog manager for renaming a regexp filter in the custom regexp filters manager
 * \details runs a pop-up window to prompt the user for the new regex
 *      as well as for the filter item to be edited
 */
class RenameFilterDialog : public QObject
{
    Q_OBJECT
public:
    /// constructor; stores the concerned cluster node reference
    RenameFilterDialog(const FilterManager::Filters& filters);
    /// destructor
    ~RenameFilterDialog(){}
    /// sets up the provided qt dialog instance
    void setupUi(QDialog *Dialog);
    /// sets up visible titles for the scene (e.g. buttons labels)
    void retranslateUi(QDialog *Dialog);
    /// getter method for boolean validation witness
    bool isValid() { return mValid; }
    /// getter method for the new feature label dialog result
    QString getNewRegexp() { return mNewRegexp; }
    /// getter method for the node type dialog result
    std::string getFilterIndex () { return mFilterIndex; }

public slots:
    /// slot for listening for whenever the user presses "Ok" in the resulting pop-up dialog window
    void renameAccept();
    /// slot for listening for whenever the user presses "Cancel" (or ESC/close window corner button) in the resulting pop-up dialog window
    void renameReject();

private:
    /// main qt dialog instance to be used
    QDialog mDialog;
    /// stores the new filter regexp user-interaction result
    QString mNewRegexp;
    /// stores the filter item index user-interaction result
    std::string mFilterIndex;

    /// concerned container with filter items
    FilterManager::Filters mFilters;
    /// boolean witness for validation; true whenever the user-interaction ends with the "Ok" button being pressed; false otherwise
    bool mValid;
    /// in-scene set of buttons for this pop-window ("Ok" and "Cancel")
    QDialogButtonBox *mpButtonBox;
    /// in scene label displaying "New regexp:"
    QLabel *mpNewRegexp;
    /// in-scene label displaying "Filter:"
    QLabel *mpFilterLabel;
    /// GUI element where the user types the new desired feature label
    QLineEdit *mpLineEdit;
    /// GUI element from which the user selects the desired feature to be renamed
    QComboBox *mpComboBox;
};

} // end namespace dialogs
} // end namespace gui
} // end namespace graph_analysis
#endif // RENAMEFILTERDIALOG_H
