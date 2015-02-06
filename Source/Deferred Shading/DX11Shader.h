#ifndef DX11_SHADER_H_INCLUDED
#define DX11_SHADER_H_INCLUDED


#include <D3DX11.h>
#include <string>
#include "File.h"
#include "DX11VertexInputLayout.h"
#include "SmartPointer.h"


class DX11Shader :public RefCounted
{
public:
	DX11Shader();
	~DX11Shader();

	bool Load(const std::string& filename,
		DX11VertexInputLayout& layout,
		const std::string& vertFuncName="VertShader",
		const std::string& pixFuncName="PixShader",
		std::string* errorString =NULL);

	DX11VertexInputLayout& GetLayout();
	
	void Use();
private:
	bool CompileShader(const std::string& szFileName, const std::string& szEntryPoint,
	ID3DBlob** ppBlobOut, 
	const std::string& szShaderModel);
	bool DX11Shader::ReadInputLayout(DX11VertexInputLayout& layout,ID3DBlob* blob);
private:
	ID3D11VertexShader* pVertShader_;
	ID3D11PixelShader* pPixShader_;
	ID3D11InputLayout* pVertLayout_;
	DX11VertexInputLayout vertInputLayout_;

};
typedef SmartPointer<DX11Shader> PDX11Shader;



#endif