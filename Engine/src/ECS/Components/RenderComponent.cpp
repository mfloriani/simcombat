#include "pch.h"
#include "RenderComponent.h"
#include "Platform\Mesh.h"
#include "Platform\Texture.h"

engine::RenderComponent::RenderComponent(Mesh* mesh, Texture* texture) 
	: mesh(mesh), texture(texture)
{

}
