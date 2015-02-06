#ifndef DX11VERTEX_INPUT_LAYOUT_H_INCLUDED
#define DX11VERTEX_INPUT_LAYOUT_H_INCLUDED
#include <vector>
namespace DX11InputLayout
{
	enum LAYOUT	{LAYOUT_POS=0,LAYOUT_COL,LAYOUT_NORM,LAYOUT_TEX,LAYOUT_TANGENT};
}
struct DX11VertexInputLayout
{
public:
	typedef std::vector<int> VertInputLayout;
	VertInputLayout layout;
	int layoutSize;
};

#endif