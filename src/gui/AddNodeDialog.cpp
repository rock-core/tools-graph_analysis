#include "AddNodeDialog.hpp"
#include <boost/foreach.hpp>


namespace graph_analysis {
namespace gui {
    
    class NodeItem;

AddNodeDialog::AddNodeDialog()
: mValid(false)
{
    setupUi(&mDialog);
    mDialog.exec();
}

void AddNodeDialog::setupUi(QDialog *Dialog)
{
    if (Dialog->objectName().isEmpty())
        Dialog->setObjectName(QString::fromUtf8("Dialog"));
    Dialog->resize(441, 169);
    buttonBox = new QDialogButtonBox(Dialog);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(230, 120, 181, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    label = new QLabel(Dialog);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(20, 16, 91, 31));
    lineEdit = new QLineEdit(Dialog);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
    lineEdit->setGeometry(QRect(120, 20, 291, 27));
    label_2 = new QLabel(Dialog);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(20, 70, 91, 31));
    comboBox = new QComboBox(Dialog);
    std::set<std::string> types = NodeItem::getSupportedTypes();
    foreach(std::string type, types)
    {
        comboBox->addItem(QString(type.c_str()));
    }
    comboBox->setObjectName(QString::fromUtf8("comboBox"));
    comboBox->setGeometry(QRect(117, 70, 291, 27));

    retranslateUi(Dialog);
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(nodeAccept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(nodeReject()));
    QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(Dialog);
} // setupUi

void AddNodeDialog::retranslateUi(QDialog *Dialog)
{
    Dialog->setWindowTitle(QApplication::translate("Dialog", "Create New Node", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("Dialog", "Node Label:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("Dialog", "Node Type:", 0, QApplication::UnicodeUTF8));
} // retranslateUi

void AddNodeDialog::nodeAccept()
{
    mValid = true;
    mNodeLabel = lineEdit->text().toStdString();
    mNodeType  = comboBox->currentText().toStdString();
}

void AddNodeDialog::nodeReject()
{
    mValid = false;
}

} // end namespace gui
} // end namespace graph_analysis
