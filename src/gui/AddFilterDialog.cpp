#include "AddFilterDialog.hpp"
#include <boost/foreach.hpp>


namespace graph_analysis {
namespace gui {

AddFilterDialog::AddFilterDialog()
: mValid(false)
{
    setupUi(&mDialog);
    mDialog.exec();
}

void AddFilterDialog::setupUi(QDialog *Dialog)
{
    if (Dialog->objectName().isEmpty())
    {
        Dialog->setObjectName(QString::fromUtf8("Dialog"));
    }
    Dialog->resize(441, 169);
    mpButtonBox = new QDialogButtonBox(Dialog);
    mpButtonBox->setObjectName(QString::fromUtf8("mpButtonBox"));
    mpButtonBox->setGeometry(QRect(230, 120, 181, 32));
    mpButtonBox->setOrientation(Qt::Horizontal);
    mpButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    mpFilterLabel = new QLabel(Dialog);
    mpFilterLabel->setObjectName(QString::fromUtf8("mpFilterLabel"));
    mpFilterLabel->setGeometry(QRect(20, 16, 91, 31));
    mpLineEdit = new QLineEdit(Dialog);
    mpLineEdit->setObjectName(QString::fromUtf8("mpLineEdit"));
    mpLineEdit->setGeometry(QRect(120, 20, 291, 27));
    mpCheckBox = new QCheckBox(Dialog);
    mpCheckBox->setObjectName(QString::fromUtf8("mpCheckBox"));
    mpCheckBox->setGeometry(QRect(120, 70, 111, 22));

    // attributing relevant titles to be displayed in the scene (e.g. buttons labels)
    retranslateUi(Dialog);
    // establishing signal-slot connections
    QObject::connect(mpButtonBox, SIGNAL(accepted()), this, SLOT(filterAccept()));
    QObject::connect(mpButtonBox, SIGNAL(rejected()), this, SLOT(filterReject()));
    QObject::connect(mpButtonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
    QObject::connect(mpButtonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(Dialog);
} // setupUi

void AddFilterDialog::retranslateUi(QDialog *Dialog)
{
    Dialog->setWindowTitle(QApplication::translate("Dialog", "Create New Regexp Filter", 0, QApplication::UnicodeUTF8));
    mpFilterLabel->setText(QApplication::translate("Dialog", "Filter regexp:", 0, QApplication::UnicodeUTF8));
    mpCheckBox->setText(QApplication::translate("Dialog", "enable filter", 0, QApplication::UnicodeUTF8));
} // retranslateUi

void AddFilterDialog::filterAccept()
{
    mValid = true;
    mFilterRegexp = mpLineEdit->text().toStdString();
    mEnable = mpCheckBox->isChecked();
}

void AddFilterDialog::filterReject()
{
    mValid = false;
}

} // end namespace gui
} // end namespace graph_analysis
