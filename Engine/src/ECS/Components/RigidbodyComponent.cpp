#include "pch.h"
#include "RigidbodyComponent.h"

using namespace engine;

RigidbodyComponent::RigidbodyComponent() 
	: mass(1.0f), velocity(0.f, 0.f, 0.f)
{

}

RigidbodyComponent::RigidbodyComponent(float mass, DirectX::XMFLOAT3 vel)
	: mass(mass), velocity(vel)
{
}
