#ifndef QTASKPREVIEW_H
#define QTASKPREVIEW_H

#include <QWidget>

#include <graph_analysis/BaseGraph.hpp>

class QTreeWidgetItem;

namespace Ui
{
class QTaskPreview;
}

class QTaskPreview : public QWidget
{
    Q_OBJECT
public:
    QTaskPreview(QWidget* parent);
    ~QTaskPreview();
    Ui::QTaskPreview* mpUi;

    void setGraph(graph_analysis::BaseGraph::Ptr graph);

signals:

public slots:
    void updatePreview(QTreeWidgetItem*, QTreeWidgetItem*);

private:
    graph_analysis::BaseGraph::Ptr mpBaseGraph;

private slots:
};

#endif /*QTASKPREVIEW_H*/
