/**
 * \file double_to_str_precision.h
 * \brief Declaration of double_to_str_precision.
 *
 * Copyright (C) 2014 Aleric Inglewood.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTILS_DOUBLE_TO_STR_PRECISION_H
#define UTILS_DOUBLE_TO_STR_PRECISION_H

#ifndef USE_PCH
#include <string>
#endif

/// Convert a double to a string with a minimal and maximal precision.
std::string double_to_str_precision(double d, int min, int max);

#endif // UTILS_DOUBLE_TO_STR_PRECISION_H
