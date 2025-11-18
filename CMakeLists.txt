cmake_minimum_required(VERSION 3.16)

project(PatientManagementSystem LANGUAGES CXX)

# ---------------- C++ standard ----------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------- Options ----------------
option(USE_QT_GUI "Build with Qt GUI support" ON)

# ---------------- Global includes ----------------
include_directories(${CMAKE_SOURCE_DIR}/include)

# ---------------- Gather files ----------------
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.h" "${CMAKE_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE GUI_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/gui/*.cpp"
    "${CMAKE_SOURCE_DIR}/gui/*.h"
    "${CMAKE_SOURCE_DIR}/gui/*.ui"
)

# --- NEW: Gather data files (.json and .txt) ---
file(GLOB DATA_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/data/*.json"
    "${CMAKE_SOURCE_DIR}/data/*.txt"
)
# -----------------------------------------------

# ---------------- Helper: detect MinGW on Windows ----------------
set(USE_MINGW OFF)
if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(USE_MINGW ON)
endif()

# ---------------- Qt GUI target ----------------
if(USE_QT_GUI)
    find_package(Qt6 COMPONENTS Widgets REQUIRED)
    qt_standard_project_setup()

    # --- Resource file (nếu có) ---
    # resources (edit path nếu khác)
    set(RESOURCE_FILES ${CMAKE_SOURCE_DIR}/gui/resources.qrc)

    # ensure we compile rcc
    qt_add_resources(GUI_RESOURCES ${RESOURCE_FILES})

    # then add GUI_RESOURCES into the executable
    qt_add_executable(${PROJECT_NAME}
        ${CMAKE_SOURCE_DIR}/main.cpp
        ${SRC_FILES}
        ${GUI_FILES}
        ${GUI_RESOURCES}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

    # --- NEW: Post-Build Copy Data Files ---
    # Lệnh này sẽ sao chép các file data sang thư mục của file thực thi sau khi build.
    if(DATA_FILES)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DATA_FILES}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying required data files (.json, .txt) to executable directory."
        )
    endif()
    # ---------------------------------------

    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE OFF)
    if(USE_MINGW)
        target_link_options(${PROJECT_NAME} PRIVATE "-Wl,--subsystem,console")
    endif()

else()
    add_executable(${PROJECT_NAME}
        main.cpp
        ${SRC_FILES}
    )
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

    # --- NEW: Post-Build Copy Data Files (for non-GUI build) ---
    if(DATA_FILES)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DATA_FILES}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying required data files (.json, .txt) to executable directory."
        )
    endif()
    # ---------------------------------------
endif()

# ---------------- Extra files for Qt Creator ----------------
add_custom_target(ExtraFiles SOURCES
    ${HEADER_FILES}
    ${DATA_FILES} # <-- ADDED DATA_FILES HERE
    README.md
    LICENSE
)