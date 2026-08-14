#pragma once

#ifndef INCLUDE_VIEW_PROFILES_EXPRESSION_H_
#define INCLUDE_VIEW_PROFILES_EXPRESSION_H_

#include "expressions/expression.h"
#include "context.h"

#include <any>

class ViewProfilesExpression : public Expression {
    public:
        std::any Execute(Context& ctx) const override;
};

#endif