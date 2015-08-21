#include "SwapFiltersDialog.hpp"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>


namespace graph_analysis {
namespace gui {

SwapFiltersDialog::SwapFiltersDialog(const FilterManager::Filters& filters)
: mFilters(filters)
, mValid(false)
{
    setupUi(&mDialog);
    mDialog.exec();
}

void SwapFiltersDialog::setupUi(QDialog *Dialog)
{
    if (Dialog->objectName().isEmpty())
        Dialog->setObjectName(QString::fromUtf8("Dialog"));
    Dialog->resize(441, 169);
    mpButtonBox = new QDialogButtonBox(Dialog);
    mpButtonBox->setObjectName(QString::fromUtf8("mpButtonBox"));
    mpButtonBox->setGeometry(QRect(230, 120, 181, 32));
    mpButtonBox->setOrientation(Qt::Horizontal);
    mpButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    mpFilter1Label = new QLabel(Dialog);
    mpFilter1Label->setObjectName(QString::fromUtf8("mpFilter1Label"));
    mpFilter1Label->setGeometry(QRect(20, 16, 91, 31));
    mpFilter2Label = new QLabel(Dialog);
    mpFilter2Label->setObjectName(QString::fromUtf8("mpFilter2Label"));
    mpFilter2Label->setGeometry(QRect(20, 70, 91, 31));
    // populating the drop-down boxes with all the present filters
    mpComboBox1 = new QComboBox(Dialog);
    mpComboBox1->setObjectName(QString::fromUtf8("mpComboBox1"));
    mpComboBox1->setGeometry(QRect(117, 16, 291, 27));
    mpComboBox2 = new QComboBox(Dialog);
    mpComboBox2->setObjectName(QString::fromUtf8("mpComboBox2"));
    mpComboBox2->setGeometry(QRect(117, 70, 291, 27));
    // fetching all filters in the concerned provided filters container
    foreach(FilterItem *item, mFilters)
    {
        std::string item_index = boost::lexical_cast<std::string>(item->getIndex());
        QString option = QString(item_index.c_str())  + QString(": ") + item->getLabel();
        mpComboBox1->addItem(option);
        mpComboBox2->addItem(option);
    }

    // attributing relevant titles to be displayed in the scene (e.g. buttons labels)
    retranslateUi(Dialog);
    // establishing signal-slot connections
    QObject::connect(mpButtonBox, SIGNAL(accepted()), this, SLOT(swapAccept()));
    QObject::connect(mpButtonBox, SIGNAL(rejected()), this, SLOT(swapReject()));
    QObject::connect(mpButtonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
    QObject::connect(mpButtonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(Dialog);
} // setupUi

void SwapFiltersDialog::retranslateUi(QDialog *Dialog)
{
    Dialog -> setWindowTitle(QApplication::translate("Dialog", "Swap Regexp Filters", 0, QApplication::UnicodeUTF8));
    mpFilter1Label    ->   setText(QApplication::translate("Dialog", "Filter Index:", 0, QApplication::UnicodeUTF8));
    mpFilter2Label    ->   setText(QApplication::translate("Dialog", "Filter Index:", 0, QApplication::UnicodeUTF8));
} // retranslateUi

void SwapFiltersDialog::swapAccept()
{
    mValid = true;
    mFilter1Index  = mpComboBox1->currentText().toStdString();
    mFilter2Index  = mpComboBox2->currentText().toStdString();
}

void SwapFiltersDialog::swapReject()
{
    mValid = false;
}

} // end namespace gui
} // end namespace graph_analysis
