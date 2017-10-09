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

    connect(mUi->columnScalingSlider, SIGNAL(sliderMoved(int)),
            SLOT(updateColumnScalingFromSlider(int)));
    connect( mUi->rowScalingSlider, SIGNAL(sliderMoved(int)),
            SLOT(updateRowScalingFromSlider(int)));
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

void GridLayout::updateRowScalingFromSlider(int value)
{
    layouts::GridLayout::Ptr gridLayout;
    if(gridLayout = getGridLayout())
    {
        gridLayout->setRowScalingFactor(value/10.0);
        mUi->rowScalingText->setPlainText( QString::number( value / 10.0 ) );

    }
}

void GridLayout::updateColumnScalingFromSlider(int value)
{
    layouts::GridLayout::Ptr gridLayout;
    if(gridLayout = getGridLayout())
    {
        gridLayout->setColumnScalingFactor(value/10.0);
        mUi->columnScalingText->setPlainText( QString::number( value / 10.0 ) );
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
