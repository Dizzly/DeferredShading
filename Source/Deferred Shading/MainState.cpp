#include "MainState.h"
#include "DX11Camera.h"
#include "WindowsWindow.h"
#include "AngleConvert.h"
#include "DX11Input.h"
#include "DX11Timer.h"
#include <AntTweakBar\AntTweakBar.h>
//pre defined in base class
//---
#include "DX11Camera.h"
//--
using namespace DX11InputLayout;


float g_rot = 0;
bool g_forwardShade = false;
bool g_addLights = false;
int g_lightsPerSec = 10;
float g_lightAddTimer = 0;
const int g_posRange = 9;
const float g_maxCol = 0.3;

int g_numOfLights = 2000;

static float g_oldAtt = 0.145f;
static float g_att = 0.7f;

static bool g_rotate = true;

enum SCENE_CHOICE { NO_SCENE = 0, S_SCENE, N_SCENE, C_SCENE,P_SCENE };

MainState::MainState()
{
	world_ = XMMatrixIdentity();
	view_ = XMMatrixIdentity();
	proj_ = XMMatrixIdentity();
	waitTimer_ = 0;
	startTime_ = TheTimer::Instance()->TotalTime();
	sceneChoice_ =P_SCENE;
	bar_ = 0;
}

MainState::~MainState()
{}



void MainState::BindWorld()
{
	XMMATRIX inverse = XMMatrixIdentity();
	XMVECTOR det = XMMatrixDeterminant(world_);
	if (det.m128_f32[0] != 0)
	{
		inverse = XMMatrixInverse(&det, world_);
		inverse = XMMatrixTranspose(inverse);
	}
	worldBuf_.Update(WorldStruct(XMMatrixTranspose(world_),
		XMMatrixTranspose(inverse)));
}

void MainState::BindView()
{
	Vec3f pos = baseCam_->GetPos();
	XMVECTOR eyePos = XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
	viewBuf_.Update(ViewStruct(XMMatrixTranspose(view_), eyePos));
}

void MainState::BindProj()
{
	projBuf_.Update(XMMatrixTranspose(proj_));
}

