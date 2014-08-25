#include "Utils.hpp"
#include <QTreeWidget>

namespace omviz
{

QTreeWidgetItem* Utils::createTreeWidgetItem(const std::string& label, const std::string& data)
{
    QTreeWidgetItem* item = new QTreeWidgetItem( (QTreeWidget*) 0, QStringList(QString("%1").arg(label.c_str())) );
    if(!data.empty())
    {
        QVariant value = QString(data.c_str());
        item->setData(0, Qt::ToolTipRole, value);
    }
    return item;
}

}
