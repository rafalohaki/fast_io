#include<cmath>
#include<utility>
#include<stdexcept>
#include"oop.h"
#include"type_erasure.h"
#include"../../include/fast_io.h"


//This is an example to teach you how to write type erasure instead of OOP

int main()
{
	std::vector<type_erasure::shape> shapes;
	shapes.emplace_back(std::in_place_type<type_erasure::circle>,4.5);
	shapes.emplace_back(std::in_place_type<type_erasure::rectangle>,4.0,5.0);
	shapes.emplace_back(std::in_place_type<type_erasure::square>,5.0);

	shapes.emplace_back(std::in_place_type<oop::circle>,24.5);
	shapes.emplace_back(std::in_place_type<oop::rectangle>,24.0,5.4);
	shapes.emplace_back(std::in_place_type<oop::square>,25.0);

	shapes.push_back(shapes.front());	//type erasure type is copyable.

	for(auto const& e : shapes)
		println(area(e));
}