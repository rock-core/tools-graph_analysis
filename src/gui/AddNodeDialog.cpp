#include "AddNodeDialog.hpp"
#include <boost/foreach.hpp>


namespace graph_analysis {
namespace gui {

AddNodeDialog::AddNodeDialog()
: mValid(false)
{
    setupUi(&mDialog);
    mDialog.exec();
}

void AddNodeDialog::setupUi(QDialog *Dialog)
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
    mpNodeLabel = new QLabel(Dialog);
    mpNodeLabel->setObjectName(QString::fromUtf8("mpNodeLabel"));
    mpNodeLabel->setGeometry(QRect(20, 16, 91, 31));
    mpLineEdit = new QLineEdit(Dialog);
    mpLineEdit->setObjectName(QString::fromUtf8("mpLineEdit"));
    mpLineEdit->setGeometry(QRect(120, 20, 291, 27));
    mpNodeTypeLabel = new QLabel(Dialog);
    mpNodeTypeLabel->setObjectName(QString::fromUtf8("mpNodeTypeLabel"));
    mpNodeTypeLabel->setGeometry(QRect(20, 70, 91, 31));
    mpComboBox = new QComboBox(Dialog);
    // fetching all supported and allowed node types
//    std::set<std::string> types = VertexTypeManager::getInstance()->getSupportedTypes();
//    foreach(std::string type, types)
//    {
//        mpComboBox->addItem(QString(type.c_str()));
//    }
    mpComboBox->addItem(QString("cluster"));
    mpComboBox->setObjectName(QString::fromUtf8("mpComboBox"));
    mpComboBox->setGeometry(QRect(117, 70, 291, 27));

    // attributing relevant titles to be displayed in the scene (e.g. buttons labels)
    retranslateUi(Dialog);
    // establishing signal-slot connections
    QObject::connect(mpButtonBox, SIGNAL(accepted()), this, SLOT(nodeAccept()));
    QObject::connect(mpButtonBox, SIGNAL(rejected()), this, SLOT(nodeReject()));
    QObject::connect(mpButtonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
    QObject::connect(mpButtonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(Dialog);
} // setupUi

void AddNodeDialog::retranslateUi(QDialog *Dialog)
{
    Dialog->setWindowTitle(QApplication::translate("Dialog", "Create New Node", 0, QApplication::UnicodeUTF8));
    mpNodeLabel->setText(QApplication::translate("Dialog", "Node Label:", 0, QApplication::UnicodeUTF8));
    mpNodeTypeLabel->setText(QApplication::translate("Dialog", "Node Type:", 0, QApplication::UnicodeUTF8));
} // retranslateUi

void AddNodeDialog::nodeAccept()
{
    mValid = true;
    mNodeLabel = mpLineEdit->text().toStdString();
    mNodeType  = mpComboBox->currentText().toStdString();
}

void AddNodeDialog::nodeReject()
{
    mValid = false;
}

} // end namespace gui
} // end namespace graph_analysis
