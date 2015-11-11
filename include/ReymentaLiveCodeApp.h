#include "cinder/ImageIO.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"

#include "CinderAwesomium.h"
// transparent window
#include "dwmapi.h"
// parameters
#include "ParameterBag.h"
// utils
#include "Batchass.h"
// UserInterface
#include "CinderImGui.h"

#include "CodeEditor.h"

#if defined( CINDER_COCOA )
#include "cinderSyphon.h"
#else
// spout
#include "SpoutWrapper.h"
#endif
// VisualStudio doesn't seem to support initializer_list
// yet so let's use boost::assign::list_of instead
#if defined( CINDER_MSW )
#include "boost/assign/list_of.hpp"
using namespace boost::assign;
#endif
using namespace ci;
using namespace ci::app;
using namespace ph;
using namespace std;
using namespace Reymenta;

// imgui
#define IM_ARRAYSIZE(_ARR)			((int)(sizeof(_ARR)/sizeof(*_ARR)))

class ReymentaLiveCodeApp : public App {
public:
	void setup();
	void shutdown();
	void update();
	void draw();
	void resize();

	void mouseMove(MouseEvent event);
	void mouseDown(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void mouseUp(MouseEvent event);
	void mouseWheel(MouseEvent event);

	void keyDown(KeyEvent event);
	void keyUp(KeyEvent event);
private:

	CodeEditorRef				mCodeEditor;
	Awesomium::WebCore*			mWebCorePtr;
	Awesomium::WebView*			mWebViewPtr;

	gl::TextureRef				mWebTexture;
	gl::TextureRef				mLoadingTexture;
	gl::TextureRef				mTexture;
	gl::GlslProgRef				mShader;
	Font						mFont;
	bool						mFirstLaunch;
	// Parameters
	ParameterBagRef				mParameterBag;
	// Utils
	BatchassRef					mBatchass;

	// imgui
	char						buf[32];
	float						color[4];
	int							xPos;
	int							yPos;
	int							largeW;
	int							largeH;
	int							margin;
	bool						removeUI;
};
