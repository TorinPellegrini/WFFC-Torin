#include <string>
#include "DisplayChunk.h"
#include "Game.h"


template <typename T>
T Clamp(T value, T min, T max)
{
	return (value < min) ? min : (value > max) ? max : value;
}

using namespace DirectX;
using namespace DirectX::SimpleMath;

DisplayChunk::DisplayChunk()
{
	//terrain size in meters. note that this is hard coded here, we COULD get it from the terrain chunk along with the other info from the tool if we want to be more flexible.
	m_terrainSize = 512;
	m_terrainHeightScale = 0.25;  //convert our 0-256 terrain to 64
	m_textureCoordStep = 1.0 / (TERRAINRESOLUTION-1);	//-1 becuase its split into chunks. not vertices.  we want tthe last one in each row to have tex coord 1
	m_terrainPositionScalingFactor = m_terrainSize / (TERRAINRESOLUTION-1);
}


DisplayChunk::~DisplayChunk()
{
}

void DisplayChunk::PopulateChunkData(ChunkObject * SceneChunk)
{
	m_name = SceneChunk->name;
	m_chunk_x_size_metres = SceneChunk->chunk_x_size_metres;
	m_chunk_y_size_metres = SceneChunk->chunk_y_size_metres;
	m_chunk_base_resolution = SceneChunk->chunk_base_resolution;
	m_heightmap_path = SceneChunk->heightmap_path;
	m_tex_diffuse_path = SceneChunk->tex_diffuse_path;
	m_tex_splat_alpha_path = SceneChunk->tex_splat_alpha_path;
	m_tex_splat_1_path = SceneChunk->tex_splat_1_path;
	m_tex_splat_2_path = SceneChunk->tex_splat_2_path;
	m_tex_splat_3_path = SceneChunk->tex_splat_3_path;
	m_tex_splat_4_path = SceneChunk->tex_splat_4_path;
	m_render_wireframe = SceneChunk->render_wireframe;
	m_render_normals = SceneChunk->render_normals;
	m_tex_diffuse_tiling = SceneChunk->tex_diffuse_tiling;
	m_tex_splat_1_tiling = SceneChunk->tex_splat_1_tiling;
	m_tex_splat_2_tiling = SceneChunk->tex_splat_2_tiling;
	m_tex_splat_3_tiling = SceneChunk->tex_splat_3_tiling;
	m_tex_splat_4_tiling = SceneChunk->tex_splat_4_tiling;
}

void DisplayChunk::RenderBatch(std::shared_ptr<DX::DeviceResources>  DevResources)
{
	auto context = DevResources->GetD3DDeviceContext();

	m_terrainEffect->Apply(context);
	context->IASetInputLayout(m_terrainInputLayout.Get());

	m_batch->Begin();
	for (size_t i = 0; i < TERRAINRESOLUTION-1; i++)	//looping through QUADS.  so we subtrack one from the terrain array or it will try to draw a quad starting with the last vertex in each row. Which wont work
	{
		for (size_t j = 0; j < TERRAINRESOLUTION-1; j++)//same as above
		{
			m_batch->DrawQuad(m_terrainGeometry[i][j], m_terrainGeometry[i][j+1], m_terrainGeometry[i+1][j+1], m_terrainGeometry[i+1][j]); //bottom left bottom right, top right top left.
		}
	}
	m_batch->End();
}

void DisplayChunk::InitialiseBatch()
{
	//build geometry for our terrain array
	//iterate through all the vertices of our required resolution terrain.
	int index = 0;

	for (size_t i = 0; i < TERRAINRESOLUTION; i++)
	{
		for (size_t j = 0; j < TERRAINRESOLUTION; j++)
		{
			index = (TERRAINRESOLUTION * i) + j;
			m_terrainGeometry[i][j].position =			Vector3(j*m_terrainPositionScalingFactor-(0.5*m_terrainSize), (float)(m_heightMap[index])*m_terrainHeightScale, i*m_terrainPositionScalingFactor-(0.5*m_terrainSize));	//This will create a terrain going from -64->64.  rather than 0->128.  So the center of the terrain is on the origin
			m_terrainGeometry[i][j].normal =			Vector3(0.0f, 1.0f, 0.0f);						//standard y =up
			m_terrainGeometry[i][j].textureCoordinate =	Vector2(((float)m_textureCoordStep*j)*m_tex_diffuse_tiling, ((float)m_textureCoordStep*i)*m_tex_diffuse_tiling);				//Spread tex coords so that its distributed evenly across the terrain from 0-1
			
		}
	}
	CalculateTerrainNormals();
	
}

