#pragma once

#define DECLARE_STYLE_FIELD_SETTER(styleType, fieldType, fieldName) \
    styleType& Set##fieldName(const fieldType& value) { \
    fieldName = value; \
    return *this; \
    }
    