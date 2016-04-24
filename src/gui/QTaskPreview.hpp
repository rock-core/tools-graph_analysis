#ifndef QTASKPREVIEW_H
#define QTASKPREVIEW_H

#include <QWidget>

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

signals:

public slots:
    void updatePreview(QTreeWidgetItem*, QTreeWidgetItem*);

private slots:
};

#endif /*QTASKPREVIEW_H*/
