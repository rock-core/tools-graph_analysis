#ifndef OMVIZ_PROPERTIESWIDGET_PROPERTIES_WIDGET_HPP
#define OMVIZ_PROPERTIESWIDGET_PROPERTIES_WIDGET_HPP

#include <QWidget>
#include <QTabWidget>

namespace Ui
{
    class PropertiesWidget;
}

namespace omviz
{

class PropertiesWidget : public QTabWidget
{
    Q_OBJECT
public:
    PropertiesWidget(QWidget* parent = 0);
    ~PropertiesWidget();

signals:
    void layoutChanged(const QString& layoutName);

private:
    Ui::PropertiesWidget* mUi;

};

} // end namespace omviz
#endif // OMVIZ_PROPERTIESWIDGET_PROPERTIES_WIDGET_HPP