void MainState::Update()
{

    if (TheDX11Input::Instance()->IsKeyDown(DIK_1))
    {
        g_forwardShade = false;
    }
    if (TheDX11Input::Instance()->IsKeyDown(DIK_2))
    {
        g_forwardShade = true;
    }

    if (TheDX11Input::Instance()->IsKeyDown(DIK_Q))
    {
        sceneChoice_ = S_SCENE;
    }
    else if (TheDX11Input::Instance()->IsKeyDown(DIK_W))
    {
        sceneChoice_ = N_SCENE;
    }
    else if (TheDX11Input::Instance()->IsKeyDown(DIK_E))
    {
        sceneChoice_ = C_SCENE;
    }
    else if (TheDX11Input::Instance()->IsKeyDown(DIK_R))
    {
        sceneChoice_ = P_SCENE;
    }

    if (TheDX11Input::Instance()->IsKeyDown(DIK_LEFTARROW))
    {
        g_rotate = false;
        float x = cos(DegToRad(g_rot));
        float z = sin(DegToRad(g_rot));

            g_rot += 60 * TheTimer::Instance()->DeltaTime();
        x *= 31;
        z *= 31;
        float y = sin(DegToRad(g_rot));//to move the camera up and down as well
        y *= 5;

        cam_.SetPos(Vec3f(0, 0, -24));
    }
    if (TheDX11Input::Instance()->IsKeyDown(DIK_RIGHTARROW))
    {
        g_rotate = false;
        float x = cos(DegToRad(g_rot));
        float z = sin(DegToRad(g_rot));

            g_rot -= 60 * TheTimer::Instance()->DeltaTime();

        x *= 31;
        z *= 31;
        float y = sin(DegToRad(g_rot));//to move the camera up and down as well
        y *= 5;

        cam_.SetPos(Vec3f(0, 0, -24));
    }


    if (TheDX11Input::Instance()->IsKeyDown(DIK_I))
    {
        Vec3f v = cam_.GetLookVec();
        v.Normalise();
        v *= 6;
        cam_.SetPos(cam_.GetPos() + v);
    }

	if (g_att != g_oldAtt)
	{
		pointSet_.SetAttenuationAll(Vec3f(0, 0, g_att));
		g_oldAtt = g_att;
	}
	if (waitTimer_ > 0)
	{
		waitTimer_ -= 1 * TheTimer::Instance()->DeltaTime();
	}
	else
	{

		if (!g_addLights)
		{
            if(g_rotate)
            {
			float x = cos(DegToRad(g_rot));
			float z = sin(DegToRad(g_rot));
			if (g_rotate)
			{
				g_rot += 6 * TheTimer::Instance()->DeltaTime();
			}
			x *= 31;
			z *= 31;
			float y = sin(DegToRad(g_rot));//to move the camera up and down as well
			y *= 5;

			cam_.SetPos(Vec3f(0, 0, -24));
            }
		}
		else
		{
			cam_.SetPos(Vec3f(0, 5, -31));
		}


		if (g_addLights&&g_lightAddTimer > 1)
		{
			for (int i = 0; i < g_lightsPerSec; ++i)
			{

				float r = g_maxCol*(rand() % 100) / 100;
				float g = g_maxCol*(rand() % 100) / 100;
				float b = g_maxCol*(rand() % 100) / 100;

				float x = (rand() % g_posRange * 2) - g_posRange;
				float y = (rand() % g_posRange * 2) - g_posRange;
				float z = (rand() % g_posRange * 2) - g_posRange;
				PointLight p;
				p.SetAmbient(Vec4f(0, 0, 0, 0));
				p.SetDiffuse(Vec4f(r, g, b, 0));
				p.SetPos(Vec3f(x, y, z));
				p.SetRange(70);
				p.SetAttenuation(Vec3f(0, 0, 0.4));
				if (!g_forwardShade)
				{
					pointSet_.AddLight(p);
				}
				else
				{
					forwardSet_.AddPointLight(p);
				}
			}
			g_lightAddTimer = 0;
		}
		if (g_addLights)
		{
			g_lightAddTimer += TheTimer::Instance()->DeltaTime();
		}
		if (TheTimer::Instance()->TotalTime() > 60 + 10 + startTime_)
		{
		}
	}
}


void MainState::Draw()
{
	if (waitTimer_ > 0 || sceneChoice_ == NO_SCENE)
	{
		waitTimer_ -= 1 * TheTimer::Instance()->DeltaTime();
	}
	else
	{

		SetWorld(XMMatrixIdentity());
		BindWorld();


		cam_.Use();
		if (!g_forwardShade)
		{
			gBuffer_.BindToWrite();
			preDeff_.Use();
		}
		else
		{
			forwardShader_.Use();
			forwardSet_.Use();
		}


		if (sceneChoice_ == S_SCENE)
		{
			DrawSimpleScene();
		}
		else if (sceneChoice_ == N_SCENE)
		{
			DrawNormalScene();
		}
		else if (sceneChoice_ == C_SCENE)
		{
			DrawComplexScene();
		}
		else if (sceneChoice_ ==P_SCENE)
		{
			DrawPrettyScene();
		}


		if (!g_forwardShade)
		{
			DX11Transform quad;
			quad.Use();
			TheDX11Graphics::Instance()->SetRenderTarget();
			gBuffer_.BindToRead(1); //it will have to set the current draw states view and projection maticies to 0

			pointSet_.Render(cam_,g_numOfLights);
			cam2d_.Use();
			dirSet_.Render();
			quad.Stop();
		}
		//render each light, with blending to the normal render target.
		//will need to overload the lights

		TwDraw();
		ClearResources();
	}
}

