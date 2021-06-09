#include "pch.h"
#include "TransformComponent.h"

using namespace engine;
using namespace DirectX;

TransformComponent::TransformComponent() 
	: position(XMFLOAT3(0.f,0.f,0.f)), rotation(XMFLOAT3(0.f, 0.f, 0.f)), scale(XMFLOAT3(1.f, 1.f, 1.f)), enabled(true)
{
}

TransformComponent::TransformComponent(DirectX::XMFLOAT3 position)
	: position(position), rotation(XMFLOAT3(0.f, 0.f, 0.f)), scale(XMFLOAT3(1.f, 1.f, 1.f)), enabled(true)
{
}

TransformComponent::TransformComponent(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation)
	: position(position), rotation(rotation), scale(XMFLOAT3(1.f, 1.f, 1.f)), enabled(true)
{
}

TransformComponent::TransformComponent(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
	: position(position), rotation(rotation), scale(scale), enabled(true)
{
}
