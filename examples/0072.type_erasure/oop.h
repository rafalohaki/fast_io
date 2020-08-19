#pragma once

namespace oop
{
class shape
{
public:
	virtual ~shape() = default;
	virtual double area() const = 0;
};

class rectangle : public shape
{
	double length,width;
public:
	rectangle(double l,double w):length(l),width(w){}
	double area() const override
	{
		return length*width;
	}
	double get_length() const
	{
		return length;
	}
	double get_width() const
	{
		return width;
	}
};

class circle: public shape
{
	double radius;
public:
	circle(double r):radius(r){}
	double area() const override
	{
		return std::numbers::pi*radius*radius;
	}
	double get_radius() const
	{
		return radius;
	}
};

class square : public shape
{
	double length;
public:
	square(double l):length(l){}
	double area() const override
	{
		return length*length;
	}
	double get_length() const
	{
		return length;
	}
};

}