#include "SwapPortsDialog.hpp"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>


namespace graph_analysis {
namespace gui {

SwapPortsDialog::SwapPortsDialog(NodeItem *nodeItem)
: mpNodeItem(nodeItem)
, mValid(false)
{
    setupUi(&mDialog);
    mDialog.exec();
}

void SwapPortsDialog::setupUi(QDialog *Dialog)
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
    label_2 = new QLabel(Dialog);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(20, 70, 91, 31));
    warning = new QLabel(Dialog);
    warning->setObjectName(QString::fromUtf8("warning"));
    warning->setGeometry(QRect(20, 105, 70, 31));
    warningText = new QLabel(Dialog);
    warningText->setObjectName(QString::fromUtf8("warning"));
    warningText->setGeometry(QRect(20, 130, 200, 31));
    comboBox1 = new QComboBox(Dialog);
    foreach(NodeItem::VTuple tuple, mpNodeItem->getVertices())
    {
        std::string item = boost::lexical_cast<std::string>(tuple.first) + ": " + tuple.second->getLabel();
        comboBox1->addItem(QString(item.c_str()));
    }
    comboBox1->setObjectName(QString::fromUtf8("comboBox1"));
    comboBox1->setGeometry(QRect(117, 16, 291, 27));

    comboBox2 = new QComboBox(Dialog);
    foreach(NodeItem::VTuple tuple, mpNodeItem->getVertices())
    {
        std::string item = boost::lexical_cast<std::string>(tuple.first) + ": " + tuple.second->getLabel();
        comboBox2->addItem(QString(item.c_str()));
    }
    comboBox2->setObjectName(QString::fromUtf8("comboBox2"));
    comboBox2->setGeometry(QRect(117, 70, 291, 27));

    retranslateUi(Dialog);
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(nodeAccept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(nodeReject()));
    QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(Dialog);
} // setupUi

void SwapPortsDialog::retranslateUi(QDialog *Dialog)
{
    Dialog -> setWindowTitle(QApplication::translate("Dialog", "Swap Ports", 0, QApplication::UnicodeUTF8));
    label  -> setText(QApplication::translate("Dialog", "Port ID:", 0, QApplication::UnicodeUTF8));
    label_2-> setText(QApplication::translate("Dialog", "Port ID:", 0, QApplication::UnicodeUTF8));
    warning-> setText(QApplication::translate("Dialog", "Warning:", 0, QApplication::UnicodeUTF8));
    warningText -> setText(QApplication::translate("Dialog", "Edges are not affected by the change!", 0, QApplication::UnicodeUTF8));
} // retranslateUi

void SwapPortsDialog::nodeAccept()
{
    mValid = true;
    mPort1ID  = comboBox1->currentText().toStdString();
    mPort2ID  = comboBox2->currentText().toStdString();
}

void SwapPortsDialog::nodeReject()
{
    mValid = false;
}

} // end namespace gui
} // end namespace graph_analysis
