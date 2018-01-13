#include "GridLayout.hpp"
#include "ui_grid_layout.h"
#include "../GraphLayoutManager.hpp"
#include <iostream>

namespace graph_analysis {
namespace gui {
namespace dialogs {

GridLayout::GridLayout(QWidget* parent)
    : QDialog(parent)
    , mUi(new Ui::Dialog)
{
    mUi->setupUi(this);
    this->show();

    connect(mUi->columnScalingSpinBox, SIGNAL(valueChanged(double)),
            SLOT(updateColumnScaling(double)));
    connect( mUi->rowScalingSpinBox, SIGNAL(valueChanged(double)),
            SLOT(updateRowScaling(double)));

    connect(mUi->columnOffsetSpinBox, SIGNAL(valueChanged(int)),
            SLOT(updateColumnOffset(int)));
    connect(mUi->rowOffsetSpinBox, SIGNAL(valueChanged(int)),
            SLOT(updateRowOffset(int)));


    layouts::GridLayout::Ptr gridLayout;
    if(gridLayout = getGridLayout())
    {
        mUi->columnScalingSpinBox->setValue( gridLayout->getColumnScalingFactor());
        mUi->rowScalingSpinBox->setValue( gridLayout->getRowScalingFactor());
        mUi->columnOffsetSpinBox->setValue( gridLayout->getColumnOffset());
        mUi->rowOffsetSpinBox->setValue( gridLayout->getRowOffset());
    }
}

GridLayout::~GridLayout()
{
    delete mUi;
}

int GridLayout::execute()
{
    GridLayout gridLayout;
    return gridLayout.exec();
}

void GridLayout::updateRowScaling(double value)
{
    layouts::GridLayout::Ptr gridLayout;
    if(gridLayout = getGridLayout())
    {
        gridLayout->setRowScalingFactor(value);
    }
}

void GridLayout::updateColumnScaling(double value)
{
    layouts::GridLayout::Ptr gridLayout;
    if(gridLayout = getGridLayout())
    {
        gridLayout->setColumnScalingFactor(value);
    }
}

void GridLayout::updateColumnOffset(int value)
{
    layouts::GridLayout::Ptr gridLayout;
    if(gridLayout = getGridLayout())
    {
        gridLayout->setColumnOffset(value);
    }
}

void GridLayout::updateRowOffset(int value)
{
    layouts::GridLayout::Ptr gridLayout;
    if(gridLayout = getGridLayout())
    {
        gridLayout->setRowOffset(value);
    }
}

layouts::GridLayout::Ptr GridLayout::getGridLayout() const
{
    GraphLayoutManager* layoutManager = GraphLayoutManager::getInstance();
    GraphLayout::Ptr layout = layoutManager->getGraphLayout("grid-layout-default");
    layouts::GridLayout::Ptr gridLayout = dynamic_pointer_cast<layouts::GridLayout>(layout);
    return gridLayout;
}

} // end namespace dialogs
} // end namespace gui
} // end graph_analysis
