##### Library ######
add_library(
  fitsGen SHARED
  src/EventClassifier.cxx src/Ft1File.cxx src/Ft2File.cxx src/FtFileBase.cxx
  src/MeritFile.cxx src/MeritFile2.cxx src/XmlEventClassifier.cxx
)

target_link_libraries(
  fitsGen
  PUBLIC facilities tip astro embed_python evtUtils
)

target_include_directories(
  fitsGen
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:>
)

target_compile_definitions(fitsGen PRIVATE BUILD_WITHOUT_ROOT)


###### Tests ######
add_executable(test_fitsGen src/test/test.cxx)
target_link_libraries(test_fitsGen PRIVATE fitsGen)
target_include_directories(
  test_fitsGen PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:>
)

if(NOT APPLE)
  target_compile_definitions(fitsGen PRIVATE TRAP_FPE)
endif()

###############################################################
# Installation
###############################################################
install(DIRECTORY fitsGen DESTINATION ${FERMI_INSTALL_INCLUDEDIR})
install(DIRECTORY data DESTINATION ${FERMI_INSTALL_DATADIR}/fitsGen)
install(DIRECTORY pfiles/ DESTINATION ${FERMI_INSTALL_PFILESDIR})
install(DIRECTORY python/ DESTINATION ${FERMI_INSTALL_PYTHON})

install(
  TARGETS fitsGen test_fitsGen
  EXPORT fermiTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
