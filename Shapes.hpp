#pragma once
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <CommonStates.h>
#include <Effects.h>
#include <wrl/client.h>
#include <SimpleMath.h>

class ShapeRenderer
{
private:
	std::unique_ptr<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>> m_Batch;
	std::unique_ptr<DirectX::BasicEffect> m_Effect;
	std::unique_ptr<DirectX::CommonStates> m_States;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	
public:

	void Initialize(ID3D11Device* Device, ID3D11DeviceContext* Context, int Width, int Height);
	void BeginFrame(ID3D11DeviceContext* Context);
	void EndFrame();
	void OnResize(int Width, int Height);

	// Drawing Functions
	void DrawLine(float StartX, float StartY, float EndX, float EndY, DirectX::XMVECTORF32 Colour); // Uses built in DrawLine

	void DrawFilledRect(float X, float Y, float Width, float Height, DirectX::XMVECTORF32 Colour); // Uses built in DrawQuad
	void DrawRect(float X, float Y, float Width, float Height, DirectX::XMVECTORF32 Colour); // Chains DrawLine calls

	void DrawFilledTriangle(float X1, float Y1, float X2, float Y2, float X3, float Y3, DirectX::XMVECTORF32 Colour); // Uses built in DrawTriangle
	void DrawTriangle(float X1, float Y1, float X2, float Y2, float X3, float Y3, DirectX::XMVECTORF32 Colour); // Chains DrawLine calls
};