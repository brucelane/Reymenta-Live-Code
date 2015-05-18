#version 130
uniform vec3  iResolution;   // viewport resolution (in pixels)
uniform vec3  iColor;
uniform float iGlobalTime;
uniform float iZoom;
uniform float iAlpha;
uniform vec4  iMouse;
uniform sampler2D iChannel0;

// http://www.ustream.tv/recorded/51333511
float map( in vec3 p )
{
  return length(p) - 1.0;
}

void main( void )
{
	vec2 uv = gl_FragCoord.xy / iResolution.xy;

    vec2 p = -1.0 + 2.0*uv;
    p.x *= iResolution.x /iResolution.y;
	vec3 col = vec3(0.0);
  	// camera origin
    vec3 ro = vec3(0.0,0.0,2.0);
  	// ray direction
  	vec3 rd = normalize(vec3(p,-1.0));
    float tmax = 20.0;
  	float h = 0.001;
  	float t = 0.0;
  	for (int i = 0; i<100; i++)
	{
	  	if (h<0.001 || t>tmax) break;
	  	h = map( ro + t*rd );
	  	t += h;
	}  
    if (t<tmax)
    {
     	col = vec3(1.0, 0.4,0.2);   
    }
	gl_FragColor = vec4(col,1.0);
}
