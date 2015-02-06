#include "DX11Shader.h"
#include <d3dcompiler.h>
#include <xnamath.h>
#include <vector>
#include <assert.h>
#include "DX11Graphics.h"

using namespace DX11InputLayout;


DX11Shader::DX11Shader(): pVertShader_(NULL), pPixShader_(NULL), pVertLayout_(NULL)
{

}

DX11Shader::~DX11Shader()
{
	if(pVertShader_) pVertShader_->Release();
	if(pPixShader_) pPixShader_->Release();
	if(pVertLayout_) pVertLayout_->Release();
}

void DX11Shader::Use()
{
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->IASetInputLayout(pVertLayout_);

	TheDX11Graphics::Instance()->GetD3DDeviceContext()->VSSetShader(pVertShader_,NULL,0);
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->PSSetShader(pPixShader_,NULL,0);
}

bool DX11Shader::Load(const std::string& filename,
		DX11VertexInputLayout& input,
	const std::string& vertFuncName,
	const std::string& pixFuncName,
	std::string* errorString)
{
	HRESULT hr;

	ID3DBlob* pVSBlob = NULL;

	hr= CompileShader(filename, vertFuncName, &pVSBlob,"vs_4_0")? S_OK : E_FAIL;

	if( FAILED( hr ) )
	{
		MessageBoxA( NULL,
			(filename+" has an error in its Vertex Shader").c_str(), "Error", MB_OK );
		assert(0);
		return false;
	}
	hr = TheDX11Graphics::Instance()->GetD3DDevice()->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pVertShader_);
	if( FAILED( hr ) )
	{	
		return false;
	}
	

	if(!ReadInputLayout(input,pVSBlob))
	{
	assert(0);
	}
	vertInputLayout_=input;

	pVSBlob->Release();
	pVSBlob=NULL;

	ID3DBlob* pPSBlob = NULL;
	hr = CompileShader(filename, pixFuncName, &pPSBlob,"ps_4_0" ) ? S_OK : E_FAIL;
	if( FAILED( hr ) )
	{
		MessageBoxA( NULL,
			(filename+" has an error in its Pixel Shader").c_str(), "Error", MB_OK );
		return false;
	}

	// Create the pixel shader
	hr = TheDX11Graphics::Instance()->GetD3DDevice()->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &pPixShader_ );
	if( FAILED( hr ) )
	{
		assert(0);
		return false;
	}
	pPSBlob->Release();
	pPSBlob=NULL;
	return true;
}


bool DX11Shader::ReadInputLayout(DX11VertexInputLayout& layout,ID3DBlob* blob)
{
	HRESULT hr;
int size=0;
	int numOfElements=0;
	//if you want dynamic sizes switch this static array to a vector
	D3D11_INPUT_ELEMENT_DESC aLayout [10];
	//then we loop though
	for(std::vector<int>::iterator it=layout.layout.begin();
		it!=layout.layout.end();
		++it)
	{
		if(LAYOUT_POS==(*it))
		{
			//so we check to see if theres a position element
			//which we know is a vec3
			D3D11_INPUT_ELEMENT_DESC pos ={
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 
				0, size, D3D11_INPUT_PER_VERTEX_DATA, 0
			};
			//we add the position to the array for layout
			aLayout[numOfElements]=pos;
			//increase the number of elements
			++numOfElements;
			//and add its size so that we can offset by it later
			size+=sizeof(XMFLOAT3);
		}
		if(LAYOUT_COL==(*it))
		{
			D3D11_INPUT_ELEMENT_DESC col={
				"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 
				size, D3D11_INPUT_PER_VERTEX_DATA, 0
			};
			aLayout[numOfElements]=col;
			++numOfElements;
			size+=sizeof(XMFLOAT3);
		}
		if(LAYOUT_NORM==(*it))
		{
			D3D11_INPUT_ELEMENT_DESC norm ={
				"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 
				0, size, D3D11_INPUT_PER_VERTEX_DATA, 0
			};
			aLayout[numOfElements]=norm;
			++numOfElements;
			size+=sizeof(XMFLOAT3);
		}
		if(LAYOUT_TEX==(*it))
		{
			D3D11_INPUT_ELEMENT_DESC tex ={
				"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 
				0, size, D3D11_INPUT_PER_VERTEX_DATA, 0
			};
			aLayout[numOfElements]=tex;
			++numOfElements;
			size+=sizeof(XMFLOAT2);
		}
		if(LAYOUT_TANGENT==(*it))
		{
			D3D11_INPUT_ELEMENT_DESC tan ={
				"TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,
				0,size,D3D11_INPUT_PER_VERTEX_DATA,0
			};
			aLayout[numOfElements]=tan;
			++numOfElements;
			size+=sizeof(XMFLOAT3);
		}
	}
	hr = TheDX11Graphics::Instance()->GetD3DDevice()->CreateInputLayout(aLayout,numOfElements, blob->GetBufferPointer(),
		blob->GetBufferSize(), &pVertLayout_ );
	GetLastError();
	if( FAILED( hr ) )
		return false;
	// Set the input layout
	return true;
}

DX11VertexInputLayout& DX11Shader::GetLayout()
{
return vertInputLayout_;
}

bool DX11Shader::CompileShader(const std::string& szFileName, 
	const std::string& szEntryPoint,
	ID3DBlob** ppBlobOut, 
	const std::string& szShaderModel)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob=NULL;
	hr = D3DX11CompileFromFileA(szFileName.c_str(), NULL, NULL, szEntryPoint.c_str(), szShaderModel.c_str(), 
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
	if( FAILED(hr) )
	{
		if( pErrorBlob != NULL )
		{
		OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer());
		}
		if( pErrorBlob ) {pErrorBlob->Release();}
		return false;
	}
	if( pErrorBlob ) pErrorBlob->Release();

	return true;
}

