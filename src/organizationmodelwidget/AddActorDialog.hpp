#ifndef OMVIZ_ORGANIZATIONMODELWIDGET_ADD_ACTOR_DIALOG_HPP
#define OMVIZ_ORGANIZATIONMODELWIDGET_ADD_ACTOR_DIALOG_HPP

#include <QDialog>
#include <owl_om/OrganizationModel.hpp>

namespace Ui
{
    class AddActorDialog;
}

namespace omviz 
{

class AddActorDialog : public QDialog
{
    Q_OBJECT
public:
    AddActorDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~AddActorDialog();

    void setModelList(const owl_om::IRIList& iriList);

 //   owl_om::IRI getInstance() const { return mInstance; }
    owl_om::IRI getModel() const { return mModel; }

public slots:
    void checkAndStoreValues();

private: 
    Ui::AddActorDialog* mUi;

  //  owl_om::IRI mInstance;
    owl_om::IRI mModel;
};

} // end namespace omviz
#endif // OMVIZ_ORGANIZATIONMODELWIDGET_ADD_ACTOR_DIALOG_HPP
