#include "cinder/ImageIO.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"

#include "CinderAwesomium.h"
// transparent window
#include "dwmapi.h"
// parameters
#include "ParameterBag.h"

#include "CodeEditor.h"

#if defined( CINDER_COCOA )
#include "cinderSyphon.h"
#else
#include "Spout.h"
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

class ReymentaLiveCodeApp : public AppBasic {
public:
	void prepareSettings(Settings *settings);
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

	float					iGlobalTime;        // shader playback time (in seconds)
	gl::GlslProg			mShader;
	CodeEditorRef			mCodeEditor;
	Awesomium::WebCore*		mWebCorePtr;
	Awesomium::WebView*		mWebViewPtr;

	gl::Texture				mWebTexture;
	gl::Texture				mLoadingTexture;

	Font					mFont;
	// Parameters
	ParameterBagRef				mParameterBag;
	//const string&				vert="void main() { gl_FrontColor = gl_Color; gl_TexCoord[0] = gl_MultiTexCoord0; gl_Position = ftransform(); }";
};
