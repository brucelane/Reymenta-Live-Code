uniform vec3  iResolution;  // viewport resolution (in pixels)
uniform vec3  iColor;
uniform float iGlobalTime;
uniform float iZoom;
uniform float iAlpha;
uniform vec4  iMouse;

in Vertex
{
	vec2 	uv;
} vertex;

out vec4 oColor;

void main( void )
{
	vec4 color 	= vec4( sin(iGlobalTime*iResolution.y/100.0), sin(iGlobalTime*iResolution.x/100.0), 0.0, 1.0 );
	oColor 		= color;
}