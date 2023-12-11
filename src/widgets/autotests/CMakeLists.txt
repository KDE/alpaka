# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org
#
# SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

macro(add_kllmwidgets_test _source)
    set(_test ${_source})
    get_filename_component(_name ${_source} NAME_WE )
    add_executable( ${_name} ${_test} ${ARGN} ${_name}.h)
    add_test(NAME ${_name} COMMAND ${_name} )
    ecm_mark_as_test(${_name})
    target_link_libraries( ${_name} Qt::Test KLLMWidgets)

endmacro()
add_kllmwidgets_test(kllmmodelcomboboxtest.cpp)
