#include "Utils.hpp"
#include <QTreeWidget>
#include <boost/algorithm/string.hpp>

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

std::string Utils::escapeString(const std::string& value)
{
    std::string txt = value;
    boost::replace_all(txt, "/","\\/");
    boost::replace_all(txt, "[","\\[");
    boost::replace_all(txt, "]","\\]");
    boost::replace_all(txt, "+","\\+");
    boost::replace_all(txt, "*","\\*");
    return txt;
}

}
