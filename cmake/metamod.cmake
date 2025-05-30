# Menu System
# Copyright (C) 2024-2025 komashchenko & Wend4r
# Licensed under the GPLv3 license. See LICENSE file in the project root for details.

if(NOT METAMOD_DIR)
	message(FATAL_ERROR "METAMOD_DIR is empty")
endif()

set(METAMOD_COMPILE_DEFINITIONS
	${METAMOD_COMPILE_DEFINITIONS}

	META_PLUGIN_AUTHOR="${PROJECT_AUTHOR}"
	META_PLUGIN_PREFIX="${PROJECT_NAME_SUBSTRING}"
	META_PLUGIN_PREFIX_LOWER="${PROJECT_NAME_LOWER}"
	META_PLUGIN_PREFIX_UPPER="${PROJECT_NAME_UPPER}"
	META_PLUGIN_NAME="${PROJECT_DESCRIPTION}"
	META_PLUGIN_DESCRIPTION="${PROJECT_DESCRIPTION_FULL}"
	META_PLUGIN_URL="${PROJECT_HOMEPAGE_URL}"
	META_PLUGIN_LICENSE="${PROJECT_LICENSE}"
	META_PLUGIN_VERSION="${PROJECT_VERSION}"
	META_PLUGIN_DATE="${PROJECT_BUILD_DATE} ${PROJECT_BUILD_TIME}"
	META_PLUGIN_LOG_TAG="${PROJECT_NAME_UPPER}"

	META_IS_SOURCE2
)

set(METAMOD_INCLUDE_DIRS
	${METAMOD_INCLUDE_DIRS}

	${METAMOD_DIR}/core/sourcehook
	${METAMOD_DIR}/core
)
