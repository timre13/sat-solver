#pragma once

#include "Token.h"

namespace Parser
{

Token::tokenVec_t parse(const std::string& formula);

} // namespace Parser
