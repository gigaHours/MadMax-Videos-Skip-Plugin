#include "plugin.h"

DLLATTATCH;

void PluginAttach(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
}

struct BINK
{
	unsigned int Width;
	unsigned int Height;
	unsigned int Frames;
	unsigned int FrameNum;
	unsigned int LastFrameNum;
	unsigned int FrameRate;
	unsigned int FrameRateDiv;
	unsigned int ReadError;
	unsigned int OpenFlags;
	unsigned int BinkType;
	unsigned int Size;
};

class CVideoManager
{
public:

	struct SVideoManagerContext
	{
		volatile unsigned int m_BinkOpenSignal;
		BINK* m_HBink;
		/*mm::std::string m_FilePath;
		unsigned int m_StreamNameHash;
		BINKFRAMEBUFFERS m_bink_frames;
		void* m_SharedSysTexture;
		int m_DroppedFrames;*/
	};

	void* m_SysYTexture[2];
	void* m_SysCrTexture[2];
	void* m_SysCbTexture[2];
	CVideoManager::SVideoManagerContext m_VMC;
	/*
	volatile unsigned int* m_BinkOpenSignal;
	Graphics::HVertexBuffer_t* m_FullScreenPolygonVB;
	Graphics::HVertexDeclaration_t* m_FullScreenPolygonVDecl;
	CVertexProgramHolder m_VideoVP;
	CFragmentProgramHolder m_VideoFP;
	const NGraphicsEngine::SResourceContext* m_ResourceContext;
	NGraphicsEngine::CRenderPass* m_FullScreenVideoPass;
	NGraphicsEngine::CRenderBlockFullScreenVideo* m_RenderBlock;
	bool m_SentVideoIsPlaying;
	bool m_Playing;
	unsigned __int64 m_AudioSyncT0;
	float m_SettingsVolume;
	bool m_SubtitlesStopped;
	int m_FrameToStartTransition;
	float m_Transparency;
	bool m_Transition;
	bool m_ToldGameToResume;
	float m_AudioFade;
	*/

	CMETHODV(0x1405D9DE0, 0x142211330, bool, RunPostInit())
	CMETHODV(0x1405D9D00, 0x142211220, void, StopVideo())
};


void (*CVideoManager__Update_orig)(CVideoManager* thiz, float dt);
void CVideoManager__Update_hook(CVideoManager* thiz, float dt)
{
	if (thiz->RunPostInit() && thiz->m_VMC.m_HBink)
	{
		auto bink = thiz->m_VMC.m_HBink;
		if (bink->FrameNum < bink->Frames)
			thiz->StopVideo();
	}
}

void PluginHooks() {
	HookMgr::Install(ADDRESS(0x1405DE290, 0x142215D90), CVideoManager__Update_hook, CVideoManager__Update_orig);
}