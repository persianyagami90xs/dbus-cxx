# Check to see if we want to use the bundled libcppgenerate or not
if( TOOLS_BUNDLED_CPPGENERATE )
    include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
    ExternalProject_Add( cppgenerate 
                         SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libcppgenerate
                         DOWNLOAD_COMMAND ""
                         INSTALL_COMMAND "")
    ExternalProject_Get_Property( cppgenerate BINARY_DIR )
    set( cppgenerate_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libcppgenerate )
    set( cppgenerate_LDFLAGS ${BINARY_DIR}/libcppgenerate.a )
else()
    pkg_check_modules( cppgenerate REQUIRED cppgenerate-static )
endif( TOOLS_BUNDLED_CPPGENERATE )

pkg_check_modules( popt REQUIRED popt )

set( TOOLS_INCLUDES ${dbus_INCLUDE_DIRS} ${sigc_INCLUDE_DIRS} ${cppgenerate_INCLUDE_DIRS} )
set( TOOLS_LINK dbus-cxx ${dbus_LDFLAGS} ${sigc_LDFLAGS} ${popt_LDFLAGS} ${expat_LDFLAGS} -lrt )

link_directories( ${CMAKE_BINARY_DIR} )

include_directories( ${CMAKE_SOURCE_DIR}/dbus-cxx
    ${CMAKE_BINARY_DIR}/dbus-cxx 
    ${dbus_INCLUDE_DIRS} 
    ${sigc_INCLUDE_DIRS}
    ${cppgenerate_INCLUDE_DIRS} )

add_executable( dbus-cxx-xml2cpp 
    xml2cpp.cpp
    code-generator.cpp 
    signal-info.cpp
    method-info.cpp
    property-info.cpp
    interface-info.cpp
    node-info.cpp
)
target_link_libraries( dbus-cxx-xml2cpp ${TOOLS_LINK} ${cppgenerate_LDFLAGS} )
install( TARGETS dbus-cxx-xml2cpp DESTINATION bin )
set_property( TARGET dbus-cxx-xml2cpp PROPERTY CXX_STANDARD 17 )

if( TOOLS_BUNDLED_CPPGENERATE )
    add_dependencies( dbus-cxx-xml2cpp cppgenerate )
endif( TOOLS_BUNDLED_CPPGENERATE )

add_executable( dbus-cxx-introspect
    introspect.cpp )
target_link_libraries( dbus-cxx-introspect ${TOOLS_LINK} )
install( TARGETS dbus-cxx-introspect DESTINATION bin )
set_property( TARGET dbus-cxx-introspect PROPERTY CXX_STANDARD 17 )