void DisplayChunk::LoadHeightMap(std::shared_ptr<DX::DeviceResources>  DevResources)
{
	auto device = DevResources->GetD3DDevice();
	auto devicecontext = DevResources->GetD3DDeviceContext();

	//load in heightmap .raw
	FILE *pFile = NULL;

	// Open The File In Read / Binary Mode.

	pFile = fopen(m_heightmap_path.c_str(), "rb");
	// Check To See If We Found The File And Could Open It
	if (pFile == NULL)
	{
		// Display Error Message And Stop The Function
		MessageBox(NULL, L"Can't Find The Height Map!", L"Error", MB_OK);
		return;
	}

	// Here We Load The .RAW File Into Our pHeightMap Data Array
	// We Are Only Reading In '1', And The Size Is (Width * Height)
	fread(m_heightMap, 1, TERRAINRESOLUTION*TERRAINRESOLUTION, pFile);

	fclose(pFile);

	//load in texture diffuse
	
	//load the diffuse texture
	std::wstring texturewstr = StringToWCHART(m_tex_diffuse_path);
	HRESULT rs;	
	rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), NULL, &m_texture_diffuse);	//load tex into Shader resource	view and resource
	
	//setup terrain effect
	m_terrainEffect = std::make_unique<BasicEffect>(device);
	m_terrainEffect->EnableDefaultLighting();
	m_terrainEffect->SetLightingEnabled(true);
	m_terrainEffect->SetTextureEnabled(true);
	m_terrainEffect->SetTexture(m_texture_diffuse);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_terrainEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	//setup batch
	DX::ThrowIfFailed(
		device->CreateInputLayout(VertexPositionNormalTexture::InputElements,
			VertexPositionNormalTexture::InputElementCount,
			shaderByteCode,
			byteCodeLength,
			m_terrainInputLayout.GetAddressOf())
		);

	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormalTexture>>(devicecontext);

	
}

void DisplayChunk::SaveHeightMap()
{
/*	for (size_t i = 0; i < TERRAINRESOLUTION*TERRAINRESOLUTION; i++)
	{
		m_heightMap[i] = 0;
	}*/

	FILE *pFile = NULL;

	// Open The File In Read / Binary Mode.
	pFile = fopen(m_heightmap_path.c_str(), "wb+");;
	// Check To See If We Found The File And Could Open It
	if (pFile == NULL)
	{
		// Display Error Message And Stop The Function
		MessageBox(NULL, L"Can't Find The Height Map!", L"Error", MB_OK);
		return;
	}

	fwrite(m_heightMap, 1, TERRAINRESOLUTION*TERRAINRESOLUTION, pFile);
	fclose(pFile);
	
}

void DisplayChunk::UpdateTerrain()
{
	//all this is doing is transferring the height from the heigtmap into the terrain geometry.
	int index;
	for (size_t i = 0; i < TERRAINRESOLUTION; i++)
	{
		for (size_t j = 0; j < TERRAINRESOLUTION; j++)
		{
			index = (TERRAINRESOLUTION * i) + j;
			m_terrainGeometry[i][j].position.y = (float)(m_heightMap[index])*m_terrainHeightScale;	
		}
	}
	CalculateTerrainNormals();

}

void DisplayChunk::GenerateHeightmap()
{
	//insert how YOU want to update the heigtmap here! :D
}

void DisplayChunk::CalculateTerrainNormals()
{
	int index1, index2, index3, index4;
	DirectX::SimpleMath::Vector3 upDownVector, leftRightVector, normalVector;



	for (int i = 0; i<(TERRAINRESOLUTION - 1); i++)
	{
		for (int j = 0; j<(TERRAINRESOLUTION - 1); j++)
		{
			upDownVector.x = (m_terrainGeometry[i + 1][j].position.x - m_terrainGeometry[i - 1][j].position.x);
			upDownVector.y = (m_terrainGeometry[i + 1][j].position.y - m_terrainGeometry[i - 1][j].position.y);
			upDownVector.z = (m_terrainGeometry[i + 1][j].position.z - m_terrainGeometry[i - 1][j].position.z);

			leftRightVector.x = (m_terrainGeometry[i][j - 1].position.x - m_terrainGeometry[i][j + 1].position.x);
			leftRightVector.y = (m_terrainGeometry[i][j - 1].position.y - m_terrainGeometry[i][j + 1].position.y);
			leftRightVector.z = (m_terrainGeometry[i][j - 1].position.z - m_terrainGeometry[i][j + 1].position.z);


			leftRightVector.Cross(upDownVector, normalVector);	//get cross product
			normalVector.Normalize();			//normalise it.

			m_terrainGeometry[i][j].normal = normalVector;	//set the normal for this point based on our result
		}
	}
}

