find_package(Boost REQUIRED regex)

rock_library(omviz
    SOURCES
      filterwidget/AddRegexFilterDialog.cpp
      filterwidget/FilterWidget.cpp
      graphwidget/EdgeItem.cpp
      graphwidget/EdgeTypeManager.cpp
      graphwidget/GraphWidget.cpp
      graphwidget/NodeItem.cpp
      graphwidget/NodeTypeManager.cpp
      graphwidget/graphitem/Simple.cpp
      graphwidget/graphitem/Resource.cpp
      graphwidget/graphitem/edges/Simple.cpp
      graphwidget/GVGraph.cpp
      planningwidget/PlanningWidget.cpp
      planningwidget/PlanningProblemDialog.cpp
      organizationmodelwidget/OrganizationModelWidget.cpp
      organizationmodelwidget/AddActorDialog.cpp
      propertieswidget/PropertiesWidget.cpp
      IRIEdge.cpp
      IRINode.cpp
      MainWindow.cpp
      Utils.cpp
    HEADERS
      filterwidget/AddRegexFilterDialog.hpp
      filterwidget/FilterWidget.hpp
      graphwidget/EdgeItem.hpp
      graphwidget/EdgeTypeManager.hpp
      graphwidget/GraphWidget.hpp
      graphwidget/NodeItem.hpp
      graphwidget/NodeTypeManager.hpp
      graphwidget/filters/Filters.hpp
      graphwidget/graphitem/Label.hpp
      graphwidget/graphitem/Simple.hpp
      graphwidget/graphitem/Resource.hpp
      graphwidget/graphitem/edges/Simple.hpp
      graphwidget/GVGraph.hpp
      planningwidget/PlanningWidget.hpp
      planningwidget/PlanningProblemDialog.hpp
      organizationmodelwidget/OrganizationModelWidget.hpp
      organizationmodelwidget/AddActorDialog.hpp
      propertieswidget/PropertiesWidget.hpp
      IRIEdge.cpp
      IRINode.hpp
      MainWindow.hpp
      Utils.hpp
    MOC
        MainWindow.hpp
        filterwidget/AddRegexFilterDialog.hpp
        filterwidget/FilterWidget.hpp
        graphwidget/GraphWidget.hpp
        planningwidget/PlanningWidget.hpp
        planningwidget/PlanningProblemDialog.hpp
        organizationmodelwidget/OrganizationModelWidget.hpp
        organizationmodelwidget/AddActorDialog.hpp
        propertieswidget/PropertiesWidget.hpp
    DEPS_PKGCONFIG graph_analysis owl_om owlapi
    DEPS_PLAIN Boost_REGEX
    UI
        ui/add_actor_dialog.ui
        ui/add_regex_filter_dialog.ui
        ui/filter_widget.ui
        ui/omviz.ui
        ui/organization_model_widget.ui
        ui/planning_problem_dialog.ui
        ui/planning_widget.ui
        ui/properties_widget.ui
)
target_link_libraries(omviz gvc)

rock_executable(omviz_main Main.cpp
    DEPS omviz)

rock_executable(omviz_bin graphwidget/Main.cpp
    DEPS omviz)

rock_library(omviz_elasticnodes
    SOURCES elasticnodes/Edge.cpp
      elasticnodes/GraphWidget.cpp
      elasticnodes/Node.cpp
    HEADERS elasticnodes/Edge.hpp
      elasticnodes/GraphWidget.hpp
      elasticnodes/Node.hpp
    MOC elasticnodes/Edge.hpp
      elasticnodes/GraphWidget.hpp
      elasticnodes/Node.hpp
)
rock_executable(omviz_elasticnodes_bin elasticnodes/Main.cpp
    DEPS omviz_elasticnodes)

QT4_ADD_RESOURCES(Resource_SOURCES diagramscene/diagramscene.qrc)

rock_library(omviz_diagramscene
    SOURCES
        diagramscene/arrow.cpp
        diagramscene/diagramitem.cpp
        diagramscene/diagramscene.cpp
        diagramscene/diagramtextitem.cpp
        diagramscene/mainwindow.cpp
    HEADERS
        diagramscene/arrow.h
        diagramscene/diagramitem.h
        diagramscene/diagramscene.h
        diagramscene/diagramtextitem.h
        diagramscene/mainwindow.h
    MOC
        diagramscene/diagramitem.h
        diagramscene/diagramscene.h
        diagramscene/diagramtextitem.h
        diagramscene/mainwindow.h
)

rock_executable(omviz_diagramscene_bin diagramscene/main.cpp ${Resource_SOURCES}
    DEPS omviz_diagramscene)

