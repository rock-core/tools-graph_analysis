#include "RenameFilterDialog.hpp"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>


namespace graph_analysis {
namespace gui {
namespace dialogs {

RenameFilterDialog::RenameFilterDialog(const FilterManager::Filters& filters)
: mFilters(filters)
, mValid(false)
{
    setupUi(&mDialog);
    mDialog.exec();
}

void RenameFilterDialog::setupUi(QDialog *Dialog)
{
    if (Dialog->objectName().isEmpty())
        Dialog->setObjectName(QString::fromUtf8("Dialog"));
    Dialog->resize(441, 169);
    mpButtonBox = new QDialogButtonBox(Dialog);
    mpButtonBox->setObjectName(QString::fromUtf8("mpButtonBox"));
    mpButtonBox->setGeometry(QRect(230, 120, 181, 32));
    mpButtonBox->setOrientation(Qt::Horizontal);
    mpButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    mpNewRegexp = new QLabel(Dialog);
    mpNewRegexp->setObjectName(QString::fromUtf8("mpNewRegexp"));
    mpNewRegexp->setGeometry(QRect(20, 16, 91, 31));
    mpLineEdit = new QLineEdit(Dialog);
    mpLineEdit->setObjectName(QString::fromUtf8("mpLineEdit"));
    mpLineEdit->setGeometry(QRect(120, 20, 291, 27));
    mpFilterLabel = new QLabel(Dialog);
    mpFilterLabel->setObjectName(QString::fromUtf8("mpFilterLabel"));
    mpFilterLabel->setGeometry(QRect(20, 70, 91, 31));
    mpComboBox = new QComboBox(Dialog);
    // fetching all filters in the concerned provided filters container
    foreach(FilterItem *item, mFilters)
    {
        std::string item_index = boost::lexical_cast<std::string>(item->getIndex());
        mpComboBox->addItem(QString(item_index.c_str())  + QString(": ") + item->getLabel());
    }
    mpComboBox->setObjectName(QString::fromUtf8("mpComboBox"));
    mpComboBox->setGeometry(QRect(117, 70, 291, 27));

    // attributing relevant titles to be displayed in the scene (e.g. buttons labels)
    retranslateUi(Dialog);
    // establishing signal-slot connections
    QObject::connect(mpButtonBox, SIGNAL(accepted()), this, SLOT(renameAccept()));
    QObject::connect(mpButtonBox, SIGNAL(rejected()), this, SLOT(renameReject()));
    QObject::connect(mpButtonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
    QObject::connect(mpButtonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(Dialog);
} // setupUi

void RenameFilterDialog::retranslateUi(QDialog *Dialog)
{
#if QT_VERSION < 0x050000
    Dialog->setWindowTitle(QApplication::translate("Dialog", "Rename Filter", 0, QApplication::UnicodeUTF8));
    mpNewRegexp->setText(QApplication::translate("Dialog", "New regexp:", 0, QApplication::UnicodeUTF8));
    mpFilterLabel->setText(QApplication::translate("Dialog", "Filter:", 0, QApplication::UnicodeUTF8));
#else
    Dialog->setWindowTitle(QApplication::translate("Dialog", "Rename Filter"));
    mpNewRegexp->setText(QApplication::translate("Dialog", "New regexp:"));
    mpFilterLabel->setText(QApplication::translate("Dialog", "Filter:"));
#endif
} // retranslateUi

void RenameFilterDialog::renameAccept()
{
    mValid = true;
    mNewRegexp = mpLineEdit->text();
    mFilterIndex  = mpComboBox->currentText().toStdString();
}

void RenameFilterDialog::renameReject()
{
    mValid = false;
}

} // end namespace dialogs
} // end namespace gui
} // end namespace graph_analysis
