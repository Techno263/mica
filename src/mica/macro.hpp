#pragma once

#define _MICA_INTERNAL_CONCAT(a_, b_) a_##b_

#define MICA_CONCAT(a_, b_) _MICA_INTERNAL_CONCAT(a_, b_)

#define MICA_TMP_VAR(prefix_) MICA_CONCAT(prefix_, __LINE__)

#define MICA_TMP_VAR_PREFIX _temporary__variable__

#define MICA_TMP_VAR_DEFAULT MICA_TMP_VAR(MICA_TMP_VAR_PREFIX)