void MainState::OnActive()
{
	unsigned int seed = 17;

	TwInit(TW_DIRECT3D11, TheDX11Graphics::Instance()->GetD3DDevice());

	bar_ = TwNewBar("TweakBar");
	TwAddVarRW(bar_, "Number of Lights", TW_TYPE_INT32, &g_numOfLights,
		"Min=0 Max=8000  Step=1 Help='Selects number of lights in scene'");
	TwAddVarRW(bar_, "Light Attenuation", TW_TYPE_FLOAT, &g_att,
		"Min=0 Max=4 Step = 0.001 Help ='Attenuation values for all lights'");
	
	TwAddVarRW(bar_, "Scene Rotation", TW_TYPE_BOOL16, &g_rotate,
		"Help ='Toggles scene rotation'");

	srand(seed);
	DX11VertexInputLayout gBuf;
	gBuf.layout.push_back(LAYOUT_POS);
	gBuf.layout.push_back(LAYOUT_NORM);
	gBuf.layout.push_back(LAYOUT_TEX);
	gBuf.layoutSize = sizeof(float)* 8;

	DX11VertexInputLayout lay;
	lay.layout.push_back(LAYOUT_POS);
	lay.layout.push_back(LAYOUT_TEX);
	lay.layoutSize = sizeof(float)* 5;

	DX11VertexInputLayout justPos;
	justPos.layout.push_back(LAYOUT_POS);
	justPos.layoutSize = sizeof(float)* 3;


	if (!preDeff_.Load("DeferredShader.fx", gBuf, "VS", "PS"))
	{
		assert(0);
	}

	if (!shader_.Load("TestShader.fx", lay, "VS", "PS"))
	{
		assert(0);
	}

	if (!ball_.Init(gBuf, 3, 2))
	{
		assert(0);
	}

	if (!gBuffer_.Init())
	{
		assert(0);
	}

	if (!quad_.Init(lay))
	{
		assert(0);
	}

	if (!wood_.Load("Free-Wood-Texture-48.jpg"))
	{
		assert(0);
	}

	if (!tRexSkin_.Load("Trex_Diffuse.jpg"))
	{
		assert(0);
	}

	if (!white_.Load("white.jpg"))
	{
		assert(0);
	}
	if (!cube_.Init(gBuf))
	{
		assert(0);
	}

	dirLightShader_.Load("DeferredDirLightShader.fx", justPos, "VS", "PS");
	pointLightShader_.Load("DeferredPointLightShader.fx", justPos, "VS", "PS");
	forwardSet_.Init(10);
	if (!forwardShader_.Load("ForwardShader.fx", gBuf, "VS", "PS"))
	{
		assert(0);
	}

	if (!mesh_.Load("NewRex.OBJ", gBuf))
	{
		assert(0);
	}
	int startingLights = 0;
	if (!g_addLights)
	{
		startingLights = 8000;
	}
	dirSet_.Init(1, dirLightShader_);
	pointSet_.Init(2, pointLightShader_);



	for (int i = 0; i < startingLights; ++i)
	{
		float r = g_maxCol*(rand() % 100) / 100;
		float g = g_maxCol*(rand() % 100) / 100;
		float b = g_maxCol*(rand() % 100) / 100;

		float x = (rand() % g_posRange * 2) - g_posRange;
		float y = (rand() % g_posRange * 2) - g_posRange;
		float z = (rand() % g_posRange * 2) - g_posRange;
		PointLight p;
		p.SetAmbient(Vec4f(0, 0, 0, 0));
		p.SetDiffuse(Vec4f(r, g, b, 0));
		p.SetPos(Vec3f(x, y, z));
		p.SetRange(14);
		p.SetAttenuation(Vec3f(0, 1, 0.6));
		pointSet_.AddLight(p);
		forwardSet_.AddPointLight(p);
	}

	Vec2i ss = TheWinWindow::Instance()->GetClientBounds();

	baseCam_ = &cam_;
	cam_.SetPos(Vec3f(0, 30, 400.0f));
	cam_.LookAt(Vec3f(0, 0, 0));
	cam_.SetUp(Vec3f(0.0f, 1.0f, 0.0f));
	cam_.SetLens(Vec4f(XM_PIDIV4, (float)ss.x / (float)ss.y, 0.01f, 1000.0f));

	cam2d_.SetPos(Vec3f(0, 0, 1));
	cam2d_.LookAt(Vec3f(0, 0, -1));
	cam2d_.SetUp(Vec3f(0.0f, 1.0f, 0.0f));
	cam2d_.SetOrtho(true);
	cam2d_.SetLens(Vec4f(1, 1, 0.01f, 1000));



	worldBuf_.Init(11);
	viewBuf_.Init(12);
	projBuf_.Init(13);

	worldBuf_.Bind();
	viewBuf_.Bind();
	projBuf_.Bind();

	BindWorld();
	BindProj();
	cam_.Use();

	shader_.Use();

}

