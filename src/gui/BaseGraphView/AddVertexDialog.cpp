#include "AddVertexDialog.hpp"

namespace graph_analysis
{
namespace gui
{

AddVertexDialog::AddVertexDialog(QWidget* parent)
    : QDialog(parent)
{
}

QString AddVertexDialog::getClassname() const { return QString(); }

QString AddVertexDialog::getLabel() const { return QString(); }

} // end namespace dialogs
} // end namespace graph_analysis
