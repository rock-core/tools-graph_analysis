#ifndef OMVIZ_ORGANIZATIONMODELWIDGET_ORGANIZATION_MODEL_WIDGET_HPP
#define OMVIZ_ORGANIZATIONMODELWIDGET_ORGANIZATION_MODEL_WIDGET_HPP

#include <QTabWidget>
#include <owl_om/OrganizationModel.hpp>

class QTreeWidget;
class QTreeWidgetItem;

namespace Ui
{
    class OrganizationModelWidget;
}

namespace omviz
{

class OrganizationModelWidget : public QTabWidget
{
    Q_OBJECT

public:
    OrganizationModelWidget(QWidget* parent = 0);
    ~OrganizationModelWidget();

    void setModel(owl_om::OrganizationModel::Ptr organizationModel) { mpOrganizationModel = organizationModel; }

signals:
    void modelChanged();
    // The item that is currently selected changed
    void currentSelectionChanged(QString itemDataParent, QString itemDataSelection);

public slots:
    void updateFromModel();
    void refresh();
    void createNewFromModel();
    void itemClicked(QTreeWidgetItem* item, int column);

private:
    QTreeWidgetItem* createWidgetItem(const std::string& label);
    void updateTreeWidget(QTreeWidget* treeWidget, const owlapi::model::IRI& modelType = owlapi::model::IRI(), bool directInstances = false);

    QTreeWidgetItem* getParentInstance(QTreeWidgetItem* item);

    owl_om::OrganizationModel::Ptr mpOrganizationModel;
    Ui::OrganizationModelWidget* mUi;

    QTreeWidgetItem* mCurrentWidgetItem;
};

} // end namespace omviz
#endif // OMVIZ_ORGANIZATIONMODELWIDGET_ORGANIZATION_MODEL_WIDGET_HPP
