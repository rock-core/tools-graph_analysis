/********************************************************************************
** Form generated from reading UI file 'add_filter_dialog_gui.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ADDFILTERDIALOG_H
#define ADDFILTERDIALOG_H

#include <string>
#include <QObject>
#include <QtGui/QAction>
#include <QtCore/QVariant>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

namespace graph_analysis {
namespace gui {
namespace dialogs {

/**
 * \file AddFilterDialog.hpp
 * \class AddFilterDialog
 * \brief pop-up dialog manager for adding a custom regex filter to the filters manager
 * \details runs a pop-up dialog for prompting the user for a filter regex text and for
 *      whether the new filter should be checked on init or not
 */
class AddFilterDialog : public QObject
{
    Q_OBJECT
public:
    /// empty constructor
    AddFilterDialog();
    /// destructor
    ~AddFilterDialog(){}
    /// sets up the provided qt dialog instance
    void setupUi(QDialog *Dialog);
    /// sets up visible titles for the scene (e.g. buttons labels)
    void retranslateUi(QDialog *Dialog);
    /// getter method for boolean validation witness
    bool isValid() { return mValid; }
    /// getter method for the filter regexp dialog result
    std::string getFilterRegexp() { return mFilterRegexp; }
    /// getter method for the filter-enabling dialog result
    bool isEnabled() { return mEnable; }

public slots:
    /// slot for listening for whenever the user presses "Ok" in the resulting pop-up dialog window
    void filterAccept();
    /// slot for listening for whenever the user presses "Cancel" (or ESC/close window corner button) in the resulting pop-up dialog window
    void filterReject();

private:
    /// main qt dialog instance to be used
    QDialog mDialog;
    /// stores the filter regexp user-interaction result
    std::string mFilterRegexp;
    /// stores the filter-enabled user-interaction result
    bool mEnable;

    /// boolean witness for validation; true whenever the user-interaction ends with the "Ok" button being pressed; false otherwise
    bool mValid;
    /// in-scene set of buttons for this pop-window ("Ok" and "Cancel")
    QDialogButtonBox *mpButtonBox;
    /// in scene label displaying "Filter regexp:"
    QLabel *mpFilterLabel;
    /// GUI element where the user types the desired filter regexp
    QLineEdit *mpLineEdit;
    /// GUI element from which the user enables or disables the corresponding filter checkbox on init
    QCheckBox *mpCheckBox;
};

} // end namespace dialogs
} // end namespace gui
} // end namespace graph_analysis
#endif // ADDFILTERDIALOG_H