void MainState::OnInactive()
{}

void MainState::ClearResources()
{
	ID3D11ShaderResourceView* reset[14] = { 0 };
	TheDX11Graphics::Instance()->GetD3DDeviceContext()->PSSetShaderResources(0, 14, reset);
}

void MainState::DrawSimpleScene()
{
	tRexSkin_.Use();
	DX11Transform t;

	t.Translate(Vec3f(0, -5, 0));
	t.RotateY(-(g_rot / 16));
	t.Scale(Vec3f(0.05, 0.05, 0.05));
	t.AddUse();
	mesh_.Draw();
	t.Stop();

}

void MainState::DrawNormalScene()
{
	wood_.Use();
	DX11Transform t;

	t.Translate(Vec3f(-14, 0, 0));
	t.Scale(Vec3f(3, 3, 3));
	t.AddUse();
	ball_.Draw();
	t.Stop();
	t.Reset();

	t.Translate(Vec3f(0, 14, 0));
	t.Scale(Vec3f(2, 2, 2));
	t.AddUse();
	ball_.Draw();
	t.Stop();
	t.Reset();

	wood_.Stop();

	tRexSkin_.Use();

	t.Translate(Vec3f(0, -1, 0));
	t.Scale(Vec3f(0.05, 0.05, 0.05));
	t.RotateZ(g_rot / 16);
	t.AddUse();
	mesh_.Draw();
	t.Stop();
	t.Reset();

	t.Scale(Vec3f(0.02, 0.02, 0.02));

	t.RotateY(g_rot / 16);
	t.RotateZ(90);

	t.AddUse();
	mesh_.Draw();
	t.Stop();
	t.Reset();
}

void MainState::DrawComplexScene()
{
	wood_.Use();
	DX11Transform t;
	for (int i = -5; i < 5; ++i)
	{
		for (int j = -5; j < 5; ++j)
		{

			t.Translate(Vec3f(i * 1.5, -3, j * 1.5));
			t.Scale(Vec3f(0.25, 0.25, 0.25));
			t.AddUse();
			ball_.Draw();
			t.Stop();
			t.Reset();
		}
	}
	tRexSkin_.Use();

	t.Scale(Vec3f(0.02, 0.02, 0.02));
	t.Translate(Vec3f(5, -1.5, -5));
	t.RotateY(g_rot / 16);
	t.Use();
	mesh_.Draw();

	t.Translate(Vec3f(-5, -1.5, 5));
	t.RotateY(-(g_rot / 16));
	t.Use();
	mesh_.Draw();

	t.Translate(Vec3f(5, -1.5, 5));
	t.RotateY(-(g_rot / 8));
	t.Use();
	mesh_.Draw();

	t.Translate(Vec3f(-5, -1.5, -5));
	t.RotateY(g_rot / 8);
	t.Use();
	mesh_.Draw();

	t.Stop();
	t.Reset();
}

void MainState::DrawPrettyScene()
{

	DX11Transform t;
	DX11Transform j;
	wood_.Use();
		j.RotateY(g_rot / 32);
	j.Scale(Vec3f(2, 2, 2));
	j.AddUse();
	for (int i = -5; i < 5; ++i)
	{
		for (int j = -5; j < 5; ++j)
		{

			t.Translate(Vec3f(i * 1.5, -2, j * 1.5));
			t.Scale(Vec3f(0.25, 0.25, 0.25));
			t.AddUse();
			ball_.Draw();
			t.Stop();
			t.Reset();
			t.Translate(Vec3f(i * 1.5, 2, j * 1.5));
			t.Scale(Vec3f(0.25, 0.25, 0.25));
			t.AddUse();
			ball_.Draw();
			t.Stop();
			t.Reset();
		}
	}
	j.Stop();
	j.Reset();
	tRexSkin_.Use();
	j.Translate(Vec3f(0, -3,0));
	j.RotateY(-g_rot / 32);
	j.Scale(Vec3f(0.02, 0.02, 0.02));
	j.AddUse();
	mesh_.Draw();
	j.Stop();

}