#pragma once

#define _MICA_INTERNAL_CONCAT(a, b) a##b

#define MICA_CONCAT(a, b) _MICA_INTERNAL_CONCAT(a, b)

#define MICA_TMP_VAR(prefix) MICA_CONCAT(prefix, __LINE__)

#define MICA_TMP_VAR_PREFIX _temporary__variable__

#define MICA_TMP_VAR_DEFAULT MICA_TMP_VAR(MICA_TMP_VAR_PREFIX)