float DisplayChunk::SampleTerrainHeight(float worldX, float worldZ) const
{
	// Convert world coordinates to terrain grid coordinates
	float halfSize = m_terrainSize * 0.5f;
	float gridX = (worldX + halfSize) / m_terrainPositionScalingFactor;
	float gridZ = (worldZ + halfSize) / m_terrainPositionScalingFactor;

	int x0 = (int)floorf(gridX);
	int z0 = (int)floorf(gridZ);
	int x1 = x0 + 1;
	int z1 = z0 + 1;

	// Clamp to valid range
	x0 = Clamp(x0, 0, TERRAINRESOLUTION - 1);
	z0 = Clamp(z0, 0, TERRAINRESOLUTION - 1);
	x1 = Clamp(x1, 0, TERRAINRESOLUTION - 1);
	z1 = Clamp(z1, 0, TERRAINRESOLUTION - 1);

	// Interpolate bilinearly
	float fracX = gridX - x0;
	float fracZ = gridZ - z0;

	float h00 = m_terrainGeometry[z0][x0].position.y;
	float h10 = m_terrainGeometry[z0][x1].position.y;
	float h01 = m_terrainGeometry[z1][x0].position.y;
	float h11 = m_terrainGeometry[z1][x1].position.y;

	float h0 = h00 + fracX * (h10 - h00);
	float h1 = h01 + fracX * (h11 - h01);

	return h0 + fracZ * (h1 - h0);
}

DirectX::SimpleMath::Vector3 DisplayChunk::SampleTerrainNormal(float worldX, float worldZ) const
{
	float halfSize = m_terrainSize * 0.5f;
	float gridX = (worldX + halfSize) / m_terrainPositionScalingFactor;
	float gridZ = (worldZ + halfSize) / m_terrainPositionScalingFactor;

	int x0 = (int)floorf(gridX);
	int z0 = (int)floorf(gridZ);
	int x1 = x0 + 1;
	int z1 = z0 + 1;

	// Clamp to valid range
	x0 = Clamp(x0, 0, TERRAINRESOLUTION - 1);
	z0 = Clamp(z0, 0, TERRAINRESOLUTION - 1);
	x1 = Clamp(x1, 0, TERRAINRESOLUTION - 1);
	z1 = Clamp(z1, 0, TERRAINRESOLUTION - 1);

	float fracX = gridX - x0;
	float fracZ = gridZ - z0;

	const Vector3& n00 = m_terrainGeometry[z0][x0].normal;
	const Vector3& n10 = m_terrainGeometry[z0][x1].normal;
	const Vector3& n01 = m_terrainGeometry[z1][x0].normal;
	const Vector3& n11 = m_terrainGeometry[z1][x1].normal;

	Vector3 n0 = n00 + (n10 - n00) * fracX;
	Vector3 n1 = n01 + (n11 - n01) * fracX;
	Vector3 finalNormal = n0 + (n1 - n0) * fracZ;

	finalNormal.Normalize();
	return finalNormal;
}

void DisplayChunk::ModifyTerrainCircle(DirectX::SimpleMath::Vector3 circleCenter, float radius, bool raise)
{
	float heightModifier = raise ? 1.0f : -1.0f;  // Modify height in raw byte units

	// Loop through heightmap coordinates
	for (int z = 0; z < TERRAINRESOLUTION; ++z)
	{
		for (int x = 0; x < TERRAINRESOLUTION; ++x)
		{
			// Calculate world position of this heightmap point
			float worldX = x * m_terrainPositionScalingFactor - (0.5f * m_terrainSize);
			float worldZ = z * m_terrainPositionScalingFactor - (0.5f * m_terrainSize);

			// Calculate distance to the center of the circle
			float dx = worldX - circleCenter.x;
			float dz = worldZ - circleCenter.z;
			float distance = sqrtf(dx * dx + dz * dz);

			if (distance <= radius)
			{
				int index = z * TERRAINRESOLUTION + x;

				// Safely modify the raw heightmap value (clamp between 0 and 255)
				int newHeight = static_cast<int>(m_heightMap[index]) + static_cast<int>(heightModifier);
				m_heightMap[index] = static_cast<unsigned char>(Clamp(newHeight, 0, 255));
			}
		}
	}

	// Update geometry and recalculate normals
	UpdateTerrain();
	CalculateTerrainNormals();
	//SaveHeightMap();

}
