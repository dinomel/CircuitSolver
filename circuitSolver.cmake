set(CIRCUITSOLVER_NAME circuitSolver)				#Naziv prvog projekta u solution-u

file(GLOB CIRCUITSOLVER_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB CIRCUITSOLVER_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB CIRCUITSOLVER_INC_MODEL  ${CMAKE_CURRENT_LIST_DIR}/src/model/*.h)
file(GLOB CIRCUITSOLVER_INC_CORE  ${CMAKE_CURRENT_LIST_DIR}/src/core/*.h)
file(GLOB CIRCUITSOLVER_INC_PROPERTY  ${CMAKE_CURRENT_LIST_DIR}/src/property/*.h)
file(GLOB CIRCUITSOLVER_INC_TD  ${MY_INC}/td/*.h)
file(GLOB CIRCUITSOLVER_INC_GUI ${MY_INC}/gui/*.h)

#Application icon
set(CIRCUITSOLVER_PLIST  ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/AppIcon.plist)
if(WIN32)
	set(CIRCUITSOLVER_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.rc)
else()
	set(CIRCUITSOLVER_WINAPP_ICON ${CMAKE_CURRENT_LIST_DIR}/res/appIcon/winAppIcon.cpp)
endif()

# add executable
add_executable(${CIRCUITSOLVER_NAME} ${CIRCUITSOLVER_INCS} ${CIRCUITSOLVER_INC_MODEL} ${CIRCUITSOLVER_INC_CORE}  ${CIRCUITSOLVER_INC_PROPERTY}  ${CIRCUITSOLVER_SOURCES}
			${CIRCUITSOLVER_INC_TD}  ${CIRCUITSOLVER_INC_GUI}  ${CIRCUITSOLVER_WINAPP_ICON})

source_group("inc"            FILES ${CIRCUITSOLVER_INCS})
source_group("inc\\model"            FILES ${CIRCUITSOLVER_INC_MODEL})
source_group("inc\\core"            FILES ${CIRCUITSOLVER_INC_CORE})
source_group("inc\\property"            FILES ${CIRCUITSOLVER_INC_PROPERTY})
source_group("inc\\td"        FILES ${CIRCUITSOLVER_INC_TD})
source_group("inc\\gui"        FILES ${CIRCUITSOLVER_INC_GUI})
source_group("src"            FILES ${CIRCUITSOLVER_SOURCES})

target_link_libraries(${CIRCUITSOLVER_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} 
										optimized ${MU_LIB_RELEASE}  optimized ${NATGUI_LIB_RELEASE})

setTargetPropertiesForGUIApp(${CIRCUITSOLVER_NAME} ${CIRCUITSOLVER_PLIST})

setAppIcon(${CIRCUITSOLVER_NAME} ${CMAKE_CURRENT_LIST_DIR})

setIDEPropertiesForGUIExecutable(${CIRCUITSOLVER_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${CIRCUITSOLVER_NAME})
