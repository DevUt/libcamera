/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2019, Google Inc.
 *
 * geometry.h - Geometry-related structure
 */

#ifndef __LIBCAMERA_GEOMETRY_H__
#define __LIBCAMERA_GEOMETRY_H__

#include <string>

namespace libcamera {

struct Rectangle {
	int x;
	int y;
	unsigned int width;
	unsigned int height;

	const std::string toString() const;
};

bool operator==(const Rectangle &lhs, const Rectangle &rhs);
static inline bool operator!=(const Rectangle &lhs, const Rectangle &rhs)
{
	return !(lhs == rhs);
}

struct Size {
	Size()
		: Size(0, 0)
	{
	}

	Size(unsigned int w, unsigned int h)
		: width(w), height(h)
	{
	}

	unsigned int width;
	unsigned int height;

	const std::string toString() const;
};

bool operator==(const Size &lhs, const Size &rhs);
bool operator<(const Size &lhs, const Size &rhs);

static inline bool operator!=(const Size &lhs, const Size &rhs)
{
	return !(lhs == rhs);
}

static inline bool operator<=(const Size &lhs, const Size &rhs)
{
	return lhs < rhs || lhs == rhs;
}

static inline bool operator>(const Size &lhs, const Size &rhs)
{
	return !(lhs <= rhs);
}

static inline bool operator>=(const Size &lhs, const Size &rhs)
{
	return !(lhs < rhs);
}

class SizeRange
{
public:
	SizeRange()
		: hStep(0), vStep(0)
	{
	}

	SizeRange(const Size &size)
		: min(size), max(size), hStep(1), vStep(1)
	{
	}

	SizeRange(const Size &minSize, const Size &maxSize)
		: min(minSize), max(maxSize), hStep(1), vStep(1)
	{
	}

	SizeRange(const Size &minSize, const Size &maxSize,
		  unsigned int hstep, unsigned int vstep)
		: min(minSize), max(maxSize), hStep(hstep), vStep(vstep)
	{
	}

	bool contains(const Size &size) const;

	std::string toString() const;

	Size min;
	Size max;
	unsigned int hStep;
	unsigned int vStep;
};

bool operator==(const SizeRange &lhs, const SizeRange &rhs);
static inline bool operator!=(const SizeRange &lhs, const SizeRange &rhs)
{
	return !(lhs == rhs);
}

} /* namespace libcamera */

#endif /* __LIBCAMERA_GEOMETRY_H__ */
