# Menu System
# Copyright (C) 2024-2025 komashchenko & Wend4r
# Licensed under the GPLv3 license. See LICENSE file in the project root for details.

if(NOT SOURCESDK_DIR)
	message(FATAL_ERROR "SOURCESDK_DIR is empty")
endif()

set(SOURCESDK_BINARY_DIR "sourcesdk")
set(SOURCESDK_CONFIGURE_EXPORT_MAP OFF)

set(SOURCESDK_PROTOS
	netmessages
	network_connection
	networkbasetypes
	networksystem_protomessages
	connectionless_netmessages
	usermessages

	# steammessages
	# engine_gcmessages
	# gcsdk_gcmessages
	# cstrike15_gcmessages
	# cstrike15_usermessages

	usercmd
	cs_usercmd
)

add_subdirectory(${SOURCESDK_DIR} ${SOURCESDK_BINARY_DIR})

function(get_sourcesdk_target_property VAR_NAME TARGET PROPERTY)
	get_target_property(PROPERTY_VALUE ${TARGET} ${PROPERTY})

	if("${PROPERTY_VALUE}" MATCHES "PROPERTY_VALUE-NOTFOUND")
		set(${VAR_NAME} PARENT_SCOPE)
	else()
		set(${VAR_NAME} ${PROPERTY_VALUE} PARENT_SCOPE)
	endif()
endfunction()

get_sourcesdk_target_property(SOURCESDK_COMPILE_OPTIONS ${SOURCESDK_BINARY_DIR} COMPILE_OPTIONS)
get_sourcesdk_target_property(SOURCESDK_LINK_OPTIONS ${SOURCESDK_BINARY_DIR} LINK_OPTIONS)

get_sourcesdk_target_property(SOURCESDK_COMPILE_DEFINITIONS ${SOURCESDK_BINARY_DIR} COMPILE_DEFINITIONS)
get_sourcesdk_target_property(SOURCESDK_INCLUDE_DIRS ${SOURCESDK_BINARY_DIR} INCLUDE_DIRECTORIES)

get_sourcesdk_target_property(SOURCESDK_LINK_LIBRARIES ${SOURCESDK_BINARY_DIR} LINK_LIBRARIES)
