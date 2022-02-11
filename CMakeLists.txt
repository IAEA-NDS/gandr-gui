cmake_minimum_required(VERSION 3.12)
project(gandr-gui VERSION 0.1.0)
set( CMAKE_CXX_STANDARD 14 )

include( FetchContent )


#######################################################################
# Declare project dependencies
#######################################################################

FetchContent_Declare( cereal
    GIT_REPOSITORY  https://github.com/USCiLab/cereal
    GIT_TAG         origin/master
    GIT_SHALLOW     TRUE
    )

FetchContent_MakeAvailable(
  cereal)

#include(CTest)
#enable_testing()

find_package(wxWidgets REQUIRED COMPONENTS net core base richtext)
include(${wxWidgets_USE_FILE})
include_directories("include")

add_library(gandr-gui-lib
  src/AddCovDialog.cpp
  src/AddMaterialWizard.cpp
  src/AddRunWizard.cpp
  src/AddSensDialog.cpp
  src/ENDFReader.cpp
  src/EditGridWindow.cpp
  src/EditIntegSensDialog.cpp
  src/EndfNumbers.cpp
  src/EventListCtrl.cpp
  src/EventListPanel.cpp
  src/ExforCovDialog.cpp
  src/ExforIndexGridDialog.cpp
  src/ExforReader.cpp
  src/GandrSettingsFrame.cpp
  src/GanrunConfiguration.cpp
  src/GlobalConfiguration.cpp
  src/GlobalSettingsDialog.cpp
  src/GridButton.cpp
  src/GridEditPanel.cpp
  src/IdefDialog.cpp
  src/InitSetupWizard.cpp
  src/IntegConfiguration.cpp
  src/RunConfiguration.cpp
  src/csv.cpp
  src/gen_utils.cpp
  src/sens_reader.cpp
)

target_link_libraries( gandr-gui-lib
${wxWidgets_LIBRARIES}
cereal)

add_executable(gandr-gui 
  GandrGUIMain.cpp
  GandrGUIApp.cpp
)

target_link_libraries(gandr-gui 
  cereal
  gandr-gui-lib
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
