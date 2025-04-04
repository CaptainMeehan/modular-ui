#pragma once
#include "UIVertex.hpp"

struct UI_Quad
{
	UI_Vertex& GetTopLeft() { return vertices[0]; }
	UI_Vertex& GetTopRight() { return vertices[1]; }
	UI_Vertex& GetBottomRight() { return vertices[2]; }
	UI_Vertex& GetBottomLeft() { return vertices[3]; }
	void SetTopLeftPos(const JPH::Float3& aPosition) { vertices[0].position = aPosition; }
	void SetTopRightPos(const JPH::Float3& aPosition) { vertices[1].position = aPosition; }
	void SetBottomRightPos(const JPH::Float3& aPosition) { vertices[2].position = aPosition; }
	void SetBottomLeftPos(const JPH::Float3& aPosition) { vertices[3].position = aPosition; }

	UI_Vertex& operator[](size_t aIndex) { return vertices[aIndex]; }
	std::array<UI_Vertex, 4> vertices;
};
