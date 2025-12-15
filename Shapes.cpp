#include "Shapes.hpp"
#include "Text.hpp"

void ShapeRenderer::Initialize(ID3D11Device* Device, ID3D11DeviceContext* Context, int Width, int Height)
{
	using namespace DirectX;
	m_Batch = std::make_unique<DX11::PrimitiveBatch<DX11::VertexPositionColor>>(Context);
	m_Effect = std::make_unique<BasicEffect>(Device);
	m_States = std::make_unique<CommonStates>(Device);

	m_Effect->SetVertexColorEnabled(true);

	m_Effect->SetProjection(
		XMMatrixOrthographicOffCenterLH(
			0.0f, static_cast<float>(Width),
			static_cast<float>(Height), 0.0f,
			0.0f, 1.0f
		)
	);

	void const* ShaderByteCode;
	size_t ByteCodeLength;
	m_Effect->GetVertexShaderBytecode(&ShaderByteCode, &ByteCodeLength);

	Device->CreateInputLayout(
		DX11::VertexPositionColor::InputElements,
		DX11::VertexPositionColor::InputElementCount,
		ShaderByteCode,
		ByteCodeLength,
		m_InputLayout.GetAddressOf()
	);
}

void ShapeRenderer::BeginFrame(ID3D11DeviceContext* Context)
{
	Context->OMSetBlendState(m_States->AlphaBlend(), nullptr, 0xFFFFFFFF);
	Context->OMSetDepthStencilState(m_States->DepthNone(), 0);
	Context->RSSetState(m_States->CullNone());

	Context->IASetInputLayout(m_InputLayout.Get());

	m_Effect->Apply(Context);
	m_Batch->Begin();
}

void ShapeRenderer::EndFrame()
{
	m_Batch->End();
}

void ShapeRenderer::OnResize(int Width, int Height)
{
	if (!m_Effect)
		return;
	m_Effect->SetProjection(
		DirectX::XMMatrixOrthographicOffCenterLH(
			0.0f, static_cast<float>(Width),
			static_cast<float>(Height), 0.0f,
			0.0f, 1.0f
		)
	);
}

void ShapeRenderer::DrawLine(float StartX, float StartY, float EndX, float EndY, DirectX::XMVECTORF32 Colour)
{
	DirectX::DX11::VertexPositionColor V1({ StartX, StartY, 0.f }, Colour);
	DirectX::DX11::VertexPositionColor V2({ EndX, EndY, 0.f }, Colour);
	m_Batch->DrawLine(V1, V2);
}

void ShapeRenderer::DrawFilledRect(float X, float Y, float Width, float Height, DirectX::XMVECTORF32 Colour)
{
	DirectX::DX11::VertexPositionColor V[4] =
	{
		{{X,         Y,         0.f}, Colour}, // top-left
		{{X + Width, Y,         0.f}, Colour}, // top-right
		{{X + Width, Y + Height,0.f}, Colour}, // bottom-right
		{{X,         Y + Height,0.f}, Colour}, // bottom-left
	};

	m_Batch->DrawQuad(V[0], V[1], V[2], V[3]);
}

void ShapeRenderer::DrawRect(float X, float Y, float Width, float Height, DirectX::XMVECTORF32 Colour)
{
	DrawLine(X, Y, X + Width, Y, Colour);               // Top
	DrawLine(X + Width, Y, X + Width, Y + Height, Colour); // Right
	DrawLine(X + Width, Y + Height, X, Y + Height, Colour); // Bottom
	DrawLine(X, Y + Height, X, Y, Colour);                 // Left
}

void ShapeRenderer::DrawFilledTriangle(float X1, float Y1, float X2, float Y2, float X3, float Y3, DirectX::XMVECTORF32 Colour)
{
	DirectX::DX11::VertexPositionColor V[3] =
	{
		{{X1, Y1, 0.f}, Colour},
		{{X2, Y2, 0.f}, Colour},
		{{X3, Y3, 0.f}, Colour},
	};

	m_Batch->DrawTriangle(V[0], V[1], V[2]);
}

void ShapeRenderer::DrawTriangle(float X1, float Y1, float X2, float Y2, float X3, float Y3, DirectX::XMVECTORF32 Colour)
{
	DrawLine(X1, Y1, X2, Y2, Colour);
	DrawLine(X2, Y2, X3, Y3, Colour);
	DrawLine(X3, Y3, X1, Y1, Colour);
}