/********************************************************************************
** Form generated from reading UI file 'add_node_dialog_gui.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef SWAPFILTERSDIALOG_H
#define SWAPFILTERSDIALOG_H

#include "../FilterManager.hpp"

#include <set>
#include <string>
#include <QObject>
#include <QtCore/QVariant>
#if QT_VERSION < 0x050000
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#else
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#endif

namespace graph_analysis {
namespace gui {
namespace dialogs {

/**
 * \file SwapFiltersDialog.hpp
 * \class SwapFiltersDialog
 * \brief pop-up dialog manager for swapping two custom regexp filters in the filters manager in the layers tab of the command panel
 * \details runs a pop-up dialog for prompting the user for the 2 filters to be swapped
 */
class SwapFiltersDialog : public QObject
{
    Q_OBJECT
public:
    /// constructor; stores the concerned cluster node reference
    SwapFiltersDialog(const FilterManager::Filters& filters);
    /// destructor
    ~SwapFiltersDialog(){}
    /// sets up the provided qt dialog instance
    void setupUi(QDialog *Dialog);
    /// sets up visible titles for the scene (e.g. buttons labels)
    void retranslateUi(QDialog *Dialog);
    /// getter method for boolean validation witness
    bool isValid() { return mValid; }
    /// getter method for the first feature ID dialog result
    std::string getFilter1Index () { return mFilter1Index;  }
    /// getter method for the second feature ID dialog result
    std::string getFilter2Index () { return mFilter2Index;  }

public slots:
    /// slot for listening for whenever the user presses "Ok" in the resulting pop-up dialog window
    void swapAccept();
    /// slot for listening for whenever the user presses "Cancel" (or ESC/close window corner button) in the resulting pop-up dialog window
    void swapReject();

private:
    /// main qt dialog instance to be used
    QDialog mDialog;
    /// stores the chosen first filter index as text user-interaction result
    std::string mFilter1Index;
    /// stores the chosen second filter index as text user-interaction result
    std::string mFilter2Index;

    /// concerned container with filter items
    FilterManager::Filters mFilters;
    /// boolean witness for validation; true whenever the user-interaction ends with the "Ok" button being pressed; false otherwise
    bool mValid;
    /// in-scene set of buttons for this pop-window ("Ok" and "Cancel")
    QDialogButtonBox *mpButtonBox;
    /// in scene label displaying "Filter Index:"
    QLabel *mpFilter1Label;
    /// in scene label displaying "Filter Index:"
    QLabel *mpFilter2Label;
    /// GUI element from which the user selects the first desired regexp filter for swapping
    QComboBox *mpComboBox1;
    /// GUI element from which the user selects the second desired regexp filter for swapping
    QComboBox *mpComboBox2;
};

} // end namespace dialogs
} // end namespace gui
} // end namespace graph_analysis
#endif // SWAPFILTERSDIALOG_H
