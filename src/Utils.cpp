#include "Utils.hpp"
#include <QTreeWidget>

namespace omviz
{

QTreeWidgetItem* Utils::createTreeWidgetItem(const std::string& label)
{
    return new QTreeWidgetItem( (QTreeWidget*) 0, QStringList(QString("%1").arg(label.c_str())) );
}

}
