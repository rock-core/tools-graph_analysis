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
    mpButtonBox = new QDialogButtonBox(Dialog);
    mpButtonBox->setObjectName(QString::fromUtf8("mpButtonBox"));
    mpButtonBox->setGeometry(QRect(230, 120, 181, 32));
    mpButtonBox->setOrientation(Qt::Horizontal);
    mpButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    mpPort1Label = new QLabel(Dialog);
    mpPort1Label->setObjectName(QString::fromUtf8("mpPort1Label"));
    mpPort1Label->setGeometry(QRect(20, 16, 91, 31));
    mpPort2Label = new QLabel(Dialog);
    mpPort2Label->setObjectName(QString::fromUtf8("mpPort2Label"));
    mpPort2Label->setGeometry(QRect(20, 70, 91, 31));
#ifndef LABEL_SWAPPING
    // warning note gets displayed only if the ports swap without their adjacent edges (deprecated)
    mpWarningLabel = new QLabel(Dialog);
    mpWarningLabel->setObjectName(QString::fromUtf8("mpWarningLabel"));
    mpWarningLabel->setGeometry(QRect(20, 105, 70, 31));
    mpWarningTextLabel = new QLabel(Dialog);
    mpWarningTextLabel->setObjectName(QString::fromUtf8("warning"));
    mpWarningTextLabel->setGeometry(QRect(20, 130, 200, 31));
#endif
    // populating the first drop-down box with all the ports
    mpComboBox1 = new QComboBox(Dialog);
    foreach(NodeItem::VTuple tuple, mpNodeItem->getVertices())
    {
        std::string item = boost::lexical_cast<std::string>(tuple.first) + ": " + tuple.second->getLabel();
        mpComboBox1->addItem(QString(item.c_str()));
    }
    mpComboBox1->setObjectName(QString::fromUtf8("mpComboBox1"));
    mpComboBox1->setGeometry(QRect(117, 16, 291, 27));

    // populating the second drop-down box with all the ports
    mpComboBox2 = new QComboBox(Dialog);
    foreach(NodeItem::VTuple tuple, mpNodeItem->getVertices())
    {
        std::string item = boost::lexical_cast<std::string>(tuple.first) + ": " + tuple.second->getLabel();
        mpComboBox2->addItem(QString(item.c_str()));
    }
    mpComboBox2->setObjectName(QString::fromUtf8("mpComboBox2"));
    mpComboBox2->setGeometry(QRect(117, 70, 291, 27));

    // attributing relevant titles to be displayed in the scene (e.g. buttons labels)
    retranslateUi(Dialog);
    // establishing signal-slot connections
    QObject::connect(mpButtonBox, SIGNAL(accepted()), this, SLOT(swapAccept()));
    QObject::connect(mpButtonBox, SIGNAL(rejected()), this, SLOT(swapReject()));
    QObject::connect(mpButtonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
    QObject::connect(mpButtonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(Dialog);
} // setupUi

void SwapPortsDialog::retranslateUi(QDialog *Dialog)
{
    Dialog -> setWindowTitle(QApplication::translate("Dialog", "Swap Ports", 0, QApplication::UnicodeUTF8));
    mpPort1Label    -> setText(QApplication::translate("Dialog", "Port ID:", 0, QApplication::UnicodeUTF8));
    mpPort2Label    -> setText(QApplication::translate("Dialog", "Port ID:", 0, QApplication::UnicodeUTF8));
#ifndef LABEL_SWAPPING
    // warning note gets displayed only if the ports swap without their adjacent edges (deprecated)
    mpWarningLabel      -> setText(QApplication::translate("Dialog", "Warning:", 0, QApplication::UnicodeUTF8));
    mpWarningTextLabel  -> setText(QApplication::translate("Dialog", "Edges are not affected by the change!", 0, QApplication::UnicodeUTF8));
#endif
} // retranslateUi

void SwapPortsDialog::swapAccept()
{
    mValid = true;
    mPort1ID  = mpComboBox1->currentText().toStdString();
    mPort2ID  = mpComboBox2->currentText().toStdString();
}

void SwapPortsDialog::swapReject()
{
    mValid = false;
}

} // end namespace gui
} // end namespace graph_analysis
